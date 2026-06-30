#ifndef WEB_ASSETS_H
#define WEB_ASSETS_H

#include <Arduino.h>

// ==========================================
// index.html
// ==========================================
const char INDEX_HTML[] PROGMEM = R"rawhtml(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>IoT Sentinel - ESP32 Network Security Scanner</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div class="app-container">
        <!-- Sidebar Navigation -->
        <aside class="sidebar">
            <div class="sidebar-header">
                <div class="logo-icon">
                    <svg viewBox="0 0 24 24" width="28" height="28" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                        <path d="M12 22s8-4 8-10V5l-8-3-8 3v7c0 6 8 10 8 10z"/>
                    </svg>
                </div>
                <div class="logo-text">
                    <h2>IoT Sentinel</h2>
                    <span>Network Auditor</span>
                </div>
            </div>
            
            <nav class="nav-menu">
                <a href="#dashboard" class="nav-item active" id="btn-dash">
                    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="7" height="9" rx="1"/><rect x="14" y="3" width="7" height="5" rx="1"/><rect x="14" y="12" width="7" height="9" rx="1"/><rect x="3" y="16" width="7" height="5" rx="1"/></svg>
                    Dashboard
                </a>
                <a href="#scanner" class="nav-item" id="btn-scan">
                    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><path d="M12 2v20M2 12h20"/></svg>
                    Scan Subnet
                </a>
                <a href="#logs" class="nav-item" id="btn-logs">
                    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="currentColor" stroke-width="2"><path d="M4 19.5A2.5 2.5 0 0 1 6.5 17H20M4 19.5A2.5 2.5 0 0 0 6.5 22H20M4 19.5v-15A2.5 2.5 0 0 1 6.5 2M20 2v20"/></svg>
                    Live Logs
                </a>
                <a href="#encyclopedia" class="nav-item" id="btn-ency">
                    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="currentColor" stroke-width="2"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"/><line x1="12" y1="9" x2="12" y2="13"/><line x1="12" y1="17" x2="12.01" y2="17"/></svg>
                    Mitigation Info
                </a>
                <a href="#settings" class="nav-item" id="btn-settings">
                    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 1 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-4 0v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 1 1-2.83-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1 0-4h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 1 1 2.83-2.83l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 4 0v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 1 1 2.83 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg>
                    Settings
                </a>
            </nav>

            <div class="system-status">
                <div class="status-row">
                    <span class="label">IP Address:</span>
                    <span class="value" id="status-ip">Connecting...</span>
                </div>
                <div class="status-row">
                    <span class="label">SSID:</span>
                    <span class="value" id="status-ssid">...</span>
                </div>
                <div class="status-row">
                    <span class="label">RAM Free:</span>
                    <span class="value" id="status-ram">-- KB</span>
                </div>
            </div>
        </aside>

        <!-- Main Content Area -->
        <main class="main-content">
            <!-- Header Panel -->
            <header class="top-bar">
                <div class="top-bar-left">
                    <button id="sidebar-toggle" class="btn btn-mini btn-outline mobile-only" aria-label="Toggle Navigation Menu">
                        <svg viewBox="0 0 24 24" width="20" height="20" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                            <line x1="3" y1="12" x2="21" y2="12"></line>
                            <line x1="3" y1="6" x2="21" y2="6"></line>
                            <line x1="3" y1="18" x2="21" y2="18"></line>
                        </svg>
                    </button>
                    <h1 id="page-title">Security Dashboard</h1>
                </div>
                <div class="scan-pill-container">

                    <div id="scan-indicator" class="scan-pill idle">
                        <span class="pulse-dot"></span>
                        <span id="scan-status-text">System Idle</span>
                    </div>
                </div>
            </header>

            <!-- Tab Content: Dashboard -->
            <section id="tab-dashboard" class="tab-pane active">
                <!-- Stat Cards Grid -->
                <div class="stats-grid">
                    <div class="stat-card border-glow-blue">
                        <div class="card-icon blue-bg">
                            <svg viewBox="0 0 24 24" width="24" height="24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><path d="M8 14s1.5 2 4 2 4-2 4-2M9 9h.01M15 9h.01"/></svg>
                        </div>
                        <div class="card-details">
                            <h3>Security Grade</h3>
                            <h2 id="metric-grade">A</h2>
                            <p id="metric-grade-sub">All clear</p>
                        </div>
                    </div>

                    <div class="stat-card border-glow-green">
                        <div class="card-icon green-bg">
                            <svg viewBox="0 0 24 24" width="24" height="24" fill="none" stroke="currentColor" stroke-width="2"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"/><circle cx="9" cy="7" r="4"/><path d="M23 21v-2a4 4 0 0 0-3-3.87M16 3.13a4 4 0 0 1 0 7.75"/></svg>
                        </div>
                        <div class="card-details">
                            <h3>Hosts Online</h3>
                            <h2 id="metric-hosts">0</h2>
                            <p>Active devices</p>
                        </div>
                    </div>

                    <div class="stat-card border-glow-red">
                        <div class="card-icon red-bg">
                            <svg viewBox="0 0 24 24" width="24" height="24" fill="none" stroke="currentColor" stroke-width="2"><polygon points="7.86 2 16.14 2 22 7.86 22 16.14 16.14 22 7.86 22 2 16.14 2 7.86 7.86 2"/><line x1="12" y1="8" x2="12" y2="12"/><line x1="12" y1="16" x2="12.01" y2="16"/></svg>
                        </div>
                        <div class="card-details">
                            <h3>Critical Threats</h3>
                            <h2 id="metric-critical" class="threat-glow">0</h2>
                            <p>Open Telnet/FTP</p>
                        </div>
                    </div>

                    <div class="stat-card border-glow-amber">
                        <div class="card-icon amber-bg">
                            <svg viewBox="0 0 24 24" width="24" height="24" fill="none" stroke="currentColor" stroke-width="2"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"/><line x1="12" y1="9" x2="12" y2="13"/><line x1="12" y1="17" x2="12.01" y2="17"/></svg>
                        </div>
                        <div class="card-details">
                            <h3>Medium Risks</h3>
                            <h2 id="metric-medium">0</h2>
                            <p>Exposed Web panels</p>
                        </div>
                    </div>
                </div>

                <!-- Mid Section: Chart and Progress -->
                <div class="dashboard-mid-row">
                    <div class="chart-container glass">
                        <div class="chart-header">
                            <h3>Network Security Mix</h3>
                        </div>
                        <div class="chart-content">
                            <!-- Pure SVG Pie Chart and legend -->
                            <div class="svg-chart-wrapper">
                                <svg width="160" height="160" viewBox="0 0 36 36" class="donut-chart">
                                    <circle class="donut-ring" cx="18" cy="18" r="15.915" fill="transparent" stroke="#151b2d" stroke-width="3"></circle>
                                    <circle id="donut-segment-low" class="donut-segment" cx="18" cy="18" r="15.915" fill="transparent" stroke="#38ef7d" stroke-width="3" stroke-dasharray="100 0" stroke-dashoffset="25"></circle>
                                    <circle id="donut-segment-med" class="donut-segment" cx="18" cy="18" r="15.915" fill="transparent" stroke="#ff9900" stroke-width="3" stroke-dasharray="0 100" stroke-dashoffset="0"></circle>
                                    <circle id="donut-segment-high" class="donut-segment" cx="18" cy="18" r="15.915" fill="transparent" stroke="#ff3366" stroke-width="3" stroke-dasharray="0 100" stroke-dashoffset="0"></circle>
                                    <g class="chart-text">
                                        <text x="50%" y="50%" class="chart-number" id="chart-pct">100%</text>
                                        <text x="50%" y="62%" class="chart-label">Secure</text>
                                    </g>
                                </svg>
                            </div>
                            <div class="chart-legend">
                                <div class="legend-item"><span class="dot green-dot"></span> Low Risk</div>
                                <div class="legend-item"><span class="dot amber-dot"></span> Medium Risk</div>
                                <div class="legend-item"><span class="dot red-dot"></span> High Threat</div>
                            </div>
                        </div>
                    </div>

                    <div class="quick-control glass">
                        <h3>Subnet Audit Progress</h3>
                        <div class="progress-details">
                            <div class="progress-info-row">
                                <span id="progress-ip">Target: Not scanning</span>
                                <span id="progress-percent">0%</span>
                            </div>
                            <div class="progress-bar-container">
                                <div class="progress-fill" id="progress-bar" style="width: 0%"></div>
                            </div>
                        </div>
                        <div class="action-buttons">
                            <button id="btn-start-scan" class="btn btn-primary glow-button">
                                <span class="spinner-icon hidden" id="scan-spinner"></span>
                                Trigger Subnet Scan
                            </button>
                            <button id="btn-stop-scan" class="btn btn-danger glow-button-red hidden">
                                Stop Scan
                            </button>
                            <button id="btn-clear-scan" class="btn btn-secondary">Clear Results</button>
                        </div>
                    </div>
                </div>

                <!-- Device Table -->
                <div class="table-container glass">
                    <div class="table-header">
                        <h3>Audited Devices</h3>
                        <div class="header-actions">
                            <div class="table-filter">
                                <span class="active-filter" id="filter-all">All (<span id="count-all">0</span>)</span>
                                <span id="filter-high">Threats (<span id="count-high">0</span>)</span>
                            </div>
                            <div class="export-actions">
                                <button id="btn-export-json" class="btn btn-mini btn-outline">
                                    <svg viewBox="0 0 24 24" width="12" height="12" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4M7 10l5 5 5-5M12 15V3"/></svg> Export JSON
                                </button>
                                <button id="btn-export-csv" class="btn btn-mini btn-outline">
                                    <svg viewBox="0 0 24 24" width="12" height="12" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4M7 10l5 5 5-5M12 15V3"/></svg> Export CSV
                                </button>
                            </div>
                        </div>
                    </div>
                    <div class="responsive-table">
                        <table>
                            <thead>
                                <tr>
                                    <th>IP Address</th>
                                    <th>Fingerprint / Node Type</th>
                                    <th>Open Ports</th>
                                    <th>Risk Assessment</th>
                                    <th>Score / RTT</th>
                                    <th>Action</th>
                                </tr>
                            </thead>
                            <tbody id="device-table-body">
                                <tr>
                                    <td colspan="6" class="table-empty">No device data available. Perform a subnet scan to discover active nodes.</td>
                                </tr>
                            </tbody>
                        </table>
                    </div>
                </div>
            </section>

            <!-- Tab Content: Active Subnet Scan Control -->
            <section id="tab-scanner" class="tab-pane">
                <div class="glass scanner-panel">
                    <h2>Subnet Network Scanner</h2>
                    <p class="description">
                        This module probes IP addresses in your local subnet (derived from the ESP32's current IP and subnet mask). 
                        It establishes ultra-fast TCP connections to identify live hosts and audit active vulnerabilities.
                    </p>

                    <div class="scan-settings">
                        <div class="settings-card">
                            <h3>Subnet Details</h3>
                            <div class="details-list">
                                <div class="detail-item">
                                    <span>Subnet Mask:</span>
                                    <strong id="scan-submask">Connecting...</strong>
                                </div>
                                <div class="detail-item">
                                    <span>IP Range:</span>
                                    <strong id="scan-iprange">Calculating...</strong>
                                </div>
                                <div class="detail-item">
                                    <span>Current Scope:</span>
                                    <strong>254 IP addresses (Class C)</strong>
                                </div>
                            </div>
                        </div>

                        <div class="settings-card">
                            <h3>Audited Ports</h3>
                            <div class="ports-tag-list">
                                <span class="port-tag critical">Port 21 (FTP)</span>
                                <span class="port-tag warning">Port 22 (SSH)</span>
                                <span class="port-tag critical">Port 23 (Telnet)</span>
                                <span class="port-tag info">Port 53 (DNS)</span>
                                <span class="port-tag warning">Port 80 (HTTP)</span>
                                <span class="port-tag info">Port 443 (HTTPS)</span>
                                <span class="port-tag warning">Port 1883 (MQTT)</span>
                                <span class="port-tag info">Port 8080 (HTTP-Alt)</span>
                            </div>
                        </div>
                    </div>

                    <div class="scanner-actions">
                        <button id="btn-big-scan" class="btn btn-primary btn-large glow-button-blue">
                            Run Subnet Security Audit
                        </button>
                    </div>
                </div>
            </section>

            <!-- Tab Content: Live Logs Console -->
            <section id="tab-logs" class="tab-pane">
                <div class="terminal-container glass">
                    <div class="terminal-header">
                        <div class="window-buttons">
                            <span></span><span></span><span></span>
                        </div>
                        <div class="terminal-title">audit_scanner_log.txt</div>
                        <button class="btn btn-mini" id="btn-clear-logs">Clear Logs</button>
                    </div>
                    <div class="terminal-body" id="log-console">
                        <div class="log-line text-muted">[SYSTEM] Welcome to IoT Sentinel terminal console. Logs will display below in real-time.</div>
                    </div>
                </div>
            </section>

            <!-- Tab Content: Mitigation Info -->
            <section id="tab-encyclopedia" class="tab-pane">
                <div class="encyclopedia-grid">
                    <div class="ency-card glass">
                        <div class="ency-header critical-border">
                            <span class="risk-badge red-bg">CRITICAL THREAT</span>
                            <h3>Telnet Protocol (Port 23) Exposed</h3>
                        </div>
                        <p class="ency-desc">
                            Telnet transmits all sessions (including user logins and administrative passwords) in plaintext. An attacker on the same local network can intercept this traffic using ARP poisoning or passive sniffing to obtain full credentials.
                        </p>
                        <div class="ency-mitigation">
                            <strong>Remediation:</strong> Immediately disable Telnet in the device configuration pages and substitute it with secure SSH communication (Port 22) which enforces end-to-end encryption.
                        </div>
                    </div>

                    <div class="ency-card glass">
                        <div class="ency-header critical-border">
                            <span class="risk-badge red-bg">CRITICAL THREAT</span>
                            <h3>FTP Server (Port 21) Enabled</h3>
                        </div>
                        <p class="ency-desc">
                            File Transfer Protocol (FTP) lacks security encryption. Session handshake parameters, usernames, keys, and transferred documents are sent in raw ASCII text, making them completely vulnerable to MITM (Man-in-the-Middle) sniffing.
                        </p>
                        <div class="ency-mitigation">
                            <strong>Remediation:</strong> Switch to SFTP (Secure FTP over SSH) or secure cloud transmission. If FTP is required for legacy nodes, implement strict IP address filters (ACLs).
                        </div>
                    </div>

                    <div class="ency-card glass">
                        <div class="ency-header warning-border">
                            <span class="risk-badge amber-bg">MEDIUM RISK</span>
                            <h3>HTTP Server (Port 80) Unsecured</h3>
                        </div>
                        <p class="ency-desc">
                            HTTP lacks transport layer protection. Administrators logging into configuration portals are vulnerable to session sniffing. Many older smart switches and IoT sensors expose unauthenticated root terminals over Port 80.
                        </p>
                        <div class="ency-mitigation">
                            <strong>Remediation:</strong> Enable HTTPS (Port 443) and force SSL redirects. Enforce robust, non-default credentials for all admin accounts.
                        </div>
                    </div>

                    <div class="ency-card glass">
                        <div class="ency-header warning-border">
                            <span class="risk-badge amber-bg">MEDIUM RISK</span>
                            <h3>MQTT Broker (Port 1883) Exposed</h3>
                        </div>
                        <p class="ency-desc">
                            Message Queuing Telemetry Transport (MQTT) is the backbone of IoT communication. Port 1883 hosts unencrypted MQTT transmissions. If left open without login checks, anyone on the LAN can subscribe to topic directories and read/control IoT states.
                        </p>
                        <div class="ency-mitigation">
                            <strong>Remediation:</strong> Enforce MQTT client authentication and shift traffic to port 8883 (MQTT over TLS) to encrypt sensor streams.
                        </div>
                    </div>
                </div>
            </section>

            <!-- Tab Content: Settings & Configuration -->
            <section id="tab-settings" class="tab-pane">
                <div class="glass settings-panel">
                    <h2>Sentinel Control Configuration</h2>
                    <p class="description">
                        Manage NVS parameters of the security audit scanner, adjust execution timing constraints, and set dashboard preferences.
                    </p>
                    
                    <form id="settings-form" class="settings-grid-form">
                        <div class="settings-card">
                            <h3>Scan Engine Parameters</h3>
                            <div class="form-group">
                                <label for="setting-timeout">Port Scan Timeout (ms)</label>
                                <input type="number" id="setting-timeout" min="10" max="1000" value="120" required>
                                <span class="help-text">Socket timeout for connection probes. Lower is faster but may miss ports on slow networks.</span>
                            </div>
                            
                            <div class="form-group">
                                <label for="setting-delay">Scan Delay between Hosts (ms)</label>
                                <input type="number" id="setting-delay" min="0" max="5000" value="0">
                                <span class="help-text">Time delay between successive device audits. Keeps CPU temperature low and prevents router flooding.</span>
                            </div>
                            
                            <div class="form-group">
                                <label for="setting-maxdev">Maximum Devices to Record</label>
                                <input type="number" id="setting-maxdev" min="5" max="250" value="50">
                                <span class="help-text">Caps discovered device array size to protect ESP32 memory bounds.</span>
                            </div>
                        </div>
                        
                        <div class="settings-card">
                            <h3>Audited TCP Ports</h3>
                            <div class="ports-checkbox-list">
                                <label class="checkbox-label"><input type="checkbox" id="port-chk-0" checked> Port 21 (FTP)</label>
                                <label class="checkbox-label"><input type="checkbox" id="port-chk-1" checked> Port 22 (SSH)</label>
                                <label class="checkbox-label"><input type="checkbox" id="port-chk-2" checked> Port 23 (Telnet)</label>
                                <label class="checkbox-label"><input type="checkbox" id="port-chk-3" checked> Port 53 (DNS)</label>
                                <label class="checkbox-label"><input type="checkbox" id="port-chk-4" checked> Port 80 (HTTP)</label>
                                <label class="checkbox-label"><input type="checkbox" id="port-chk-5" checked> Port 443 (HTTPS)</label>
                                <label class="checkbox-label"><input type="checkbox" id="port-chk-6" checked> Port 1883 (MQTT)</label>
                                <label class="checkbox-label"><input type="checkbox" id="port-chk-7" checked> Port 8080 (HTTP-Alt)</label>
                            </div>
                            <span class="help-text">Select ports to scan on active hosts. Disabling unused ports speeds up scanning.</span>
                        </div>
                        
                        <div class="settings-card">
                            <h3>UI and Visual Preferences</h3>
                            <div class="form-group">
                                <label for="setting-theme">Dashboard Color Theme</label>
                                <select id="setting-theme">
                                    <option value="dark">Cybernetic Dark Mode</option>
                                    <option value="light">High-Contrast Light Mode</option>
                                </select>
                            </div>
                            
                            <div class="form-group">
                                <label class="toggle-container">
                                    <input type="checkbox" id="setting-refresh" checked>
                                    <span class="toggle-slider"></span>
                                    Enable Real-time AJAX Polling
                                </label>
                            </div>
                        </div>

                        <div class="settings-card">
                            <h3>Wi-Fi Network Reconfiguration</h3>
                            <div class="form-group">
                                <label for="setting-ssid">SSID (Network Name)</label>
                                <input type="text" id="setting-ssid" placeholder="Enter network SSID name">
                            </div>
                            <div class="form-group">
                                <label for="setting-pass">SSID Password</label>
                                <input type="password" id="setting-pass" placeholder="Enter security passphrase">
                                <span class="help-text">SSID and password updates are stored directly in Wi-Fi memory, connecting upon reboot/reconnect.</span>
                            </div>
                        </div>
                        
                        <div class="settings-actions">
                            <button type="submit" class="btn btn-primary glow-button-blue" id="btn-save-settings">Save Configurations</button>
                            <span id="save-status-msg" class="status-msg"></span>
                        </div>
                    </form>
                </div>
            </section>
        </main>
    </div>

    <!-- Device Audit Modal Detail -->
    <div id="audit-modal" class="modal-overlay hidden">
        <div class="modal glass">
            <div class="modal-header">
                <h2>Vulnerability Analysis Report</h2>
                <button class="close-btn" id="modal-close">&times;</button>
            </div>
            <div class="modal-body" id="modal-content">
                <!-- Injected via JavaScript -->
            </div>
        </div>
    </div>

    <script src="app.js"></script>
</body>
</html>
)rawhtml";

