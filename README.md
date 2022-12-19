# Henryk's Microstick II projects

Here are various Henryk's project for [Microstick II][Microstick II] 
board.

# List of projects

* [pic24fj-blink.X//](pic24fj-blink.X/) - simple LED blinking demo
  for [PIC24FJ64GB002][PIC24FJ64GB002]  using MCC tool, utilizes
  Timer1 Interrupt.
* [pic24hj-blink.X/](pic24hj-blink.X/) - simple LED blinking demo
  for [PIC24HJ128GP502][PIC24HJ128GP502] without MCC tool (not supported). Mostly
  copied from [microstick_demo_v2013_06_26.zip][Microstick_demo_v2013_06_26]

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
