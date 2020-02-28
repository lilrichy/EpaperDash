/*
 Name:		StockTicker.ino
 Created:	2/27/2020 8:23:01 PM
 Author:	richa
*/

#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "LiLRichyNetz";
const char* password = "Monster1984";

String btcUrl = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd&include_last_updated_at=true";

String payload;

void setup()
{
	//WiFi Connecting Begin.
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	delay(10);

	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	//WiFi Connected.
}

void loop() {

	Serial.println("Attempting to connecto to:");
	Serial.println(btcUrl);

	// wait for WiFi connection
	if (WiFi.status() == WL_CONNECTED)
	{
		std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

		//client->setFingerprint(fingerprint);
		client->setInsecure();

		HTTPClient https;

		if (https.begin(*client, btcUrl)) {  // HTTPS
			Serial.println("[HTTPS] GET...");
			int httpCode = https.GET();

			if (httpCode > 0)
			{
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTP] GET... code: %d\n", httpCode);

				// file found at server
				//if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
				payload = https.getString();
				Serial.println("Json: ");
				Serial.println(payload);

				const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 40;
				DynamicJsonDocument doc(capacity);

				deserializeJson(doc, payload);

				float bitcoin_usd = doc["bitcoin"]["usd"]; // 8656.78
				long bitcoin_last_updated_at = doc["bitcoin"]["last_updated_at"]; // 1582908892

				Serial.println("Json Result: ");
				Serial.println("BTC: ");
				Serial.println(bitcoin_usd);
				Serial.println("Last updated: ");
				Serial.println(bitcoin_last_updated_at);

			}
			else {
				Serial.printf("[HTTP] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}

			https.end();
		}
		else {
			Serial.printf("[HTTP} Unable to connect\n");
		}
	}

delay(60000);

}