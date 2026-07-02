<div align="center">

# 🛡️ IoT Sentinel

### ESP32-Based Local Network Security Assessment Platform

*A lightweight embedded cybersecurity platform for discovering devices, assessing network exposure, and visualizing security insights in real time.*

![ESP32](https://img.shields.io/badge/ESP32-IoT-blue?style=for-the-badge&logo=espressif)
![Arduino](https://img.shields.io/badge/Arduino-IDE-00979D?style=for-the-badge&logo=arduino)
![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus)
![HTML5](https://img.shields.io/badge/HTML-5-E34F26?style=for-the-badge&logo=html5)
![CSS3](https://img.shields.io/badge/CSS-3-1572B6?style=for-the-badge&logo=css3)
![JavaScript](https://img.shields.io/badge/JavaScript-ES6-F7DF1E?style=for-the-badge&logo=javascript)
![License](https://img.shields.io/badge/License-MIT-success?style=for-the-badge)

---

### 🚀 Lightweight • Embedded • Cybersecurity • Real-Time Dashboard

</div>

---

# 📖 Overview

**IoT Sentinel** is an embedded cybersecurity platform that transforms an **ESP32** into a lightweight **Local Network Security Assessment Appliance**.

The platform discovers devices connected to a local network, performs lightweight service enumeration, evaluates potential security risks using a rule-based engine, and visualizes results through an interactive web dashboard.

Unlike conventional desktop-based scanners, IoT Sentinel demonstrates how **resource-constrained IoT hardware** can perform practical network assessment while maintaining low power consumption and minimal hardware requirements.

---

# ✨ Features

## 🌐 Network Discovery

- Automatic Wi-Fi connection
- Local subnet detection
- Active host discovery
- Device inventory generation
- Scan progress tracking

---

## 🔍 Service Enumeration

- TCP Port Scanning
- Common Service Detection
- Configurable Port Lists
- Non-blocking Scan Engine
- Optimized for ESP32

---

## 🛡️ Security Assessment

- Rule-Based Risk Analysis
- Device Security Score
- Network Security Grade
- Security Recommendations
- Threat Classification

---

## 📊 Real-Time Dashboard

- Modern Cyber UI
- Live Progress
- Risk Charts
- Device Table
- Live Logs
- Responsive Design

---

## 📄 Reporting

- JSON Reports
- CSV Reports
- Scan Summary
- Device Statistics

---

# 🏗 System Architecture

> *(Insert architecture diagram here)*

```
                Browser
                    │
          HTTP REST API
                    │
                    ▼
              ESP32 Web Server
                    │
    ┌───────────────┼───────────────┐
    │               │               │
 WiFi Manager   Scan Manager   Dashboard API
                    │
                    ▼
          Network Discovery
                    │
                    ▼
            Port Scanner
                    │
                    ▼
            Risk Engine
                    │
                    ▼
             JSON Generator
                    │
                    ▼
          Real-Time Dashboard
```

---

# ⚙️ Technology Stack

| Category | Technology |
|-----------|------------|
| Hardware | ESP32 DevKit V1 |
| Language | C++ |
| IDE | Arduino IDE |
| Networking | WiFi, TCP/IP |
| Backend | ESP32 WebServer |
| Frontend | HTML5, CSS3, JavaScript |
| Charts | Chart.js |
| Data | JSON |
| Storage | Preferences (NVS) |

---

# 📁 Project Structure

```text
IoT-Sentinel/
│
├── src/
│   ├── main.ino
│   ├── wifi_manager.cpp
│   ├── scanner.cpp
│   ├── port_scanner.cpp
│   ├── risk_engine.cpp
│   ├── webserver.cpp
│
├── data/
│   ├── index.html
│   ├── style.css
│   └── app.js
│
├── docs/
│
└── README.md
```

---

# 🔄 Workflow

```text
ESP32 Boot
      │
      ▼
Connect to Wi-Fi
      │
      ▼
Launch Web Server
      │
      ▼
User Opens Dashboard
      │
      ▼
Start Scan
      │
      ▼
Discover Active Devices
      │
      ▼
Port Scanning
      │
      ▼
Risk Assessment
      │
      ▼
Generate Report
      │
      ▼
Display Dashboard
```

---

# 📊 Security Assessment

| Service | Port | Risk |
|---------|------|------|
| FTP | 21 | 🔴 High |
| SSH | 22 | 🟢 Low |
| Telnet | 23 | 🔴 High |
| DNS | 53 | 🟢 Low |
| HTTP | 80 | 🟠 Medium |
| HTTPS | 443 | 🟢 Low |
| MQTT | 1883 | 🟠 Medium |
| HTTP Alternate | 8080 | 🟠 Medium |

---

# 📸 Dashboard

> Add screenshots here.

### Dashboard Overview

```
/assets/dashboard.png
```

### Device Discovery

```
/assets/devices.png
```

### Risk Analysis

```
/assets/risk.png
```

### Live Logs

```
/assets/logs.png
```

---

# 🚀 Roadmap

- [x] Project Planning
- [x] Dashboard UI
- [ ] REST API
- [ ] Device Discovery
- [ ] TCP Port Scanner
- [ ] Risk Assessment Engine
- [ ] Live Logging
- [ ] Report Export
- [ ] Device Fingerprinting
- [ ] Network Topology
- [ ] Historical Reports

---

# 🎯 Learning Outcomes

This project demonstrates practical implementation of:

- Embedded Systems
- Internet of Things
- Computer Networks
- TCP/IP
- REST APIs
- Network Discovery
- Service Enumeration
- Cybersecurity Fundamentals
- Dashboard Design
- Embedded Web Development

---

# ⚠️ Disclaimer

This project is intended **strictly for educational and defensive security purposes**.

IoT Sentinel performs lightweight network discovery and security assessment on authorized local networks. It **does not perform penetration testing, exploit vulnerabilities, or bypass authentication mechanisms**.

Only use this software on networks and devices for which you have explicit permission.

---

# 🤝 Contributing

Contributions are welcome!

If you'd like to improve IoT Sentinel:

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Open a Pull Request

---

# 📄 License

Distributed under the **MIT License**.

---

<div align="center">

## 👨‍💻 Author

### Rajveer Soni

**Computer Science Engineering Student**

Embedded Systems • IoT • Cybersecurity • Full Stack Development

⭐ **If you found this project useful, please consider giving it a star!**

</div>

This is the output of the scan that shows the no. of IP's available (wifi network) on which the scan is performed to check whether the port is secure(close) or not(open).
Open port means it may be possible to be hacked, so the project is all about to check the ports are secured(closed) or not.
<img width="1902" height="980" alt="image" src="https://github.com/user-attachments/assets/cfba03a0-ee61-4fa6-a4a8-1f9578b4e8f8" />
