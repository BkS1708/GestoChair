# Gesto-Chair — Flowcharts

This folder contains the **flowcharts and system diagrams** used to document the architecture, communication, control logic, and safety mechanisms of the **Gesto-Chair** project.

The diagrams provide a visual reference for understanding how different input sources communicate with the wheelchair and how commands are processed at the receiver.

---

## 📊 Flowcharts

### 1. System Architecture & Command Flow

**File:** `GestoChair_MovementFlowchart.png`

Provides the overall flow of the Gesto-Chair control system, covering:

- Joystick input
- Accelerometer-based gesture input
- Mobile phone Bluetooth input
- Command mapping
- HC-05 wireless communication
- Arduino receiver
- Motor control
- Watchdog-based safety stopping

---

### 2. HC-05 Master–Slave Configuration

**Files:**
- `GestoChair_Masterslave.png`
- `GestoChair_Masterslave_Vertical.png`

Documents the configuration and initialization process for the two HC-05 Bluetooth modules.

The diagram covers:

- Pairing the two HC-05 modules
- Configuring one module as **Master**
- Configuring the other as **Slave**
- Binding the Master to the Slave
- Configuring a common UART baud rate
- Automatic connection on power-up
- Establishing the wireless serial communication link between the two Arduinos

The **vertical version** provides the same process in a more presentation-friendly layout.

---

### 3. Mobile Phone / HC-05 Control Source

**File:** `GestoChair_MobileORHC05.png`

Illustrates how the wheelchair receiver can accept commands from different control sources.

It documents:

- The default HC-05 Master → HC-05 Slave communication path
- Mobile phone → HC-05 Slave communication
- Bluetooth terminal / mobile application commands
- Switching between control sources
- Processing commands on the receiver Arduino

This represents the project's **multi-source control capability**, allowing the same receiver-side motor-control system to work with different command sources.

---

### 4. Heartbeat Communication

**File:** `GestoChair_HeartbeatCommunication.png`

Documents the communication relationship between the transmitter and receiver Arduinos.

The diagram focuses on:

- Real-time command transmission
- Single-character command communication
- HC-05 wireless serial communication
- Receiver-side command processing
- Continuous communication monitoring
- Watchdog-based response to communication loss

This diagram helps explain how the receiver determines whether the transmitter is still actively communicating.

---

### 5. Watchdog Timer & Fail-Safe Mechanism

**Files:**
- `GestoChair_WatchdogTimer.png`
- `GestoChair_WatchdogTimerNew.png`

These diagrams document the **watchdog timer safety mechanism** implemented on the receiver Arduino.

The receiver tracks the time since the last received command. If no valid command is received within the configured timeout period, the safety mechanism is triggered.

The flowcharts cover possible causes such as:

- Bluetooth disconnection
- Signal interference
- Transmitter failure
- Loss of incoming commands

When a timeout occurs, the receiver executes the motor stop routine and keeps the wheelchair stopped until valid communication resumes.

The `GestoChair_WatchdogTimerNew.png` file contains the updated version of the watchdog flowchart.

---

## 📁 Files in This Folder

```text
Flowcharts/
│
├── GestoChair_MovementFlowchart.png
├── GestoChair_Masterslave.png
├── GestoChair_Masterslave_Vertical.png
├── GestoChair_MobileORHC05.png
├── GestoChair_HeartbeatCommunication.png
├── GestoChair_WatchdogTimer.png
└── GestoChair_WatchdogTimerNew.png
