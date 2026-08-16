# 🦽 Gesto-Chair

### Gesture-Controlled Smart Wheelchair with Bluetooth Communication

> **An assistive mobility system that enables wireless wheelchair control through hand gestures and joystick input, with a fail-safe communication architecture.**

---

## 🚀 Overview

**Gesto-Chair** is a smart wheelchair control system designed to provide an intuitive and wireless method of controlling wheelchair movement.

The system combines:

* 🕹️ **Joystick-based control**
* ✋ **Accelerometer-based gesture control**
* 📡 **HC-05 Bluetooth wireless communication**
* 🔄 **Real-time single-character command transmission**
* 🛡️ **Communication heartbeat / watchdog safety mechanism**
* 🔌 **Modular transmitter–receiver architecture**

The system uses two Arduino-based units:

* **Transmitter Arduino** — reads the joystick and accelerometer, determines the desired movement, and wirelessly transmits commands.
* **Receiver Arduino** — receives the commands through Bluetooth and interfaces with the wheelchair motor-control circuitry.

The project is designed around a simple principle:

> **The user controls the wheelchair naturally, while the receiver continuously monitors communication to ensure that loss of control data can trigger a safe stop.**

---

## ✨ Key Features

| Feature                   | Description                                                                           |
| ------------------------- | ------------------------------------------------------------------------------------- |
| 🕹️ Joystick Control      | Directional movement using an analog joystick                                         |
| ✋ Gesture Control         | Movement based on accelerometer orientation                                           |
| 🔀 Dual Control Modes     | Switch between joystick and gesture operation                                         |
| 📡 Wireless Communication | HC-05 Bluetooth modules provide the serial communication link                         |
| ⚡ Real-Time Commands      | Lightweight single-character movement protocol                                        |
| 💓 Heartbeat Transmission | Commands are periodically transmitted to maintain communication                       |
| 🛡️ Fail-Safe Design      | Communication timeout can trigger an automatic motor stop                             |
| 🔧 Modular Architecture   | Transmitter and receiver are separated for easier development and testing             |
| 📱 Expandable Control     | The receiver architecture can also accept commands from alternative Bluetooth sources |

---

# 🏗️ System Architecture

Gesto-Chair follows a **transmitter–receiver architecture**.

```text
                    ┌─────────────────────────┐
                    │      USER INPUTS        │
                    ├─────────────────────────┤
                    │  🕹️ Joystick            │
                    │  ✋ Accelerometer        │
                    └────────────┬────────────┘
                                 │
                                 ▼
                    ┌─────────────────────────┐
                    │   TRANSMITTER ARDUINO   │
                    │                         │
                    │  Input Processing       │
                    │  Mode Selection        │
                    │  Command Generation     │
                    └────────────┬────────────┘
                                 │
                          Single Character
                         F / B / L / R / S
                                 │
                                 ▼
                    ┌─────────────────────────┐
                    │       HC-05 MASTER      │
                    └────────────┬────────────┘
                                 │
                         📡 Bluetooth Link
                                 │
                                 ▼
                    ┌─────────────────────────┐
                    │       HC-05 SLAVE       │
                    └────────────┬────────────┘
                                 │
                                 ▼
                    ┌─────────────────────────┐
                    │    RECEIVER ARDUINO     │
                    │                         │
                    │ Command Processing      │
                    │ Safety Monitoring       │
                    │ Motor Control           │
                    └────────────┬────────────┘
                                 │
                                 ▼
                    ┌─────────────────────────┐
                    │   MOTOR CONTROLLER /    │
                    │   WHEELCHAIR MOTORS     │
                    └─────────────────────────┘
```

---

# 🕹️ Control Modes

## 1. Joystick Mode

The joystick provides two analog axes:

* **X-axis** → Left / Right
* **Y-axis** → Forward / Backward

The transmitter continuously reads the joystick position and compares it against configurable threshold values.

| Joystick Condition    | Command        |
| --------------------- | -------------- |
| Y > 650               | `F` — Forward  |
| Y < 350               | `B` — Backward |
| X < 350               | `L` — Left     |
| X > 650               | `R` — Right    |
| Within neutral region | `S` — Stop     |

The neutral region acts as a **dead zone**, preventing small joystick fluctuations from unintentionally moving the wheelchair.

---

## 2. Gesture Mode

The accelerometer provides two analog axes:

* **X-axis**
* **Y-axis**

The transmitter maps accelerometer readings to the same movement-command protocol used by the joystick.

| Accelerometer Condition | Command        |
| ----------------------- | -------------- |
| X > 360                 | `F` — Forward  |
| X < 300                 | `B` — Backward |
| Y < 300                 | `R` — Right    |
| Y > 360                 | `L` — Left     |
| Within neutral region   | `S` — Stop     |

