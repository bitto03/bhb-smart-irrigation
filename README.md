# ESP32 Firebase Soil Moisture Monitor & Relay Controller

This project uses an **ESP32** board to read **soil moisture** data and control a **relay** based on input from a **Firebase Realtime Database**. It also displays soil moisture and relay status on an **I2C LCD**.

##  Features

* Connects to WiFi
*  Sends real-time soil moisture data to Firebase
*  Reads relay state from Firebase to control devices like a water pump
*  Displays current soil moisture and relay state on a 16x2 LCD
*  Relay is active-low (LOW = ON, HIGH = OFF)

##  Hardware Required

| Component                          | Quantity  |
| ---------------------------------- | --------- |
| ESP32 Board                        | 1         |
| Soil Moisture Sensor               | 1         |
| 4-Channel Relay Module (or Single) | 1         |
| 16x2 I2C LCD Display               | 1         |
| Jumper Wires                       | As needed |
| Breadboard (optional)              | 1         |


##  Pin Configuration

| Component            | ESP32 Pin        |
| -------------------- | ---------------- |
| Relay IN             | GPIO 19          |
| Soil Moisture Output | GPIO 34 (Analog) |
| I2C LCD SDA          | GPIO 21          |
| I2C LCD SCL          | GPIO 22          |


##  How It Works

1. **WiFi Setup**: ESP32 connects to your home WiFi.
2. **Soil Moisture Reading**: Reads analog value (0-4095) and converts it to a percentage.
3. **Firebase Sync**:

   * Sends soil moisture (raw and percent) to `/sensor/soil` and `/sensor/soil_percent`.
   * Reads relay state from `/led/state` (0 = OFF, 1 = ON).
4. **Relay Control**: Based on Firebase value, turns the relay ON/OFF.
5. **LCD Output**: Updates every 2 seconds with soil data and relay status.


##  Firebase Setup

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Create a new project.
3. Go to **Project Settings > Service Accounts > Database Secrets** (for legacy token).
4. Enable **Realtime Database**, set rules to public for testing:

   ```json
   {
     "rules": {
       ".read": true,
       ".write": true
     }
   }

5. Get:

   * **Database URL** (e.g. `your-project-id.firebaseio.com`)
   * **Legacy token** (for authentication)



##  Configuration

Update the following in the code:

cpp
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"

#define FIREBASE_HOST "your-project-id.firebaseio.com"
#define FIREBASE_AUTH "your_firebase_legacy_token"




## Firebase Data Structure

```json
{
  "sensor": {
    "soil": 3071,
    "soil_percent": 34
  },
  "led": {
    "state": 1
  }
}
```

##  Example LCD Output
Soil: 34%
Relay: ON

ESP32 | Firebase | IoT Agriculture
