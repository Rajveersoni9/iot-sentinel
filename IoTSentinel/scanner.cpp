#include "scanner.h"
#include "port_scanner.h"
#include "risk_engine.h"
#include "wifi_manager.h"
#include "settings_manager.h"
#include <lwip/sockets.h>
#include <fcntl.h>

SubnetScanner subnetScanner;

SubnetScanner::SubnetScanner() 
  : _state(STATE_IDLE), _progress(0), _currentHostIndex(0), _scanDeviceIndex(0), 
    _scanStartTime(0), _scanEndTime(0), _lastTickTime(0), _currentHostIP(""), _currentPortString(""),
    _lowCount(0), _mediumCount(0), _highCount(0), _networkScore(100), _networkGrade("A") {
  _logs.reserve(60);
}

void SubnetScanner::startScan() {
  if (_state == STATE_STARTING || _state == STATE_DISCOVERING || _state == STATE_PORT_SCANNING || _state == STATE_RISK_ANALYSIS) {
    addLog("WARN", "Scan already in progress. Command ignored.");
    return;
  }
  _state = STATE_STARTING;
}

void SubnetScanner::stopScan() {
  if (_state != STATE_IDLE && _state != STATE_COMPLETED && _state != STATE_STOPPED) {
    _state = STATE_STOPPED;
    _progress = 100;
    _scanEndTime = millis();
    addLog("WARN", "Network scan manually cancelled by user.");
  }
}

void SubnetScanner::clearScan() {
  _state = STATE_IDLE;
  _progress = 0;
  _currentHostIndex = 0;
  _scanDeviceIndex = 0;
  _discoveredDevices.clear();
  _logs.clear();
  _currentHostIP = "";
  _currentPortString = "";
  _lowCount = 0;
  _mediumCount = 0;
  _highCount = 0;
  _networkScore = 100;
  _networkGrade = "A";
  addLog("INFO", "Scan results cleared. Ready for audit.");
}

void SubnetScanner::handle() {
  // If scan is not active, do nothing
  if (_state == STATE_IDLE || _state == STATE_COMPLETED || _state == STATE_STOPPED) {
    return;
  }

  // Rate limit ticks based on configured scan delay
  if (millis() - _lastTickTime < (unsigned long)settingsManager.scanDelay) {
    return;
  }
  _lastTickTime = millis();

  switch (_state) {
    case STATE_STARTING:
      processStarting();
      break;
    case STATE_DISCOVERING:
      processDiscovering();
      break;
    case STATE_PORT_SCANNING:
      processPortScanning();
      break;
    case STATE_RISK_ANALYSIS:
      processRiskAnalysis();
      break;
    default:
      break;
  }
  
  // Yield control to watchdog
  yield();
}

void SubnetScanner::processStarting() {
  addLog("INFO", "Starting local network security audit...");
  
  // Resolve local IP base
  IPAddress localIp = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP() : WiFi.softAPIP();
  _subnetBase = localIp;
  
  String subnetRange = String(localIp[0]) + "." + String(localIp[1]) + "." + String(localIp[2]) + ".1 - 254";
  addLog("INFO", "Network range scope: " + subnetRange);
  
  _discoveredDevices.clear();
  _progress = 0;
  _scanStartTime = millis();
  _currentHostIndex = 1;
  _state = STATE_DISCOVERING;
}