// ==========================================
// style.css
// ==========================================
const char STYLE_CSS[] PROGMEM = R"rawcss(/*
 * IoT Sentinel - Stylesheet
 * Custom cybersecurity theme with glassmorphism, CSS variables, and fluid transitions.
 */

@import url('https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;500;600;700&family=JetBrains+Mono:wght@400;500&display=swap');

:root {
    --bg-dark: #070a13;
    --bg-panel: #0d1222;
    --bg-sidebar: #04060d;
    --glass-bg: rgba(15, 22, 42, 0.75);
    --glass-border: rgba(255, 255, 255, 0.05);
    --text-primary: #f8fafc;
    --text-secondary: #94a3b8;
    --text-muted: #64748b;
    
    /* Neon Risk/Accent Colors */
    --accent-blue: #3b82f6;
    --accent-blue-glow: rgba(59, 130, 246, 0.35);
    --accent-green: #10b981;
    --accent-green-glow: rgba(16, 185, 129, 0.35);
    --accent-amber: #f59e0b;
    --accent-amber-glow: rgba(245, 158, 11, 0.35);
    --accent-red: #ef4444;
    --accent-red-glow: rgba(239, 68, 68, 0.35);
    --accent-purple: #8b5cf6;
    --accent-purple-glow: rgba(139, 92, 246, 0.35);
    
    --font-sans: 'Outfit', -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
    --font-mono: 'JetBrains+Mono', monospace;
    --radius-lg: 12px;
    --radius-md: 8px;
    --transition-fast: 0.2s ease;
    --transition-normal: 0.3s ease;
}

* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

body {
    background-color: var(--bg-dark);
    color: var(--text-primary);
    font-family: var(--font-sans);
    min-height: 100vh;
    overflow-x: hidden;
}

/* App Container Layout */
.app-container {
    display: flex;
    min-height: 100vh;
}

/* Sidebar Navigation */
.sidebar {
    width: 280px;
    background-color: var(--bg-sidebar);
    border-right: 1px solid var(--glass-border);
    display: flex;
    flex-direction: column;
    padding: 24px;
    position: fixed;
    height: 100vh;
    z-index: 100;
}

.sidebar-header {
    display: flex;
    align-items: center;
    gap: 12px;
    margin-bottom: 40px;
}

.logo-icon {
    color: var(--accent-blue);
    display: flex;
    align-items: center;
    justify-content: center;
    filter: drop-shadow(0 0 8px var(--accent-blue-glow));
}

.logo-text h2 {
    font-size: 1.25rem;
    font-weight: 700;
    letter-spacing: 0.5px;
}

.logo-text span {
    font-size: 0.75rem;
    color: var(--text-secondary);
    text-transform: uppercase;
    letter-spacing: 1px;
}

.nav-menu {
    display: flex;
    flex-direction: column;
    gap: 8px;
    flex-grow: 1;
}

.nav-item {
    display: flex;
    align-items: center;
    gap: 12px;
    padding: 12px 16px;
    color: var(--text-secondary);
    text-decoration: none;
    font-size: 0.95rem;
    border-radius: var(--radius-md);
    transition: var(--transition-fast);
    border: 1px solid transparent;
}

.nav-item svg {
    transition: var(--transition-fast);
}

.nav-item:hover {
    color: var(--text-primary);
    background-color: rgba(255, 255, 255, 0.02);
}

.nav-item.active {
    color: var(--text-primary);
    background-color: rgba(59, 130, 246, 0.08);
    border-color: rgba(59, 130, 246, 0.15);
    font-weight: 500;
}

.nav-item.active svg {
    color: var(--accent-blue);
    filter: drop-shadow(0 0 4px var(--accent-blue-glow));
}

.system-status {
    border-top: 1px solid var(--glass-border);
    padding-top: 20px;
    display: flex;
    flex-direction: column;
    gap: 10px;
}

.status-row {
    display: flex;
    justify-content: space-between;
    font-size: 0.8rem;
}

.status-row .label {
    color: var(--text-muted);
}

.status-row .value {
    color: var(--text-secondary);
    font-family: var(--font-mono);
}

/* Main Content Area */
.main-content {
    margin-left: 280px;
    flex-grow: 1;
    padding: 32px 40px;
    width: calc(100% - 280px);
}

.top-bar {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 32px;
    border-bottom: 1px solid var(--glass-border);
    padding-bottom: 16px;
}

.top-bar h1 {
    font-size: 1.75rem;
    font-weight: 600;
    letter-spacing: -0.5px;
}

/* Scan Status Pill */
.scan-pill {
    display: flex;
    align-items: center;
    gap: 8px;
    padding: 6px 14px;
    border-radius: 20px;
    font-size: 0.85rem;
    font-weight: 500;
    border: 1px solid transparent;
}

.scan-pill.idle {
    background-color: rgba(148, 163, 184, 0.08);
    border-color: rgba(148, 163, 184, 0.15);
    color: var(--text-secondary);
}

.scan-pill.active {
    background-color: rgba(59, 130, 246, 0.12);
    border-color: rgba(59, 130, 246, 0.25);
    color: var(--accent-blue);
    box-shadow: 0 0 10px rgba(59, 130, 246, 0.1);
}

.pulse-dot {
    width: 8px;
    height: 8px;
    border-radius: 50%;
    background-color: currentColor;
}

.active .pulse-dot {
    animation: pulse 1.5s infinite;
}

@keyframes pulse {
    0% { transform: scale(0.9); opacity: 1; }
    50% { transform: scale(1.3); opacity: 0.4; }
    100% { transform: scale(0.9); opacity: 1; }
}

/* Glassmorphism panels */
.glass {
    background: var(--glass-bg);
    backdrop-filter: blur(12px);
    -webkit-backdrop-filter: blur(12px);
    border: 1px solid var(--glass-border);
    border-radius: var(--radius-lg);
}

/* Stats Cards */
.stats-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
    gap: 20px;
    margin-bottom: 28px;
}

.stat-card {
    background-color: var(--bg-panel);
    border: 1px solid var(--glass-border);
    border-radius: var(--radius-lg);
    padding: 20px;
    display: flex;
    align-items: center;
    gap: 16px;
    transition: transform var(--transition-fast), box-shadow var(--transition-fast);
}

.stat-card:hover {
    transform: translateY(-2px);
}

.border-glow-blue:hover { border-color: var(--accent-blue); box-shadow: 0 4px 20px rgba(59, 130, 246, 0.15); }
.border-glow-green:hover { border-color: var(--accent-green); box-shadow: 0 4px 20px rgba(16, 185, 129, 0.15); }
.border-glow-amber:hover { border-color: var(--accent-amber); box-shadow: 0 4px 20px rgba(245, 158, 11, 0.15); }
.border-glow-red:hover { border-color: var(--accent-red); box-shadow: 0 4px 20px rgba(239, 68, 68, 0.15); }

.card-icon {
    width: 48px;
    height: 48px;
    border-radius: var(--radius-md);
    display: flex;
    align-items: center;
    justify-content: center;
}

.blue-bg { background-color: rgba(59, 130, 246, 0.1); color: var(--accent-blue); }
.green-bg { background-color: rgba(16, 185, 129, 0.1); color: var(--accent-green); }
.amber-bg { background-color: rgba(245, 158, 11, 0.1); color: var(--accent-amber); }
.red-bg { background-color: rgba(239, 68, 68, 0.1); color: var(--accent-red); }

.card-details h3 {
    font-size: 0.8rem;
    font-weight: 500;
    color: var(--text-secondary);
    margin-bottom: 4px;
}

.card-details h2 {
    font-size: 1.6rem;
    font-weight: 700;
}

.card-details p {
    font-size: 0.75rem;
    color: var(--text-muted);
}

.threat-glow {
    color: var(--accent-red);
    text-shadow: 0 0 10px var(--accent-red-glow);
}

/* Mid Section Layout */
.dashboard-mid-row {
    display: grid;
    grid-template-columns: 1fr 1.25fr;
    gap: 20px;
    margin-bottom: 28px;
}

.chart-container, .quick-control {
    padding: 24px;
}

.chart-header h3, .quick-control h3 {
    font-size: 1rem;
    font-weight: 600;
    margin-bottom: 20px;
    letter-spacing: 0.25px;
}

.chart-content {
    display: flex;
    align-items: center;
    justify-content: space-around;
    gap: 20px;
}

.svg-chart-wrapper {
    position: relative;
    width: 160px;
    height: 160px;
}

.donut-chart {
    transform: rotate(-90deg);
}

.donut-segment {
    transition: stroke-dasharray var(--transition-normal);
}

.chart-text {
    transform: rotate(90deg);
    transform-origin: 50% 50%;
}

.chart-number {
    font-size: 0.5rem;
    font-weight: 700;
    fill: var(--text-primary);
    text-anchor: middle;
}

.chart-label {
    font-size: 0.22rem;
    fill: var(--text-secondary);
    text-anchor: middle;
    text-transform: uppercase;
    letter-spacing: 0.2px;
}

.chart-legend {
    display: flex;
    flex-direction: column;
    gap: 12px;
}

.legend-item {
    display: flex;
    align-items: center;
    gap: 10px;
    font-size: 0.85rem;
    color: var(--text-secondary);
}

.dot {
    width: 10px;
    height: 10px;
    border-radius: 50%;
}
.green-dot { background-color: var(--accent-green); box-shadow: 0 0 6px var(--accent-green-glow); }
.amber-dot { background-color: var(--accent-amber); box-shadow: 0 0 6px var(--accent-amber-glow); }
.red-dot { background-color: var(--accent-red); box-shadow: 0 0 6px var(--accent-red-glow); }

.quick-control {
    display: flex;
    flex-direction: column;
    justify-content: space-between;
}

.progress-details {
    margin-bottom: 20px;
}

.progress-info-row {
    display: flex;
    justify-content: space-between;
    font-size: 0.85rem;
    color: var(--text-secondary);
    margin-bottom: 8px;
    font-family: var(--font-mono);
}

