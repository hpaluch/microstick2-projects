# Henryk's Microstick II projects

Here are various Henryk's project for [Microstick II][Microstick II] 
board.

# List of projects

## Project: [PIC24FJ Thermometer](pic24fj-temp.X/)

Thermometer with:
* [PIC24FJ64GB002][PIC24FJ64GB002]
* [BQ-M512RD][BQ-M512RD] - 4 digit LED display, common anode, multiplexed
* temperature sensor: 1-wire DS18B20

![PIC24FJ Thermometer with DS18B20 - prototype](https://raw.githubusercontent.com/hpaluch/microstick2-projects/master/assets/pic24fj-temp.jpg)

Other parts:
- BC328 PNP tranzistor (4pcs) - (Ic should be >= 100 mA)
  - https://www.tme.eu/Document/7638705e8f42a6d61c15822cfca87f14/bc327.pdf
- Rezistor 4k7 (4pcs)

Schematics:

![PIC24FJ 4-digit LED Thermometer schema](https://raw.githubusercontent.com/hpaluch/microstick2-projects/master/kicad/pic24fj-temp/pic24fj-temp.svg)

NOTE: On-board Red LED is on while measurement is in progress.

This project complements my existing PIC16F630 Thermometer (with 2-digit display and same DS18B20 sensor) from:
- https://github.com/hpaluch/temp_meter_16f630

## Project: [PIC24FJ with LCD3310](pic24fj-lcd3310.X/)

Controlling OLIMEX LCD3310 (SPI) - display from Nokia 3310 phones(???)

![PIC24FJ with Olimex LCD 3310](https://raw.githubusercontent.com/hpaluch/microstick2-projects/master/pic24fj-lcd3310.X/assets/pic24fj-lcd3310-image.jpg)

Project folder: [pic24fj-lcd3310.X/](pic24fj-lcd3310.X/)
Status:
- displays `xBUILD_DATEvBUILD_VERSION` on 1st line
- displays `xHello, world!EF>` on every line of LCD but first and last
- and then starts rolling text (called marquee in HTML) `Hello!` on last line.
- NOTE: in my case that rolling textline is noticeably smeared - I don't know why (CPU collision?)...

Notes:
- OLIMEX LCD3310 details:
  - https://www.olimex.com/Products/Modules/LCD/MOD-LCD3310/open-source-hardware
  - https://github.com/OLIMEX/UEXT-MODULES/tree/master/MOD-LCD3310
- SPI Traps:
  - `/SS1EN` (Slave select enabled) - is valid only when:
    - in Slave mode
    - or Master Frame mode
  - so in our case we must use regular GPIO PIN for for `/CS`

Here is Scope + analyzer of Display initializaiton over SPI:

![PIC24FJ LCD3310 SPI Init](https://raw.githubusercontent.com/hpaluch/microstick2-projects/master/pic24fj-lcd3310.X/assets/ad2-lcd3310-init.png)

## Trivial projects

* [pic24fj-blink.X/](pic24fj-blink.X/) - simple LED blinking demo
  for [PIC24FJ64GB002][PIC24FJ64GB002]  using MCC tool, utilizes
  Timer1 Interrupt.
* [pic24hj-blink.X/](pic24hj-blink.X/) - simple LED blinking demo
  for [PIC24HJ128GP502][PIC24HJ128GP502] without MCC tool (device not supported). 
  Uses Timer1 Interrupt to toggle LED.

# Board notes

My board includes these MCUs (list from [Microstick II Site][Microstick II]):

* ~[PIC24FJ64GA002][PIC24FJ64GA002]~ - not included in my case
* [PIC24FJ64GB002][PIC24FJ64GB002] this
  one probably replaces above [PIC24FJ64GA002][PIC24FJ64GA002]
  which is fortune, because it has USB OTG support (!),
  see dedicated [USB CDC (Virtual COM port) project](https://github.com/hpaluch/pic24fj-usb-cdc-mcc.X)
* [PIC24HJ128GP502][PIC24HJ128GP502] - no MCC tool support
* [dsPIC33FJ128MC802][dsPIC33FJ128MC802] - no MCC tool support
* [PIC32MX250F128B][PIC32MX250F128B] - supported with Harmony tool

So to summarize:
The MCC Classic tool supports only (as of 2022-12-19):
* [PIC24FJ64GB002][PIC24FJ64GB002]

The MCC Harmony tool supports only:
* [PIC32MX250F128B][PIC32MX250F128B]

# Resources

* [Official Microstick II Page][Microstick II]
* [Henryk's Microstick Notes](https://github.com/hpaluch/hpaluch.github.io/wiki/Microstick-II-board-notes)

How to produce complete generated code listing:
- https://stackoverflow.com/questions/52059357/what-is-the-xc16-command-to-obtain-disassembly-listing-disasm-file
Please note that official Microchip's advice (to use `-Wa,-a=listing.lst` is pretty useless, because
all extern symbols and final code address is known at (or after) linking stage only.
- I use modified After Build command (Project Properties -> Conf:default -> Building -> Execute this line after build):
  ```
  ${MP_CC_DIR}\xc16-objdump -S ${ImageDir}\${PROJECTNAME}.${IMAGE_TYPE}.elf > ${ImageDir}\${PROJECTNAME}.${IMAGE_TYPE}.lst
  ```

[Microstick II]: https://www.microchip.com/DevelopmentTools/ProductDetails/dm330013-2
[Microstick_demo_v2013_06_26]: https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/CodeExamples/microstick_demo_v2013_06_26.zip
[PIC24FJ64GA002]: https://www.microchip.com/en-us/product/PIC24FJ64GA002
[PIC24FJ64GB002]: https://www.microchip.com/en-us/product/PIC24FJ64GB002
[PIC24HJ128GP502]: https://www.microchip.com/en-us/product/PIC24HJ128GP502
[dsPIC33FJ128MC802]: https://www.microchip.com/en-us/product/dsPIC33FJ128MC802
[PIC32MX250F128B]: https://www.microchip.com/en-us/product/PIC32MX250F128B
[BQ-M512RD]: https://www.americanbrightled.com/pdffiles/led-components/led-displays/BQ-M512RD.pdf
