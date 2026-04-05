# LuxAdvertiser

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](./LICENSE)
[![Open Source](https://img.shields.io/badge/Open%20Source-OSI-3DA639?logo=opensourceinitiative&logoColor=white)](https://opensource.org/)
[![Open Hardware](https://img.shields.io/badge/Open%20Hardware-OSHWA-F8A100)](https://www.oshwa.org/definition/)

🌱 A small DIY BLE lux sensor for Home Assistant using BTHome, built around the Seeed Studio XIAO nRF52840.

## ✨ Why This Project?

This project measures ambient light with a VEML7700 and advertises the lux value over BLE using BTHome.

It is designed to be simple and practical:
- 🔌 USB-powered (or external USB battery pack)
- 🧠 No battery management complexity
- 🏡 Perfect for long-term indoor plant monitoring

### 🌱 Gardening Use Case

Put it near a plant spot, collect lux history, and verify if that location is actually good for your plant. The goal is a tiny sensor you can place and use remotely.

## 🚀 Features

- **Lux Measurement**: VEML7700 ambient light readings.
- **BTHome Protocol**: Native-friendly with Home Assistant BLE integration.
- **Simple Power Model**: USB only, no onboard battery monitoring.
- **Boot LED Feedback**: Quick color flashes show system status.

## 🧰 Hardware Requirements

- Seeed Studio XIAO nRF52840
- VEML7700 Ambient Light Sensor
- USB-C power source or external USB battery pack
- Wires

## 📦 BOM

- 1x M2x10 screw
- 1x M2 heat insert

## 🔌 Wiring Diagram

![LuxAdvertiser wiring diagram](https://raw.githubusercontent.com/FLo-ABB/LuxAdvertiser/main/img/wiring.png)

## 🖨️ 3D Enclosure

![LuxAdvertiser 3D enclosure](https://raw.githubusercontent.com/FLo-ABB/LuxAdvertiser/main/img/3dEnclosure.PNG)

The enclosure was designed in OnShape and is intended to be 3D printed.

[OnShape Model](https://cad.onshape.com/documents/a6520b5bcc45139a07dec6a0/w/8785962cd5c47eca463f3edf/e/562fdbec90560b909e2e8cf4?renderMode=3&uiState=69d23bb86be0f977970a4154) 

## 📸 In Situ Photography

Placeholder: add an in situ photo of the assembled device in its real placement.

## 📈 Home Assistant Chart

Placeholder: add a screenshot of the Home Assistant lux history chart.

## 📌 Pinout

### Seeed Studio XIAO nRF52840 and VEML7700 Connections

| XIAO BLE Pin | Function | VEML7700 Pin |
|-----|----------|--------------|
| 3V3 | Power Output |  VCC |
| GND | Ground | GND |
| D4 (P0.04) | I2C SDA | SDA |
| D5 (P0.05) | I2C SCL | SCL |
| LED_BUILTIN (P0.26) | Status Indicator | - |


## 💻 Software Setup

### Prerequisites

- PlatformIO
- Arduino IDE or VS Code with PlatformIO extension
- Home Assistant with BLE integration enabled

### Installation

1. Clone this repository:
  ```bash
  git clone https://github.com/FLo-ABB/LuxAdvertiser.git
  cd LuxAdvertiser
  ```
2. Open the project in PlatformIO / VS Code.
3. Build and upload:
  ```bash
  platformio run --target upload
  ```
4. Connect the hardware as described in the pinout section.

### Home Assistant Integration

1. Enable BLE integration in Home Assistant.
2. Power on the device. It should appear as **LuxAdvertiser**.
3. Add the BTHome device. The lux sensor should be auto-discovered.

#### 🌤️ Optional Dashboard Ideas

- Create a chart card with lux history over time.
- Add weather data (e.g., OpenWeatherMap) for context.
- Use HA automation/scripts to ask an LLM for plant-light recommendations.

## ▶️ Usage

- On power-up, the device runs a boot sequence:
  - **Blue flash (once)**: BLE setup succeeded.
  - **Green flash (once, after blue)**: Lux sensor setup succeeded.
  - **Red flash (once)**: BLE or sensor setup failed for that attempt.
- If BLE or sensor setup fails, initialization retries every 10 seconds.
- BLE advertising starts only when **both** BLE and sensor are ready.
- Once fully initialized, the status LED is off during normal operation.

## ⚙️ Configuration

- **Advertising interval**: edit `lastAdvertise > 30000` in `src/ble_manager.cpp` (milliseconds).
- **Sensor settings**: adjust gain/integration in `src/lux_sensor.cpp`.

## 🧯 Troubleshooting

If it acts weird, no panic, follow this flow.

1. **Power and wiring check**
  - Confirm the board is powered over USB.
  - Confirm VEML7700 wiring: VCC->3V3, GND->GND, SDA->D4, SCL->D5.
2. **Observe LED sequence after power-on**
  - Blue once + green once: startup successful.
  - Red once: last startup step failed.
  - Red every 10s: repeated startup failure (BLE or sensor).
3. **Verify BLE visibility**
  - Scan with nRF Connect first.
  - If visible in nRF Connect but not Home Assistant, check HA BLE integration/range.
4. **Validate sensor readiness**
  - If there is no green flash, focus on I2C wiring and sensor power.
  - Advertising is intentionally blocked until sensor initialization succeeds.
5. **Check runtime behavior**
  - Advertising updates every 30s by design.
  - For longer unattended runtime, use a larger USB battery pack or reduce advertising frequency.

### Symptom Table

| Symptom | Likely Cause | Action |
|---------|--------------|--------|
| No LED activity | No power/boot issue | Check USB power and board seating |
| Red flash at boot | BLE or sensor init failed | Recheck wiring and reboot |
| Red flash every 10s | Startup retry still failing | Verify VEML7700 power/I2C and BLE environment |
| Not visible in Home Assistant | HA-side BLE issue | Test with nRF Connect, then check HA BLE integration |
| Visible in BLE but wrong/no data | Sensor not ready or invalid reading | Confirm green flash and sensor wiring |

## 🤝 Contributing

Pull requests are welcome. Hobby project spirit: keep it simple, test on real hardware, and share what you learn.

## 📄 License

Released under the MIT License. See [LICENSE](./LICENSE).

## 🧡 Open Source and Open Hardware Notes

- This repository (firmware, documentation, enclosure model) is published as an open-source hobby project under MIT.
- Hardware modules used here are off-the-shelf components from their respective vendors.
- Check vendor pages for component-specific hardware design files and licensing details.

## 🙏 Acknowledgments

- BTHome protocol: https://bthome.io/
- ArduinoBLE library
- Adafruit VEML7700 library
- Seeed Studio XIAO nRF52840 documentation: https://wiki.seeedstudio.com/XIAO_BLE/