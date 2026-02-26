#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "Your ID"
#define BLYNK_TEMPLATE_NAME "LED ESP32Copy"
#define BLYNK_AUTH_TOKEN "YOUR_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "esp_task_wdt.h"

#define RELAY_PIN 26

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

BlynkTimer timer;

/* -------- WIFI + BLYNK KONTROL -------- */
void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi koptu, yeniden baglaniyor...");
    WiFi.disconnect(true);
    delay(100);
    WiFi.begin(ssid, pass);
  }

  if (!Blynk.connected()) {
    Serial.println("Blynk offline, yeniden baglaniyor...");
    Blynk.connect(3000);
  }
}

/* -------- BLYNK -------- */
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0);
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  digitalWrite(RELAY_PIN, value ? HIGH : LOW); // aktif-LOW röle
}

/* -------- SETUP -------- */
void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // açılışta KAPALI

  WiFi.begin(ssid, pass);
  Blynk.config(BLYNK_AUTH_TOKEN);

  timer.setInterval(10000L, checkConnection);

  // 🔥 Watchdog sadece task'a ekleniyor
  esp_task_wdt_add(NULL);

  Serial.println("ESP32 basladi");
}

/* -------- LOOP -------- */
void loop() {
  esp_task_wdt_reset();   // kilitlenirse reset
  Blynk.run();
  timer.run();
}
