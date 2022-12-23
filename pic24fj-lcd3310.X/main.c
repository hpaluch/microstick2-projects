/**
  @File Name
    main.c

  @Summary
    Demo - using OLIMEX LCD3310 display (SPI) from PIC24FJ64GB002,
    Currently we use SPI clock 250 kHz for easier debug.

    See links below for OLIMEX LCD3310 details:
    - https://www.olimex.com/Products/Modules/LCD/MOD-LCD3310/open-source-hardware
    - https://github.com/OLIMEX/UEXT-MODULES/tree/master/MOD-LCD3310
 
    Uses Microstick II board with  PIC24FJ64GB002, switch S1 in position A,
    Jumper J5 Closed.
 
    Used PINs:
    - RA0/PIN2 - on-board red LED blinking at 5 Hz
    - RA3/CLKO/PIN10 - instruction clock output - 1 MHz
    LCD display connections:
    - PIC socket               LCD pin
    - RB7/SPI1:SCK1OUT/PIN16   SCK/PIN9  - SPI1 SCK (clock output)
    - RB8/SPI1:SDI1/PIN17      N/A       - SPI1 SDI (SPI data input)
    - RB9/SPI1:SDO1/PIN18      MOSI/PIN8 - SPI1 SDO (SPI data output)
    - RB10/SPI1:SS1OUT/PIN21   /CS/PIN10 - SPI1 SS (SPI Slave Select output)
    - RB11/PIN22               /RES/PIN5 - LCD /RESET
    - RB13/PIN24               D/C - LCD DATA and /COMMAND
    - GND/PIN8                 GND/PIN2
    - 3.3V/TP1 (test point)    3.3V/PIN1
  
  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Device            :  PIC24FJ64GB002
        DFP:              :  PIC24F-GA-GB_DFP 1.8.217
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70 (using v2.00)
        MPLAB 	          :  MPLAB X v5.50 (using v6.05)
        MCC Plugin        :  v5.2.2, Core 5.4.14
*/

/*
 (C) Copyright, Henryk Paluch 
 
 This project uses portions of code from:
 - https://github.com/OLIMEX/UEXT-MODULES/blob/master/MOD-LCD3310/Software/Arduino(AVR)/lcd3310_GPIO.c
 
 Original Copyright:  
  
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

// for __delay_us())
#define FCY 1000000UL 
#include "mcc_generated_files/mcc.h"
#include <libpic30.h>  // __delay_us())

#include <stdint.h>
// type aliases like Linux kernel
typedef uint8_t u8;

// automatically overrides weak function in tmr1.c:
void TMR1_CallBack(void)
{
    // toggle LED at 10 Hz rate => blinking at 5 Hz
    LED_RA0_Toggle();
}

// LCD stuff mostly copied and ported from:
// https://github.com/OLIMEX/UEXT-MODULES/blob/master/MOD-LCD3310/Software/Arduino(AVR)/lcd3310_GPIO.c
// see also datasheet:
// https://github.com/OLIMEX/UEXT-MODULES/blob/master/MOD-LCD3310/Hardware/TLS8204V12.pdf

#define LCD_START_LINE_ADDR	(66-8)

#if LCD_START_LINE_ADDR	> 66
#error "Invalid LCD starting line address"
#endif 

#define LCD_set_command_mode LCD_DC_SetLow
#define LCD_set_data_mode LCD_DC_SetHigh

void LCD_init(void)
{
    // minimum RESET pulse is 3us and RESET time is additional 3us
    LCD_RESET_SetLow(); // active low
    __delay_us(20);
    LCD_RESET_SetHigh();
    __delay_us(20);

    // Arduino source: LCDSend(0x21, SEND_CMD); // LCD Extended Commands. 
    LCD_set_command_mode();
    // LCDSend(0xC8, SEND_CMD); // Set LCD Vop (Contrast). 0xC8
    SPI1_Exchange8bit( 0xc8 );    

    // MUST be in this order (S6 bit first than S5 to S0)
    // LCDSend(0x04 | !!(LCD_START_LINE_ADDR & (1u << 6)), SEND_CMD); // Set Temp S6 for start line
    // LCDSend(0x40 | (LCD_START_LINE_ADDR & ((1u << 6) - 1)), SEND_CMD); // Set Temp S[5:0] for start line
    SPI1_Exchange8bit( 0x04 | !!(LCD_START_LINE_ADDR & (1u << 6)) );    
    SPI1_Exchange8bit(0x40 | (LCD_START_LINE_ADDR & ((1u << 6) - 1)));
    SPI1_Exchange8bit(0x12); // LCD bias mode 1:68.
    SPI1_Exchange8bit(0x20); // LCD Standard Commands, Horizontal addressing mode.
    SPI1_Exchange8bit(0x08); // LCD blank
    SPI1_Exchange8bit(0x0C); // LCD in normal mode. 

    // try to send sample data
    //                    D7 D6 D5 D4 D3 D2 D1 D0
    // Set X addr of RAM  1  X6 X5 X4 X3 X2 X1 X0
    SPI1_Exchange8bit(0x80); // set X address to 0
    //                    D7 D6 D5 D4 D3 D2 D1 D0
    // Set Y addr of RAM  0  1  Y5 Y4 Y3 Y2 Y1 Y0
    SPI1_Exchange8bit(0x40); // set Y address to 0
    // Send some data
    LCD_set_data_mode();
    SPI1_Exchange8bit(0xff);
    SPI1_Exchange8bit(0x00);
    SPI1_Exchange8bit(0x55);
    SPI1_Exchange8bit(0xaa);  
    LCD_set_command_mode();
}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    LCD_init();
    TMR1_Start();
    INTERRUPT_GlobalEnable();

    while (1)
    {
        // TODO: Display control via SPI
    }

    return 1;
}

