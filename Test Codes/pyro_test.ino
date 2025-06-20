#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "monkee's sirius";
const char* password = "wolf";

const int pyro1_con = 41;
const int pyro2_con = 42;
const int pyro1_en = 47;
const int pyro2_en = 48;

WebServer server(80);

void handleRoot() {
  int EM1 = digitalRead(pyro1_con);
  int EM2 = digitalRead(pyro2_con);

  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Ematch Control</title>";
  html += "<style>button{padding:20px;margin:10px;font-size:20px;} .circle{width:20px;height:20px;border-radius:50%;display:inline-block;margin-left:10px;}</style></head><body>";
  html += "<h2>ESP32 Ematch Control</h2>";

  html += "<form action='/fire1'><button>Ematch 1</button></form>";
  html += "<span class='circle' style='background-color:" + String(EM1 ? "green" : "red") + ";'></span><br>";

  html += "<form action='/fire2'><button>Ematch 2</button></form>";
  html += "<span class='circle' style='background-color:" + String(EM2 ? "green" : "red") + ";'></span><br>";

  html += "<script>setTimeout(()=>{location.reload();}, 1000);</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void fire1() {
  digitalWrite(pyro1_en, HIGH);
  delay(500); // Optional: how long the pyro pin stays HIGH
  digitalWrite(pyro1_en, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void fire2() {
  digitalWrite(pyro2_en, HIGH);
  delay(500);
  digitalWrite(pyro2_en, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  // Set pin modes
  pinMode(pyro1_con, INPUT);
  pinMode(pyro2_con, INPUT);
  pinMode(pyro1_en, OUTPUT);
  pinMode(pyro2_en, OUTPUT);
  digitalWrite(pyro1_en, LOW);
  digitalWrite(pyro2_en, LOW);

  // Start Wi-Fi AP
  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  // Setup server routes
  server.on("/", handleRoot);
  server.on("/fire1", fire1);
  server.on("/fire2", fire2);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
