# Medical Button
  
## Overview
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

### Project high level diagram
![](https://user-images.githubusercontent.com/35429810/72743764-f348ad80-3bb4-11ea-933f-69881c9ea256.png)

### Responsibilities
* #Oleg Farenyuk# - consultations, reviews
* Maksym Maystrenko - menthorship, definition, electronics, microcontrollers, coding, consultations
* Bohdan Yaremkiv - menthorship, definition, electronics, microcontrollers, coding, consultations
* Zenovii Popenyuk - LoRa, LoRa Server, RTOS
* Olya Levandivska, Olya Prots, Marusia Kulyk - RTOS, LoRa, GPRS, GPS
* Denys Datsko, Danylo Sluzhynskyi - GSM, GPRS, documentation


## Modules, we are using
The button will be based on [PSOC 6 BLE PIONEER KIT](https://www.cypress.com/documentation/development-kitsboards/psoc-6-ble-pioneer-kit-cy8ckit-062-ble).
![psoc](https://user-images.githubusercontent.com/35429810/71412816-fb660980-2657-11ea-8a27-c2d4c0a31732.png)


##### GSM module:
We started working with [GSM](https://www.faranux.com/product/sim800l-v2-0-5v-wirelessgsm-gprs-module-quad-band/) module with chip SIM800L, but in future it may be replaced with a better one. There are some problems, as the module does not work well with most of SIM cards. We also tried to make a bigger module work ([SIM900 Module](https://www.itead.cc/sim900-sim900a-gsm-gprs-minimum-system-module.html)). It uses SIM900 chip instead of SIM800, and we were able to make it work with one of SIM cards.
##### GPS module
The module, we worked with is [Ublox NEO-6M GPS module](https://www.electroschematics.com/neo-6m-gps-module/). The problem is that the signal is usually too weak. So, it is under a question, what module and antena to use in the final device.
##### LoraWAN module
For connection via LoRaWAN, we will use Onethinx Core Module: ![Onethinx Core Module](https://www.thethingsnetwork.org/forum/uploads/default/original/3X/e/6/e6b56725293fd1abe7fb3765db2312da00840c89.png) module

 
### Working process
#### What is done already
- GPS is working well, data is succesfully transmited via UART.
- GPS module can be configured from PSOC via UART, all the data is parsed and presented in convenient way
- A bigger GSM module seem to work well with one of SIM cards (we haven't sent any SMS's, as there was no enough money to do so) 
- All the processes are run under RTOS
- Every device has its own unique id (64 bit number, containing different unique data about each device)
- After pressing a big button, the GPS coordinates and unique id are transmited via GPRS


#### What should be done
- Make a GPRS module in base station receive the data
- Take care about encryption of the data
- Discuss, what identification methods should be used and implement them
- Make a box for the button and test it
