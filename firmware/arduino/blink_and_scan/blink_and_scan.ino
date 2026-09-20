#include <Arduino.h>
#include <WiFi.h>

namespace {

constexpr gpio_num_t kLedPin = GPIO_NUM_21;
constexpr uint32_t kSerialBaud = 115200;
constexpr uint32_t kBlinkIntervalMs = 500;
constexpr uint32_t kScanIntervalMs = 15000;

bool ledState = false;
bool scanInProgress = false;
uint32_t lastBlinkMs = 0;
uint32_t lastScanStartMs = 0;

void serviceLed() {
  const uint32_t now = millis();
  if (now - lastBlinkMs < kBlinkIntervalMs) {
    return;
  }

  lastBlinkMs = now;
  ledState = !ledState;
  digitalWrite(static_cast<uint8_t>(kLedPin), ledState ? HIGH : LOW);
}

void printScanResults(int networkCount) {
  Serial.println();

  if (networkCount <= 0) {
    Serial.println("[WiFi] Scan complete: no networks found.");
    return;
  }

  Serial.printf("[WiFi] Scan complete: %d network(s)\n", networkCount);
  Serial.println("  # | RSSI      | CH | Security | SSID");
  Serial.println("----+-----------+----+----------+------------------------------");

  for (int i = 0; i < networkCount; ++i) {
    const char *security = "open";
    if (WiFi.encryptionType(i) != WIFI_AUTH_OPEN) {
      security = "secured";
    }

    Serial.printf(
        "%3d | %4d dBm | %2d | %-8s | %s\n",
        i + 1,
        WiFi.RSSI(i),
        WiFi.channel(i),
        security,
        WiFi.SSID(i).c_str());
  }
}

void startScan() {
  WiFi.scanDelete();

  // async=true, show_hidden=true.
  const int result = WiFi.scanNetworks(true, true);
  if (result == WIFI_SCAN_FAILED) {
    Serial.println("[WiFi] Failed to start scan.");
    scanInProgress = false;
    return;
  }

  scanInProgress = true;
  lastScanStartMs = millis();
  Serial.println("[WiFi] 2.4 GHz scan started...");
}

void serviceWifiScan() {
  if (!scanInProgress) {
    if (millis() - lastScanStartMs >= kScanIntervalMs) {
      startScan();
    }
    return;
  }

  const int result = WiFi.scanComplete();
  if (result == WIFI_SCAN_RUNNING) {
    return;
  }

  if (result == WIFI_SCAN_FAILED) {
    Serial.println("[WiFi] Scan failed.");
  } else {
    printScanResults(result);
  }

  WiFi.scanDelete();
  scanInProgress = false;
  lastScanStartMs = millis();
}

}  // namespace

void setup() {
  pinMode(static_cast<uint8_t>(kLedPin), OUTPUT);
  digitalWrite(static_cast<uint8_t>(kLedPin), LOW);

  // On ESP32-S3, enable "USB CDC On Boot" in the board configuration so
  // Serial is carried by the native USB connector.
  Serial.begin(kSerialBaud);

  const uint32_t serialWaitStart = millis();
  while (!Serial && millis() - serialWaitStart < 3000) {
    serviceLed();
    delay(10);
  }

  Serial.println();
  Serial.println("ESP32-S3 v3 board verification");
  Serial.println("Native USB CDC serial online.");
  Serial.println("GPIO21 status LED enabled.");

  WiFi.mode(WIFI_STA);

  // Stay disconnected from an AP; this program only verifies reception.
  WiFi.disconnect(false, false);
  delay(100);

  Serial.print("Wi-Fi STA mode enabled. Station MAC: ");
  Serial.println(WiFi.macAddress());

  // Start immediately; later scans run every kScanIntervalMs after completion.
  lastScanStartMs = millis() - kScanIntervalMs;
}

void loop() {
  serviceLed();
  serviceWifiScan();
  delay(1);
}