.progress-bar-container {
    width: 100%;
    height: 8px;
    background-color: rgba(255, 255, 255, 0.03);
    border-radius: 4px;
    overflow: hidden;
}

.progress-fill {
    height: 100%;
    background: linear-gradient(90deg, var(--accent-blue), var(--accent-purple));
    box-shadow: 0 0 8px var(--accent-blue-glow);
    border-radius: 4px;
    transition: width 0.3s ease;
}

.action-buttons {
    display: flex;
    gap: 12px;
}

/* Buttons styling */
.btn {
    padding: 10px 20px;
    border-radius: var(--radius-md);
    border: none;
    font-size: 0.9rem;
    font-weight: 500;
    cursor: pointer;
    transition: var(--transition-fast);
    display: inline-flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
}

.btn-primary {
    background-color: var(--accent-blue);
    color: #ffffff;
}

.btn-primary:hover {
    background-color: #2563eb;
}

.glow-button {
    box-shadow: 0 4px 14px var(--accent-blue-glow);
}

.glow-button:hover {
    box-shadow: 0 6px 20px rgba(59, 130, 246, 0.5);
}

.glow-button-blue {
    background-color: var(--accent-purple);
    box-shadow: 0 4px 14px var(--accent-purple-glow);
    color: #ffffff;
}

.glow-button-blue:hover {
    background-color: #7c3aed;
    box-shadow: 0 6px 20px rgba(139, 92, 246, 0.5);
}

.btn-secondary {
    background-color: rgba(255, 255, 255, 0.03);
    color: var(--text-secondary);
    border: 1px solid var(--glass-border);
}

.btn-secondary:hover {
    color: var(--text-primary);
    background-color: rgba(255, 255, 255, 0.06);
}

.btn-mini {
    padding: 4px 10px;
    font-size: 0.75rem;
    border-radius: 4px;
    background-color: rgba(255, 255, 255, 0.05);
    color: var(--text-secondary);
    border: 1px solid var(--glass-border);
}

.btn-mini:hover {
    color: var(--text-primary);
    background-color: rgba(255, 255, 255, 0.1);
}

.btn-large {
    padding: 14px 28px;
    font-size: 1rem;
    font-weight: 600;
}

/* Spinner Icon */
.spinner-icon {
    width: 16px;
    height: 16px;
    border: 2px solid rgba(255, 255, 255, 0.25);
    border-top: 2px solid #ffffff;
    border-radius: 50%;
    animation: spin 0.8s linear infinite;
}

@keyframes spin {
    to { transform: rotate(360deg); }
}

.hidden {
    display: none !important;
}

/* Device Table Styling */
.table-container {
    padding: 24px;
}

.table-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 20px;
}

.table-header h3 {
    font-size: 1rem;
    font-weight: 600;
}

.table-filter {
    display: flex;
    gap: 16px;
    font-size: 0.85rem;
    color: var(--text-secondary);
}

.table-filter span {
    cursor: pointer;
    padding: 4px 8px;
    border-radius: 4px;
    transition: var(--transition-fast);
}

.table-filter span:hover {
    color: var(--text-primary);
}

.table-filter .active-filter {
    color: var(--accent-blue);
    background-color: rgba(59, 130, 246, 0.08);
    font-weight: 500;
}

.responsive-table {
    overflow-x: auto;
}

table {
    width: 100%;
    border-collapse: collapse;
    text-align: left;
}

th {
    padding: 12px 16px;
    color: var(--text-muted);
    font-weight: 500;
    font-size: 0.85rem;
    border-bottom: 1px solid var(--glass-border);
    text-transform: uppercase;
    letter-spacing: 0.5px;
}

td {
    padding: 16px;
    font-size: 0.9rem;
    border-bottom: 1px solid rgba(255, 255, 255, 0.02);
}

tr:last-child td {
    border-bottom: none;
}

tr:hover td {
    background-color: rgba(255, 255, 255, 0.01);
}

.table-empty {
    text-align: center;
    color: var(--text-muted);
    padding: 40px !important;
}

.td-ip {
    font-family: var(--font-mono);
    font-weight: 500;
    color: var(--text-primary);
}

.td-hostname {
    color: var(--text-secondary);
}

.td-ports {
    font-family: var(--font-mono);
    color: var(--accent-blue);
}

/* Risk Tag */
.risk-tag {
    display: inline-flex;
    padding: 4px 10px;
    border-radius: 12px;
    font-size: 0.75rem;
    font-weight: 600;
    letter-spacing: 0.25px;
}

.risk-tag.risk-low { background-color: rgba(16, 185, 129, 0.1); color: var(--accent-green); border: 1px solid rgba(16, 185, 129, 0.15); }
.risk-tag.risk-medium { background-color: rgba(245, 158, 11, 0.1); color: var(--accent-amber); border: 1px solid rgba(245, 158, 11, 0.15); }
.risk-tag.risk-high { background-color: rgba(239, 68, 68, 0.1); color: var(--accent-red); border: 1px solid rgba(239, 68, 68, 0.15); }

/* Score Bar */
.score-cell {
    display: flex;
    align-items: center;
    gap: 8px;
    width: 140px;
}

.score-num {
    font-size: 0.85rem;
    font-weight: 600;
    width: 24px;
    text-align: right;
}

.score-bar-bg {
    flex-grow: 1;
    height: 6px;
    background-color: rgba(255, 255, 255, 0.03);
    border-radius: 3px;
    overflow: hidden;
}

.score-bar-fill {
    height: 100%;
    border-radius: 3px;
}

.score-bar-fill.bar-low { background-color: var(--accent-green); }
.score-bar-fill.bar-medium { background-color: var(--accent-amber); }
.score-bar-fill.bar-high { background-color: var(--accent-red); }

/* Navigation Tab Display Control */
.tab-pane {
    display: none;
}

.tab-pane.active {
    display: block;
    animation: fadeIn 0.3s ease;
}

@keyframes fadeIn {
    from { opacity: 0; transform: translateY(6px); }
    to { opacity: 1; transform: translateY(0); }
}

/* Scanner Configuration tab panel */
.scanner-panel {
    padding: 32px;
}

.scanner-panel h2 {
    font-size: 1.5rem;
    font-weight: 600;
    margin-bottom: 12px;
}

.scanner-panel p.description {
    color: var(--text-secondary);
    margin-bottom: 30px;
    max-width: 700px;
    line-height: 1.6;
}

.scan-settings {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 24px;
    margin-bottom: 40px;
}

.settings-card {
    background-color: rgba(255, 255, 255, 0.01);
    border: 1px solid var(--glass-border);
    border-radius: var(--radius-md);
    padding: 20px;
}

.settings-card h3 {
    font-size: 0.95rem;
    font-weight: 600;
    margin-bottom: 16px;
    border-bottom: 1px solid var(--glass-border);
    padding-bottom: 10px;
}

.details-list {
    display: flex;
    flex-direction: column;
    gap: 12px;
}

.detail-item {
    display: flex;
    justify-content: space-between;
    font-size: 0.9rem;
}

.detail-item span {
    color: var(--text-secondary);
}

.detail-item strong {
    font-family: var(--font-mono);
}

.ports-tag-list {
    display: flex;
    flex-wrap: wrap;
    gap: 8px;
}

.port-tag {
    font-size: 0.8rem;
    padding: 6px 12px;
    border-radius: 6px;
    font-family: var(--font-mono);
    border: 1px solid transparent;
}
.port-tag.critical { background-color: rgba(239, 68, 68, 0.05); color: var(--accent-red); border-color: rgba(239, 68, 68, 0.1); }
.port-tag.warning { background-color: rgba(245, 158, 11, 0.05); color: var(--accent-amber); border-color: rgba(245, 158, 11, 0.1); }
.port-tag.info { background-color: rgba(59, 130, 246, 0.05); color: var(--accent-blue); border-color: rgba(59, 130, 246, 0.1); }

/* Live Terminal Log Viewer */
.terminal-container {
    display: flex;
    flex-direction: column;
    height: calc(100vh - 180px);
    overflow: hidden;
}

.terminal-header {
    background-color: #0b0f19;
    padding: 10px 16px;
    display: flex;
    align-items: center;
    border-bottom: 1px solid var(--glass-border);
}

.window-buttons {
    display: flex;
    gap: 6px;
    margin-right: 20px;
}

.window-buttons span {
    width: 10px;
    height: 10px;
    border-radius: 50%;
    background-color: #334155;
}

.terminal-title {
    font-size: 0.8rem;
    color: var(--text-muted);
    font-family: var(--font-mono);
    flex-grow: 1;
}

.terminal-body {
    background-color: #020617;
    flex-grow: 1;
    padding: 20px;
    overflow-y: auto;
    font-family: var(--font-mono);
    font-size: 0.85rem;
    line-height: 1.6;
    color: #10b981; /* Neon green terminal font */
    box-shadow: inset 0 0 20px rgba(0, 0, 0, 0.8);
}

.log-line {
    margin-bottom: 4px;
    white-space: pre-wrap;
    word-break: break-all;
}