void SubnetScanner::processDiscovering() {
  if (_currentHostIndex > 254) {
    addLog("INFO", "Subnet discovery complete. Found " + String(_discoveredDevices.size()) + " active nodes.");
    
    IPAddress localIp = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP() : WiFi.softAPIP();
    if (_discoveredDevices.empty() || (_discoveredDevices.size() == 1 && _discoveredDevices[0].ip == localIp.toString())) {
      // Nothing to audit but ourselves
      _state = STATE_RISK_ANALYSIS;
    } else {
      _state = STATE_PORT_SCANNING;
      _scanDeviceIndex = 0;
    }
    return;
  }

  IPAddress targetIp(_subnetBase[0], _subnetBase[1], _subnetBase[2], _currentHostIndex);
  IPAddress localIp = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP() : WiFi.softAPIP();
  
  // If probing ourselves, append standard ESP32 record directly
  if (targetIp == localIp) {
    ScannedDevice dev;
    dev.ip = targetIp.toString();
    dev.name = "IoT Sentinel (This ESP32)";
    dev.open_ports = {80}; // Serves Web Dashboard
    dev.responseTime = 1;
    fingerprintDevice(dev);
    dev.score = RiskEngine::calculateScore(dev.open_ports);
    dev.risk = RiskEngine::evaluateRiskLevel(dev.open_ports);
    _discoveredDevices.push_back(dev);
    
    _currentHostIndex++;
    _progress = (_currentHostIndex * 70) / 254;
    return;
  }

  _currentHostIP = targetIp.toString();
  _currentPortString = "Probing host availability...";
  
  unsigned long responseTimeMs = 0;
  if (pingHost(targetIp, responseTimeMs)) {
    ScannedDevice dev;
    dev.ip = targetIp.toString();
    dev.name = "Resolving Hostname...";
    dev.responseTime = responseTimeMs;
    dev.score = 0;
    dev.risk = "LOW";
    
    addLog("OK", "Host discovered: " + dev.ip + " (RTT: " + String(responseTimeMs) + "ms)");
    _discoveredDevices.push_back(dev);
    
    // Check if we hit the limit of max devices configured in Settings
    if ((int)_discoveredDevices.size() >= settingsManager.maxDevices) {
      addLog("WARN", "Maximum device limit reached (" + String(settingsManager.maxDevices) + "). Stopping discovery.");
      _currentHostIndex = 255; // Break loop
      return;
    }
  }

  _currentHostIndex++;
  _progress = (_currentHostIndex * 70) / 254;
}

void SubnetScanner::processPortScanning() {
  if (_scanDeviceIndex >= _discoveredDevices.size()) {
    _state = STATE_RISK_ANALYSIS;
    return;
  }

  ScannedDevice& dev = _discoveredDevices[_scanDeviceIndex];
  
  // Skip scanning ourselves (ports are set statically)
  if (dev.name.indexOf("This ESP32") != -1) {
    _scanDeviceIndex++;
    _progress = 70 + (_scanDeviceIndex * 25) / _discoveredDevices.size();
    return;
  }

  _currentHostIP = dev.ip;
  _currentPortString = "Scanning open ports...";
  
  std::vector<int> targetPorts = settingsManager.getEnabledPortsList();
  if (targetPorts.empty()) {
    dev.open_ports.clear();
    addLog("INFO", "Port scanning disabled or no ports enabled for: " + dev.ip);
  } else {
    IPAddress deviceIp;
    deviceIp.fromString(dev.ip);
    std::vector<int> openPorts = PortScanner::scanDevicePorts(deviceIp, targetPorts);
    dev.open_ports = openPorts;

    
    String portsStr = "";
    if (openPorts.empty()) {
      portsStr = "None";
    } else {
      for (size_t i = 0; i < openPorts.size(); i++) {
        portsStr += String(openPorts[i]) + (i < openPorts.size() - 1 ? ", " : "");
      }
    }
    addLog("INFO", "Port audit complete on " + dev.ip + " | Open: " + portsStr);
  }

  _scanDeviceIndex++;
  _progress = 70 + (_scanDeviceIndex * 25) / _discoveredDevices.size();
}

void SubnetScanner::processRiskAnalysis() {
  addLog("INFO", "Executing device fingerprinting and threat scoring...");
  
  for (auto& dev : _discoveredDevices) {
    // Fingerprint only external discovered devices (ourselves is already fingerprinted)
    if (dev.name.indexOf("This ESP32") == -1) {
      fingerprintDevice(dev);
      dev.score = RiskEngine::calculateScore(dev.open_ports);
      dev.risk = RiskEngine::evaluateRiskLevel(dev.open_ports);
    }
  }
  
  evaluateSecurityMetrics();
  _state = STATE_COMPLETED;
  _progress = 100;
  _scanEndTime = millis();
  
  addLog("OK", "Subnet security audit finished. Network Grade: " + _networkGrade + " (Index: " + String(_networkScore) + "/100)");
}

