#include "wifi_manager.h"
#include <Preferences.h>
#include <DNSServer.h>
#include <ESPmDNS.h>

Preferences preferences;
DNSServer dnsServer;
const byte DNS_PORT = 53;

WiFiManager wifiManager;

WiFiManager::WiFiManager() : _isConnected(false), _isAPMode(false), _activeSSID(""), _lastReconnectAttempt(0) {}

void WiFiManager::begin() {
  preferences.begin("wifi-config", false);
  
  // Read saved SSID/Password, fall back to default user-supplied credentials
  String savedSSID = preferences.getString("ssid", "Aditya");
  String savedPass = preferences.getString("password", "Harekrishna");
  
  Serial.print("Connecting to Wi-Fi SSID: ");
  Serial.println(savedSSID);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(savedSSID.c_str(), savedPass.c_str());
  
  unsigned long startAttempt = millis();
  // 15 seconds timeout
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    _isConnected = true;
    _isAPMode = false;
    _activeSSID = savedSSID;
    Serial.println("Wi-Fi connected successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    // Initialize mDNS responder for plug-and-run access
    if (MDNS.begin("security")) {
      Serial.println("mDNS responder started: http://security.local");
      MDNS.addService("http", "tcp", 80);
    } else {
      Serial.println("Error setting up mDNS responder!");
    }
  } else {
    Serial.println("Failed to connect to local Wi-Fi router. Swapping to Access Point mode...");
    startAP();
  }
}

void WiFiManager::startAP() {
  _isConnected = false;
  _isAPMode = true;
  _activeSSID = "IoT_Sentinel_Config";
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(_activeSSID.c_str());
  
  // Set up Captive DNS server to redirect all traffic to 192.168.4.1
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", IPAddress(192, 168, 4, 1));
  
  Serial.println("Captive configuration Access Point active.");
  Serial.print("SSID: ");
  Serial.println(_activeSSID);
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void WiFiManager::handle() {
  if (_isAPMode) {
    dnsServer.processNextRequest();
  } else {
    // If not connected, retry connecting non-blockingly every 10 seconds
    if (WiFi.status() != WL_CONNECTED) {
      _isConnected = false;
      unsigned long now = millis();
      if (now - _lastReconnectAttempt > 10000) {
        _lastReconnectAttempt = now;
        Serial.println("[WIFI] Connection lost. Attempting reconnection...");
        WiFi.disconnect();
        // Retrieve SSID and password from NVS just in case they were updated
        String savedSSID = preferences.getString("ssid", "Aditya");
        String savedPass = preferences.getString("password", "Harekrishna");
        WiFi.begin(savedSSID.c_str(), savedPass.c_str());
      }
    } else {
      if (!_isConnected) {
        _isConnected = true;
        Serial.print("[WIFI] Reconnected successfully! IP: ");
        Serial.println(WiFi.localIP());
        // Re-setup mDNS responder
        MDNS.begin("security");
      }
    }
  }
}

bool WiFiManager::isConnected() const {
  return _isConnected || _isAPMode;
}

String WiFiManager::getLocalIP() const {
  if (_isAPMode) {
    return WiFi.softAPIP().toString();
  }
  return WiFi.localIP().toString();
}

String WiFiManager::getSSID() const {
  return _activeSSID;
}

bool WiFiManager::isAPMode() const {
  return _isAPMode;
}
