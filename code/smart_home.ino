#define BLYNK_TEMPLATE_ID "From Blynk App"
#define BLYNK_TEMPLATE_NAME "From Blynk App"
#define BLYNK_AUTH_TOKEN "From Blynk App"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define WIFI_SSID "WiFi Name"
#define WIFI_PASSWORD "WiFi Password"

#define Light1 D0
#define Fan1   D1
#define Light2 D2
#define Fan2   D3
#define SensorCrnt A0  

const float SENSOR_SENSITIVITY = 0.185;  
const float ADC_REF_VOLTAGE = 1.0;      
const float SENSOR_VCC = 5.0;           
const int ADC_MAX = 1023;                

float sensorZero = 512;  
const float MIN_CURRENT = 0.001;         
const float MAX_CURRENT = 2.0;

BlynkTimer timer;

float readCurrent() {
    int rawValue = analogRead(SensorCrnt);
    float voltage = ((rawValue / (float)ADC_MAX) * ADC_REF_VOLTAGE) * (SENSOR_VCC / ADC_REF_VOLTAGE);
    float current = (voltage - (sensorZero / ADC_MAX) * SENSOR_VCC) / SENSOR_SENSITIVITY;
    return abs(current);
}

void calibrateSensor() {
    long sum = 0;
    for (int i = 0; i < 100; i++) {
        sum += analogRead(SensorCrnt);
        delay(10);
    }
    sensorZero = sum / 100.0;
    Serial.println("Calibrated Zero Point: " + String(sensorZero));
}

void checkDeviceStatus() {   
    float current = readCurrent();
    bool malfunction = false;
    String reason = "Normal";

    if (current < MIN_CURRENT) {
        malfunction = true;
        reason = "Under-Current: Load too low.";
    } else if (current > MAX_CURRENT) {
        malfunction = true;
        reason = "Over-Current: Possible short circuit!";
    }
    
    Blynk.virtualWrite(V10, current);
    Blynk.virtualWrite(V11, malfunction ? "YES" : "NO");
    Blynk.virtualWrite(V12, reason);

    Serial.println("\nCurrent: " + String(current, 4) + " A");
    Serial.println(malfunction ? "⚠️ Malfunction Detected: " + reason : "✅ Device Normal");
    Serial.println();
}

void setup() {
    Serial.begin(9600);
    
    pinMode(Light1, OUTPUT);
    pinMode(Fan1, OUTPUT);
    pinMode(Light2, OUTPUT);
    pinMode(Fan2, OUTPUT);
    pinMode(SensorCrnt, INPUT);

    digitalWrite(Light1, HIGH);
    digitalWrite(Fan1, HIGH);
    digitalWrite(Light2, HIGH);
    digitalWrite(Fan2, HIGH);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected: " + WiFi.localIP().toString());
    } else {
        Serial.println("\nWiFi Failed. Running in Offline Mode.");
    }

    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
    
    calibrateSensor();
    timer.setInterval(5000L, checkDeviceStatus);
}

void loop() {
    Blynk.run();
    timer.run();
}

BLYNK_WRITE(V3) {
    digitalWrite(Light1, param.asInt() ? LOW : HIGH);
    Serial.println("Light1: " + String(param.asInt() ? "ON" : "OFF"));
}

BLYNK_WRITE(V4) {
    digitalWrite(Fan1, param.asInt() ? LOW : HIGH);
    Serial.println("Fan1: " + String(param.asInt() ? "ON" : "OFF"));
}

BLYNK_WRITE(V5) {
    digitalWrite(Light2, param.asInt() ? LOW : HIGH);
    Serial.println("Light2: " + String(param.asInt() ? "ON" : "OFF"));
}

BLYNK_WRITE(V6) {
    digitalWrite(Fan2, param.asInt() ? LOW : HIGH);
    Serial.println("Fan2: " + String(param.asInt() ? "ON" : "OFF"));
}
