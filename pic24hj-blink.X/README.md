# Blinkding LED for PIC24HJ and Microstick II

Here is introductory blinking LED demo for:
* [Microstick II][Microstick II]  board
* [PIC24HJ128GP502][PIC24HJ128GP502]  (included with Microstick II)

What it does:
* blinking LED on board at 1 Hz using Timer1 Interrupt,
  mostly copied demo `microstick_demo_v2013_06_26.zip`)
  to single file...

What is the challenge? It is NOT supported by MCC Tool. When
you start it you get dreaded Message:

> The selected device is not supported by any of the MCC content types.
> 
> How to fix this: change your project device to one
> supported by the MCC content types.

It is really wise advice :-) 

You can find list in Release Notes *for MCC Libraries*:
- https://ww1.microchip.com/downloads/en/DeviceDoc/release_notes_pic24_dspic_pic32mm_v1_171_1.pdf 

Tested on:
* MPLAB X IDE v6.05
* DFP 1.3.64
* ~MCC Plugin 5.2.2~ (not used, because PIC24HJ is not supported by MCC)
* XC16 v2.0


# Resources

* [Official Microstick II Page][Microstick II]
* [Henryk's Microstick Notes](https://github.com/hpaluch/hpaluch.github.io/wiki/Microstick-II-board-notes)

[Microstick_demo_v2013_06_26]: https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/CodeExamples/microstick_demo_v2013_06_26.zip
[Microstick II]: https://www.microchip.com/DevelopmentTools/ProductDetails/dm330013-2
[PIC24HJ128GP502]: https://www.microchip.com/en-us/product/PIC24HJ128GP502
