# Henryk's Microstick II projects

Here are various Henryk's project for [Microstick II][Microstick II] 
board.

# List of projects

## Project: [PIC24FJ with LCD3310](pic24fj-lcd3310.X/))

Controlling OLIMEX LCD3310 (SPI) - display from Nokia 3310 phones(???)

![PIC24FJ with Olimex LCD 3310](https://raw.githubusercontent.com/hpaluch/microstick2-projects/br-pic24fj-olimex-lcd3310/pic24fj-lcd3310.X/assets/pic24fj-lcd3310-image.jpg)

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

![PIC24FJ LCD3310 SPI Init](https://raw.githubusercontent.com/hpaluch/microstick2-projects/br-pic24fj-olimex-lcd3310/pic24fj-lcd3310.X/assets/ad2-lcd3310-init.png)

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

[Microstick II]: https://www.microchip.com/DevelopmentTools/ProductDetails/dm330013-2
[Microstick_demo_v2013_06_26]: https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/CodeExamples/microstick_demo_v2013_06_26.zip
[PIC24FJ64GA002]: https://www.microchip.com/en-us/product/PIC24FJ64GA002
[PIC24FJ64GB002]: https://www.microchip.com/en-us/product/PIC24FJ64GB002
[PIC24HJ128GP502]: https://www.microchip.com/en-us/product/PIC24HJ128GP502
[dsPIC33FJ128MC802]: https://www.microchip.com/en-us/product/dsPIC33FJ128MC802
[PIC32MX250F128B]: https://www.microchip.com/en-us/product/PIC32MX250F128B
