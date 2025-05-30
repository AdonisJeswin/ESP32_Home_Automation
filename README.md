# ESP32 Home Automation Web Server

This project uses an ESP32 microcontroller to create a web-based home automation system. It reads temperature and humidity from a DHT11 sensor and allows manual and automatic control of a fan and bulb using relay modules. A live dashboard is generated with SVG gauges for a responsive UI.

---

## 🔧 Features
- 🌡️ Real-time temperature and humidity monitoring via DHT11  
- 💡 Web-based control for Fan and Bulb  
- 🔁 Fan automatically turns on when temperature exceeds 35°C  
- 🧭 Clean UI with SVG temperature and humidity gauges  
- 🔄 Auto-refreshing web interface (every 1 second)  
- ✋ Manual override for relays  

---

## 🧰 Hardware Required

| Component              | Quantity   |
|------------------------|------------|
| ESP32 Dev Board        | 1          |
| DHT11 Sensor           | 1          |
| 2-Channel Relay Module | 1          |
| Fan or Motor           | 1          |
| Bulb or LED            | 1          |
| Jumper Wires           | As needed  |
| Breadboard             | 1          |

---

## 🔌 Circuit Diagram

### **Connections**

[ESP32] [Relay Module]
GPIO 5 ------> IN1 (Fan)
GPIO 12 ------> IN2 (Bulb)

DHT11:
VCC ------> 3.3V
GND ------> GND
Data ------> GPIO 2


---

## 🚀 Installation Steps

1. Open the project `.ino` file in the **Arduino IDE**.
2. Install the following libraries via Library Manager:
    - `DHT sensor library` by Adafruit
    - `Adafruit Unified Sensor`
3. Go to **Tools > Board > ESP32 Dev Module**.
4. Replace the Wi-Fi credentials in the code:
    ```cpp
    const char* ssid = "YOUR_WIFI_SSID";
    const char* password = "YOUR_WIFI_PASSWORD";
    ```
5. Upload the code to your ESP32 board.
6. Open the **Serial Monitor** (baud rate: `115200`) to get the IP address.
7. Open your browser and navigate to the IP address to access the web interface.

---

## 🌐 Web Interface Controls

- 🌀 **Fan ON/OFF** — via buttons  
- 💡 **Bulb ON/OFF** — via buttons  
- 🌡️ **Fan Auto ON** — if temperature > 35°C  
- 🔄 **Page auto-refresh** — every 1 second  

---

## 📄 License

This project is open source and available under the **MIT License**.

---

## 👨‍💻 Credits

Developed by:
- Adonis Jeswin  
- Krishal Haria
- Yash Singh  
- Nimish Arekar  
- Riya Gholap  
- Krishang Dijesh  

Based on open-source libraries and ESP32 web server techniques.


---