This allows the user to control wheelchair movement through the orientation/tilt of the controller.

---

# 🔀 Switching Between Modes

A physical mode switch connected to the transmitter Arduino determines the active control source.

```text
Mode = 0  →  Joystick Control
Mode = 1  →  Gesture Control
```

Pressing the switch toggles between the two modes.

The transmitter then continuously processes the active input source and generates the corresponding movement command.

---

# 📡 Bluetooth Communication

The project uses **HC-05 Bluetooth modules** to create a wireless serial communication link between the transmitter and receiver.

### HC-05 Configuration

The intended architecture uses:

```text
Transmitter Arduino
       │
       ▼
HC-05 Master
       │
       │ Bluetooth
       ▼
HC-05 Slave
       │
       ▼
Receiver Arduino
```

The modules communicate using a **9600 baud UART connection**.

The command protocol is intentionally lightweight, using only a single character for each movement command.

---

# 🔤 Command Protocol

| Character | Action   |
| :-------: | -------- |
|    `F`    | Forward  |
|    `B`    | Backward |
|    `L`    | Left     |
|    `R`    | Right    |
|    `S`    | Stop     |

This approach keeps the wireless protocol simple and minimizes communication overhead.

The same command representation can be generated from different input sources, meaning that the receiver does not need to know whether a command originated from the joystick or the accelerometer.

---

# 💓 Heartbeat Communication

The transmitter periodically sends the current command over Bluetooth.

The current implementation uses a:

```text
Send Interval = 200 ms
```

Therefore, the transmitter continuously refreshes the communication link rather than sending data only when the user changes direction.

This provides a simple **heartbeat mechanism** that allows the receiver to determine whether communication is still active.

---

# 🛡️ Fail-Safe / Watchdog Mechanism

Safety is an important part of the receiver-side architecture.

The receiver maintains:

```cpp
lastCommandTime
```

and uses a configurable timeout:

```cpp
const int timeout = 2000;
```

representing **2 seconds**.

The intended safety behavior is:

```text
Valid command received
        ↓
Update lastCommandTime
        ↓
Execute command
        ↓
Continue monitoring
        ↓
No command received?
        ↓
Timeout exceeded
        ↓
STOP ALL MOTORS
```

A timeout can occur because of situations such as:

* Bluetooth disconnection
* Transmitter failure
* Signal interference
* Loss of communication
* Controller shutdown

The purpose of the watchdog is to prevent the wheelchair from continuing indefinitely based on an outdated movement command.

> ⚠️ **Safety Note:** This project is an experimental/educational prototype. It should not be used as a mobility aid without appropriate hardware-level safety validation, redundancy, emergency-stop mechanisms, and supervised testing.

---

# 💻 Software

The current Main Branch contains two primary Arduino programs.

### 📤 Transmitter

**`SenderCode_JoystickAccel_BT_FailSafe.ino`**

Responsible for:

* Reading joystick values
* Reading accelerometer values
* Switching between control modes
* Mapping sensor values to movement commands
* Sending commands through HC-05
* Periodically transmitting the current command
* Providing serial debugging information

### 📥 Receiver

**`RecvCode_JoystickAccel_BT_Failsafe.ino`**

Responsible for:

* Initializing the HC-05 Bluetooth interface
* Receiving wireless characters
* Tracking the latest command timestamp
* Providing motor-control functions
* Providing a safe motor-stop function
* Monitoring the communication safety state

---

# 🔧 Hardware

The core prototype uses an Arduino-based transmitter and receiver architecture.

### Transmitter Side

* Arduino
* Analog joystick
* Analog accelerometer
* Mode-selection switch
* HC-05 Bluetooth module

### Receiver Side

* Arduino
* HC-05 Bluetooth module
* Motor driver
* Wheelchair DC motors

### Communication

```text
Arduino TX  →  HC-05
HC-05       →  Wireless Bluetooth Link
HC-05       →  Arduino RX
```

---

# 📌 Pin Configuration

## Transmitter

| Component       | Pin       |
| --------------- | --------- |
| Joystick X      | `A0`      |
| Joystick Y      | `A1`      |
| Accelerometer X | `A5`      |
| Accelerometer Y | `A6`      |
| Mode Switch     | `D11`     |
| Bluetooth RX/TX | `D5 / D4` |

The transmitter uses:

```cpp
SoftwareSerial bluetooth(5,4);
```

---

## Receiver

| Motor Control   | Arduino Pin |
| --------------- | ----------- |
| IN1             | `D12`       |
| IN2             | `D13`       |
| IN3             | `D14`       |
| IN4             | `D15`       |
| Bluetooth RX/TX | `D4 / D5`   |

The receiver uses:

