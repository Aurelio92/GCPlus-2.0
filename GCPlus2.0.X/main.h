#ifndef _MAIN_H_
#define _MAIN_H_

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "nvm.h"
#include "config.h"
#include "adc.h"
#include "buttons.h"
#include "si.h"
#include "rumble.h"

#define GCP2_VERSION_MAJ 2U
#define GCP2_VERSION_MIN 3U
#define GCP2_VERSION (((GCP2_VERSION_MAJ & 0x1FFF) << 3) | (GCP2_VERSION_MIN & 0x0007))

#define GCP2_HWVERSION_MAJ 2U
#define GCP2_HWVERSION_MIN 0U
#define GCP2_HWVERSION (((GCP2_HWVERSION_MAJ & 0x1FFF) << 3) | (GCP2_HWVERSION_MIN & 0x0007))

#define PAYLOAD_ADDR 0x2000 //4096 words reserved to the bootloader

#define ABS(n) ((n >= 0) ? (n) : (-n))

/*! \fn void portsInit(void)
    \brief initialize GPIO ports and PPS to the proper state
*/
void portsInit(void);

/*! \fn void bootBootloader(void)
    \brief cleans up the stack, sets the new interrupt base address and jumps to the bootloader address
*/
void bootBootloader(void);

#endif
