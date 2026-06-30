#include "sentinel_webserver.h"
#include "web_assets.h"
#include "scanner.h"
#include "wifi_manager.h"
#include "settings_manager.h"
#include <Preferences.h>

WebServerWrapper webServerWrapper;

WebServerWrapper::WebServerWrapper() : _server(80) {}

void WebServerWrapper::begin() {
  // Bind class member functions as WebServer callbacks
  _server.on("/", std::bind(&WebServerWrapper::handleRoot, this));
  _server.on("/style.css", std::bind(&WebServerWrapper::handleStyle, this));
  _server.on("/app.js", std::bind(&WebServerWrapper::handleAppJs, this));
  
  // REST API Endpoints
  _server.on("/api/status", HTTP_GET, std::bind(&WebServerWrapper::handleApiStatus, this));
  _server.on("/api/devices", HTTP_GET, std::bind(&WebServerWrapper::handleApiDevices, this));
  _server.on("/api/logs", HTTP_GET, std::bind(&WebServerWrapper::handleApiLogs, this));
  _server.on("/api/scan/start", HTTP_POST, std::bind(&WebServerWrapper::handleApiScanStart, this));
  _server.on("/api/scan/stop", HTTP_POST, std::bind(&WebServerWrapper::handleApiScanStop, this));
  _server.on("/api/settings", HTTP_GET, std::bind(&WebServerWrapper::handleApiSettingsGet, this));
  _server.on("/api/settings", HTTP_POST, std::bind(&WebServerWrapper::handleApiSettingsPost, this));
  
  // Legacy / Fallback endpoints for backward-compatibility
  _server.on("/api/scan", HTTP_GET, std::bind(&WebServerWrapper::handleLegacyScan, this));
  _server.on("/api/start", HTTP_POST, std::bind(&WebServerWrapper::handleLegacyStart, this));
  _server.on("/api/clear", HTTP_POST, std::bind(&WebServerWrapper::handleLegacyClear, this));
  
  // Custom routing fallback for Captive Portal redirects
  _server.onNotFound(std::bind(&WebServerWrapper::handleNotFound, this));
  
  _server.begin();
  Serial.println("HTTP Web Server running on Port 80.");
}

void WebServerWrapper::handle() {
  _server.handleClient();
}

void WebServerWrapper::handleRoot() {
  if (checkCaptivePortalRedirect()) {
    return;
  }
  // Send dashboard index.html from PROGMEM
  _server.send_P(200, "text/html", INDEX_HTML);
}

void WebServerWrapper::handleStyle() {
  _server.send_P(200, "text/css", STYLE_CSS);
}

void WebServerWrapper::handleAppJs() {
  _server.send_P(200, "application/javascript", APP_JS);
}

