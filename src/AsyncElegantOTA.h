#ifndef AsyncElegantOTA_h
#define AsyncElegantOTA_h

#warning AsyncElegantOTA.loop(); is deprecated, please remove it from loop() if defined. This function will be removed in a future release.

#include "Arduino.h"
#include "stdlib_noniso.h"

#if defined(ESP8266)
    #include "ESP8266WiFi.h"
    #include "ESPAsyncTCP.h"
    #include "flash_hal.h"
    #include "FS.h"
#elif defined(ESP32)
    #include "WiFi.h"
    #include "AsyncTCP.h"
    #include "Update.h"
    #include "esp_int_wdt.h"
    #include "esp_task_wdt.h"
#endif

#include "Hash.h"
#include "ESPAsyncWebServer.h"
#include "FS.h"

#include "elegantWebpage.h"


class AsyncElegantOtaClass{

    public:

        void setID(const char* id){
            _id = id;
        }

        void begin(AsyncWebServer *server, const char* username = "", const char* password = ""){
            _server = server;

            if(strlen(username) > 0){
                _authRequired = true;
                _username = username;
                _password = password;
            }else{
                _authRequired = false;
                _username = "";
                _password = "";
            }

            _server->on("/update/identity", HTTP_GET, [&](AsyncWebServerRequest *request){
                if(_authRequired){
                    if(!request->authenticate(_username.c_str(), _password.c_str())){
                        return request->requestAuthentication();
                    }
                }
                #if defined(ESP8266)
                    request->send(200, "application/json", "{\"id\": \""+_id+"\", \"hardware\": \"ESP8266\"}");
                #elif defined(ESP32)
                    request->send(200, "application/json", "{\"id\": \""+_id+"\", \"hardware\": \"ESP32\"}");
                #endif
            });

            _server->on("/update", HTTP_GET, [&](AsyncWebServerRequest *request){
                if(_authRequired){
                    if(!request->authenticate(_username.c_str(), _password.c_str())){
                        return request->requestAuthentication();
                    }
                }
                AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", ELEGANT_HTML, ELEGANT_HTML_SIZE);
                response->addHeader("Content-Encoding", "gzip");
                request->send(response);
            });

            _server->on("/update", HTTP_POST, [&](AsyncWebServerRequest *request) {
                if(_authRequired){
                    if(!request->authenticate(_username.c_str(), _password.c_str())){
                        return request->requestAuthentication();
                    }
                }
                // the request handler is triggered after the upload has finished... 
                // create the response, add header, and send response
                AsyncWebServerResponse *response = request->beginResponse((Update.hasError())?500:200, "text/plain", (Update.hasError())?"FAIL":"OK");
                response->addHeader("Connection", "close");
                response->addHeader("Access-Control-Allow-Origin", "*");
                request->send(response);
                restart();
            }, [&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
                //Upload handler chunks in data
                if(_authRequired){
                    if(!request->authenticate(_username.c_str(), _password.c_str())){
                        return request->requestAuthentication();
                    }
                }

                if (!index) {
                    if(!request->hasParam("MD5", true)) {
                        return request->send(400, "text/plain", "MD5 parameter missing");
                    }

                    if(!Update.setMD5(request->getParam("MD5", true)->value().c_str())) {
                        return request->send(400, "text/plain", "MD5 parameter invalid");
                    }

                    #if defined(ESP8266)
                        int cmd = (filename == "filesystem") ? U_FS : U_FLASH;
                        Update.runAsync(true);
                        size_t fsSize = ((size_t) &_FS_end - (size_t) &_FS_start);
                        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
                        if (!Update.begin((cmd == U_FS)?fsSize:maxSketchSpace, cmd)){ // Start with max available size
                    #elif defined(ESP32)
                        int cmd = (filename == "filesystem") ? U_SPIFFS : U_FLASH;
                        if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) { // Start with max available size
                    #endif
                        Update.printError(Serial);
                        return request->send(400, "text/plain", "OTA could not begin");
                    }
                }

                // Write chunked data to the free sketch space
                if(len){
                    if (Update.write(data, len) != len) {
                        return request->send(400, "text/plain", "OTA could not begin");
                    }
                }
                    
                if (final) { // if the final flag is set then this is the last frame of data
                    if (!Update.end(true)) { //true to set the size to the current progress
                        Update.printError(Serial);
                        return request->send(400, "text/plain", "Could not end OTA");
                    }
                }else{
                    return;
                }
            });
        }

        // deprecated, keeping for backward compatibility
        void loop() {
        }
        
        void restart() {
            yield();
            delay(1000);
            yield();
            ESP.restart();
        }

    private:
        AsyncWebServer *_server;

        String getID(){
            String id = "";
            #if defined(ESP8266)
                id = String(ESP.getChipId());
            #elif defined(ESP32)
                id = String((uint32_t)ESP.getEfuseMac(), HEX);
            #endif
            id.toUpperCase();
            return id;
        }

        String _id = getID();
        String _username = "";
        String _password = "";
        bool _authRequired = false;

};

AsyncElegantOtaClass AsyncElegantOTA;
#endif
