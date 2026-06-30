#ifndef PORT_SCANNER_H
#define PORT_SCANNER_H

#include <Arduino.h>
#include <vector>
#include <lwip/sockets.h>

struct SocketScanState {
  int fd;
  int port;
  bool isChecking;
  unsigned long startTime;
};

class PortScanner {
public:
  // Starts a fast scan of specific security ports on a single target IP.
  // Performs non-blocking socket connect probes.
  static std::vector<int> scanDevicePorts(IPAddress ip, const std::vector<int>& portsToScan);
};

#endif
