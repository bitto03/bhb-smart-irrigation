#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi credentials
#define WIFI_SSID "Use yours wifi ssid"
#define WIFI_PASSWORD "wifi pass"

// Firebase configuration
#define FIREBASE_HOST "firebaseio.com" //use your firebase host 
#define FIREBASE_AUTH "***********************************" // use your firebase auth

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Pin definitions
const int relayPin = 19;             // Relay control pin
const int soilMoisturePin = 34;      // Analog pin for soil sensor

// LCD initialization (address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nWiFi Connected!");

  // Configure Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Pin modes
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Relay OFF initially (active LOW)

  // LCD setup
  lcd.begin(16, 2);            // Initialize 16x2 LCD
  lcd.backlight();             // Turn on the LCD backlight
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  Serial.println("Setup complete.");
}

void loop() {
  // 1️⃣ Read soil moisture
  int moistureValue = analogRead(soilMoisturePin);  // 0 (wet) to 4095 (dry)
  int moisturePercent = map(moistureValue, 4095, 0, 0, 100); // Adjust if needed

  Serial.println("Soil Value: " + String(moistureValue));
  Serial.println("Soil %   : " + String(moisturePercent) + "%");

  //  Send soil moisture to Firebase
  if (Firebase.setInt(fbdo, "/sensor/soil", moistureValue)) {
    Serial.println(" Soil raw value sent");
  } else {
    Serial.println(" Error sending soil: " + fbdo.errorReason());
  }

  Firebase.setInt(fbdo, "/sensor/soil_percent", moisturePercent);

  // Read relay state from Firebase
  if (Firebase.getInt(fbdo, "/led/state")) {
    int state = fbdo.intData();
    digitalWrite(relayPin, state ? LOW : HIGH);  // LOW = ON, HIGH = OFF
    Serial.println(" Relay state set to: " + String(state));

    //  Update the LCD with soil moisture and relay state
    lcd.clear();                          // Clear previous data
    lcd.setCursor(0, 0);                  // Set cursor to first line
    lcd.print("Soil: " + String(moisturePercent) + "%");  // Display soil moisture percentage
    lcd.setCursor(0, 1);                  // Set cursor to second line
    lcd.print("Relay: " + String(state ? "ON" : "OFF"));  // Display relay state (ON/OFF)
  } else {
    Serial.println(" Failed to read /led/state: " + fbdo.errorReason());
  }

  delay(2000);  // Wait 2 seconds
}