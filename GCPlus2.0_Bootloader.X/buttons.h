#ifndef _BUTTONS_H
#define _BUTTONS_H

typedef union {
    struct {
        //PORTA
        unsigned    : 4;
        unsigned A  : 1;
        unsigned B  : 1;
        unsigned Y  : 1;
        unsigned X  : 1;

        //PORTB
        unsigned DR : 1;
        unsigned DL : 1;
        unsigned    : 3;
        unsigned Z2 : 1;
        unsigned    : 2;

        //PORTC
        unsigned Z  : 1;
        unsigned ST : 1;
        unsigned RD : 1;
        unsigned RA : 1;
        unsigned LA : 1;
        unsigned LD : 1;
        unsigned DU : 1;
        unsigned DD : 1;
    };
    struct {
        uint8_t PORTA;
        uint8_t PORTB;
        uint8_t PORTC;
    };
} inButtons_t;

typedef union {
    struct {
        unsigned A  : 1;
        unsigned B  : 1;
        unsigned X  : 1;
        unsigned Y  : 1;
        unsigned ST : 1;
        unsigned    : 3;
        unsigned DL : 1;
        unsigned DR : 1;
        unsigned DD : 1;
        unsigned DU : 1;
        unsigned Z  : 1;
        unsigned RD : 1;
        unsigned LD : 1;
        unsigned er : 1;
        uint8_t LA;
        uint8_t RA;
    };
    struct
    {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte6;
        uint8_t byte7;
    };
} outButtons_t;

#ifdef  __cplusplus
extern "C" {
#endif

void buttonsInit(void);
void buttonsUpdate(void);
uint8_t* buttonsGetMessage(uint8_t analogMode, uint8_t triggersMode);

#ifdef  __cplusplus
}
#endif

#endif