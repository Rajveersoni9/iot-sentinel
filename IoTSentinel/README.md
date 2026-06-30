# IoT Sentinel - Local Network Security platform for ESP32

**IoT Sentinel** is a premium, plug-and-run local network vulnerability scanner and security assessment platform built on the ESP32 architecture. It discovers active hosts in the local Class C subnet, audits open risk ports via fast non-blocking TCP socket multiplexing, runs lightweight rules-based device fingerprinting, and ranks security status through an integrated threat scoring engine. All results, logs, and settings configurations are presented on a responsive dark cyber-themed web dashboard hosted directly from the device.

---

## Final Project Architecture

```
                                  +-----------------------+
                                  |     USER BROWSER      |
                                  | (Dashboard UI Panel)  |
                                  +-----------+-----------+
                                              |
                                     HTTP REST APIs / AJAX
                                              |
                                              v
+---------------------------------------------+----------------------------------------------+
| ESP32 DEVICE BACKEND                                                                       |
|                                                                                            |
|   +--------------------------+  +--------------------------+  +--------------------------+ |
|   |    HTTP WebServer        |  |     WiFi Manager         |  |   Settings Manager       | |
|   |    (Port 80 Wrapper)     |  |   (Auto-reconnect/mDNS)  |  |    (Preferences NVS)     | |
|   +------------+-------------+  +------------+-------------+  +------------+-------------+ |
|                |                             |                             |               |
|                +-----------------------------+-----------------------------+               |
|                                              |                                             |
|                                              v                                             |
|                                 +------------+-------------+                               |
|                                 |       Scan Manager       |                               |
|                                 |      (State Machine)     |                               |
|                                 +------------+-------------+                               |
|                                              |                                             |
|                      +-----------------------+-----------------------+                     |
|                      |                                               |                     |
|                      v                                               v                     |
|         +------------+-------------+                     +-----------+--------------+      |
|         |       Port Scanner       |                     |    Risk Assessment &     |      |
|         |  (Non-blocking Sockets)  |                     |  Fingerprinting Engine   |      |
|         +------------+-------------+                     +--------------------------+      |
|                      |                                                                     |
+----------------------|---------------------------------------------------------------------+
                       |
                TCP Connection Probes
                       |
                       v
        +--------------+--------------+
        |   LOCAL SUBNET DEVICES      |
        |  (Gateway, IP Cameras, TV)  |
        +-----------------------------+
```

---

## Core System Modules

1. **Wi-Fi Manager**: Orchestrates network connections. Starts a soft Access Point (`IoT_Sentinel_Config` at `192.168.4.1`) with a Captive DNS Portal if no configuration exists or if local router connection fails. Integrates automatic non-blocking reconnection loops and initializes mDNS (`http://security.local`).
2. **Settings Manager**: Serializes and deserializes configurations (such as port lists to scan, scan timeout, inter-host delay, auto refresh settings, UI theme, SSID parameters) to the ESP32 non-volatile storage (NVS) using the `Preferences` API.
3. **Scan Manager**: Uses a 7-stage state machine (`IDLE`, `STARTING`, `DISCOVERING`, `PORT_SCANNING`, `RISK_ANALYSIS`, `COMPLETED`, `STOPPED`) executing asynchronously in the main `loop()` thread, preventing hardware watchdog resets.
4. **Port Scanner**: Runs multi-socket multiplexing using POSIX `select()` to verify connection statuses on target ports concurrently.
5. **Risk Assessment Engine**: Scores risk levels based on exposed ports (Telnet, FTP, MQTT, HTTP) and fingerprints host types (Router, NAS, IP Camera, Smart TV, Laptop, ESP32) based on port combinations and gateway details.

---

## HTTP REST API Documentation

### 1. `GET /api/status`
Returns the status indicators of the active scan, system information, aggregate network safety metrics, and current host progress.
* **Response Schema**:
  ```json
  {
      "status": "DISCOVERING",
      "scanning": true,
      "progress": 42,
      "current_ip": "192.168.1.15",
      "current_port": "Probing host availability...",
      "sys_ip": "192.168.1.10",
      "sys_ssid": "HomeNetwork",
      "sys_free_ram": 182490,
      "hosts_found": 3,
      "scan_duration": 8,
      "scan_speed": 14.5,
      "remaining_hosts": 148,
      "net_score": 85,
      "net_grade": "B",
      "risk_distribution": {
          "low": 2,
          "medium": 1,
          "high": 0
      }
  }
  ```

### 2. `GET /api/devices`
Fetches the current array of audited hosts in the subnet.
* **Response Schema**:
  ```json
  [
      {
          "ip": "192.168.1.1",
          "name": "Gateway Router",
          "risk": "LOW",
          "score": 10,
          "type": "Router",
          "rtt": 2,
          "open_ports": [53, 80]
      }
  ]
  ```

### 3. `GET /api/logs`
Streams the system log buffer containing timestamps and severity categorizations.
* **Response Schema**:
  ```json
  [
      "[00:01:05] [INFO] Starting local network security audit...",
      "[00:01:07] [OK] Host discovered: 192.168.1.1 (RTT: 2ms)"
  ]
  ```

### 4. `POST /api/scan/start`
Triggers a new security audit sweep.
* **Response**: `{"status":"ok"}`

### 5. `POST /api/scan/stop`
Cancels the active scanner execution.
* **Response**: `{"status":"ok"}`

### 6. `GET /api/settings`
Fetches current NVS settings parameters.
* **Response**:
  ```json
  {
      "timeout": 120,
      "delay": 0,
      "ports": 255,
      "theme": "dark",
      "refresh": true,
      "maxdev": 50
  }
  ```

### 7. `POST /api/settings`
Saves incoming configuration parameters to NVS. Accepts URL-encoded form parameters.
* **Post Parameters**:
  * `timeout` (int) - Socket timeout in milliseconds
  * `delay` (int) - Delay between hosts in milliseconds
  * `maxdev` (int) - Maximum devices array capacity
  * `theme` (string) - "dark" or "light"
  * `refresh` (string) - "true" or "false"
  * `ports` (int) - Bitmask representing active ports (bit 0 = port 21, bit 1 = port 22...)
  * `ssid` (string) - Wi-Fi network SSID (Optional)
  * `password` (string) - Wi-Fi network passcode (Optional)
* **Response**: `{"status":"ok"}`

---

## Installation & Setup Instructions

1. **Hardware Assembly**:
   * Connect an ESP32 DevKit V1 (38-pin or compatible board) to a USB power supply. There is no need for a serial data cable once flashed.
2. **Software Setup**:
   * Open Arduino IDE.
   * Verify that the ESP32 Board Core (v2.x or v3.x) is installed.
   * Add the project files (`IoTSentinel.ino`, `scanner.h/cpp`, `wifi_manager.h/cpp`, `settings_manager.h/cpp`, `port_scanner.h/cpp`, `risk_engine.h/cpp`, `sentinel_webserver.h/cpp`, `web_assets.h`) to the sketch folder.
3. **Firmware Upload**:
   * Connect the board to your computer via USB.
   * Select your board port and click **Upload**.
4. **Plug-and-Run Operations**:
   * Once flashed, unplug the board and plug it into any USB power wall adapter.
   * The device will attempt to connect to its default network.
   * If connection fails, the device launches an access point named `IoT_Sentinel_Config`. Connect your phone/PC to this Wi-Fi, load `http://192.168.4.1` on your browser, and input your router's SSID credentials in the settings form.
   * Once configured and connected to your local network, access the dashboard at `http://security.local` from any local client device.
