/**

  @File Name
    pic24fj-temp.X/main.c

  @Summary
    Temperature meter with PIC24FJ and 4 digit multiplexed LED display

    Used I/O pins:
    - RA0/PIN2 - on-board RED LED blinking at 1 Hz
 
  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        DFP               :  PIC24F-GA-GB_DFP v1.8.217
        Device            :  PIC24FJ64GB002
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70 (used v2.00)
        MPLAB 	          :  MPLAB X v5.50 (used v6.05)
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

/**
  Section: Included Files
*/
#include "mcc_generated_files/mcc.h"

#include<stdint.h>
typedef uint16_t u16;

volatile u16 counter = 0;

// automatically overrides weak function in tmr1.c:
// TMR1 Period is 5 ms ( 200 Hz)
// we plan to multiplex 4 digits on LED display, so 
// LED display frequency is 200 Hz / 4 = 50 Hz
void TMR1_CallBack(void)
{
    counter++;
    // Blink LED at 1Hz - toggle must be at 2 Hz (1:100) to get freq 1 Hz
    if (counter % 100 == 0){
        RED_LED_RA0_Toggle();
    }
    // TODO: Multiplex display
}


int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    INTERRUPT_GlobalEnable();
    TMR1_Start();

    while (1)
    {
        // TODO: Measure Temperature, output on LED display
    }

    return 1;
}
/**
 End of File
*/