```cpp
SoftwareSerial bluetooth(4,5);
```

---

# 📁 Repository Structure

```text
Gesto-Chair/
│
├── Main/
│   ├── SenderCode_JoystickAccel_BT_FailSafe.ino
│   └── RecvCode_JoystickAccel_BT_Failsafe.ino
│
├── Flowcharts/
│   ├── GestoChair_MovementFlowchart.png
│   ├── GestoChair_Masterslave.png
│   ├── GestoChair_Masterslave_Vertical.png
│   ├── GestoChair_MobileORHC05.png
│   ├── GestoChair_HeartbeatCommunication.png
│   ├── GestoChair_WatchdogTimer.png
│   └── GestoChair_WatchdogTimerNew.png
│
└── README.md
```

> The repository may contain additional folders and documentation as the project continues to evolve.

---

# ⚙️ Setup

### 1. Install Arduino IDE

Install the Arduino IDE and connect the transmitter and receiver Arduino boards.

### 2. Install Required Library

The project uses:

```cpp
#include <SoftwareSerial.h>
```

`SoftwareSerial` is used to establish serial communication with the HC-05 Bluetooth modules.

### 3. Upload Transmitter Code

Upload:

```text
SenderCode_JoystickAccel_BT_FailSafe.ino
```

to the transmitter Arduino.

### 4. Upload Receiver Code

Upload:

```text
RecvCode_JoystickAccel_BT_Failsafe.ino
```

to the receiver Arduino.

### 5. Configure HC-05 Modules

Configure one HC-05 as the **Master** and the other as the **Slave**, then pair/bind the modules.

Both modules should use the same UART configuration:

```text
9600 baud
```

### 6. Connect the Hardware

Connect:

* Joystick
* Accelerometer
* Mode switch
* Motor driver
* Motors
* HC-05 modules

according to the pin configuration documented above.

---

# 🧪 Testing

The system can be tested progressively:

### Stage 1 — Sensor Testing

Verify that:

* Joystick readings change correctly.
* Accelerometer readings respond to movement.
* Neutral positions remain inside the intended dead zones.

### Stage 2 — Command Generation

Verify that the transmitter generates:

```text
F / B / L / R / S
```

according to the selected input.

### Stage 3 — Bluetooth Communication

Verify that commands transmitted by the Master HC-05 are correctly received by the Slave HC-05.

### Stage 4 — Receiver Testing

Verify that the receiver correctly interprets the movement commands.

### Stage 5 — Safety Testing

Test communication-loss scenarios and verify that the wheelchair transitions to a safe stopped state.

> **Always perform initial motor testing with the drive system safely lifted/off-load and use an accessible physical emergency stop.**

---

# 📊 Design Philosophy

Gesto-Chair follows a few key design principles:

### 🎯 Simple Command Protocol

Movement is represented using only five characters:

```text
F B L R S
```

This keeps communication lightweight and easy to debug.

### 🔌 Separation of Responsibilities

The transmitter handles:

> **Input → Command**

while the receiver handles:

> **Command → Movement**

This makes the system modular and easier to expand.

### 🛡️ Receiver-Side Safety

Safety-critical behavior is handled on the receiver side so that communication failures can be detected close to the motor-control layer.

### 🔄 Multiple Input Sources

Joystick and gesture control generate the same command format, allowing different interfaces to share the same downstream control architecture.

---

# 📚 Documentation

Detailed visual documentation of the system can be found in the:

**[`Flowcharts/`](./Flowcharts)**

folder.

It contains diagrams covering:

* Overall system architecture
* HC-05 Master–Slave configuration
* Mobile / Bluetooth control
* Heartbeat communication
* Watchdog timer
* Fail-safe behavior
* Command flow

---

# 🔮 Future Improvements

Potential future improvements include:

* 📱 Dedicated mobile application
* 🎙️ Voice-controlled wheelchair operation
* 🧠 Intelligent gesture classification
* 🚨 Dedicated hardware emergency-stop circuit
* 🔋 Battery monitoring
* 📍 Obstacle detection
* 📏 Distance sensing
* ⚡ Motor speed control using PWM
* 🔐 More robust wireless communication
* 🛡️ Redundant safety mechanisms
* 📊 Real-time telemetry and diagnostics
* 🤖 Autonomous / semi-autonomous navigation

---

# 👨‍💻 Project

**Gesto-Chair** is an assistive robotics project focused on combining **human-friendly interfaces with embedded systems and wireless control**.

The project explores how relatively simple hardware and communication protocols can be combined to create a modular control system for assistive mobility.

---

## ⭐ If You Find This Project Interesting

Feel free to explore the repository, study the architecture, experiment with the control logic, and build upon the system.

**Gesto-Chair — Control mobility through intuitive human interaction.**
