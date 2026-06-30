/*
 * IoT Sentinel - ESP32 Network Security Scanner
 * 
 * Description:
 * An ESP32-based network vulnerability scanner. Connects to local Wi-Fi, 
 * sweeps the subnet to discover live nodes, performs fast non-blocking port 
 * auditing, assigns threat risk levels, and hosts a premium dark cyber-themed 
 * web dashboard.
 * 
 * Target Hardware: ESP32 (38-pin DevKit or compatible)
 * Connection Settings: Serial Monitor at 115200 baud
 * Author: Advanced Agentic Coding
 */

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#include "wifi_manager.h"
#include "scanner.h"
#include "sentinel_webserver.h"
#include "settings_manager.h"

void setup() {
  // 1. Initialize Serial debugging
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n========================================================");
  Serial.println("         IoT Sentinel Security Auditor Initializing      ");
  Serial.println("========================================================");

  // 1.5 Load settings from NVS flash storage
  settingsManager.begin();

  // 2. Initialize Wi-Fi connection / fallback AP portal
  wifiManager.begin();
  
  // 3. Initialize HTTP Web Server on port 80
  webServerWrapper.begin();

  
  // 4. Initialize Scanner memory logs
  subnetScanner.clearScan();
  
  Serial.println("[SYSTEM] Startup completed successfully. Waiting for commands...");
  Serial.println("========================================================");
}

void loop() {
  // 1. Process DNS queries if active in AP mode (Captive Portal)
  wifiManager.handle();
  
  // 2. Handle incoming client HTTP requests
  webServerWrapper.handle();
  
  // 3. Tick active subnet scanner incremental sweeps
  subnetScanner.handle();
  
  // 4. Yield CPU control to prevent FreeRTOS watchdog timeouts
  delay(2);
}