.log-line.text-muted { color: var(--text-muted); }
.log-line.log-ok { color: #38ef7d; }
.log-line.log-info { color: #00c6ff; }
.log-line.log-warn { color: var(--accent-amber); }
.log-line.log-crit { color: var(--accent-red); font-weight: bold; text-shadow: 0 0 4px var(--accent-red-glow); }

/* Mitigation Encyclopedia */
.encyclopedia-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
    gap: 20px;
}

.ency-card {
    padding: 24px;
    display: flex;
    flex-direction: column;
}

.ency-header {
    display: flex;
    flex-direction: column;
    gap: 8px;
    margin-bottom: 14px;
    border-left: 3px solid transparent;
    padding-left: 12px;
}

.critical-border { border-color: var(--accent-red); }
.warning-border { border-color: var(--accent-amber); }

.risk-badge {
    align-self: flex-start;
    padding: 2px 8px;
    border-radius: 4px;
    font-size: 0.65rem;
    font-weight: 700;
    color: #ffffff;
}

.ency-header h3 {
    font-size: 1rem;
    font-weight: 600;
}

.ency-desc {
    font-size: 0.9rem;
    color: var(--text-secondary);
    line-height: 1.5;
    margin-bottom: 16px;
    flex-grow: 1;
}

.ency-mitigation {
    background-color: rgba(255, 255, 255, 0.01);
    border: 1px dashed var(--glass-border);
    padding: 14px;
    border-radius: var(--radius-md);
    font-size: 0.85rem;
    color: var(--text-primary);
    line-height: 1.5;
}

.ency-mitigation strong {
    color: var(--accent-green);
    display: block;
    margin-bottom: 4px;
}

/* Modals styling */
.modal-overlay {
    position: fixed;
    top: 0;
    left: 0;
    width: 100vw;
    height: 100vh;
    background-color: rgba(2, 6, 17, 0.8);
    display: flex;
    align-items: center;
    justify-content: center;
    z-index: 1000;
    backdrop-filter: blur(8px);
    transition: opacity var(--transition-fast);
}

.modal {
    width: 580px;
    max-width: 90%;
    max-height: 85vh;
    overflow-y: auto;
    border-radius: var(--radius-lg);
    box-shadow: 0 10px 30px rgba(0, 0, 0, 0.5);
    animation: modalSlide 0.3s ease;
}

@keyframes modalSlide {
    from { opacity: 0; transform: translateY(-20px) scale(0.95); }
    to { opacity: 1; transform: translateY(0) scale(1); }
}

.modal-header {
    padding: 20px 24px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    border-bottom: 1px solid var(--glass-border);
}

.modal-header h2 {
    font-size: 1.2rem;
    font-weight: 600;
}

.close-btn {
    background: none;
    border: none;
    color: var(--text-secondary);
    font-size: 1.75rem;
    cursor: pointer;
    transition: var(--transition-fast);
}

.close-btn:hover {
    color: var(--text-primary);
}

.modal-body {
    padding: 24px;
}

.modal-detail-row {
    display: flex;
    justify-content: space-between;
    margin-bottom: 14px;
    font-size: 0.95rem;
    border-bottom: 1px solid rgba(255, 255, 255, 0.02);
    padding-bottom: 8px;
}

.modal-detail-row .label {
    color: var(--text-secondary);
}

.modal-detail-row .value {
    font-weight: 500;
    font-family: var(--font-mono);
}

.modal-section-title {
    font-size: 0.85rem;
    font-weight: 600;
    text-transform: uppercase;
    letter-spacing: 0.5px;
    color: var(--text-muted);
    margin-top: 24px;
    margin-bottom: 12px;
}

.modal-ports-list {
    display: flex;
    flex-direction: column;
    gap: 8px;
    margin-bottom: 24px;
}

.modal-port-item {
    display: flex;
    align-items: center;
    gap: 12px;
    padding: 10px 14px;
    border-radius: var(--radius-md);
    background-color: rgba(255, 255, 255, 0.01);
    border: 1px solid var(--glass-border);
}

.modal-port-item .port-badge {
    font-family: var(--font-mono);
    font-weight: 600;
    font-size: 0.85rem;
    padding: 2px 6px;
    border-radius: 4px;
    background-color: rgba(255, 255, 255, 0.05);
}

.modal-port-item .port-desc {
    font-size: 0.85rem;
    color: var(--text-secondary);
    flex-grow: 1;
}

.modal-remediation {
    background-color: rgba(16, 185, 129, 0.03);
    border: 1px solid rgba(16, 185, 129, 0.15);
    padding: 16px;
    border-radius: var(--radius-md);
    font-size: 0.9rem;
    line-height: 1.5;
}

.modal-remediation h4 {
    color: var(--accent-green);
    margin-bottom: 6px;
    font-weight: 600;
}

/* Custom Scrollbar */
::-webkit-scrollbar {
    width: 6px;
    height: 6px;
}

::-webkit-scrollbar-track {
    background: transparent;
}

::-webkit-scrollbar-thumb {
    background: rgba(255, 255, 255, 0.1);
    border-radius: 3px;
}

::-webkit-scrollbar-thumb:hover {
    background: rgba(255, 255, 255, 0.25);
}

/* Header and Export Layout */
.header-actions {
    display: flex;
    align-items: center;
    justify-content: space-between;
    width: 100%;
    flex-wrap: wrap;
    gap: 16px;
}

.export-actions {
    display: flex;
    gap: 8px;
}

.btn-outline {
    background: transparent;
    border: 1px solid var(--glass-border);
    color: var(--text-secondary);
}

.btn-outline:hover {
    background: rgba(255, 255, 255, 0.05);
    color: var(--text-primary);
    border-color: var(--text-secondary);
}

/* Settings Form Layout */
.settings-panel {
    padding: 32px;
}

.settings-panel h2 {
    font-size: 1.5rem;
    font-weight: 600;
    margin-bottom: 12px;
}

.settings-panel p.description {
    color: var(--text-secondary);
    margin-bottom: 30px;
    max-width: 700px;
    line-height: 1.6;
}

.settings-grid-form {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
    gap: 24px;
}

.settings-card {
    background-color: var(--bg-panel);
    border: 1px solid var(--glass-border);
    border-radius: var(--radius-lg);
    padding: 24px;
    display: flex;
    flex-direction: column;
    gap: 16px;
}

.settings-card h3 {
    font-size: 1rem;
    font-weight: 600;
    margin-bottom: 16px;
    border-bottom: 1px solid var(--glass-border);
    padding-bottom: 10px;
}

.form-group {
    display: flex;
    flex-direction: column;
    gap: 8px;
}

.form-group label {
    font-size: 0.85rem;
    font-weight: 500;
    color: var(--text-secondary);
}

.form-group input[type="number"],
.form-group input[type="text"],
.form-group input[type="password"],
.form-group select {
    background-color: rgba(0, 0, 0, 0.2);
    border: 1px solid var(--glass-border);
    border-radius: var(--radius-md);
    padding: 10px 14px;
    color: var(--text-primary);
    font-family: var(--font-sans);
    font-size: 0.9rem;
    transition: var(--transition-fast);
}

.form-group input:focus,
.form-group select:focus {
    outline: none;
    border-color: var(--accent-blue);
    box-shadow: 0 0 0 2px var(--accent-blue-glow);
}

.help-text {
    font-size: 0.75rem;
    color: var(--text-muted);
    line-height: 1.4;
}

/* Ports Checkbox List */
.ports-checkbox-list {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 12px;
}

.checkbox-label {
    display: flex;
    align-items: center;
    gap: 8px;
    font-size: 0.9rem;
    color: var(--text-secondary);
    cursor: pointer;
    user-select: none;
}

.checkbox-label input[type="checkbox"] {
    accent-color: var(--accent-blue);
    width: 16px;
    height: 16px;
    cursor: pointer;
}

/* Toggle Switch */
.toggle-container {
    display: flex;
    align-items: center;
    gap: 12px;
    font-size: 0.9rem;
    color: var(--text-secondary);
    cursor: pointer;
    position: relative;
    user-select: none;
}

/* Actions and messages */
.settings-actions {
    grid-column: 1 / -1;
    display: flex;
    align-items: center;
    gap: 16px;
    margin-top: 12px;
}

.status-msg {
    font-size: 0.9rem;
    font-weight: 500;
    transition: opacity 0.3s ease;
}

.status-msg.success { color: var(--accent-green); }
.status-msg.error { color: var(--accent-red); }

/* Stop Button Glow */
.btn-danger {
    background-color: var(--accent-red);
    color: #ffffff;
}

.btn-danger:hover {
    background-color: #dc2626;
}

.glow-button-red {
    box-shadow: 0 4px 14px var(--accent-red-glow);
}

.glow-button-red:hover {
    box-shadow: 0 6px 20px rgba(239, 68, 68, 0.5);
}

/* Light theme overrides */
body.light-theme {
    --bg-dark: #f1f5f9;
    --bg-panel: #ffffff;
    --bg-sidebar: #0f172a; /* Keep sidebar dark for cool contrast */
    --glass-bg: rgba(255, 255, 255, 0.75);
    --glass-border: rgba(15, 23, 42, 0.08);
    --text-primary: #0f172a;
    --text-secondary: #475569;
    --text-muted: #94a3b8;
}

body.light-theme .glass {
    box-shadow: 0 4px 20px rgba(15, 23, 42, 0.05);
}

body.light-theme .terminal-body {
    background-color: #0f172a;
    color: #38ef7d;
}

body.light-theme .form-group input,
body.light-theme .form-group select {
    background-color: #f8fafc;
}

/* Header and Menu Toggle Layout */
.top-bar-left {
    display: flex;
    align-items: center;
    gap: 12px;
}

.mobile-only {
    display: none !important;
}

/* Responsive Media Queries */
@media screen and (max-width: 1024px) {
    .dashboard-mid-row {
        grid-template-columns: 1fr;
    }
}

@media screen and (max-width: 768px) {
    .mobile-only {
        display: inline-flex !important;
        align-items: center;
        justify-content: center;
        width: 36px;
        height: 36px;
        padding: 0;
        border-radius: var(--radius-md);
    }
    
    .sidebar {
        position: fixed;
        left: 0;
        top: 0;
        height: 100vh;
        width: 280px;
        transform: translateX(-100%);
        transition: transform var(--transition-normal);
        box-shadow: 10px 0 30px rgba(0, 0, 0, 0.5);
    }
    
    .sidebar.open {
        transform: translateX(0);
    }
    
    .main-content {
        margin-left: 0;
        width: 100%;
        padding: 20px;
    }
    
    .top-bar {
        padding-bottom: 12px;
        margin-bottom: 24px;
    }
    
    .top-bar h1 {
        font-size: 1.4rem;
    }
    
    .stats-grid {
        grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
        gap: 16px;
    }
    
    .stat-card {
        padding: 16px;
    }
    
    .chart-content {
        flex-direction: column;
        text-align: center;
        gap: 24px;
    }
    
    .chart-legend {
        flex-direction: row;
        justify-content: center;
        flex-wrap: wrap;
        gap: 16px;
    }
    
    .settings-grid-form {
        grid-template-columns: 1fr;
    }
    
    .action-buttons {
        flex-direction: column;
        width: 100%;
        gap: 8px;
    }
    
    .action-buttons .btn {
        width: 100%;
    }
    
    .header-actions {
        flex-direction: column;
        align-items: flex-start;
        gap: 12px;
    }
    
    .export-actions {
        width: 100%;
        display: flex;
        gap: 8px;
    }
    
    .export-actions .btn {
        flex: 1;
    }
}


)rawcss";

