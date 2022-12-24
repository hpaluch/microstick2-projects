/**
  @File Name
    main.c

  @Summary
    Demo - using OLIMEX LCD3310 display (SPI) from PIC24FJ64GB002,

    See links below for OLIMEX LCD3310 details:
    - https://www.olimex.com/Products/Modules/LCD/MOD-LCD3310/open-source-hardware
    - https://github.com/OLIMEX/UEXT-MODULES/tree/master/MOD-LCD3310
    - resolution 84x48 (84 columns, 48 lines)
 
    - this program uses 5x8 font, it offers:
      6-lines (6*8=48)
      16-characters + 4 free pixels (16*5+4 = 84)

    - internal LCD RAM is 102x68 where display can show only 84x48  
 
    Uses Microstick II board with  PIC24FJ64GB002, switch S1 in position A,
    Jumper J5 Closed.
 
    Clock:
    - f_RC = 8 MHz (fixed)
    - f_RCDIV = f_RC / 2 = 4 MHz
    - f_CPU   = f_RCDIV / 2 = 2 MHz
  
    Used PINs:
    - RA0/PIN2 - on-board red LED blinking at 5 Hz
    - RA3/CLKO/PIN10 - instruction clock output - f_cy =  2 MHz
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

const char *BUILD_VER = "v0.09";

// for __delay_us())
#define FCY 1000000UL 
#include "mcc_generated_files/mcc.h"
#include <libpic30.h>  // __delay_us())

#include <stdint.h>
// type aliases like Linux kernel
typedef uint8_t u8;
typedef uint16_t u16;

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

#define LCD_START_LINE_ADDR	(66-2)

#if LCD_START_LINE_ADDR	> 66
#error "Invalid LCD starting line address"
#endif 

#define LCD_COLUMNS 84
#define LCD_ROWS 48
// one TEXT line has height 8 pixels
#define LCD_TEXTLINES (48/8)

typedef enum {
    SEND_CMD = 0,
    SEND_DATA,
} t_cmd_data;

void LCDSend(u8 val,t_cmd_data dc)
{
    LCD_CS_SetLow(); // activate /CS
    if (dc == SEND_DATA){
        LCD_DC_SetHigh(); // sending DATA -> D/C=1
    } else {
        LCD_DC_SetLow();  // sending COMMAND -> D/C=0
    }
    SPI1_Exchange8bit( val );
    LCD_CS_SetHigh(); // deactivate /CS
}

void LCDcls(void)
{
    u8 x,y;
    
    for (y = 0; y < LCD_TEXTLINES; y++)
    {
        // must be in H1H0=00 mode!
        LCDSend(0x80, SEND_CMD);
        LCDSend(0x40 | y, SEND_CMD);
        for (x = 0; x < LCD_COLUMNS; x++)
        {
            LCDSend(0x00, SEND_DATA); // empty
        }
    } 
}

void LCD_init(void)
{  
    LCD_CS_SetHigh();
    LCD_DC_SetLow();
    // minimum RESET pulse is 3us and RESET time is additional 3us
    LCD_RESET_SetLow(); // active low
    __delay_us(20);
    LCD_RESET_SetHigh();
    __delay_us(20);

    // These commands are copied from Arduino source
    // Function Set:  0  0  1 MX  MY PD H1 H0
    // 0x21 =         0  0  1  0   0  0  0  1
    LCDSend(0x21, SEND_CMD); // LCD Extended Commands.
    // now we are in mode H1H0=01
    // Set EVR  1  EV6 EV5 EV4  EV3 EV2 EV1 EV0, EV=8
    // 0xc8 =   1   1    0   0    1   0   0   0
    LCDSend(0xc8, SEND_CMD);    // Set LCD Vop (Contrast). 0xC8

    // Set start line S6   0 0 0 0  0 1 0 S6
    // 0x04 + s6bit        0 0 0 0  0 1 0 s6
    // MUST be in this order (S6 bit first than S5 to S0)
    LCDSend(0x04 | !!(LCD_START_LINE_ADDR & (1u << 6)), SEND_CMD);    // S6
    // Set start line  0  1  S5 S4  S3 S2 S1 S0
    // 0x40 + s5to0    0  1  s5 s4  s3  s2 s1 s0
    LCDSend(0x40 | (LCD_START_LINE_ADDR & ((1u << 6) - 1)), SEND_CMD);  // S5,S4,S3,S2,S1,S0
    // System bias set  0 0 0 1  0 BS2 BS1 BS0
    // 0x14             0 0 0 1  0   1   0   0  
    LCDSend(0x14, SEND_CMD); // LCD bias // was 0x12 or 0x13
    // Function Set  0  0  1 MX MY PD H1 H0
    // 0x20          0  0  1  0  0  0  0  0
    LCDSend(0x20, SEND_CMD); // LCD Standard Commands, Horizontal addressing mode.
    // now we are in H1H0=00 mode
    // Display Control  0 0 0 0  1 D 0 E Sets display configuration
    // 0x08             0 0 0 0  1 0 0 0 
    LCDSend(0x08, SEND_CMD); // LCD blank (DE=00)
    // Display Control  0 0 0 0  1 D 0 E Sets display configuration   
    // 0x0c             0 0 0 0  1 1 0 0
    LCDSend(0x0C, SEND_CMD); // LCD in normal mode.(DE=10)

    // try to clear screen
    // NOTE we must be in H1H0=00 mode(!)
    LCDcls();
}

const unsigned char FontLookup [][5] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00}, // sp
    { 0x00, 0x00, 0x2f, 0x00, 0x00}, // !
    { 0x00, 0x07, 0x00, 0x07, 0x00}, // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14}, // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12}, // $
    { 0xc4, 0xc8, 0x10, 0x26, 0x46}, // %
    { 0x36, 0x49, 0x55, 0x22, 0x50}, // &
    { 0x00, 0x05, 0x03, 0x00, 0x00}, // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00}, // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00}, // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14}, // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    { 0x00, 0x00, 0x50, 0x30, 0x00}, // ,
    { 0x10, 0x10, 0x10, 0x10, 0x10}, // -
    { 0x00, 0x60, 0x60, 0x00, 0x00}, // .
    { 0x20, 0x10, 0x08, 0x04, 0x02}, // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00}, // :
    { 0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00}, // <
    { 0x14, 0x14, 0x14, 0x14, 0x14}, // =
    { 0x00, 0x41, 0x22, 0x14, 0x08}, // >
    { 0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E}, // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    { 0x46, 0x49, 0x49, 0x49, 0x31}, // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    { 0x63, 0x14, 0x08, 0x14, 0x63}, // X
    { 0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00}, // [
    { 0x55, 0x2A, 0x55, 0x2A, 0x55}, // 55
    { 0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04}, // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40}, // _
    { 0x00, 0x01, 0x02, 0x04, 0x00}, // '
    { 0x20, 0x54, 0x54, 0x54, 0x78}, // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38}, // b
    { 0x38, 0x44, 0x44, 0x44, 0x20}, // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F}, // d
    { 0x38, 0x54, 0x54, 0x54, 0x18}, // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02}, // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E}, // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78}, // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00}, // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00}, // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00}, // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00}, // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78}, // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78}, // n
    { 0x38, 0x44, 0x44, 0x44, 0x38}, // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08}, // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C}, // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08}, // r
    { 0x48, 0x54, 0x54, 0x54, 0x20}, // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20}, // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
    { 0x44, 0x28, 0x10, 0x28, 0x44}, // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44}, // z
    { 0x08, 0x6C, 0x6A, 0x19, 0x08}, // { (lighting)
    { 0x0C, 0x12, 0x24, 0x12, 0x0C}, // | (heart)
    { 0x7E, 0x7E, 0x7E, 0x7E, 0x7E}, // square
}; 

void LCDputchar(u8 c)
{
    u8 i;
    
    if (c<32)
        c = '?';
    if (c > 127)
        c= 127;
    for(i=0;i<5;i++){
        u8 bmp = FontLookup[c-32][i];
        LCDSend(bmp, SEND_DATA);
    }
}

void LCDputs(const char *str)
{
    const char *p;
    for(p=str; *p != '\0';p++){
        LCDputchar(*p);
    }
}

// returns bytes written
u8 BUFFERputchar(u8 c, u8 *buf, u8 buf_limit)
{
    u8 i;
    u8 bmp;
    
    if (c<32)
        c = '?';
    if (c > 127)
        c= 127;
    for(i=0;i<5;i++){
        if (i>=buf_limit)
            break;
        bmp = FontLookup[c-32][i];
        buf[i] = bmp;
    }
    return i;
}

// return bytes written
u8 BUFFERputs(const char *str,u8 *buf, u8 buflen)
{
    u8 n=0,total=0;
    u8 *b = buf;
    const char *p;
    for(p=str; *p != '\0';p++){
        if (buflen ==0)
            break;
        n = BUFFERputchar(*p,b,buflen);
        b += n;
        buflen -= n;
        total += n;
    }
    return total;
}

const char *ROLL_TEXT = "Hello!";
// backend RAM for "Hello!" text
// 6 -number of character in text, 5 is width of single char
u8 ROLL_BUFFER[6*5];

int main(void)
{
    u8 y;
    u8 x;
    // initialize the device
    SYSTEM_Initialize();
    LCD_init();
    TMR1_Start();
    INTERRUPT_GlobalEnable();
    
    for(y=0;y!=LCD_TEXTLINES;y++){
        LCDSend(0x80, SEND_CMD); // set X address to 0
        LCDSend(0x40+y, SEND_CMD); // set Y address to 1
        LCDputchar(y+'0');
        if (y==0){
            LCDputs(__DATE__);
            LCDputs(BUILD_VER);
        } else {
            LCDputs("Hello, world!EF");
            // 4 pixels out of 84 are left
            for(x=0;x<4;x++)
            {
                // 2 angled lines converging like '>'
                LCDSend( (u8)(1 << x) | (128 >> x), SEND_DATA);
            }
            
        }
    }
    // render text for scrolling to PIC RAM
    BUFFERputs(ROLL_TEXT,ROLL_BUFFER,sizeof(ROLL_BUFFER));
    __delay_ms(1000); // wait a bit and then start rolling text in bottom line
    while (1)
    {
        u8 ofs;
        for(ofs=0;ofs!=sizeof(ROLL_BUFFER);ofs++){
            LCDSend(0x80, SEND_CMD); // set X address to 0
            LCDSend(0x40+(LCD_TEXTLINES-1), SEND_CMD); // set Y address to last line
            // copy whole line to LCD
            for(x=0;x<LCD_COLUMNS;x++){
                LCDSend( ROLL_BUFFER[ (x+ofs) % ((u8)sizeof(ROLL_BUFFER)) ],
                        SEND_DATA);
            }
            __delay_ms(200);
        }
    }

    return 1;
}
