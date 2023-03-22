// CONFIG1L
#pragma config FEXTOSC = OFF    // External Oscillator Selection (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_64MHZ// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = OFF     // PRLOCKED One-Way Set Enable bit (PRLOCK bit can be set and cleared repeatedly)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = OFF     // Multi-vector enable bit (Interrupt contoller does not use vector table to prioritze interrupts)
#pragma config IVT1WAY = OFF    // IVTLOCK bit One-way set enable bit (IVTLOCK bit can be cleared and set repeatedly)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P85 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.85V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be set and cleared repeatedly (subject to the unlock sequence))
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_12// WDT Period selection bits (Divider ratio 1:131072)
#pragma config WDTE = ON        // WDT operating mode (WDT enabled regardless of sleep)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = LFINTOSC// WDT input clock selector (WDT reference clock is the 31.0 kHz LFINTOSC)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_4096// Boot Block Size selection bits (Boot Block size is 4096 words)
#pragma config BBEN = ON       // Boot Block enable bit (Boot block enabled)
#pragma config SAFEN = ON       // Storage Area Flash enable bit (SAF enabled)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = ON        // Configuration Register Write Protection bit (Configuration registers (300000-30000Bh) write-protected)
#pragma config WRTC = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

#include "main.h"

void main(void) {
    uint8_t i;
    uint8_t cmdLen = 0;
    char cmd[0x24];
    char msgAnswer[0x24];
    uint8_t gcpLocked = 1;
    uint16_t addr;

    //Bootloader buffer stuff
    uint8_t flashBuffer[64];
    uint8_t flashBufferIdx = 0;

    /*//Clock is already be set at 64MHz in the configuration register. This is useless
    // NOSC HFINTOSC; NDIV 1;
    OSCCON1 = 0x60;
    // CSWHOLD may proceed; SOSCPWR Low power;
    OSCCON3 = 0x00;
    // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled;
    OSCEN = 0x00;
    // HFFRQ 64_MHz;
    OSCFRQ = 0x08;
    // TUN 0;
    OSCTUNE = 0x00;*/

    portsInit();

    //Read buttons for boot combos
    inButtons_t inBut;
    inBut.PORTA = PORTA;
    inBut.PORTB = PORTB;
    inBut.PORTC = PORTC;

    configInit();
    if (config.triggersMode == TRIG_MODE_ANALOG) {
        ANSELC |= 0x18; //Enable analog on triggers
        WPUC &= ~0x18; //Disable weak pullups on triggers
    }

    ADCInit(config.SXChan, config.SYChan, config.CXChan, config.CYChan);
    buttonsInit();
    SIInit();
    rumbleInit();
    //Restore configuration if X+Y+A are all pressed
    if (!inBut.X && !inBut.Y && !inBut.A) {
        configSetDefault();
        configFlashAll();
    }

    buttonsBuildLUTs();

    INTCON0 = 0x80; //Interrupts enabled with no priority

    while (1) {
        inBut.PORTA = PORTA;
        inBut.PORTB = PORTB;
        inBut.PORTC = PORTC;
        //If X+Y+Start are all pressed, don't reset the WDT
        if (inBut.X || inBut.Y || inBut.ST) {
            asm("clrwdt");
        }
        buttonsUpdate();
        cmdLen = SIGetCommand(cmd);

        if (cmdLen > 0) {
            switch(cmd[0]) {
                case SI_CMD_ID:
                case SI_CMD_RESET:
                    msgAnswer[0] = 0x09;
                    msgAnswer[1] = 0x00;
                    msgAnswer[2] = 0x03;
                    SISendMessage(msgAnswer, 3);
                break;

                case SI_CMD_POLL:
                    //Handle rumble
                    switch (cmd[2]) {
                        case 1:
                            rumbleSpin(config.rumbleIntensity);
                        break;

                        case 2:
                            rumbleBrake();
                        break;

                        default:
                            rumbleStop();
                        break;
                    }
                    //Answer
                    SISendMessage(buttonsGetMessage(cmd[1], config.triggersMode), 8);
                break;

                case SI_CMD_ORIGINS:
                case SI_CMD_CALIB:
                    msgAnswer[0] = 0x00;
                    msgAnswer[1] = 0x80;
                    msgAnswer[2] = 0x80;
                    msgAnswer[3] = 0x80;
                    msgAnswer[4] = 0x80;
                    msgAnswer[5] = 0x80;
                    msgAnswer[6] = 0x00;
                    msgAnswer[7] = 0x00;
                    msgAnswer[8] = 0x00;
                    msgAnswer[9] = 0x00;
                    SISendMessage(msgAnswer, 10);
                break;

                case GCP_CMD_LOCKUNLOCK:
                    if ((cmd[1] == 0x47) && (cmd[2] == 0x43) && (cmd[3] == 0x2B) && (cmd[4] == 0x32) && cmdLen == 5) {
                        gcpLocked = 0;
                    } else {
                        gcpLocked = 1;
                    }
                    msgAnswer[0] = GCP_ERR_NONE;
                    SISendMessage(msgAnswer, 1);
                break;

                case GCP_CMD_GETVER:
                    if (!gcpLocked) {
                        msgAnswer[0] = 0x02; //Payload
                        msgAnswer[1] = GCP2_VERSION & 0xFFUL;
                        msgAnswer[2] = (GCP2_VERSION >> 8) & 0xFF;
                        msgAnswer[3] = GCP2_HWVERSION & 0xFFUL;
                        msgAnswer[4] = (GCP2_HWVERSION >> 8) & 0xFF;
                        SISendMessage(msgAnswer, 5);
                    } else {
                        msgAnswer[0] = GCP_ERR_LOCKED;
                        SISendMessage(msgAnswer, 1);
                    }
                break;

                case GCP_CMD_WRITEEEPROM:
                    if (!gcpLocked) {
                        if (cmdLen > 3) {
                            addr = cmd[2];
                            addr = addr << 8;
                            addr |= cmd[1];
                            for (i = 3; i < cmdLen; i++) {
                                EEPROMWriteByte(addr++, cmd[i]);
                            }
                            msgAnswer[0] = GCP_ERR_NONE;
                            SISendMessage(msgAnswer, 1);
                        } else {
                            msgAnswer[0] = GCP_ERR_WRONGARG;
                            SISendMessage(msgAnswer, 1);
                        }
                    } else {
                        msgAnswer[0] = GCP_ERR_LOCKED;
                        SISendMessage(msgAnswer, 1);
                    }
                break;

                case GCP_CMD_READEEPROM:
                    if (!gcpLocked) {
                        if (cmdLen == 4) {
                            addr = cmd[2];
                            addr = addr << 8;
                            addr |= cmd[1];
                            uint8_t len = cmd[3];
                            if (len == 0) {
                                msgAnswer[0] = GCP_ERR_WRONGARG;
                                SISendMessage(msgAnswer, 1);
                            } else {
                                msgAnswer[0] = GCP_ERR_NONE;
                                for (i = 0; i < len && i < 0x20; i++) {
                                    msgAnswer[i + 1] = EEPROMReadByte(addr++);
                                }
                                SISendMessage(msgAnswer, len + 1);
                            }
                        } else {
                            msgAnswer[0] = GCP_ERR_WRONGARG;
                            SISendMessage(msgAnswer, 1);
                        }
                    } else {
                        msgAnswer[0] = GCP_ERR_LOCKED;
                        SISendMessage(msgAnswer, 1);
                    }
                break;

                case GCP_CMD_RESET:
                    asm("reset");
                break;

                case GCP_CMD_BOOTBL:
                    if (!gcpLocked) {
                        msgAnswer[0] = GCP_ERR_NONE;
                        SISendMessage(msgAnswer, 1);
                        bootBootloader();
                    } else {
                        msgAnswer[0] = GCP_ERR_LOCKED;
                        SISendMessage(msgAnswer, 1);
                    }
                break;

                case GCP_CMD_SETMAPBYTE0:
                    if (!gcpLocked) {
                        if (cmdLen == (N_BUTTONS + 1)) {
                            buttonsSetMapByte0(&cmd[1]);
                            msgAnswer[0] = GCP_ERR_NONE;
                            SISendMessage(msgAnswer, 1);
                        } else {
                            msgAnswer[0] = GCP_ERR_WRONGARG;
                            SISendMessage(msgAnswer, 1);
                        }
                    } else {
                        msgAnswer[0] = GCP_ERR_LOCKED;
                        SISendMessage(msgAnswer, 1);
                    }
                break;

                case GCP_CMD_SETMAPBYTE1:
                    if (!gcpLocked) {
                        if (cmdLen == (N_BUTTONS + 1)) {
                            buttonsSetMapByte1(&cmd[1]);
                            msgAnswer[0] = GCP_ERR_NONE;
                            SISendMessage(msgAnswer, 1);
                        } else {
                            msgAnswer[0] = GCP_ERR_WRONGARG;
                            SISendMessage(msgAnswer, 1);
                        }
                    } else {
                        msgAnswer[0] = GCP_ERR_LOCKED;
                        SISendMessage(msgAnswer, 1);
                    }
                break;

                case GCP_CMD_GETMAPBYTE0:
                    if (!gcpLocked) {
                        uint8_t* btnMsg = buttonsGetMapByte0();
                        msgAnswer[0] = GCP_ERR_NONE;
                        for (i = 0; i < N_BUTTONS; i++) {
                            msgAnswer[i + 1] = btnMsg[i];
                        }
                        SISendMessage(msgAnswer, N_BUTTONS + 1);
                    } else {
                        msgAnswer[0] = GCP_ERR_LOCKED;
                        SISendMessage(msgAnswer, 1);
                    }
                break;

                case GCP_CMD_GETMAPBYTE1:
                    if (!gcpLocked) {
                        uint8_t* btnMsg = buttonsGetMapByte1();
                        msgAnswer[0] = GCP_ERR_NONE;
                        for (i = 0; i < N_BUTTONS; i++) {
                            msgAnswer[i + 1] = btnMsg[i];
                        }
                        SISendMessage(msgAnswer, N_BUTTONS + 1);
                    } else {
                        msgAnswer[0] = GCP_ERR_LOCKED;
                        SISendMessage(msgAnswer, 1);
                    }
                break;

                case GCP_CMD_REBUILDLUT:
                    if (!gcpLocked) {
                        buttonsBuildLUTs();
                        msgAnswer[0] = GCP_ERR_NONE;
                        SISendMessage(msgAnswer, 1);
                    } else {
                        msgAnswer[0] = GCP_ERR_LOCKED;
                        SISendMessage(msgAnswer, 1);
                    }
                break;

                default: //Unknown command
                    SIClear();
                break;
            }
        }
    }

    return;
}

