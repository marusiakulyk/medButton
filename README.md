# Medical Button

## Overview
**Medical button** is a device for calling emergency help in field conditions and a base station for it  
**Purpose of the device** - to help people easily and extremely quickly call for the emergency help in field conditions

### General requirements for the device

- Connection with base station with **GSM module**, or reserve radiochannel **LoRaWAN**.
- Creating unique **identification** for every device. If enemy get this device it must be detected and **doesn't effect on another devices**.
- Data transfer should be **encrypted**, to prevent enemy getting information about location of soldier.
- Methods of identification and confirmation should not be very complicated, as the device will be usually used by injured people in field conditions.
- It may be useful to send information about the state of a person, calling for help (heart rate, blood pressure, etc.)
- The device should consume extremely small amount of power to be able to work for a long time without recharging
- The device soud be resistent to poor storing conditions (water, dust, mud, low and high temperatures)

## Devices, we are using
The button will be based on [PSOC 6 BLE PIONEER KIT](https://www.cypress.com/documentation/development-kitsboards/psoc-6-ble-pioneer-kit-cy8ckit-062-ble).

##### GSM module:
We started working with [GSM](https://www.faranux.com/product/sim800l-v2-0-5v-wirelessgsm-gprs-module-quad-band/) module with chip SIM800L, but in future it will be replaced with a better one.
##### GPS module
The module, we worked with is [Ublox NEO-6M GPS module](https://www.electroschematics.com/neo-6m-gps-module/). The problem is that the signal is usually too weak. So, it is under a question, what module and antena to use in the final device.
##### Lora
// TODO

