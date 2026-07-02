<div align="center">

# 🛡️ IoT Sentinel

### ESP32-Based Local Network Security Assessment Platform

*A lightweight embedded cybersecurity platform for discovering devices, assessing network exposure, and visualizing security insights in real time.*

<p align="center">
  <img src="assets/dashboard.png" width="100%" alt="IoT Sentinel Dashboard"/>
</p>

<p align="center">
  <b>Embedded Cybersecurity • ESP32 • Local Network Assessment • Real-Time Dashboard</b>
</p>

![ESP32](https://img.shields.io/badge/ESP32-WROOM32-blue?style=for-the-badge&logo=espressif)
![Arduino](https://img.shields.io/badge/Arduino-IDE-00979D?style=for-the-badge&logo=arduino)
![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5)
![CSS3](https://img.shields.io/badge/CSS3-1572B6?style=for-the-badge&logo=css3)
![JavaScript](https://img.shields.io/badge/JavaScript-ES6-F7DF1E?style=for-the-badge&logo=javascript)
![License](https://img.shields.io/badge/License-MIT-success?style=for-the-badge)

</div>

---

# 📖 Overview

IoT Sentinel is an embedded cybersecurity platform that transforms an **ESP32** into a lightweight **Local Network Security Assessment Platform**.

The system automatically connects to a local Wi-Fi network, discovers active devices, analyzes exposed services through lightweight TCP port scanning, evaluates the security posture using a rule-based assessment engine, and presents the results through a responsive real-time dashboard hosted directly on the ESP32.

The project demonstrates the integration of **Embedded Systems**, **Computer Networks**, **IoT**, **Cybersecurity**, and **Web Technologies** into a single low-cost security appliance.

---

# 📸 Project Demonstration

## Dashboard

The ESP32 hosts a responsive dashboard that provides:

- Real-time scan progress
- Device inventory
- Open ports
- Risk assessment
- Security score
- Live logs
- JSON / CSV report generation

<p align="center">
<img src="assets/dashboard.png" width="95%">
</p>

---

## Practical Implementation

The complete system running on the ESP32 development board.

<p align="center">
<img src="assets/hardware.jpg" width="80%">
</p>

The ESP32 connects to the local Wi-Fi network, performs network discovery, hosts the embedded web application, and communicates with the browser through REST APIs.

---

# ✨ Features

## 🌐 Network Discovery

- Automatic Wi-Fi Connection
- Local Subnet Detection
- Active Host Discovery
- Device Enumeration
- Scan Progress Monitoring

## 🔍 Service Enumeration

- Lightweight TCP Port Scanner
- Configurable Port List
- Service Identification
- Optimized for ESP32

## 🛡️ Security Assessment

- Rule-Based Risk Engine
- Device Security Score
- Network Security Grade
- Threat Classification
- Security Recommendations

## 📊 Web Dashboard

- Modern Cybersecurity Interface
- Responsive Design
- Live Statistics
- Risk Visualization
- Device Table
- Progress Monitoring
- Export Reports

---

# 🏗️ System Workflow

```text
ESP32 Boot
      │
      ▼
Connect to Wi-Fi
      │
      ▼
Start Embedded Web Server
      │
      ▼
User Opens Dashboard
      │
      ▼
Start Security Assessment
      │
      ▼
Network Discovery
      │
      ▼
Port Scanning
      │
      ▼
Risk Evaluation
      │
      ▼
Generate Report
      │
      ▼
Real-Time Dashboard Update
```

---

# ⚙️ Technology Stack

| Category | Technology |
|-----------|------------|
| Hardware | ESP32 DevKit V1 |
| IDE | Arduino IDE |
| Language | C++ |
| Networking | WiFi, TCP/IP |
| Backend | ESP32 WebServer |
| Frontend | HTML5, CSS3, JavaScript |
| Charts | Chart.js |
| Data Format | JSON |
| Storage | Preferences (NVS) |

---

# 📂 Project Structure

```text
IoT-Sentinel/
│
├── assets/
│   ├── dashboard.png
│   ├── hardware.jpg
│   ├── architecture.png
│   ├── workflow.png
│   └── banner.png
│
├── src/
│   ├── main.ino
│   ├── wifi_manager.cpp
│   ├── scanner.cpp
│   ├── port_scanner.cpp
│   ├── risk_engine.cpp
│   └── webserver.cpp
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

# 🚀 Future Roadmap

- Device Fingerprinting
- MAC Vendor Detection
- Historical Scan Reports
- Network Topology Mapping
- OTA Firmware Updates
- WebSocket Support
- Advanced Service Fingerprinting
- Multi-Network Profiles

---

# ⚠️ Disclaimer

IoT Sentinel is developed **strictly for educational, research, and defensive cybersecurity purposes**.

The platform performs lightweight network discovery and security assessment by identifying active hosts and commonly exposed services. It does **not** exploit vulnerabilities, bypass authentication mechanisms, or perform offensive security operations.

Use this software **only on networks and devices that you own or have explicit authorization to assess.**

---

<div align="center">

### 👨‍💻 Developed by

# Rajveer Soni

**Computer Science Engineering**

Embedded Systems • IoT • Cybersecurity

⭐ If you found this project useful, consider giving it a star.

</div>
