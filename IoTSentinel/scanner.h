#ifndef SCANNER_H
#define SCANNER_H

#include <Arduino.h>
#include <vector>

// Scan State Machine States
enum ScanState {
  STATE_IDLE,
  STATE_STARTING,
  STATE_DISCOVERING,
  STATE_PORT_SCANNING,
  STATE_RISK_ANALYSIS,
  STATE_COMPLETED,
  STATE_STOPPED
};

struct ScannedDevice {
  String ip;
  String name;
  std::vector<int> open_ports;
  String risk;     // "LOW", "MEDIUM", "HIGH"
  int score;       // 0 - 100
  String type;     // "Router", "ESP32", "NAS", "IP Camera", "Smart TV", "Desktop/Laptop", "Unknown"
  unsigned long responseTime; // ms
};

class SubnetScanner {
public:
  SubnetScanner();
  
  // Controls
  void startScan();
  void stopScan();
  void clearScan();
  
  // Tick handler (call inside main loop)
  void handle();
  
  // Getters
  bool isScanning() const;
  ScanState getState() const;
  String getStateString() const;
  int getProgress() const;
  String getCurrentIP() const;
  String getCurrentPortString() const;
  
  // Stats
  unsigned long getScanDuration() const;
  int getHostsDiscoveredCount() const;
  int getRemainingHosts() const;
  float getScanSpeed() const; // hosts per second
  
  // Security Aggregates
  int getNetworkScore() const;
  String getNetworkGrade() const;
  void getRiskDistribution(int& low, int& med, int& high) const;
  
  // Data lists
  const std::vector<ScannedDevice>& getDevices() const;
  const std::vector<String>& getLogs() const;
  
  // Logger helper
  void addLog(String level, String logText);

private:
  ScanState _state;
  int _progress;
  int _currentHostIndex;
  size_t _scanDeviceIndex;
  
  IPAddress _subnetBase;
  std::vector<ScannedDevice> _discoveredDevices;
  std::vector<String> _logs;
  
  unsigned long _scanStartTime;
  unsigned long _scanEndTime;
  unsigned long _lastTickTime;
  
  String _currentHostIP;
  String _currentPortString;
  
  int _lowCount;
  int _mediumCount;
  int _highCount;
  int _networkScore;
  String _networkGrade;
  
  // Internal scan steps
  void processStarting();
  void processDiscovering();
  void processPortScanning();
  void processRiskAnalysis();
  
  bool pingHost(IPAddress targetIp, unsigned long& responseTimeMs);
  void fingerprintDevice(ScannedDevice& dev);
  void evaluateSecurityMetrics();
};

extern SubnetScanner subnetScanner;

#endif
