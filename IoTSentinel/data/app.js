/*
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