// ==========================================
// app.js
// ==========================================
const char APP_JS[] PROGMEM = R"rawjs(/*
 * IoT Sentinel - Frontend Orchestrations
 * Implements client-side routing, AJAX polling, REST API communication, NVS settings configuration,
 * client-side JSON/CSV report generation, and dynamic theme switching.
 */

// Application State
let appState = {
    scanning: false,
    progress: 0,
    currentIp: '',
    currentPort: '',
    statusText: 'System Idle',
    devices: [],
    logs: [],
    sysInfo: {
        ip: '...',
        ssid: '...',
        freeRam: 0
    },
    metrics: {
        grade: 'A',
        gradeSub: 'All Clear',
        hostsOnline: 0,
        criticalThreats: 0,
        mediumRisks: 0,
        lowRisks: 0,
        score: 100
    },
    settings: {
        timeout: 120,
        delay: 0,
        ports: 255, // 0xFF
        theme: 'dark',
        refresh: true,
        maxdev: 50
    },
    activeFilter: 'all' // 'all' or 'high'
};

// DOM Elements
const elements = {
    navItems: document.querySelectorAll('.nav-item'),
    tabPanes: document.querySelectorAll('.tab-pane'),
    pageTitle: document.getElementById('page-title'),
    
    // System Status
    statusIp: document.getElementById('status-ip'),
    statusSsid: document.getElementById('status-ssid'),
    statusRam: document.getElementById('status-ram'),
    scanPill: document.getElementById('scan-indicator'),
    scanStatusText: document.getElementById('scan-status-text'),
    
    // Metrics
    metricGrade: document.getElementById('metric-grade'),
    metricGradeSub: document.getElementById('metric-grade-sub'),
    metricHosts: document.getElementById('metric-hosts'),
    metricCritical: document.getElementById('metric-critical'),
    metricMedium: document.getElementById('metric-medium'),
    
    // Progress
    progressIp: document.getElementById('progress-ip'),
    progressPercent: document.getElementById('progress-percent'),
    progressBar: document.getElementById('progress-bar'),
    btnStart: document.getElementById('btn-start-scan'),
    btnBigScan: document.getElementById('btn-big-scan'),
    btnStopScan: document.getElementById('btn-stop-scan'),
    btnSpinner: document.getElementById('scan-spinner'),
    btnClear: document.getElementById('btn-clear-scan'),
    
    // Donut Chart
    chartPct: document.getElementById('chart-pct'),
    donutLow: document.getElementById('donut-segment-low'),
    donutMed: document.getElementById('donut-segment-med'),
    donutHigh: document.getElementById('donut-segment-high'),
    
    // Table
    deviceTableBody: document.getElementById('device-table-body'),
    countAll: document.getElementById('count-all'),
    countHigh: document.getElementById('count-high'),
    filterAll: document.getElementById('filter-all'),
    filterHigh: document.getElementById('filter-high'),
    
    // Export Actions
    btnExportJson: document.getElementById('btn-export-json'),
    btnExportCsv: document.getElementById('btn-export-csv'),
    
    // Subnet Pane Settings
    submask: document.getElementById('scan-submask'),
    iprange: document.getElementById('scan-iprange'),
    
    // Logs Console
    logConsole: document.getElementById('log-console'),
    btnClearLogs: document.getElementById('btn-clear-logs'),
    
    // Settings Pane Form
    settingsForm: document.getElementById('settings-form'),
    settingTimeout: document.getElementById('setting-timeout'),
    settingDelay: document.getElementById('setting-delay'),
    settingMaxdev: document.getElementById('setting-maxdev'),
    settingTheme: document.getElementById('setting-theme'),
    settingRefresh: document.getElementById('setting-refresh'),
    settingSsid: document.getElementById('setting-ssid'),
    settingPass: document.getElementById('setting-pass'),
    saveStatusMsg: document.getElementById('save-status-msg'),
    
    // Modal
    modalOverlay: document.getElementById('audit-modal'),
    modalClose: document.getElementById('modal-close'),
    modalContent: document.getElementById('modal-content')
};

// Port Explanations Dictionary for Modal Reports
const PORT_DICTIONARY = {
    21: { name: 'FTP', risk: 'HIGH', desc: 'Cleartext File Transfer Protocol. Transmits login credentials and files in plaintext, exposing authentication handshakes to LAN sniffing.' },
    22: { name: 'SSH', risk: 'LOW', desc: 'Secure Shell. Secure encrypted connection channel, but represents an entry point susceptible to dictionary brute-force attacks if weak credentials are used.' },
    23: { name: 'Telnet', risk: 'HIGH', desc: 'Telnet Remote Console. Transmits session keystrokes, commands, and passwords in cleartext. Highly vulnerable to MITM interception.' },
    53: { name: 'DNS', risk: 'LOW', desc: 'Domain Name System. Resolves domain queries. If misconfigured, can be used for amplification attacks or local cache poisoning.' },
    80: { name: 'HTTP', risk: 'MEDIUM', desc: 'Unencrypted Web Interface. Credentials entered on administration pages are sent without SSL/TLS, making them visible to attackers.' },
    443: { name: 'HTTPS', risk: 'LOW', desc: 'Encrypted Web Interface. Generally secure, but administration portals should require strong credentials and have rate limiting.' },
    1883: { name: 'MQTT', risk: 'MEDIUM', desc: 'Unencrypted Message Queue. Brokers running on port 1883 expose smart home telemetry data and accept controls without encryption.' },
    8080: { name: 'HTTP-Alt', risk: 'MEDIUM', desc: 'Alternative Web Admin Console. Often hosts test programs, server panels, or secondary IoT configuration layers without encryption.' }
};

// -------------------------------------------------------------
// Navigation & Routing
// -------------------------------------------------------------
function setupNavigation() {
    elements.navItems.forEach(item => {
        item.addEventListener('click', function(e) {
            e.preventDefault();
            
            // Remove active classes
            elements.navItems.forEach(n => n.classList.remove('active'));
            elements.tabPanes.forEach(t => t.classList.remove('active'));
            
            // Add active classes to current tab
            this.classList.add('active');
            const tabId = this.getAttribute('href').replace('#', 'tab-');
            const targetTab = document.getElementById(tabId);
            if (targetTab) {
                targetTab.classList.add('active');
            }
            
            // Update Topbar Title
            const tabName = this.textContent.trim();
            elements.pageTitle.textContent = tabName === "Mitigation Info" ? "Mitigation Encyclopedia" : tabName;
        });
    });
}

// Apply Selected Theme (Dark or Light)
function applyTheme(themeName) {
    if (themeName === 'light') {
        document.body.classList.add('light-theme');
    } else {
        document.body.classList.remove('light-theme');
    }
}

// -------------------------------------------------------------
// Core REST API Calls (AJAX Polling)
// -------------------------------------------------------------
let pollIntervalId = null;

async function fetchStatus() {
    try {
        // Fetch current scan execution status
        const statusRes = await fetch('/api/status');
        if (!statusRes.ok) throw new Error('API status request failed');
        const statusData = await statusRes.json();
        
        // Fetch discovered devices list
        const devicesRes = await fetch('/api/devices');
        if (!devicesRes.ok) throw new Error('API devices request failed');
        const devicesData = await devicesRes.json();
        
        // Fetch log streams
        const logsRes = await fetch('/api/logs');
        if (!logsRes.ok) throw new Error('API logs request failed');
        const logsData = await logsRes.json();
        
        // Update local State parameters
        appState.scanning = statusData.scanning;
        appState.progress = statusData.progress;
        appState.currentIp = statusData.current_ip || '';
        appState.currentPort = statusData.current_port || '';
        appState.statusText = statusData.status || 'IDLE';
        appState.devices = devicesData || [];
        
        appState.sysInfo.ip = statusData.sys_ip || '...';
        appState.sysInfo.ssid = statusData.sys_ssid || '...';
        appState.sysInfo.freeRam = statusData.sys_free_ram || 0;
        
        // Update aggregates
        appState.metrics.grade = statusData.net_grade || 'A';
        appState.metrics.score = statusData.net_score || 100;
        appState.metrics.hostsOnline = statusData.hosts_found || 0;
        
        if (statusData.risk_distribution) {
            appState.metrics.lowRisks = statusData.risk_distribution.low || 0;
            appState.metrics.mediumRisks = statusData.risk_distribution.medium || 0;
            appState.metrics.criticalThreats = statusData.risk_distribution.high || 0;
        }

        // Add new log lines
        if (logsData && logsData.length > 0) {
            updateLogsConsole(logsData);
        }
        
        // Render updates to UI
        updateUI();
        
    } catch (error) {
        console.error('Error polling dashboard status API:', error);
        elements.statusIp.textContent = "Offline";
    }
}

function startPolling(pollingEnabled = true) {
    if (pollIntervalId) {
        clearInterval(pollIntervalId);
        pollIntervalId = null;
    }
    
    if (pollingEnabled) {
        fetchStatus(); // First execution
        pollIntervalId = setInterval(fetchStatus, 1000);
    }
}

// Fetch board settings from NVS
async function loadSettings() {
    try {
        const response = await fetch('/api/settings');
        if (!response.ok) throw new Error('Failed to load settings');
        const data = await response.json();
        
        appState.settings = data;
        
        // Populate settings form
        elements.settingTimeout.value = data.timeout;
        elements.settingDelay.value = data.delay;
        elements.settingMaxdev.value = data.maxdev;
        elements.settingTheme.value = data.theme;
        elements.settingRefresh.checked = data.refresh;
        
        // Unpack enabled ports bitmask (bit 0 = port 21, bit 1 = port 22...)
        for (let i = 0; i < 8; i++) {
            const chk = document.getElementById(`port-chk-${i}`);
            if (chk) {
                chk.checked = (data.ports & (1 << i)) !== 0;
            }
        }
        
        applyTheme(data.theme);
        startPolling(data.refresh);
        
    } catch (error) {
        console.error('Error fetching configuration settings:', error);
    }
}

