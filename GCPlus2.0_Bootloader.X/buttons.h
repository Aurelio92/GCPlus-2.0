#ifndef _BUTTONS_H_
#define _BUTTONS_H_

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

enum {
    BUTTON_A_ID = 0,
    BUTTON_B_ID,
    BUTTON_X_ID,
    BUTTON_Y_ID,
    BUTTON_ST_ID,
    BUTTON_DL_ID,
    BUTTON_DR_ID,
    BUTTON_DD_ID,
    BUTTON_DU_ID,
    BUTTON_Z_ID,
    BUTTON_RD_ID,
    BUTTON_LD_ID,
    BUTTON_LA_ID,
    BUTTON_RA_ID,
    BUTTON_Z2_ID,
    N_BUTTONS
} ButtonsID;

void buttonsInit(void);
void buttonsUpdate(void);
uint8_t* buttonsGetMessage(uint8_t analogMode, uint8_t triggersMode);
void buttonsSetMapByte0(uint8_t* map);
void buttonsSetMapByte1(uint8_t* map);
uint8_t* buttonsGetMapByte0(void);
uint8_t* buttonsGetMapByte1(void);

/*! \fn void buttonsBuildLUT(uint8_t* LUT, uint8_t minVal, uint8_t maxVal, uint8_t origin, uint8_t dz, uint8_t invert);
    \brief builds a 256 bytes LUT for analog inputs scaling
    \param LUT the destination address
    \param minVal the minimum value of the input range
    \param maxVal the maximum value of the input range
    \param origin the center position of the input value
    \param dz the deadzone radius of the input range
    \param dzMode the deadzone mode from configuration EEPROM
    \param invert a flag to invert the axis
*/
void buttonsBuildLUT(uint8_t* LUT, uint8_t minVal, uint8_t maxVal, uint8_t origin, uint8_t dz, uint8_t dzMode, uint8_t invert);

void buttonsBuildLUTs(void);

#endif