void WebServerWrapper::setCorsHeaders() {
  _server.sendHeader("Access-Control-Allow-Origin", "*");
  _server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  _server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void WebServerWrapper::handleApiStatus() {
  setCorsHeaders();
  
  int low, med, high;
  subnetScanner.getRiskDistribution(low, med, high);
  
  String json = "{";
  json += "\"status\":\"" + subnetScanner.getStateString() + "\",";
  json += "\"scanning\":" + String(subnetScanner.isScanning() ? "true" : "false") + ",";
  json += "\"progress\":" + String(subnetScanner.getProgress()) + ",";
  json += "\"current_ip\":\"" + subnetScanner.getCurrentIP() + "\",";
  json += "\"current_port\":\"" + subnetScanner.getCurrentPortString() + "\",";
  json += "\"sys_ip\":\"" + wifiManager.getLocalIP() + "\",";
  json += "\"sys_ssid\":\"" + wifiManager.getSSID() + "\",";
  json += "\"sys_free_ram\":" + String(ESP.getFreeHeap()) + ",";
  json += "\"hosts_found\":" + String(subnetScanner.getHostsDiscoveredCount()) + ",";
  json += "\"scan_duration\":" + String(subnetScanner.getScanDuration()) + ",";
  json += "\"scan_speed\":" + String(subnetScanner.getScanSpeed(), 2) + ",";
  json += "\"remaining_hosts\":" + String(subnetScanner.getRemainingHosts()) + ",";
  json += "\"net_score\":" + String(subnetScanner.getNetworkScore()) + ",";
  json += "\"net_grade\":\"" + subnetScanner.getNetworkGrade() + "\",";
  json += "\"risk_distribution\":{";
  json += "\"low\":" + String(low) + ",";
  json += "\"medium\":" + String(med) + ",";
  json += "\"high\":" + String(high);
  json += "}";
  json += "}";
  
  _server.send(200, "application/json", json);
}

void WebServerWrapper::handleApiDevices() {
  setCorsHeaders();
  
  String json = "[";
  const auto& devices = subnetScanner.getDevices();
  for (size_t i = 0; i < devices.size(); i++) {
    if (i > 0) json += ",";
    json += "{";
    json += "\"ip\":\"" + devices[i].ip + "\",";
    json += "\"name\":\"" + devices[i].name + "\",";
    json += "\"risk\":\"" + devices[i].risk + "\",";
    json += "\"score\":" + String(devices[i].score) + ",";
    json += "\"type\":\"" + devices[i].type + "\",";
    json += "\"rtt\":" + String(devices[i].responseTime) + ",";
    json += "\"open_ports\":[";
    const auto& ports = devices[i].open_ports;
    for (size_t p = 0; p < ports.size(); p++) {
      if (p > 0) json += ",";
      json += String(ports[p]);
    }
    json += "]";
    json += "}";
  }
  json += "]";
  
  _server.send(200, "application/json", json);
}

void WebServerWrapper::handleApiLogs() {
  setCorsHeaders();
  
  String json = "[";
  const auto& logs = subnetScanner.getLogs();
  for (size_t i = 0; i < logs.size(); i++) {
    if (i > 0) json += ",";
    String escapedLog = logs[i];
    escapedLog.replace("\"", "\\\"");
    json += "\"" + escapedLog + "\"";
  }
  json += "]";
  
  _server.send(200, "application/json", json);
}

void WebServerWrapper::handleApiScanStart() {
  setCorsHeaders();
  subnetScanner.startScan();
  _server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void WebServerWrapper::handleApiScanStop() {
  setCorsHeaders();
  subnetScanner.stopScan();
  _server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void WebServerWrapper::handleApiSettingsGet() {
  setCorsHeaders();
  
  String json = "{";
  json += "\"timeout\":" + String(settingsManager.scanTimeout) + ",";
  json += "\"delay\":" + String(settingsManager.scanDelay) + ",";
  json += "\"ports\":" + String(settingsManager.enabledPorts) + ",";
  json += "\"theme\":\"" + settingsManager.theme + "\",";
  json += "\"refresh\":" + String(settingsManager.autoRefresh ? "true" : "false") + ",";
  json += "\"maxdev\":" + String(settingsManager.maxDevices);
  json += "}";
  
  _server.send(200, "application/json", json);
}

void WebServerWrapper::handleApiSettingsPost() {
  setCorsHeaders();
  
  if (_server.hasArg("timeout")) {
    settingsManager.scanTimeout = _server.arg("timeout").toInt();
  }
  if (_server.hasArg("delay")) {
    settingsManager.scanDelay = _server.arg("delay").toInt();
  }
  if (_server.hasArg("ports")) {
    settingsManager.enabledPorts = (uint8_t)_server.arg("ports").toInt();
  }
  if (_server.hasArg("theme")) {
    settingsManager.theme = _server.arg("theme");
  }
  if (_server.hasArg("refresh")) {
    settingsManager.autoRefresh = (_server.arg("refresh") == "true" || _server.arg("refresh") == "1");
  }
  if (_server.hasArg("maxdev")) {
    settingsManager.maxDevices = _server.arg("maxdev").toInt();
  }
  
  // Allow configuring Wi-Fi SSID / password if submitted
  if (_server.hasArg("ssid") && _server.arg("ssid").length() > 0) {
    String newSSID = _server.arg("ssid");
    String newPass = _server.arg("password");
    
    Preferences wifiPrefs;
    wifiPrefs.begin("wifi-config", false);
    wifiPrefs.putString("ssid", newSSID);
    wifiPrefs.putString("password", newPass);
    wifiPrefs.end();
    
    Serial.println("[SETTINGS] Saved new WiFi credentials. Will connect on reconnect/reboot.");
  }
  
  settingsManager.save();
  _server.send(200, "application/json", "{\"status\":\"ok\"}");
}

// -------------------------------------------------------------
// Legacy API endpoints
// -------------------------------------------------------------
void WebServerWrapper::handleLegacyScan() {
  setCorsHeaders();
  
  // Merged legacy JSON layout for backward compatibility
  int low, med, high;
  subnetScanner.getRiskDistribution(low, med, high);
  
  String json = "{";
  json += "\"scanning\":" + String(subnetScanner.isScanning() ? "true" : "false") + ",";
  json += "\"progress\":" + String(subnetScanner.getProgress()) + ",";
  json += "\"current_ip\":\"" + subnetScanner.getCurrentIP() + "\",";
  json += "\"sys_ip\":\"" + wifiManager.getLocalIP() + "\",";
  json += "\"sys_ssid\":\"" + wifiManager.getSSID() + "\",";
  json += "\"sys_free_ram\":" + String(ESP.getFreeHeap()) + ",";
  
  // Devices
  json += "\"devices\":[";
  const auto& devices = subnetScanner.getDevices();
  for (size_t i = 0; i < devices.size(); i++) {
    if (i > 0) json += ",";
    json += "{";
    json += "\"ip\":\"" + devices[i].ip + "\",";
    json += "\"name\":\"" + devices[i].name + "\",";
    json += "\"risk\":\"" + devices[i].risk + "\",";
    json += "\"score\":" + String(devices[i].score) + ",";
    json += "\"open_ports\":[";
    const auto& ports = devices[i].open_ports;
    for (size_t p = 0; p < ports.size(); p++) {
      if (p > 0) json += ",";
      json += String(ports[p]);
    }
    json += "]";
    json += "}";
  }
  json += "],";
  
  // Logs
  json += "\"logs\":[";
  const auto& logs = subnetScanner.getLogs();
  for (size_t i = 0; i < logs.size(); i++) {
    if (i > 0) json += ",";
    String escapedLog = logs[i];
    escapedLog.replace("\"", "\\\"");
    json += "\"" + escapedLog + "\"";
  }
  json += "]";
  json += "}";
  
  _server.send(200, "application/json", json);
}

void WebServerWrapper::handleLegacyStart() {
  handleApiScanStart();
}

void WebServerWrapper::handleLegacyClear() {
  setCorsHeaders();
  subnetScanner.clearScan();
  _server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void WebServerWrapper::handleNotFound() {
  if (checkCaptivePortalRedirect()) {
    return;
  }
  
  // Redirect unknown URLs to index page
  _server.sendHeader("Location", "/", true);
  _server.send(302, "text/plain", "");
}

bool WebServerWrapper::checkCaptivePortalRedirect() {
  if (wifiManager.isAPMode()) {
    String host = _server.hostHeader();
    // Redirect requests not originating from local IP address or local mDNS alias
    if (host != "192.168.4.1" && host != "security.local" && host != "security") {
      _server.sendHeader("Location", "http://192.168.4.1/", true);
      _server.send(302, "text/plain", "");
      return true;
    }
  }
  return false;
}
