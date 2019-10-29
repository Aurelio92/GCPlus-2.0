#ifndef _MAIN_H_
#define _MAIN_H_

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "nvm.h"
#include "config.h"
#include "adc.h"
#include "buttons.h"
#include "si.h"

#define GCP2_VERSION_MAJ 2U
#define GCP2_VERSION_MIN 0U
#define GCP2_VERSION (((GCP2_VERSION_MAJ & 0x1FFF) << 13) | (GCP2_VERSION_MIN & 0x0007))

#define GCP2_HWVERSION_MAJ 2U
#define GCP2_HWVERSION_MIN 0U
#define GCP2_HWVERSION (((GCP2_HWVERSION_MAJ & 0x1FFF) << 13) | (GCP2_HWVERSION_MIN & 0x0007))

#define PAYLOAD_ADDR 0x1000

#define ABS(n) ((n >= 0) ? (n) : (-n))

/*! \fn void portsInit(void)
    \brief initialize GPIO ports and PPS to the proper state
*/
void portsInit(void);

/*! \fn void LUTBuild(uint8_t* LUT, uint8_t minVal, uint8_t maxVal, uint8_t origin, uint8_t dz, uint8_t invert);
    \brief builds a 256 bytes LUT for analog inputs scaling
    \param LUT the destination address
    \param minVal the minimum value of the input range
    \param maxVal the maximum value of the input range
    \param origin the center position of the input value
    \param dz the deadzone radius of the input range
    \param invert a flag to invert the axis
*/
void LUTBuild(uint8_t* LUT, uint8_t minVal, uint8_t maxVal, uint8_t origin, uint8_t dz, uint8_t invert);

/*! \fn void bootPayload(void)
    \brief cleans up the stack, sets the new interrupt base address and jumps to the payload address
*/
void bootPayload(void);

#endif