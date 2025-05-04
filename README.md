Smart Home Automation with Malfunction Detection

This project implements a basic smart home automation system using Blynk, ESP8266, ACS712 Current Sensor, and a 4-Channel Relay, with malfunction detection features.

📦 Components Used

- ESP8266 NodeMCU
- Arduino Uno (used only for 5V power supply)
- ACS712 Current Sensor
- 4-Channel Relay Module
- Blynk App (Legacy or Blynk IoT)
- Electrical Appliances (simulated with LEDs or real devices)

⚙️ Features

- Control appliances via Blynk App
- Monitor real-time current usage
- Detect overcurrent/malfunction and alert user
- Detect in case of abnormal current


🧠 Logic Overview

- Blynk app sends ON/OFF signals to ESP8266
- ESP8266 activates the corresponding relay
- ACS712 measures current per channel
- If abnormal current detected (threshold defined),sends alert

📲 Blynk Setup

- Create a project in Blynk
- Add 4 buttons for controlling appliances
- Add a gauge or terminal for feedback
- Update the auth token, Wi-Fi SSID, and password in `smart_home.ino`

🔐 Safety Notes

> Always be cautious when working with high voltage appliances.

📁 Code

All code is in the `code/` folder.

