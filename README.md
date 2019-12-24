# Medical Button
## Overview
![](https://img.shields.io/github/languages/code-size/ddatsko/medButton/)
![](https://img.shields.io/github/last-commit/ddatsko/medButton/)
![](https://img.shields.io/github/languages/count/ddatsko/medButton/)
**Medical button** is a device for calling emergency help in field conditions and a base station for it  
**Purpose of the device** - to help people easily and extremely quickly call for the emergency help in field conditions

### General requirements for the device

1. Connection with base station with **GSM module**, or reserve radiochannel **LoRaWAN**.
2. Creating unique **identification** for every device. If enemy get this device it must be detected and **doesn't effect on another devices**.
3. Data transfer should be **encrypted**, to prevent enemy getting information about location of soldier.
4. Methods of identification and confirmation should not be very complicated, as the device will be usually used by injured people in field conditions.
5. It may be useful to send information about the state of a person, calling for help (heart rate, blood pressure, etc.)
6. The device should consume extremely small amount of power to be able to work for a long time without recharging
7. The device soud be resistent to poor storing conditions (water, dust, mud, low and high temperatures)

## Modules, we are using
The button will be based on [PSOC 6 BLE PIONEER KIT](https://www.cypress.com/documentation/development-kitsboards/psoc-6-ble-pioneer-kit-cy8ckit-062-ble).
![psoc](https://user-images.githubusercontent.com/35429810/71412816-fb660980-2657-11ea-8a27-c2d4c0a31732.png)


##### GSM module:
We started working with [GSM](https://www.faranux.com/product/sim800l-v2-0-5v-wirelessgsm-gprs-module-quad-band/) module with chip SIM800L, but in future it will be replaced with a better one.
##### GPS module
The module, we worked with is [Ublox NEO-6M GPS module](https://www.electroschematics.com/neo-6m-gps-module/). The problem is that the signal is usually too weak. So, it is under a question, what module and antena to use in the final device.
##### Lora
// TODO

