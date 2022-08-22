
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "AP602_2";
const char* passwd = "09080411";

const char* serverName = "http://loclahost:3000/test";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(ssid, passwd);
  Serial.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds, will take this long to send the first message...");
}

void loop() {
  if((millis() - lastTime) > timerDelay) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName);

    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

    Serial.println("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();    
  } else {
    Serial.println("WiFi Disconnected");
  } 
  lastTime = millis(); 
}
