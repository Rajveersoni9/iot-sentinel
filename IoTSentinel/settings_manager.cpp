#include "settings_manager.h"
#include <Preferences.h>
#include <vector>

SettingsManager settingsManager;

// Core audited security ports array
static const int SYSTEM_PORTS[] = {21, 22, 23, 53, 80, 443, 1883, 8080};
static const int NUM_PORTS = 8;

SettingsManager::SettingsManager() 
  : scanTimeout(120), scanDelay(0), enabledPorts(0xFF), theme("dark"), autoRefresh(true), maxDevices(50) {}

void SettingsManager::begin() {
  Preferences prefs;
  prefs.begin("sentinel-conf", false);
  
  scanTimeout = prefs.getInt("timeout", 120);
  scanDelay = prefs.getInt("delay", 0);
  enabledPorts = prefs.getUChar("ports", 0xFF);
  theme = prefs.getString("theme", "dark");
  autoRefresh = prefs.getBool("refresh", true);
  maxDevices = prefs.getInt("maxdev", 50);
  
  prefs.end();
  
  Serial.println("[SETTINGS] Loaded configuration parameters from NVS successfully.");
}

void SettingsManager::save() {
  Preferences prefs;
  prefs.begin("sentinel-conf", false);
  
  prefs.putInt("timeout", scanTimeout);
  prefs.putInt("delay", scanDelay);
  prefs.putUChar("ports", enabledPorts);
  prefs.putString("theme", theme);
  prefs.putBool("refresh", autoRefresh);
  prefs.putInt("maxdev", maxDevices);
  
  prefs.end();
  
  Serial.println("[SETTINGS] Saved configuration parameters to NVS successfully.");
}

bool SettingsManager::isPortEnabled(int port) const {
  for (int i = 0; i < NUM_PORTS; i++) {
    if (SYSTEM_PORTS[i] == port) {
      return (enabledPorts & (1 << i)) != 0;
    }
  }
  return false;
}

std::vector<int> SettingsManager::getEnabledPortsList() const {
  std::vector<int> list;
  for (int i = 0; i < NUM_PORTS; i++) {
    if ((enabledPorts & (1 << i)) != 0) {
      list.push_back(SYSTEM_PORTS[i]);
    }
  }
  return list;
}
