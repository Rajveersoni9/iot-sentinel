/*
 * ESP32 Wi-Fi Security Auditor
 * 
 * Description:
 * This program scans for nearby Wi-Fi networks and evaluates their 
 * security posture based on the advertised encryption types. It identifies
 * unsecured (Open) networks and outdated protocols (like WEP) that pose 
 * a security risk.
 * 
 * Target: ESP32 (38-pin DevKit or compatible)
 * Connection: Serial Monitor at 115200 baud
 */

#include "WiFi.h"

// Define the scan interval (in milliseconds)
const unsigned long SCAN_INTERVAL = 15000; 
unsigned long lastScanTime = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n========================================");
  Serial.println("   ESP32 Wi-Fi Security Auditor Initialized");
  Serial.println("========================================");

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup completed. Beginning periodic scanning...");
}

void loop() {
  unsigned long currentMillis = millis();

  // Perform a scan periodically
  if (currentMillis - lastScanTime >= SCAN_INTERVAL || lastScanTime == 0) {
    lastScanTime = currentMillis;
    performWifiScan();
  }
}

void performWifiScan() {
  Serial.println("\nScanning for active wireless networks...");
  
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  
  Serial.println("Scan complete.");
  if (n == 0) {
    Serial.println("No networks found.");
  } else {
    Serial.print(n);
    Serial.println(" networks found:");
    Serial.println("------------------------------------------------------------------");
    Serial.printf("%-3s | %-32s | %-4s | %-10s | %-12s\n", "Num", "SSID", "RSSI", "Security", "Risk Level");
    Serial.println("------------------------------------------------------------------");
    
    for (int i = 0; i < n; ++i) {
      String ssid = WiFi.SSID(i);
      int32_t rssi = WiFi.RSSI(i);
      wifi_auth_mode_t authMode = WiFi.encryptionType(i);
      
      String securityType = getAuthModeName(authMode);
      String riskLevel = evaluateRiskLevel(authMode);
      
      Serial.printf("%-3d | %-32.32s | %-4d | %-10s | %-12s\n", 
                    i + 1, 
                    ssid.c_str(), 
                    rssi, 
                    securityType.c_str(), 
                    riskLevel.c_str());
      delay(10);
    }
    Serial.println("------------------------------------------------------------------");
  }
}

// Convert auth mode enum to human-readable string
String getAuthModeName(wifi_auth_mode_t authMode) {
  switch (authMode) {
    case WIFI_AUTH_OPEN:
      return "Open";
    case WIFI_AUTH_WEP:
      return "WEP";
    case WIFI_AUTH_WPA_PSK:
      return "WPA-PSK";
    case WIFI_AUTH_WPA2_PSK:
      return "WPA2-PSK";
    case WIFI_AUTH_WPA_WPA2_PSK:
      return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE:
      return "WPA2-Ent";
    case WIFI_AUTH_WPA3_PSK:
      return "WPA3-PSK";
    case WIFI_AUTH_WPA2_WPA3_PSK:
      return "WPA2/WPA3";
    default:
      return "Unknown";
  }
}

// Simple heuristic to evaluate risk based on encryption type
String evaluateRiskLevel(wifi_auth_mode_t authMode) {
  switch (authMode) {
    case WIFI_AUTH_OPEN:
      return "HIGH RISK (No Encryption)";
    case WIFI_AUTH_WEP:
      return "HIGH RISK (Obsolete)";
    case WIFI_AUTH_WPA_PSK:
      return "MEDIUM RISK (Outdated)";
    case WIFI_AUTH_WPA2_PSK:
    case WIFI_AUTH_WPA_WPA2_PSK:
      return "LOW RISK (Standard)";
    case WIFI_AUTH_WPA2_ENTERPRISE:
    case WIFI_AUTH_WPA3_PSK:
    case WIFI_AUTH_WPA2_WPA3_PSK:
      return "MINIMAL RISK (Secure)";
    default:
      return "UNKNOWN";
  }
}
