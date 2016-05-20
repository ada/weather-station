#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Tenda_27EE90";
const char* password = "glassspoon";

ESP8266WebServer server (8266);

void Put_Client (WiFiClient c)
{
  Serial.print ("remoteIP");
  Serial.println (c.remoteIP ());
  Serial.print ("remotePort");
  Serial.println (c.remotePort ());
  Serial.print ("localIPeIP");
  Serial.println (c.localIP ());
  Serial.print ("localPort");
  Serial.println (c.localPort ());
}

void handleRoot ()
{
  Put_Client (server.client ());
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound ()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send (404, "text/plain", message);
}

void setup(void)
{
  Serial.begin (115200);
  WiFi.begin (ssid, password);
  Serial.println ("");

  // Wait for connection
  while (WiFi.status () != WL_CONNECTED)
  {
    delay(500);
    Serial.print (".");
  }
  Serial.println ("");
  Serial.print ("Connected to ");
  Serial.println (ssid);
  Serial.print ("IP address: ");
  Serial.println (WiFi.localIP());
  Serial.print ("subnetMask: ");
  Serial.println (WiFi.subnetMask());
  Serial.print ("gatewayIP: ");
  Serial.println (WiFi.gatewayIP());

  if (MDNS.begin ("esp8266"))
  {
    Serial.println ("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/inline", []()
  {
    WiFiClient c;
    if (c.connect("192.168.0.100", 333))
    {
      Serial.println ("connected to http://193.11.134.14:333");
      // Make a HTTP request:
      c.println ("GET /search?q=arduino HTTP/1.0");
      c.println ();
      c.stop ();
    }
    else
    {
      Serial.println ("connection failed");
    }
    server.send(200, "text/plain", "this works as well");
    
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

String msg;

void loop(void)
{
  server.handleClient();
  if (Serial.available() > 0)
  {
    Serial.println (Serial.read ());
    msg = Serial.readString();
    Serial.println ("readString");
    Serial.println (msg);
    if (msg == "rst")
    {
      Serial.println ("execute rst");
      
    }
  }
}
