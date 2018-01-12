#define DEBUG 1
#include <Basecamp.hpp>

Basecamp iot;

static const int SensorPin = 33;
static const int BatteryPin = 34;
int sensorValue = 0;
int batteryLimit = 3300;
bool delaySleep = false;

uint16_t statusPacketIdSub = 0;
String delaySleepTopic;
String statusTopic;
String batteryTopic;
String batteryValueTopic;

void setup() {
  pinMode(SensorPin, INPUT_PULLDOWN);
  pinMode(BatteryPin, INPUT);
  sensorValue = digitalRead(SensorPin);
  iot.begin();
  delaySleepTopic = "cmd/" + iot.hostname + "/delaysleep";
  statusTopic = "stat/" + iot.hostname + "/status";
  batteryTopic = "stat/" + iot.hostname + "/battery";
  batteryValueTopic = "stat/" + iot.hostname + "/batteryvalue";

  iot.mqtt.onConnect(transmitStatus);
  iot.mqtt.onPublish(suspendESP);

}

void transmitStatus(bool sessionPresent) {
  iot.mqtt.subscribe(delaySleepTopic.c_str(), 1);
  Serial.println("Transmit");
  
  Serial.println(statusTopic);
  if (sensorValue == 0) {

    DEBUG_PRINTLN("Door open");
    statusPacketIdSub = iot.mqtt.publish(statusTopic.c_str(), 1, true, "open" );
    esp_sleep_enable_ext0_wakeup((gpio_num_t)SensorPin, 1);

  } else {

    statusPacketIdSub = iot.mqtt.publish(statusTopic.c_str(), 1, true, "closed" );
    DEBUG_PRINTLN("Door closed");
    esp_sleep_enable_ext0_wakeup((gpio_num_t)SensorPin, 0);

  }

  sensorValue = analogRead(BatteryPin);
  char sensorC[6];
  sprintf(sensorC, "%04i", sensorValue);
  iot.mqtt.publish(batteryValueTopic.c_str(), 1, true, sensorC);
  if (sensorValue < batteryLimit) {

    DEBUG_PRINTLN("Battery empty");
    iot.mqtt.publish(batteryTopic.c_str(), 1, true, "empty" );

  } else {

    DEBUG_PRINTLN("Battery full");
    iot.mqtt.publish(batteryTopic.c_str(), 1, true, "full" );

  }
  DEBUG_PRINTLN("Data published");
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println(topic);
    Serial.println(payload);
  if (topic == delaySleepTopic.c_str()) {

    if (payload == "true")  {
      delaySleep = true;
    } else  {
      delaySleep = false;
      suspendESP(statusPacketIdSub);
    }
  }
}

void suspendESP(uint16_t packetId) {
  delay(150);
 

  if (packetId == statusPacketIdSub) {
    if (delaySleep == true) {
      Serial.println("Delaying Sleep");
      return;
    }
    DEBUG_PRINTLN("Entering deep sleep");
    iot.mqtt.disconnect();
    esp_deep_sleep_start();

  }
}

void loop() {

}
