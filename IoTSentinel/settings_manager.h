#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <Arduino.h>

class SettingsManager {
public:
  SettingsManager();
  
  // Load settings from NVS
  void begin();
  
  // Save settings to NVS
  void save();
  
  // Current settings parameters
  int scanTimeout;      // Port scan timeout in ms (e.g. 50-500ms)
  int scanDelay;        // Delay between scanned hosts in ms (e.g. 0-250ms)
  uint8_t enabledPorts; // Bitmask of enabled ports (bit 0 = port 21, bit 1 = port 22, etc.)
  String theme;         // UI theme: "dark" or "light"
  bool autoRefresh;     // Auto refresh polling on frontend
  int maxDevices;       // Max hosts limit in subnet scan to save RAM

  // Helper methods
  bool isPortEnabled(int port) const;
  std::vector<int> getEnabledPortsList() const;
};

extern SettingsManager settingsManager;

#endif
