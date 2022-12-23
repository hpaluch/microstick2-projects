/**
  @File Name
    main.c

  @Summary
    Demo - using OLIMEX LCD3310 display (SPI) from PIC24FJ64GB002,
    Currently we use SPI clock 250 kHz for easier debug.

    Uses Microstick II board with  PIC24FJ64GB002, switch S1 in position A,
    Jumper J5 Closed.
 
    Used PINs:
    - RA0/PIN2 - on-board red LED blinking at 5 Hz
    - RB7/SPI1:SCK1OUT/PIN16 - SPI1 SCK (clock output)
    - RB8/SPI1:SDI1/PIN17 - SPI1 SDI (SPI data input)
    - RB9/SPI1:SDO1/PIN18 - SPI1 SDO (SPI data output)
    - RB10/SPI1:SS1OUT/PIN21 - SPI1 SS (SPI Slave Select output)

    OLIMEX LCD3310 hardwiring:
  
  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  PIC24FJ64GB002
        DFP:              :  PIC24F-GA-GB_DFP 1.8.217
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70 (using v2.00)
        MPLAB 	          :  MPLAB X v5.50 (using v6.05)
        MCC Plugin        :  v5.2.2, Core 5.4.14
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "mcc_generated_files/mcc.h"

// automatically overrides weak function in tmr1.c:
void TMR1_CallBack(void)
{
    // toggle LED at 10 Hz rate => blinking at 5 Hz
    LED_RA0_Toggle();
}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    INTERRUPT_GlobalEnable();
    TMR1_Start();

    while (1)
    {
        // TODO: Display control via SPI
    }

    return 1;
}

