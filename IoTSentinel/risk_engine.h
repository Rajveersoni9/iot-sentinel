#ifndef RISK_ENGINE_H
#define RISK_ENGINE_H

#include <Arduino.h>
#include <vector>

class RiskEngine {
public:
  // Evaluates risk score (0 - 100) based on list of open ports
  static int calculateScore(const std::vector<int>& openPorts);
  
  // Evaluates risk label (LOW / MEDIUM / HIGH)
  static String evaluateRiskLevel(const std::vector<int>& openPorts);
};

#endif
