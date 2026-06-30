#include "risk_engine.h"

int RiskEngine::calculateScore(const std::vector<int>& openPorts) {
  if (openPorts.empty()) {
    return 0;
  }
  
  int score = 0;
  
  for (int port : openPorts) {
    switch (port) {
      case 23: // Telnet
        score += 55;
        break;
      case 21: // FTP
        score += 45;
        break;
      case 80: // HTTP
        score += 25;
        break;
      case 1883: // MQTT
        score += 25;
        break;
      case 8080: // HTTP-Alt
        score += 20;
        break;
      case 22: // SSH
        score += 15;
        break;
      case 443: // HTTPS
        score += 10;
        break;
      case 53: // DNS
        score += 10;
        break;
      default:
        score += 5; // Fallback for other ports
        break;
    }
  }
  
  // Clamp score to max 100
  if (score > 100) {
    score = 100;
  }
  
  return score;
}

String RiskEngine::evaluateRiskLevel(const std::vector<int>& openPorts) {
  int score = calculateScore(openPorts);
  
  if (score >= 70) {
    return "HIGH";
  } else if (score >= 35) {
    return "MEDIUM";
  } else {
    return "LOW";
  }
}
