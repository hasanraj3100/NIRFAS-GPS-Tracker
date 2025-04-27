#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define WIFI_SSID "[ Client WIFI SSID should be here ]"
#define WIFI_PASSWORD "[ Client WIFI password should be here ]"
#define FIREBASE_URL "[ Endpoint URL of the database should be here ]"

#define GREEN_LED 19   // WiFi status LED
#define RED_LED 21     // GPS status LED 


String nmeaSentence = "";
float latestLatitude = 0.0;
float latestLongitude = 0.0;

unsigned long lastSentTime = 0;
const unsigned long sendInterval = 5000; // 5 seconds
 ;
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // GPS module on Serial2

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
      digitalWrite(GREEN_LED, LOW);
  }
  Serial.println("Connected to WiFi");
  digitalWrite(GREEN_LED, HIGH);
}

void loop() {
  // Read and parse GPS data
  while (Serial2.available()) {
    char c = Serial2.read();

    if (c == '\n') {
      parseGPGGA(nmeaSentence);
      nmeaSentence = "";
    } else if (c != '\r') {
      nmeaSentence += c;
    }
  }

  // Send data every 5 seconds if valid
  if (millis() - lastSentTime >= sendInterval && latestLatitude != 0.0 && latestLongitude != 0.0) {
    sendLocationToFirebase(latestLatitude, latestLongitude);
    lastSentTime = millis();
  }
   if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(GREEN_LED, LOW); 
   } else if(WiFi.status() == WL_CONNECTED) {
    digitalWrite(GREEN_LED, HIGH); 6
   }
}

float convertToDecimalDegrees(String rawCoord, String direction) {
  int degLength = (direction == "N" || direction == "S") ? 2 : 3;
  String degrees = rawCoord.substring(0, degLength);
  String minutes = rawCoord.substring(degLength);

  float deg = degrees.toFloat();
  float min = minutes.toFloat();
  float decimal = deg + (min / 60.0);

  if (direction == "S" || direction == "W") {
    decimal *= -1;
  }

  return decimal;
}

void parseGPGGA(String nmea) {
  if (!nmea.startsWith("$GPGGA")) return;

  int fieldIndex = 0;
  int lastComma = -1;
  String parts[15];

  for (int i = 0; i < nmea.length(); i++) {
    if (nmea.charAt(i) == ',') {
      parts[fieldIndex++] = nmea.substring(lastComma + 1, i);
      lastComma = i;
    }
  }
  parts[fieldIndex] = nmea.substring(lastComma + 1);

  String rawLat = parts[2];
  String latDir = parts[3];
  String rawLon = parts[4];
  String lonDir = parts[5];

  if (rawLat.length() > 0 && rawLon.length() > 0) {
    latestLatitude = convertToDecimalDegrees(rawLat, latDir);
    latestLongitude = convertToDecimalDegrees(rawLon, lonDir);

    digitalWrite(RED_LED, HIGH); // GPS fix acquired


    Serial.print("Latitude: ");
    Serial.println(latestLatitude, 6);
    Serial.print("Longitude: ");
    Serial.println(latestLongitude, 6);
  } else {
    digitalWrite(RED_LED, LOW); 
  }
}

void sendLocationToFirebase(float latitude, float longitude) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(FIREBASE_URL);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> jsonDoc;
    jsonDoc["device"] = "device 1";
    jsonDoc["latitude"] = latitude;
    jsonDoc["longitude"] = longitude;

    String jsonData;
    serializeJson(jsonDoc, jsonData);

    int httpResponseCode = http.PUT(jsonData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("Response: ");
    Serial.println(http.getString());

    http.end();
  } else {
    Serial.println("WiFi not connected. Cannot send data.");
    digitalWrite(GREEN_LED, LOW); // Turn off green LED if WiFi lost

  }
}
