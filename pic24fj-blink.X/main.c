/**
  Generated main.c file from MPLAB Code Configurator

  @File Name
    main.c

  @Summary
    Trivial LED blinking demo for Microstick II board, generated with MCC.
    Uses Timer1 with Interrupt handler to Toggle LED every 0.5 s (2 Hz)
    which gives 1 Hz (1s) blinking rate.

  @Description
    Generation Information :
        Board             : Microstick II DM330013-2
        DFP               :  PIC24F-GA-GB_DFP expected by MCC: 1.171.1 (actual DFP 1.8.217)
        Device            :  PIC24FJ64GB002
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70 (actual v2.00)
        MPLAB 	          :  MPLAB X v5.50 (actual v6.05)
        MCC               :  plugin v5.2.2, Core v5.4.14
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

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/interrupt_manager.h"

// automatically overrides weak function in tmr1.c:
void TMR1_CallBack(void)
{
    // toggle LED at 2 Hz rate => blinking at 1 Hz
    LED_Toggle();
}

int main(void)
{
    SYSTEM_Initialize();
    INTERRUPT_GlobalEnable();
    TMR1_Start();
    
    while (1)
    {
        // so far NOP
    }
    
    // never returns
    return 1;
}

