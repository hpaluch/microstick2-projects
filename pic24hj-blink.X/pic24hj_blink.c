/* 
 * File:   pic24hj_blink.c
 * Summary:
 *   Trivial LED blinking demo for:
 *     - Microstick II board
 *     - CPU: PIC24HJ128GP502
 *   Challenge: not supported by MCC tools
 *   Based on code from: microstick_demo_v2013_06_26.zip
 * Created on December 19, 2022, 1:53 PM
 */

// see microstick_demo_v2013_06_26/firmware/src/system_config/microstick2_pic24hj128gp502/system.c 
// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Mode (Internal Fast RC (FRC) w/ PLL)
#pragma config IESO = OFF               // Internal External Switch Over Mode (Start-up device with user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow Multiple Re-configurations)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled) 

#include <xc.h>

// see microstick_demo_v2013_06_26/firmware/src/system_config/microstick2_pic24hj128gp502/system_config.h
#define LED  	 _LATA0
#define LED_TRIS _TRISA0

int main(void) {

    // based on microstick_demo_v2013_06_26/firmware/src/main.c
    
    // Initialize the LED. This symbol is defined in system_config.h
    LED_TRIS = 0;

    /* Initialize Timer 1
     * Each configuration sets the MCU's oscillator to 32MHz 
     * For the LED to blink at a rate of 1Hz,
     * it must be toggled at 2Hz.
     *
     * Calculation:
     * Fcy = Fosc / 2 = 16,000,000 [Hz]
     * T1CONbits.TCKPS = 3 divides the input clock by 256.
     * PR1 = Fcy[Hz] / 256 / 2[Hz] = 0x7A12;
     */
    _T1IF = 0;
    _T1IE = 0;
    TMR1 = 0x0000;
    PR1 = 0x7A12;
    T1CONbits.TCKPS = 3;
    T1CONbits.TON = 1; 
    
    while(1){
        if (_T1IF == 1){
            _T1IF = 0;
            LED ^= 1; // Toggle the LED 
        }         
    }
    
    // never reached
    return 1;
}

