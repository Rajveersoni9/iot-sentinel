#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

// Struct to store WiFi configurations
struct WiFiConfig {
  char ssid[33];
  char password[65];
};

class WiFiManager {
public:
  WiFiManager();
  
  // Initializes WiFi. Attempts connection first, falls back to AP if timeout.
  void begin();
  
  // Handles AP captive portal DNS & client handling (call in loop)
  void handle();
  
  // Getters
  bool isConnected() const;
  String getLocalIP() const;
  String getSSID() const;
  bool isAPMode() const;

private:
  bool _isConnected;
  bool _isAPMode;
  String _activeSSID;
  unsigned long _lastReconnectAttempt;
  
  void startAP();
  void tryConnect();
};

extern WiFiManager wifiManager;

#endif
