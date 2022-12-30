/**

  @File Name
    pic24fj-temp.X/main.c

  @Summary
    Temperature meter with PIC24FJ and 4 digit multiplexed LED display

    Used I/O pins:
    - RA0/PIN2 - on-board RED LED blinking at 1 Hz
    - RA1/PIN3 - Display mux 1st Digit
    - RA2/PIN9 - Display mux 2nd Digit
    - RA3/PIN10 - Display mux 3rd Digit 
    - RA4/PIN12 - Display mux 4th Digit
    - RB4/PIN11 - seg A, PIN11
    - RB5/PIN14 - seg F, PIN10
    - RB7/PIN16 - seg B, PIN7
    - RB8, RB9 - reserved for I2C
    - RB10/PIN21 - seg E, PIN1
    - RB11/PIN22 - seg D, PIN2
    - RB13/PIN24 - seg DP, PIN3
    - RB14/PIN25 - seg C, PIN4
    - RB15/PIN26 - seg G, PIN5
   Reserved pins:
    - PGED1/PIN4 - Debugger data
    - PGEC1/PIN5 - Debugger clock
    - /MCLR/PIN1 - ICSP debugger and programmer 
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

// for __delay_xx())
#define FCY 4000000UL 
#include "mcc_generated_files/mcc.h"
#include <libpic30.h>  // __delay_xs())

#include "mcc_generated_files/mcc.h"

#include<stdint.h>
// compact type aliases from Linux kernel
typedef uint8_t u8;
typedef uint16_t u16;

// 4-bit code to 7-seg display
//     A
//   +---+
//  F| G |B
//   +---+
//  E|   |C
//   +---+  +
//     D    DP
// encoded bits:
//               7 6 5 4 3 2 1  0
// bit 7 to 0 -> A B C D E F G  DP
#define SEG_A  ( 1 << 7)
#define SEG_B  ( 1 << 6)
#define SEG_C  ( 1 << 5)
#define SEG_D  ( 1 << 4)
#define SEG_E  ( 1 << 3)
#define SEG_F  ( 1 << 2)
#define SEG_G  ( 1 << 1)
#define SEG_DP ( 1 << 0)

const u8 DISP_DEC[16] = {
  (u8) ~ (SEG_G | SEG_DP ),                // 0 -> NOT (G|DP)
  (u8) SEG_B | SEG_C,                      // 1 -> B|C
  (u8) ~ (SEG_C | SEG_F | SEG_DP),         // 2 ->  NOT(C|F|DP)
  (u8) ~ (SEG_E | SEG_F | SEG_DP),         // 3 -> NOT(E|F|DP)
  (u8) SEG_B | SEG_C | SEG_F | SEG_G,      // 4 -> F|G|B|C
  (u8) ~ (SEG_B | SEG_E | SEG_DP),         // 5 -> NOT(B|E|DP)
  (u8) ~ (SEG_B | SEG_DP),                 // 6 -> NOT(B|DP)
  (u8) SEG_A | SEG_B | SEG_C,              // 7 -> A|B|C
  (u8) ~ SEG_DP,                           // 8 -> NOT(DP)
  (u8) ~ (SEG_E | SEG_DP),                 // 9 -> NOT(E|DP)
  (u8) ~ (SEG_D | SEG_DP),                 // 10 -> 'A' -> NOT(D|DP)
  (u8) ~ (SEG_A | SEG_B | SEG_DP),         // 11 -> 'b' -> NOT(A|B|DP)
  (u8) ~ (SEG_B | SEG_C | SEG_G | SEG_DP), // 12 -> 'C' -> NOT(B|C|G|DP)
  (u8) ~ (SEG_A | SEG_F | SEG_DP),         // 13 -> 'd' -> NOT(A|F|DP)
  (u8) ~ (SEG_B | SEG_C | SEG_DP),         // 14 -> 'E' -> NOT(B|C|DP)
  (u8) ~ (SEG_B | SEG_C | SEG_D | SEG_DP)  // 15 -> 'F' -> NOT(B|C|D|DP)
};


volatile u8 disp_digits[4] = { 0,0,0,0 };
volatile u16 counter = 0;

// automatically overrides weak function in tmr1.c:
// TMR1 Period is 5 ms ( 200 Hz)
// we have to multiplex 4 digits on LED display, so 
// LED display frequency is 200 Hz / 4 = 50 Hz
void TMR1_CallBack(void)
{
    u8 mux;
    u8 digit_data;
    
    counter++;
    // Blink LED at 1Hz - toggle must be at 2 Hz (1:100) to get freq 1 Hz
    if (counter % 100 == 0){
        RED_LED_RA0_Toggle();
    }
    mux = counter & 3;
    // Multiplex display - power on just 1 tranzistor of 4 (0=ON, 1=OFF)
    switch(mux){
            // remind inverted logic 0=ON, 1=OFF
        case 0:
            DISP_MUX2_SetHigh(); DISP_MUX3_SetHigh(); DISP_MUX4_SetHigh();
            DISP_MUX1_SetLow(); // set 0=ON as last to avoid short overload
            break;
        case 1:
            DISP_MUX1_SetHigh(); DISP_MUX3_SetHigh(); DISP_MUX4_SetHigh();
            DISP_MUX2_SetLow(); // set 0=ON as last to avoid short overload
            break;
        case 2:
            DISP_MUX1_SetHigh(); DISP_MUX2_SetHigh(); DISP_MUX4_SetHigh();
            DISP_MUX3_SetLow(); // set 0=ON as last to avoid short overload
            break;
        case 3:
            DISP_MUX1_SetHigh(); DISP_MUX2_SetHigh(); DISP_MUX3_SetHigh();
            DISP_MUX4_SetLow(); // set 0=ON as last to avoid short overload            
            break;
    }
    digit_data = disp_digits[mux];
    // Output A-F, DP segment data from disp_digits, 0=ON, 1=OFF
    // inverted logic, Low=ON, High=OFF
    if (digit_data & 0x80) SEG_A_SetLow(); else SEG_A_SetHigh();
    if (digit_data & 0x40) SEG_B_SetLow(); else SEG_B_SetHigh();
    if (digit_data & 0x20) SEG_C_SetLow(); else SEG_C_SetHigh();
    if (digit_data & 0x10) SEG_D_SetLow(); else SEG_D_SetHigh();
    if (digit_data & 0x08) SEG_E_SetLow(); else SEG_E_SetHigh();
    if (digit_data & 0x04) SEG_F_SetLow(); else SEG_F_SetHigh();
    if (digit_data & 0x02) SEG_G_SetLow(); else SEG_G_SetHigh();
    if (digit_data & 0x01) SEG_DP_SetLow(); else SEG_DP_SetHigh();   
}


int main(void)
{
    u8 hex;
    // initialize the device
    SYSTEM_Initialize();
    INTERRUPT_GlobalEnable();
    TMR1_Start();

    while (1)
    {
        // TODO: Measure Temperature, output on LED display
        disp_digits[0] = DISP_DEC[ (u8)((hex) & 0xf) ];
        disp_digits[1] = DISP_DEC[ (u8)((hex+1) & 0xf) ];
        disp_digits[2] = DISP_DEC[ (u8)((hex+2) & 0xf) ];
        disp_digits[3] = DISP_DEC[ (u8)((hex+3) & 0xf) ];
        __delay_ms(1000);
        hex = (u8)((hex+1) & 0x0f);
    }

    return 1;
}
/**
 End of File
*/