bool SubnetScanner::pingHost(IPAddress targetIp, unsigned long& responseTimeMs) {
  unsigned long start = millis();
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) return false;
  
  // Set socket to non-blocking
  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
  
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(80); // Probe HTTP first
  inet_pton(AF_INET, targetIp.toString().c_str(), &serv_addr.sin_addr);
  
  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  
  fd_set writeSet;
  FD_ZERO(&writeSet);
  FD_SET(sockfd, &writeSet);
  
  struct timeval tv;
  tv.tv_sec = 0;
  // Use a rapid 20ms probe limit for subnet detection
  tv.tv_usec = 20000;
  
  int selectResult = select(sockfd + 1, NULL, &writeSet, NULL, &tv);
  bool hostActive = false;
  
  if (selectResult > 0) {
    int error = 0;
    socklen_t len = sizeof(error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) == 0) {
      if (error == 0 || error == ECONNREFUSED) {
        hostActive = true;
      }
    }
  }
  close(sockfd);
  
  // Probe HTTPS (443) if HTTP was completely silent
  if (!hostActive) {
    yield();
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd >= 0) {
      fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
      serv_addr.sin_port = htons(443);
      connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
      FD_ZERO(&writeSet);
      FD_SET(sockfd, &writeSet);
      
      selectResult = select(sockfd + 1, NULL, &writeSet, NULL, &tv);
      if (selectResult > 0) {
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) == 0) {
          if (error == 0 || error == ECONNREFUSED) {
            hostActive = true;
          }
        }
      }
      close(sockfd);
    }
  }
  
  if (hostActive) {
    responseTimeMs = millis() - start;
    if (responseTimeMs == 0) responseTimeMs = 1;
  }
  return hostActive;
}

void SubnetScanner::fingerprintDevice(ScannedDevice& dev) {
  IPAddress gateway = (WiFi.status() == WL_CONNECTED) ? WiFi.gatewayIP() : IPAddress(192, 168, 4, 1);
  bool isGateway = (dev.ip == gateway.toString());
  
  auto hasPort = [&](int port) {
    for (int p : dev.open_ports) {
      if (p == port) return true;
    }
    return false;
  };
  
  // Rules to determine device type
  if (isGateway || hasPort(53)) {
    dev.type = "Router";
    dev.name = "Gateway Router";
  } else if (hasPort(21) && (hasPort(80) || hasPort(8080))) {
    dev.type = "NAS";
    dev.name = "Network Attached Storage (NAS)";
  } else if (hasPort(1883)) {
    dev.type = "ESP32";
    dev.name = "ESP32 IoT Node";
  } else if ((hasPort(80) || hasPort(8080)) && (hasPort(21) || hasPort(23))) {
    dev.type = "IP Camera";
    dev.name = "IP Security Camera";
  } else if (hasPort(22)) {
    dev.type = "Desktop";
    dev.name = "Linux/Mac Workstation";
  } else if (hasPort(80) && hasPort(8080)) {
    dev.type = "Smart TV";
    dev.name = "Smart TV / Media Center";
  } else if (hasPort(80) || hasPort(443)) {
    dev.type = "Laptop";
    dev.name = "Personal Computer";
  } else if (dev.open_ports.empty()) {
    // Active but no open ports on the 8 scanned ports
    dev.type = "Android";
    dev.name = "Mobile Smartphone";
  } else {
    dev.type = "Unknown Device";
    dev.name = "Unknown Network Node";
  }
}

