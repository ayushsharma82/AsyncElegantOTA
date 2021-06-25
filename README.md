<p align="center"><img src="/docs/logo.svg?sanitize=true&raw=true" width="400"></p>

<hr/>
<p align="center">
<img src="https://img.shields.io/github/last-commit/ayushsharma82/AsyncElegantOTA.svg?style=for-the-badge" />
&nbsp;
<img src="https://img.shields.io/travis/com/ayushsharma82/AsyncElegantOTA/master?style=for-the-badge" />
&nbsp;
<img src="https://img.shields.io/github/license/ayushsharma82/AsyncElegantOTA.svg?style=for-the-badge" />
&nbsp;
<a href="https://www.buymeacoffee.com/6QGVpSj" target="_blank"><img src="https://img.shields.io/badge/Buy%20me%20a%20coffee-%245-orange?style=for-the-badge&logo=buy-me-a-coffee" /></a>
</p>
<hr/>


<p align="center">Perform OTAs for ESP8266 & ESP32 Asynchronously</p>
<p align="center">
AsyncElegantOTA provides a beautiful interface to upload Over the Air `.bin` updates to your ESP Modules with precise status and progress displayed over UI. This Library shows the current upload progress of your OTA and once finished, it will display the status of your OTA. This Version of Library uses AsyncWebServer. Thanks to @me-no-dev for a wonderful webserver library.
</p>

<br>
<br>

<h2 align="center">Preview</h2>
<p align="center"><img src="/docs/elegantOtaDemo.gif?raw=true"></p>

<br>
<br>

<h2>How to Install</h2>

###### Directly Through Arduino IDE ( Currently Submitted for Approval. Use Mannual Install till it gets Approved.)
Go to Sketch > Include Library > Library Manager > Search for "AsyncElegantOTA" > Install

###### Manual Install

For Windows: Download the [Repository](https://github.com/ayushsharma82/AsyncElegantOTA/archive/master.zip) and extract the .zip in Documents>Arduino>Libraries>{Place "ElegantOTA" folder Here}

For Linux: Download the [Repository](https://github.com/ayushsharma82/AsyncElegantOTA/archive/master.zip) and extract the .zip in Sketchbook>Libraries>{Place "ElegantOTA" folder Here}

###### Manually through IDE

Download the [Repository](https://github.com/ayushsharma82/AsyncElegantOTA/archive/master.zip), Go to Sketch>Include Library>Add .zip Library> Select the Downloaded .zip File.

<br>

<h2>Documentation</h2>
<p>AsyncElegantOTA is a dead simple library which does your work in just 1 Line. Honestly, It's just a wrapper library which injects it's own elegant webpage instead of the ugly upload page which comes by default in Arduino Library.</p>

 Include AsyncElegantOTA Library `#include <AsyncElegantOTA.h>` at top of your Arduino Code.
 
 Paste this - `AsyncElegantOTA.begin(&server);`  line above your `server.begin();`
 
 That's all!
 
 Now copy the IPAddress displayed over your Serial Monitor and go to `http://<IPAddress>/update` in browser. ( where `<IPAddress>` is the IP of your ESP Module)
 
 <br>
 
 #### Additional Security:
 
 If you would like to add login to your OTA webpage, then please replace `AsyncElegantOTA.begin(&server);` with `AsyncElegantOTA.begin(&server, "username", "password");`. This will prevent unauthorized requests to your OTA webpage and prevent unauthorized firmware upload to your MCU.
 
<br>

<b>Antivirus Issue:</b> If you have an antivirus on your PC with internet security, the progress bar on webpage will instantly show 100% because of request caching by your antivirus software. There is no fix for this unless you want to disable your antivirus or whitelist your local IP addresses in it. ( Same is the case with iOS, safari will cache the outgoing requests )

<br>

<h2>Tutorials</h2>
<p>Tutorials for AsyncElegantOTA is live on RandomNerdTutorials.</p>

<h3>ESP8266</h3> 
<h4>Arduino IDE:</h4>
<p>https://randomnerdtutorials.com/esp8266-nodemcu-ota-over-the-air-arduino/</p>
<h4>PlatformIO:</h4>
<p>https://randomnerdtutorials.com/esp8266-nodemcu-ota-over-the-air-vs-code/</p>
<br>

<h3>ESP32</h3>
<h4>Arduino IDE:</h4>
<p>https://randomnerdtutorials.com/esp32-ota-over-the-air-arduino/</p>
<h4>PlatformIO:</h4>
<p>https://randomnerdtutorials.com/esp32-ota-over-the-air-vs-code/</p>

<br>
 
<br>
<h2>Examples</h2>
 
<h3>For ESP8266:</h3>

```
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char* ssid = "........";
const char* password = "........";

AsyncWebServer server(80);


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
    request->send(200, "text/plain", "Hi! I am ESP8266.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
}

```

<br>
<h3>For ESP32:</h3>

```
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char* ssid = "........";
const char* password = "........";

AsyncWebServer server(80);


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
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
}

```
<br>
<br>

<h2>Contributions</h2>
<p>Every Contribution to this repository is highly appriciated! Don't fear to create pull requests which enhance or fix the library as ultimatly you are going to help everybody.</p>
<p>
If you want to donate to the author then <b>you can buy me a coffee</b>, It really helps me keep these libraries updated:
<br/><br/>
<a href="https://www.buymeacoffee.com/6QGVpSj" target="_blank"><img src="https://img.shields.io/badge/Buy%20me%20a%20coffee-%245-orange?style=for-the-badge&logo=buy-me-a-coffee" /></a>
</p>
<br/>
<br/>


<h2>License</h2>
ESP-DASH is licensed under MIT.
<br/>
<br/>
<img src="https://img.shields.io/github/license/ayushsharma82/AsyncElegantOTA.svg?style=for-the-badge" />
</div>
