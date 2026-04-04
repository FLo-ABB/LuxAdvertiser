# LuxAdvertiser

A small DIY BLE lux + battery beacon for Home Assistant using BTHome. Built around the Seeed Studio XIAO nRF52840 :)

## Description

This project is a tiny battery-powered gadget that measures ambient light (lux) and battery level, then advertises the values over BLE using BTHome. Home Assistant can pick it up automatically, so it is super handy for quick experiments.

### Gardening Use Case

Made for apartment plant nerds (like me). Put it near a plant spot for a few days, collect lux history, and check if that place is actually good for the plant. You can even combine with weather data and LLM analysis for fun and better decisions.

## Features

- **Lux Measurement**: Uses a VEML7700 for ambient light readings.
- **Battery Monitoring**: Estimates battery percentage from ADC reading.
- **BTHome Protocol**: Works nicely with Home Assistant BLE integration.
- **Low Power-ish**: Advertises in short bursts every 30s.
- **Boot LED Feedback**: Quick color flashes tell you what is working (or not).

## Hardware Requirements

- Seeed Studio XIAO nRF52840
- VEML7700 Ambient Light Sensor
- LiPo Battery (e.g., 300mAh or larger for extended runtime)
- Jumper wires for connections

## Pinout

### Seeed Studio XIAO nRF52840 Pinout

| Pin | Function | Connected To |
|-----|----------|--------------|
| 3V3 | Power Output | VEML7700 VCC |
| GND | Ground | VEML7700 GND, Battery GND |
| D4 (P0.04) | I2C SDA | VEML7700 SDA |
| D5 (P0.05) | I2C SCL | VEML7700 SCL |
| P0.14 | ADC_BAT | Battery Voltage (through divider) |
| Battery Connector | Battery Power | LiPo Battery + |
| LED_BUILTIN (P0.26) | Status Indicator | - |

### VEML7700 Sensor

| Pin | Function | Connected To |
|-----|----------|--------------|
| VCC | Power (3.3V) | XIAO 3V3 |
| GND | Ground | XIAO GND |
| SDA | I2C Data | XIAO D4 |
| SCL | I2C Clock | XIAO D5 |

### Battery

| Connection | Description |
|------------|-------------|
| + | Battery Connector on XIAO |
| - | Battery Connector GND on XIAO |
| Voltage Sense | Connected internally to P0.14 via voltage divider |

**Note**: The XIAO has a built-in battery charger (BQ25101). Ensure the battery is compatible (3.7V LiPo).

## Software Setup

### Prerequisites

- PlatformIO (for building and uploading)
- Arduino IDE or VS Code with PlatformIO extension
- Home Assistant with BLE integration enabled

### Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/FLo-ABB/LuxAdvertiser.git
   cd LuxAdvertiser
   ```

2. Open in PlatformIO/VS Code.

3. Build and upload:
   ```bash
   platformio run --target upload
   ```

4. Connect the hardware as per pinout.

### Home Assistant Integration

1. Enable BLE integration in Home Assistant.
2. Power on the device; it should appear as "LuxAdvertiser".
3. Add the BTHome device—lux and battery sensors will be auto-discovered.

#### Gardening Dashboard & LLM Analysis
- Create a dashboard with history graphs for lux over time.
- Integrate weather data (e.g., OpenWeatherMap) to correlate light with conditions.
- Use HA's OpenAI integration or scripts to send data to an LLM for plant advice (e.g., "Is this lux suitable for a Boston Fern?").

## Usage

- At power on, the device runs a boot sequence:
  - **Blue flash (once)**: BLE setup succeeded.
  - **Green flash (once, after blue)**: Lux sensor setup succeeded.
  - **Red flash (once)**: BLE or sensor setup failed for that attempt.
- If BLE or sensor setup fails, the device retries initialization every 10 seconds.
- BLE advertising starts only when **both** BLE and sensor are ready.
- Once fully initialized, the status LED is off during normal operation.
- Then just monitor lux and battery in Home Assistant.


## Battery Life

- **300mAh Battery**: ~11 days (advertising every 30 seconds).
- For longer runtime (months), implement deep sleep or use a larger battery (e.g., 2000mAh for ~3 months).

## Configuration

- **Advertising Interval**: Modify `lastAdvertise > 30000` in `ble_manager.cpp` (in ms).
- **Sensor Settings**: Adjust gain/integration in `lux_sensor.cpp`.
- **Battery Divider**: Confirm ratio in `battery.cpp` (assumed 1:1).

## Troubleshooting

If it acts weird, no panic, follow this quick flow :)

1. Power and wiring check
  - Confirm battery is charged and connected.
  - Confirm VEML7700 wiring: VCC->3V3, GND->GND, SDA->D4, SCL->D5.
2. Observe LED sequence after power-on
  - Blue once + green once: startup is successful.
  - Red once: last startup step failed.
  - Red every 10s: repeated startup failure (BLE or sensor).
3. Verify BLE visibility
  - Scan with nRF Connect first.
  - If visible in nRF Connect but not Home Assistant, check HA BLE integration/range.
4. Validate sensor readiness
  - If there is no green flash, focus on I2C wiring and sensor power.
  - Advertising is intentionally blocked until sensor initialization succeeds.
5. Check runtime behavior
  - Advertising updates every 30s by design.
  - For battery runtime issues, reduce advertising frequency or add deep sleep.

### Symptom Table

| Symptom | Likely Cause | Action |
|---------|--------------|--------|
| No LED activity | No power/boot issue | Check battery, USB power, board seating |
| Red flash at boot | BLE or sensor init failed | Recheck wiring and reboot |
| Red flash every 10s | Startup retry still failing | Verify VEML7700 power/I2C and BLE environment |
| Not visible in Home Assistant | HA-side BLE issue | Test with nRF Connect, then check HA BLE integration |
| Visible in BLE but wrong/no data | Sensor not ready or invalid reading | Confirm green flash and sensor wiring |


## Contributing

Pull requests are welcome. Hobby project spirit: keep it simple, test on real hardware, share what you learn :)

## License

MIT License. See LICENSE file.

## Acknowledgments

- BTHome protocol: https://bthome.io/
- Libraries: ArduinoBLE, Adafruit VEML7700