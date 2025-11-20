# Event Tracker Button

WiFi-connected button for recording event timestamps via HTTP webhook. Built for M5Stack AtomS3.

## Usage

- **Short press**: Record event timestamp
- **3-second hold**: Privacy mode (discards last hour)
- **10-second hold**: Debug menu (WiFi diagnostics)

Visual feedback with color-coded states and animations throughout.

## Setup

### Hardware
- M5Stack AtomS3 board

### Software
1. Install [VSCode](https://code.visualstudio.com/)
2. Install [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode)
3. Clone this repo and open in VSCode
4. Configure WiFi credentials in [src/main.cpp](src/main.cpp#L29-43)
5. Set webhook URLs in [src/main.cpp](src/main.cpp#L45-46)
6. Connect AtomS3 via USB
7. Build and upload: `pio run --target upload`

## Development

- **Build**: `pio run`
- **Upload**: `pio run --target upload`
- **Monitor serial**: `pio device monitor`
- **Clean**: `pio run --target clean`

Full design documentation in [design_resources/](design_resources/)
