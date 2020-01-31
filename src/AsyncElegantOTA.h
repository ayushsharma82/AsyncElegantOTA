#ifndef AsyncElegantOTA_h
#define AsyncElegantOTA_h

#include "Arduino.h"
#include "stdlib_noniso.h"

#if defined(ESP8266)
    #include "ESP8266WiFi.h"
    #include <ESPAsyncTCP.h>
#elif defined(ESP32)
    #include "WiFi.h"
    #include <AsyncTCP.h>
    #include <Update.h>
    #include <esp_int_wdt.h>
    #include <esp_task_wdt.h>
#endif

#include <ESPAsyncWebServer.h>

#include "elegantWebpage.h"


size_t content_len;

class AsyncElegantOtaClass{
    public:

        void begin(AsyncWebServer *server){
            _server = server;

            _server->on("/update", HTTP_GET, [&](AsyncWebServerRequest *request){
                AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", ELEGANT_HTML, ELEGANT_HTML_SIZE);
                response->addHeader("Content-Encoding", "gzip");
                request->send(response);
            });

            _server->on("/update", HTTP_POST, [&](AsyncWebServerRequest *request) {
                // the request handler is triggered after the upload has finished... 
                // create the response, add header, and send response
                AsyncWebServerResponse *response = request->beginResponse((Update.hasError())?500:200, "text/plain", (Update.hasError())?"FAIL":"OK");
                response->addHeader("Connection", "close");
                response->addHeader("Access-Control-Allow-Origin", "*");
                request->send(response);
                restartRequired = true;
            }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
                //Upload handler chunks in data
                if (!index) {
                
                    content_len = request->contentLength();
                    #if defined(ESP8266)
                        int cmd = (filename.indexOf("spiffs") > -1) ? U_FS : U_FLASH;
                        Update.runAsync(true);
                        if (!Update.begin(content_len, cmd)){ // Start with max available size
                    #elif defined(ESP32)
                        int cmd = (filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
                        if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) { // Start with max available size
                    #endif
                            Update.printError(Serial);   
                        }
                
                }

                // Write chunked data to the free sketch space
                if (Update.write(data, len) != len) {
                    Update.printError(Serial); 
                }
                    
                if (final) { // if the final flag is set then this is the last frame of data
                    if (Update.end(true)) { //true to set the size to the current progress

                    }
                }
            });
        }

        void loop(){
            if(restartRequired){
                yield();
                delay(1000);
                yield();
                #if defined(ESP8266)
                    ESP.restart();
                #elif defined(ESP32)
                    esp_task_wdt_init(1,true);
                    esp_task_wdt_add(NULL);
                    while(true);
                #endif
            }
        }

    private:
        AsyncWebServer *_server;
        bool restartRequired = false;

};

AsyncElegantOtaClass AsyncElegantOTA;
#endif
