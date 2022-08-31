 /*
  AsyncElegantOTA Callback Demo Example - This example will work for both ESP8266 & ESP32 microcontrollers.
  -----
  Author: Ayush Sharma ( https://github.com/ayushsharma82 )
  
  Important Notice: Star the repository on Github if you like the library! :)
  Repository Link: https://github.com/ayushsharma82/AsyncElegantOTA
*/

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char* ssid = "........";
const char* password = "........";

AsyncWebServer server(80);

int iCallBackCount = 0;

void MyAction_onOTAStart() {
  iCallBackCount = 0;
  Serial.printf("OTA update started %d\n\r", millis());
}

void  MyAction_onOTAProgress() {
  iCallBackCount = iCallBackCount + 1;
  Serial.printf("OTA progress %d\n\r", millis());
}

void MyAction_onOTAEnd() {
  Serial.printf("OTA update ended %d\n\r", millis());
  iCallBackCount = 0 ;
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response.");
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
    server.begin();
  Serial.println("HTTP server started");

// Add the AsyncElegantOTA callbacks
// Not all are required you can add each callback individually
//
// Watch the output on the serial monitor during OTA update.
  AsyncElegantOTA.onOTAStart(MyAction_onOTAStart);
  AsyncElegantOTA.onOTAProgress(MyAction_onOTAProgress);
  AsyncElegantOTA.onOTAEnd(MyAction_onOTAEnd);

}

void loop(void) {
}
