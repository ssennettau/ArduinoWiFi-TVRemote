#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

// Wireless Network SSID and Preshared Key
#ifndef STASSID
#define STASSID "MyIOTNetworkSSID"
#define STAPSK  "MyIOTNetworkPassphrase"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

// HTML Source Code for the Menu
const char menu[] PROGMEM =  R"rawliteral(
<html>
<head>
<title>TV Remote Menu</title>
<style type="text/css">
body { font-family: monospace; }
a { text-decoration: none; }
p { margin: 0; }
.wrap { display: inline-block; width: 100%; position: relative; text-align: center; }
.wrap::after { padding-top: 56.25%; display: block; content: ''; }
.content { display: inline-block; position: absolute; top: 0; bottom: 0; right: auto; left: auto; outline: thin dashed black; padding: 5; }
</style>
<script type="text/javascript">
function remoteCommand(id)
{
    const xhttp = new XMLHttpRequest();
    xhttp.open("GET", "/command/" + id);
    xhttp.send();
}
</script>
</head>
<body>
    <div class="wrap">
        <div class="content">
            <p><a id="power" href="#0" onclick="remoteCommand(this.id)">Power</a></p>
            <p><a id="menu_u" href="#0" onclick="remoteCommand(this.id)">&#x25B2;</a></p>
            <p><a id="menu_l" href="#0" onclick="remoteCommand(this.id)">&#x25C0;</a>&nbsp;<a id="menu_ok" href="#0" onclick="remoteCommand(this.id)">&#x25C9;</a>&nbsp;<a id="menu_r" href="#0" onclick="remoteCommand(this.id)">&#x25B6;</a></p>
            <p><a id="menu_d" href="#0" onclick="remoteCommand(this.id)">&#x25BC;</a></p>
            <p />
            <p><a id="nav_home" href="#0" onclick="remoteCommand(this.id)">&#x2616;</a>&nbsp;<a id="nav_src" href="#0" onclick="remoteCommand(this.id)">&#x2690;</a></p>
            <p><a id="nav_back" href="#0" onclick="remoteCommand(this.id)">&#x2190;</a>&nbsp;<a id="nav_sub" href="#0" onclick="remoteCommand(this.id)">&#x2630;</a></p>
            <p />
            <p>CH&nbsp;VL</p>
            <p><a id="ch_up" href="#0" onclick="remoteCommand(this.id)">&#x25B2;</a>&nbsp;&nbsp;&nbsp;<a id="vl_up" href="#0" onclick="remoteCommand(this.id)">&#x25B2;</a></p>
            <p>&nbsp;&nbsp;&nbsp;&nbsp;<a id="vl_up" href="#0" onclick="remoteCommand(this.id)">M</a></p>
            <p><a id="ch_dn" href="#0" onclick="remoteCommand(this.id)">&#x25BC;</a>&nbsp;&nbsp;&nbsp;<a id="vl_dn" href="#0" onclick="remoteCommand(this.id)">&#x25BC;</a></p>
    </div>
</div>
</body>
</html>)rawliteral";

void setup(void) {
  Serial.begin(115200);

  // Initiate connection to wireless network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Waiting for connection to wifi, and outputs details to Serial
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Confirming that multicast-DNS is running
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // Navigating to '/' returns the menu
  server.on(F("/"), []() {
    server.send(200, "text/html", menu);
  });

  // Navigating to '/menu' returns the menu
  server.on(F("/menu"), []() {
    server.send(200, "text/html", menu);
  });

  // Navigating the '/data' returns a dump of the Serial input from the Arduino
  // This is useful for debugging when commands aren't being executed
  server.on(F("/data"), []() {
    server.send(200, "text/plain", Serial.readString());
    Serial.flush();
  });

  // Navigating to '/command/*' will forward commands to the Arduino
  server.on(UriBraces("/command/{}"), []() {
    String command = server.pathArg(0);
    Serial.println(command);
    server.send(200, "text/plain", "SEND: " + command);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  // Server will continue running
  server.handleClient();
}
