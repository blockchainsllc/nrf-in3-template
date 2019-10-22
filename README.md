### NRF52840 with IN3 Template project
Use this is as your boiler plate for IN3 projects on nrf52840.

#### Dependencies
- [Nordic nRF52 SDK](https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK)
- [Nordic Command Line Tools](https://www.nordicsemi.com/Software-and-Tools/Development-Tools/nRF-Command-Line-Tools/Download)
- [Micro-ecc library](https://github.com/kmackay/micro-ecc)
  - [Installation steps](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v12.2.0%2Flib_crypto.html&cp=4_0_1_3_9_2&anchor=lib_crypto_installing)

### Supported transport modules

Change `main.c`to activate he desired transport module, `UART` is de the default. Only **ONE** transport module can be activated on the current firmware version.

- UART - **_default_** - Universal Asynchronous Receiver/Transmitter 
- BLE -  Bluetooth Low-Energy
- USB - Universal Serial Bus

#### UART - Universal Asynchronous Receiver/Transmitter
Developer need an UART to USB adapter to be able to access this interface. We used `FTDI232`for testing.
 - [Mouser.de FTDI 232](https://www.mouser.de/ProductDetail/Parallax/28024?qs=%2Fha2pyFaduj%2FYF1ey9away8PnRzDxED6kbp3OE70Ey4%3D)
 - [Bus Pirate](http://dangerousprototypes.com/docs/Bus_Pirate)
##### UART pinout on nRF5840:

| PIN NUMBER | PIN NAME | FTDI232  |
|:----------:|:--------:|:--------:|
|   P0.29    |   TXD    |   RXD    |
|   P0.31    |   RXD    |   TXD    |
|   P1.13    |   RTS    |   CTS    |
|   P1.15    |   CTS    |   DTR    |

#### BLE - Bluetooth Low-Energy
Missing pairing. Working with hardcoded parameters.

#### USB - Universal Serial Bus
*Future feature*


#### Tested on
- [Nordic nRF52840 PCA10056](https://www.nordicsemi.com/Software-and-Tools/Development-Kits/nRF52840-DK)
- [Nordic nRF52840 Dongle](https://www.nordicsemi.com/Software-and-Tools/Development-Kits/nRF52840-Dongle)

#### Usage
 1. Be sure to have all [dependencies](#dependencies) installed
 2. Compile sources with `make ./Makefile`
 3. Connect the device and flash with `make flash`
 ```
 make clean
 make
 make erase
 make flash_softdevice
 make flash
 ```
 
###### Make commands
- `make flash`: Runs `nrfjprog` to program the device with latest firmware
- `make erase`: Runs `nrfjprog` to erase the device
- `make flash_softdevice`: Runs `nrfjprog` to program the device with latest [softdevice](https://www.nordicsemi.com/Software-and-Tools/Software/S140) hex
- `make debug-server`: Runs Segger J-Link Debugger Server
- `make debug`: Uses `gcc` to compile the program and execute debugging commands 
- `make sdk_config`: Uses `java` and [CMSIS Configuration Wizard](https://infocenter.nordicsemi.com/topic/com.nordic.infocenter.sdk5.v15.2.0/sdk_config.html) to allows users to easily configure ARM source files without using the Keil uVision IDE. Loads `config/sdk_config.h` from sources.
- `make check-env`: Check for NRF5 SDK folder