// Start active subnet scan
async function triggerStartScan() {
    try {
        elements.btnSpinner.classList.remove('hidden');
        elements.btnStart.disabled = true;
        elements.btnBigScan.disabled = true;
        elements.btnStopScan.classList.remove('hidden');
        
        const response = await fetch('/api/scan/start', { method: 'POST' });
        if (!response.ok) throw new Error('Start command failed');
        
        appendSingleLogLine("[CLIENT] Scan command transmitted successfully to ESP32.", "log-info");
        appState.scanning = true;
        updateUI();
    } catch (error) {
        console.error('Error sending scan start command:', error);
        appendSingleLogLine("[CLIENT] ERROR: Failed to transmit scan command.", "log-crit");
        elements.btnSpinner.classList.add('hidden');
        elements.btnStart.disabled = false;
        elements.btnBigScan.disabled = false;
    }
}

// Cancel active scan
async function triggerStopScan() {
    try {
        const response = await fetch('/api/scan/stop', { method: 'POST' });
        if (!response.ok) throw new Error('Stop command failed');
        
        appendSingleLogLine("[CLIENT] Scan cancellation request sent.", "log-warn");
        appState.scanning = false;
        elements.btnStopScan.classList.add('hidden');
        updateUI();
    } catch (error) {
        console.error('Error canceling network scan:', error);
    }
}

// Reset scan records
async function triggerClearScan() {
    try {
        const response = await fetch('/api/clear', { method: 'POST' });
        if (!response.ok) throw new Error('Clear command failed');
        
        appState.devices = [];
        appState.progress = 0;
        appState.currentIp = '';
        appState.logs = [];
        elements.logConsole.innerHTML = '<div class="log-line text-muted">[SYSTEM] Scan results cleared by user. Console empty.</div>';
        
        updateUI();
        appendSingleLogLine("[CLIENT] Scan records reset.", "log-info");
    } catch (error) {
        console.error('Error clearing scanner logs:', error);
    }
}

// -------------------------------------------------------------
// UI Render Engines
// -------------------------------------------------------------
function updateUI() {
    // 1. Sidebar status updates
    elements.statusIp.textContent = appState.sysInfo.ip;
    elements.statusSsid.textContent = appState.sysInfo.ssid;
    elements.statusRam.textContent = Math.round(appState.sysInfo.freeRam / 1024) + " KB";
    
    // Subnet page details
    elements.submask.textContent = wifiManagerIsAPMode() ? "255.255.255.0" : "255.255.255.0";
    const ipBase = appState.sysInfo.ip.substring(0, appState.sysInfo.ip.lastIndexOf('.'));
    elements.iprange.textContent = ipBase ? `${ipBase}.1 - ${ipBase}.254` : 'Unknown';

    // 2. Scan Pill Indicator (Idle/Active state machine representation)
    if (appState.scanning) {
        elements.scanPill.className = "scan-pill active";
        elements.scanStatusText.textContent = `Scanning [${appState.statusText}]`;
        elements.btnSpinner.classList.remove('hidden');
        elements.btnStart.disabled = true;
        elements.btnBigScan.disabled = true;
        elements.btnStopScan.classList.remove('hidden');
    } else {
        elements.scanPill.className = "scan-pill idle";
        elements.scanStatusText.textContent = `System Ready [${appState.statusText}]`;
        elements.btnSpinner.classList.add('hidden');
        elements.btnStart.disabled = false;
        elements.btnBigScan.disabled = false;
        elements.btnStopScan.classList.add('hidden');
    }

    // 3. Progress details
    elements.progressPercent.textContent = appState.progress + "%";
    elements.progressBar.style.width = appState.progress + "%";
    if (appState.scanning) {
        elements.progressIp.textContent = `${appState.statusText}: ${appState.currentIp} | ${appState.currentPort}`;
    } else {
        if (appState.statusText === 'COMPLETED') {
            elements.progressIp.textContent = "Subnet Security Audit Completed";
        } else if (appState.statusText === 'STOPPED') {
            elements.progressIp.textContent = "Audit Manually Aborted";
        } else {
            elements.progressIp.textContent = "Scanner Ready";
        }
    }

    // 4. Metrics & Threat Cards
    elements.metricHosts.textContent = appState.metrics.hostsOnline;
    elements.metricCritical.textContent = appState.metrics.criticalThreats;
    elements.metricMedium.textContent = appState.metrics.mediumRisks;
    
    // Pulse critical threats red if present
    if (appState.metrics.criticalThreats > 0) {
        elements.metricCritical.classList.add('threat-glow');
    } else {
        elements.metricCritical.classList.remove('threat-glow');
    }

    elements.metricGrade.textContent = appState.metrics.grade;
    
    // Set descriptive grade subtext
    let gradeSub = "All clear";
    if (appState.metrics.grade === 'F') {
        gradeSub = "Critical vulnerabilities active!";
    } else if (appState.metrics.grade === 'D') {
        gradeSub = "Significant vulnerability exposure";
    } else if (appState.metrics.grade === 'C') {
        gradeSub = "Exposed ports need patching";
    } else if (appState.metrics.grade === 'B') {
        gradeSub = "Secured configuration";
    } else {
        gradeSub = "No open vulnerabilities found";
    }
    elements.metricGradeSub.textContent = gradeSub;
    
    // Style grade text color
    elements.metricGrade.className = ""; // Reset
    if (appState.metrics.grade === "F") elements.metricGrade.classList.add("threat-glow");
    else if (appState.metrics.grade === "C" || appState.metrics.grade === "D") elements.metricGrade.style.color = "var(--accent-amber)";
    else elements.metricGrade.style.color = "var(--accent-green)";

    // 5. Update Donut Security Index Chart
    updateDonutChart(appState.metrics.lowRisks, appState.metrics.mediumRisks, appState.metrics.criticalThreats);

    // 6. Device List Filter Counters
    elements.countAll.textContent = appState.metrics.hostsOnline;
    elements.countHigh.textContent = appState.metrics.criticalThreats;

    // 7. Render Device Table rows
    renderDeviceTable();
}

function wifiManagerIsAPMode() {
    return appState.sysInfo.ip === '192.168.4.1';
}

function updateDonutChart(low, med, high) {
    const total = low + med + high;
    if (total === 0) {
        elements.donutLow.setAttribute('stroke-dasharray', '100 0');
        elements.donutMed.setAttribute('stroke-dasharray', '0 100');
        elements.donutHigh.setAttribute('stroke-dasharray', '0 100');
        elements.chartPct.textContent = "100%";
        document.querySelector('.chart-label').textContent = "Secure";
        return;
    }

    // Convert counts to percentages
    const pctLow = Math.round((low / total) * 100);
    const pctMed = Math.round((med / total) * 100);
    const pctHigh = Math.round((high / total) * 100);

    elements.chartPct.textContent = `${appState.metrics.score}%`;
    document.querySelector('.chart-label').textContent = "Security Score";

    // Set SVG segment dashes
    elements.donutLow.setAttribute('stroke-dasharray', `${pctLow} ${100 - pctLow}`);
    elements.donutLow.setAttribute('stroke-dashoffset', '25');

    elements.donutMed.setAttribute('stroke-dasharray', `${pctMed} ${100 - pctMed}`);
    elements.donutMed.setAttribute('stroke-dashoffset', `${25 - pctLow}`);

    elements.donutHigh.setAttribute('stroke-dasharray', `${pctHigh} ${100 - pctHigh}`);
    elements.donutHigh.setAttribute('stroke-dashoffset', `${25 - pctLow - pctMed}`);
}

function renderDeviceTable() {
    elements.deviceTableBody.innerHTML = '';
    
    // Filter devices
    let filtered = appState.devices;
    if (appState.activeFilter === 'high') {
        filtered = appState.devices.filter(d => d.risk === 'HIGH');
    }

    if (filtered.length === 0) {
        elements.deviceTableBody.innerHTML = `
            <tr>
                <td colspan="6" class="table-empty">
                    ${appState.activeFilter === 'high' ? 'No critical vulnerability threats found.' : 'No devices found. Perform a subnet scan.'}
                </td>
            </tr>
        `;
        return;
    }

    filtered.forEach(dev => {
        const tr = document.createElement('tr');
        const riskClass = dev.risk === 'HIGH' ? 'risk-high' : (dev.risk === 'MEDIUM' ? 'risk-medium' : 'risk-low');
        const scoreClass = dev.risk === 'HIGH' ? 'bar-high' : (dev.risk === 'MEDIUM' ? 'bar-medium' : 'bar-low');
        
        tr.innerHTML = `
            <td class="td-ip">${dev.ip}</td>
            <td class="td-hostname">
                <strong>${dev.type || 'Unknown'}</strong><br>
                <small>${dev.name || 'Unknown Host'}</small>
            </td>
            <td class="td-ports">${dev.open_ports.length > 0 ? dev.open_ports.join(', ') : 'None'}</td>
            <td><span class="risk-tag ${riskClass}">${dev.risk}</span></td>
            <td>
                <div class="score-cell">
                    <span class="score-num">${dev.score}</span>
                    <div class="score-bar-bg">
                        <div class="score-bar-fill ${scoreClass}" style="width: ${dev.score}%"></div>
                    </div>
                </div>
                <small style="color: var(--text-muted); font-size: 0.7rem; font-family: var(--font-mono)">RTT: ${dev.rtt}ms</small>
            </td>
            <td>
                <button class="btn btn-mini btn-view-audit" data-ip="${dev.ip}">View Audit</button>
            </td>
        `;
        
        elements.deviceTableBody.appendChild(tr);
    });

    // Add click listeners to View Audit buttons
    document.querySelectorAll('.btn-view-audit').forEach(btn => {
        btn.addEventListener('click', function() {
            const ip = this.getAttribute('data-ip');
            const device = appState.devices.find(d => d.ip === ip);
            if (device) openAuditModal(device);
        });
    });
}

