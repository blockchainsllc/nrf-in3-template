### NRF52840 Cryptocell API
This repository investigates and provides examples for the NRF5 Crypotcell 310. Currently there are three examples in the repository, `build_private_key`, `generate_keys` and `sign_and_verify`. It is recommended that you experiment and learn in this order.

To get started you have to follow three simple steps:

1. clone the repo and navigate inside any example directory you want to test.
2. add **ENV** variable `SDK_ROOT=<PATH TO YOUR SDK FOLDER>`. You can download the SDK from [here](https://www.nordicsemi.com/Products/Low-power-short-range-wireless/nRF52840/Compatible-downloads#infotabs)
3. [MAC OS ONLY] you will need to install [this](https://www.segger.com/downloads/jlink/JLink_MacOSX.pkg) tool from segger. 
4. Compile the source files and flash the hex file onto nrf52840 using `make flash`