void SubnetScanner::evaluateSecurityMetrics() {
  _lowCount = 0;
  _mediumCount = 0;
  _highCount = 0;
  int totalRiskScore = 0;
  
  if (_discoveredDevices.empty()) {
    _networkScore = 100;
    _networkGrade = "A+";
    return;
  }
  
  for (const auto& dev : _discoveredDevices) {
    if (dev.risk == "HIGH") {
      _highCount++;
    } else if (dev.risk == "MEDIUM") {
      _mediumCount++;
    } else {
      _lowCount++;
    }
    totalRiskScore += dev.score;
  }
  
  int avgRisk = totalRiskScore / _discoveredDevices.size();
  _networkScore = 100 - avgRisk;
  if (_networkScore < 0) _networkScore = 0;
  
  if (_highCount > 0) {
    _networkGrade = "F";
  } else if (_networkScore >= 90) {
    _networkGrade = "A";
  } else if (_networkScore >= 80) {
    _networkGrade = "B";
  } else if (_networkScore >= 70) {
    _networkGrade = "C";
  } else if (_networkScore >= 60) {
    _networkGrade = "D";
  } else {
    _networkGrade = "F";
  }
}

bool SubnetScanner::isScanning() const {
  return (_state == STATE_STARTING || _state == STATE_DISCOVERING || _state == STATE_PORT_SCANNING || _state == STATE_RISK_ANALYSIS);
}

ScanState SubnetScanner::getState() const {
  return _state;
}

String SubnetScanner::getStateString() const {
  switch (_state) {
    case STATE_IDLE: return "IDLE";
    case STATE_STARTING: return "STARTING";
    case STATE_DISCOVERING: return "DISCOVERING";
    case STATE_PORT_SCANNING: return "PORT_SCANNING";
    case STATE_RISK_ANALYSIS: return "RISK_ANALYSIS";
    case STATE_COMPLETED: return "COMPLETED";
    case STATE_STOPPED: return "STOPPED";
    default: return "UNKNOWN";
  }
}

int SubnetScanner::getProgress() const {
  return _progress;
}

String SubnetScanner::getCurrentIP() const {
  return _currentHostIP;
}

String SubnetScanner::getCurrentPortString() const {
  return _currentPortString;
}

unsigned long SubnetScanner::getScanDuration() const {
  if (_state == STATE_IDLE) return 0;
  unsigned long endTime = (isScanning()) ? millis() : _scanEndTime;
  return (endTime - _scanStartTime) / 1000;
}

int SubnetScanner::getHostsDiscoveredCount() const {
  return _discoveredDevices.size();
}

int SubnetScanner::getRemainingHosts() const {
  if (_state == STATE_DISCOVERING) {
    return 254 - _currentHostIndex;
  } else if (_state == STATE_PORT_SCANNING) {
    return _discoveredDevices.size() - _scanDeviceIndex;
  }
  return 0;
}

float SubnetScanner::getScanSpeed() const {
  unsigned long duration = getScanDuration();
  if (duration == 0) return 0.0;
  
  int hostsScanned = 0;
  if (_state == STATE_DISCOVERING) {
    hostsScanned = _currentHostIndex;
  } else {
    hostsScanned = 254;
  }
  return (float)hostsScanned / duration;
}

int SubnetScanner::getNetworkScore() const {
  return _networkScore;
}

String SubnetScanner::getNetworkGrade() const {
  return _networkGrade;
}

void SubnetScanner::getRiskDistribution(int& low, int& med, int& high) const {
  low = _lowCount;
  med = _mediumCount;
  high = _highCount;
}

const std::vector<ScannedDevice>& SubnetScanner::getDevices() const {
  return _discoveredDevices;
}

const std::vector<String>& SubnetScanner::getLogs() const {
  return _logs;
}

void SubnetScanner::addLog(String level, String logText) {
  // Mirror to hardware Serial output
  Serial.println("[" + level + "] " + logText);
  
  if (_logs.size() >= 60) {
    _logs.erase(_logs.begin());
  }
  
  unsigned long secs = millis() / 1000;
  char buf[16];
  sprintf(buf, "[%02d:%02d:%02d] ", (int)(secs / 3600) % 24, (int)(secs / 60) % 60, (int)(secs % 60));
  
  _logs.push_back(String(buf) + "[" + level + "] " + logText);
}