void portsInit(void) {
    //Stop interrupts
    bool state = (unsigned char)GIE;
    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; //Unlock PPS

    //Data line wired to RB2
    //Reception
    T6INPPS = 0x0A; //RB2 to T6INPPS. Used as reset trigger
    SMT1SIGPPS = 0x0A; //RB2

    //Transmission
    RB2PPS = 0x04; //CLC4 to RB2

    //Rumble PWM
    RB3PPS = 0x09; //CCP1

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; //Lock PPS
    GIE = state; //Restore interrupts

    LATB = 0xEF;

    //Bootloader won't emulate analog triggers
    TRISA = 0xFF;
    TRISB = 0xE7;
    TRISC = 0xFF;
    ANSELA = 0x0F;
    ANSELB = 0x00;
    ANSELC = 0x00;
    WPUA = 0xF0;
    WPUB = 0x27;
    WPUC = 0xFF;
    ODCONA = 0x00;
    ODCONB = 0x04;
    ODCONC = 0x00;
}

void bootBootloader(void) {
    INTCON0 = 0x00; //Disable interrupts
    NCO1ACCL = 0x01;
    STKPTR = 0x00; //Clean up stack
    IVTLOCK = 0x55;
    IVTLOCK = 0xAA;
    IVTLOCK = 0;
    IVTLOCKED = 0;
    IVTBASE = 8; //Set interrupt base address
    IVTLOCK = 0x55;
    IVTLOCK = 0xAA;
    IVTLOCK = 1;
    IVTLOCKED = 1;
    //Jump to bootloader
    PCLATU = 0x00;
    PCLATH = 0x00;
    PCL = 0x00;
}
