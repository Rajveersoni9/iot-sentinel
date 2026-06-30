#ifndef SENTINEL_WEBSERVER_H
#define SENTINEL_WEBSERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

class WebServerWrapper {
public:
  WebServerWrapper();
  
  // Starts the HTTP Web Server on port 80
  void begin();
  
  // Processes incoming client connections (call in loop)
  void handle();

private:
  WebServer _server;
  
  // Static Assets
  void handleRoot();
  void handleStyle();
  void handleAppJs();
  
  // REST API Endpoints
  void handleApiStatus();      // GET /api/status
  void handleApiDevices();     // GET /api/devices
  void handleApiLogs();        // GET /api/logs
  void handleApiScanStart();   // POST /api/scan/start
  void handleApiScanStop();    // POST /api/scan/stop
  void handleApiSettingsGet(); // GET /api/settings
  void handleApiSettingsPost(); // POST /api/settings
  
  // Legacy / Helper endpoints
  void handleLegacyScan();     // GET /api/scan (for fallback)
  void handleLegacyStart();    // POST /api/start (for fallback)
  void handleLegacyClear();    // POST /api/clear (for fallback)
  
  void handleNotFound();
  
  // Captive Portal helper
  bool checkCaptivePortalRedirect();
  
  // Helper to send CORS headers
  void setCorsHeaders();
};

extern WebServerWrapper webServerWrapper;

#endif
