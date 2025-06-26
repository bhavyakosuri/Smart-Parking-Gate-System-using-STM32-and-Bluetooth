# Smart Parking Gate System using STM32 and Bluetooth

Smart Parking Gate System using STM32F401C, IR sensors, servo motor, and Bluetooth (HC-05). Tracks real-time slot availability, automates gate control, and ensures efficient parking management. Developed using Keil uVision5.

---

## 🧠 Project Overview

A compact and efficient embedded system built using the **STM32F401C microcontroller** for intelligent parking automation. The system detects vehicles using **infrared sensors**, controls entry/exit with a **servo motor**, and transmits real-time parking data via **Bluetooth (HC-05)** to paired devices (e.g., mobile apps).

This project was developed using **Keil uVision5 IDE**, and is capable of managing **4 parking slots**, updating the status dynamically based on sensor input.

---

## ✨ Features

- 🔍 IR-based Vehicle Detection  
- 🚧 Servo Motor Automated Gate Control  
- 📲 Bluetooth Communication via HC-05  
- 📈 Real-time Slot Monitoring (4 slots)  
- ⚙️ Embedded C development with Keil uVision5  
- ⚡ Fast, reliable hardware-software integration

---

## 🛠️ Hardware Components

| Component                  | Quantity |
|---------------------------|----------|
| STM32F401C Microcontroller| 1        |
| IR Sensors                | 2        |
| Servo Motor               | 1        |
| HC-05 Bluetooth Module    | 1        |
| Breadboard                | 1        |
| Jumper Wires              | As needed|
| Power Supply (5V)         | 1        |

---

## 📁 Project Structure

```

Smart-Parking-Gate-System/
├── DebugConfig/                  # Debug configuration files
├── Listings/                    # Listing outputs
├── Objects/                     # Compiled object files
├── RTE/                         # Run-Time Environment configs
├── Code/                        # Source code (.c/.h)
├── demo/                        # Project demo video
│   └── demo.mp4
├── EventRecorderStub.scvd
├── Parking.uvguix.bhavya        # User config file
├── Parking.uvoptx               # Keil options file
├── Parking                      # Project file for Keil

````

---

## 🚀 How It Works

1. IR sensors detect the presence or absence of vehicles at each slot.
2. Based on availability, the **servo motor** opens/closes the parking gate.
3. Occupancy status is updated and shared via **Bluetooth (HC-05)**.
4. Users can connect a mobile app or Bluetooth serial terminal to receive real-time updates.

---

## 🎥 Demo

> 📽️ **Project in Action**  
> Download or play the demo video:

[▶️ Click here to view demo.mp4](https://github.com/bhavyakosuri/Smart-Parking-Gate-System-using-STM32-and-Bluetooth/blob/main/demo.mp4)


---

## ⚙️ How to Run

1. Clone the repository:
   ```bash
   git clone https://github.com/bhavyakosuri/Smart-Parking-Gate-System-using-STM32-and-Bluetooth.git
````

2. Open the project in **Keil uVision5** using the file: `Parking`.

3. Connect your STM32F401C to your system.

4. Flash the compiled code to the microcontroller.

5. Connect hardware components:

   * 4 IR sensors to GPIO inputs
   * Servo motor to PWM pin
   * HC-05 to UART TX/RX pins

6. Power on the system. Pair the **HC-05** with your mobile/PC and monitor parking status.

---

## 📡 Communication Format (Bluetooth)

* **Baud Rate:** 9600
* **Data Sent:** Parking slot status (e.g., `Slot1:Occupied`, `Slot2:Free`)

---

## 📜 License

This project is licensed under the [MIT License](https://choosealicense.com/licenses/mit/).
Feel free to use, modify, and share with attribution.

---

## 👨‍💻 Author

**Bhavya Naga Sai Kosuri**
👩‍🎓 Undergraduate Student at Amrita Vishwa Vidyapeetham
📅 Project Developed: **Nov 2024**
📬 Contact: [LinkedIn](https://www.linkedin.com/in/bhavyakosuri/) | [bhavyakosuri2209@gmail.com](mailto:bhavyakosuri2209@gmail.com)

---

> 💡 *This project was developed for the Embedded Systems Course Final Project.*
> Empowering smart cities with intelligent parking solutions 🚘

```

---

### ✅ What You Need to Do:

1. Open this file: [README.md Editor on GitHub](https://github.com/bhavyakosuri/Smart-Parking-Gate-System-using-STM32-and-Bluetooth/edit/main/README.md)
2. Paste the full content above.
3. Save the file (Commit changes).
4. Make sure `demo/demo.mp4` is uploaded in your repo.

Let me know if you'd like a badge (e.g., Keil IDE badge, Made with C, etc.) or a simple **Bluetooth serial monitor interface app**.
```