// -------------------------------------------------------------
// Logs Terminal Streamer
// -------------------------------------------------------------
function updateLogsConsole(newLogs) {
    const isAtBottom = elements.logConsole.scrollHeight - elements.logConsole.clientHeight <= elements.logConsole.scrollTop + 30;
    
    newLogs.forEach(log => {
        // Prevent duplicate lines in console
        if (appState.logs.includes(log)) return;
        appState.logs.push(log);
        
        const line = document.createElement('div');
        line.className = 'log-line';
        
        if (log.includes('[OK]')) {
            line.classList.add('log-ok');
        } else if (log.includes('[WARN]')) {
            line.classList.add('log-warn');
        } else if (log.includes('[CRITICAL]')) {
            line.classList.add('log-crit');
        } else if (log.includes('[INFO]')) {
            line.classList.add('log-info');
        } else {
            line.classList.add('text-muted');
        }
        
        line.textContent = log;
        elements.logConsole.appendChild(line);
    });

    if (isAtBottom) {
        elements.logConsole.scrollTop = elements.logConsole.scrollHeight;
    }
}

function appendSingleLogLine(text, cssClass = '') {
    const line = document.createElement('div');
    line.className = `log-line ${cssClass}`;
    const timestamp = new Date().toTimeString().split(' ')[0];
    line.textContent = `[${timestamp}] ${text}`;
    elements.logConsole.appendChild(line);
    elements.logConsole.scrollTop = elements.logConsole.scrollHeight;
}

// -------------------------------------------------------------
// Modal View Auditing
// -------------------------------------------------------------
function openAuditModal(dev) {
    let portsHtml = '';
    
    if (dev.open_ports && dev.open_ports.length > 0) {
        dev.open_ports.forEach(port => {
            const info = PORT_DICTIONARY[port] || { name: `Unknown Service`, risk: 'UNKNOWN', desc: 'No detailed profile exists for this custom port. Treat open channels with caution.' };
            const tagClass = info.risk === 'HIGH' ? 'risk-high' : (info.risk === 'MEDIUM' ? 'risk-medium' : 'risk-low');
            
            portsHtml += `
                <div class="modal-port-item">
                    <span class="port-badge">Port ${port}</span>
                    <strong class="risk-tag ${tagClass}">${info.name}</strong>
                    <div class="port-desc">${info.desc}</div>
                </div>
            `;
        });
    } else {
        portsHtml = `<div class="table-empty">No exposed port channels identified during scan. Device is shielded.</div>`;
    }

    // Modal Details Grid
    elements.modalContent.innerHTML = `
        <div class="modal-detail-row">
            <span class="label">IP Address:</span>
            <span class="value">${dev.ip}</span>
        </div>
        <div class="modal-detail-row">
            <span class="label">Hardware Profile:</span>
            <span class="value">${dev.name || 'Unknown IoT Node'} (${dev.type || 'Unknown'})</span>
        </div>
        <div class="modal-detail-row">
            <span class="label">Risk Rating:</span>
            <span class="value risk-tag ${dev.risk === 'HIGH' ? 'risk-high' : (dev.risk === 'MEDIUM' ? 'risk-medium' : 'risk-low')}">${dev.risk} (${dev.score}/100)</span>
        </div>
        <div class="modal-detail-row">
            <span class="label">Response Time:</span>
            <span class="value">${dev.rtt} ms</span>
        </div>

        <div class="modal-section-title">Open Port Analysis</div>
        <div class="modal-ports-list">
            ${portsHtml}
        </div>

        <div class="modal-section-title">Remediation Roadmap</div>
        <div class="modal-remediation">
            ${dev.risk === 'HIGH' 
                ? '<h4>Action Required: Immediate Isolation</h4><p>This device exposes unsecure legacy sockets (like Telnet/FTP). Disable these protocols in the hardware config or block them using your firewall/router access lists (ACLs).</p>' 
                : (dev.risk === 'MEDIUM' 
                    ? '<h4>Action Recommended: Secure configurations</h4><p>Exposed web consoles or sensor streams present. Enforce HTTPS, change manufacturer default logins to custom passwords, and update firmware to block exploit vectors.</p>' 
                    : '<h4>Status: Secure Environment</h4><p>Device has standard secure parameters. No action is required. Monitor port logs regularly.</p>')
            }
        </div>
    `;

    elements.modalOverlay.classList.remove('hidden');
}

function closeAuditModal() {
    elements.modalOverlay.classList.add('hidden');
}

// -------------------------------------------------------------
// Report Downloads (CSV & JSON Exports)
// -------------------------------------------------------------
function downloadJsonReport() {
    if (appState.devices.length === 0) {
        alert("No device records available to export. Run a subnet scan first.");
        return;
    }
    
    const reportData = {
        platform: "IoT Sentinel Network Auditor",
        timestamp: new Date().toISOString(),
        sys_info: {
            ip: appState.sysInfo.ip,
            ssid: appState.sysInfo.ssid,
            free_ram_bytes: appState.sysInfo.freeRam
        },
        aggregates: {
            grade: appState.metrics.grade,
            score: appState.metrics.score,
            total_hosts: appState.metrics.hostsOnline,
            critical_threats: appState.metrics.criticalThreats,
            medium_risks: appState.metrics.mediumRisks,
            low_risks: appState.metrics.lowRisks
        },
        devices: appState.devices,
        logs: appState.logs
    };
    
    const blob = new Blob([JSON.stringify(reportData, null, 4)], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `iot_sentinel_report_${new Date().getTime()}.json`;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
}

function downloadCsvReport() {
    if (appState.devices.length === 0) {
        alert("No device records available to export. Run a subnet scan first.");
        return;
    }
    
    let csv = "IP Address,Device Type,Hardware Name,Open Ports,Risk Rating,Score,Response Time (ms)\r\n";
    
    appState.devices.forEach(d => {
        const portsStr = d.open_ports.join(';');
        csv += `"${d.ip}","${d.type || 'Unknown'}","${d.name || 'Unknown'}","${portsStr}","${d.risk}",${d.score},${d.rtt || 0}\r\n`;
    });
    
    const blob = new Blob([csv], { type: 'text/csv;charset=utf-8;' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `iot_sentinel_report_${new Date().getTime()}.csv`;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
}

// -------------------------------------------------------------
// NVS Settings Form Handlers
// -------------------------------------------------------------
async function saveSettings(e) {
    e.preventDefault();
    elements.saveStatusMsg.textContent = "Transmitting configuration...";
    elements.saveStatusMsg.className = "status-msg";
    
    // Calculate enabled ports bitmask from checkboxes
    let portsBitmask = 0;
    for (let i = 0; i < 8; i++) {
        const chk = document.getElementById(`port-chk-${i}`);
        if (chk && chk.checked) {
            portsBitmask |= (1 << i);
        }
    }
    
    // Prepare URL-encoded form args
    const params = new URLSearchParams();
    params.append('timeout', elements.settingTimeout.value);
    params.append('delay', elements.settingDelay.value);
    params.append('maxdev', elements.settingMaxdev.value);
    params.append('theme', elements.settingTheme.value);
    params.append('refresh', elements.settingRefresh.checked ? 'true' : 'false');
    params.append('ports', portsBitmask);
    
    // Wi-Fi configs
    if (elements.settingSsid.value) {
        params.append('ssid', elements.settingSsid.value);
        params.append('password', elements.settingPass.value);
    }
    
    try {
        const response = await fetch('/api/settings', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: params
        });
        
        if (!response.ok) throw new Error('Settings save failed');
        
        elements.saveStatusMsg.textContent = "Settings saved successfully to NVS!";
        elements.saveStatusMsg.className = "status-msg success";
        
        // Apply visual updates locally
        applyTheme(elements.settingTheme.value);
        startPolling(elements.settingRefresh.checked);
        
        // Clear status text after delay
        setTimeout(() => {
            elements.saveStatusMsg.textContent = "";
        }, 3000);
        
    } catch (error) {
        console.error('Error saving settings to ESP32:', error);
        elements.saveStatusMsg.textContent = "Error saving configuration to NVS.";
        elements.saveStatusMsg.className = "status-msg error";
    }
}

// -------------------------------------------------------------
// Filters & Control Triggers
// -------------------------------------------------------------
function setupEventListeners() {
    // Scan triggers
    elements.btnStart.addEventListener('click', triggerStartScan);
    elements.btnBigScan.addEventListener('click', triggerStartScan);
    elements.btnStopScan.addEventListener('click', triggerStopScan);
    elements.btnClear.addEventListener('click', triggerClearScan);
    
    // Reports exports
    elements.btnExportJson.addEventListener('click', downloadJsonReport);
    elements.btnExportCsv.addEventListener('click', downloadCsvReport);
    
    // Settings Submit
    elements.settingsForm.addEventListener('submit', saveSettings);
    
    // Filters
    elements.filterAll.addEventListener('click', function() {
        elements.filterAll.className = 'active-filter';
        elements.filterHigh.className = '';
        appState.activeFilter = 'all';
        renderDeviceTable();
    });
    
    elements.filterHigh.addEventListener('click', function() {
        elements.filterAll.className = '';
        elements.filterHigh.className = 'active-filter';
        appState.activeFilter = 'high';
        renderDeviceTable();
    });

    // Clear log console button
    elements.btnClearLogs.addEventListener('click', function() {
        elements.logConsole.innerHTML = '<div class="log-line text-muted">[SYSTEM] Console buffer cleared by operator.</div>';
        appState.logs = [];
    });

    // Modal closers
    elements.modalClose.addEventListener('click', closeAuditModal);
    elements.modalOverlay.addEventListener('click', function(e) {
        if (e.target === elements.modalOverlay) closeAuditModal();
    });
}

// Initialize Application
document.addEventListener('DOMContentLoaded', () => {
    setupNavigation();
    setupEventListeners();
    loadSettings(); // Load initial settings (and starts polling)

    // Sidebar drawer toggle on mobile
    const btnToggle = document.getElementById('sidebar-toggle');
    const sidebar = document.querySelector('.sidebar');

    if (btnToggle && sidebar) {
        btnToggle.addEventListener('click', (e) => {
            e.stopPropagation();
            sidebar.classList.toggle('open');
        });
        
        // Close sidebar when clicking on a nav item
        document.querySelectorAll('.nav-item').forEach(item => {
            item.addEventListener('click', () => {
                if (window.innerWidth <= 768) {
                    sidebar.classList.remove('open');
                }
            });
        });
        
        // Close sidebar when clicking anywhere else
        document.addEventListener('click', (e) => {
            if (window.innerWidth <= 768 && !sidebar.contains(e.target) && e.target !== btnToggle) {
                sidebar.classList.remove('open');
            }
        });
    }
});

)rawjs";

#endif
