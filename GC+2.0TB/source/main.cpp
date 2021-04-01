#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <ogc/lwp_watchdog.h>
#include <ogc/machine/processor.h>
#include <wiiuse/wpad.h>
#include <ogc/ipc.h>
#include <fat.h>
#include <math.h>
#include <string>
#include <list>

#include "gpio.h"
#include "i2c.h"
#include "gcplus.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

#define AVE_ADDR (0x70 << 1)

void startTest();
void padWait(int* held);
void printButtons(int held);
void testStickAxis(int ax);

std::list<std::string> errorList;

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	// Initialise the video system
	VIDEO_Init();
	PAD_Init();
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	int col, row;
	CON_GetPosition(&col, &row);

	u8 error;

	i2c_init();
	printf("\n\n");
	i2c_stop();

	for (int i = 0; i < 127; i++)
	{
		i2c_start();
		if (i2c_sendByte(i << 1))
		{
			printf("Found device on address: %02X\n", i);
		}
		i2c_stop();
	}

	printf("SI type: %08X\n", SI_GetType(0));
	printf("Press A on the Wiimote to start the test\n");
	while(1) {
		PAD_ScanPads();
		WPAD_ScanPads();


		int down = WPAD_ButtonsDown(0);
		if (down & WPAD_BUTTON_HOME) exit(0);
		if (down & WPAD_BUTTON_A) startTest();

		int held = PAD_ButtonsHeld(0);

		/*if (ticks_to_millisecs(gettime() - now) > 1000) {
			regVal = ~regVal;
			i2c_write8(0x40, 0x01, regVal, &error);
			now = gettime();
		}

		printf("\x1b[5;0H%08X %02X", held, regVal);*/

		// Wait for the next frame
		VIDEO_WaitVSync();
	}

	return 0;
}

void padWait(int* held) {
	u64 now = gettime();
	while (ticks_to_millisecs(gettime() - now) < 200) {
		PAD_ScanPads();
	}
	if (held != NULL) *held = PAD_ButtonsHeld(0);
}

void printButtons(int held) {
	if (held & PAD_BUTTON_A) printf("A ");
	if (held & PAD_BUTTON_B) printf("B ");
	if (held & PAD_BUTTON_X) printf("X ");
	if (held & PAD_BUTTON_Y) printf("Y ");
	if (held & PAD_TRIGGER_Z) printf("Z ");
	if (held & PAD_TRIGGER_R) printf("RT ");
	if (held & PAD_TRIGGER_L) printf("LT ");
	if (held & PAD_BUTTON_START) printf("START ");
	if (held & PAD_BUTTON_LEFT) printf("LEFT ");
	if (held & PAD_BUTTON_RIGHT) printf("RIGHT ");
	if (held & PAD_BUTTON_DOWN) printf("DOWN ");
	if (held & PAD_BUTTON_UP) printf("UP ");
	if (PAD_StickX(0)) printf("SX(%d) ", PAD_StickX(0));
	if (PAD_StickY(0)) printf("SY(%d) ", PAD_StickY(0));
	if (PAD_SubStickX(0)) printf("CX(%d) ", PAD_SubStickX(0));
	if (PAD_SubStickY(0)) printf("CY(%d) ", PAD_SubStickY(0));
	if (PAD_TriggerL(0)) printf("L(%u) ", PAD_TriggerL(0));
	if (PAD_TriggerR(0)) printf("R(%u) ", PAD_TriggerR(0));
}

void printButtonsToString(int held, char* str) {
	if (held & PAD_BUTTON_A) sprintf(str, "%sA ", str);
	if (held & PAD_BUTTON_B) sprintf(str, "%sB ", str);
	if (held & PAD_BUTTON_X) sprintf(str, "%sX ", str);
	if (held & PAD_BUTTON_Y) sprintf(str, "%sY ", str);
	if (held & PAD_TRIGGER_Z) sprintf(str, "%sZ ", str);
	if (held & PAD_TRIGGER_R) sprintf(str, "%sRT ", str);
	if (held & PAD_TRIGGER_L) sprintf(str, "%sLT ", str);
	if (held & PAD_BUTTON_START) sprintf(str, "%sSTART ", str);
	if (held & PAD_BUTTON_LEFT) sprintf(str, "%sLEFT ", str);
	if (held & PAD_BUTTON_RIGHT) sprintf(str, "%sRIGHT ", str);
	if (held & PAD_BUTTON_DOWN) sprintf(str, "%sDOWN ", str);
	if (held & PAD_BUTTON_UP) sprintf(str, "%sUP ", str);
	if (PAD_StickX(0)) sprintf(str, "%sSX(%d) ", str, PAD_StickX(0));
	if (PAD_StickY(0)) sprintf(str, "%sSY(%d) ", str, PAD_StickY(0));
	if (PAD_SubStickX(0)) sprintf(str, "%sCX(%d) ", str, PAD_SubStickX(0));
	if (PAD_SubStickY(0)) sprintf(str, "%sCY(%d) ", str, PAD_SubStickY(0));
	if (PAD_TriggerL(0)) sprintf(str, "%sL(%u) ", str, PAD_TriggerL(0));
	if (PAD_TriggerR(0)) sprintf(str, "%sR(%u) ", str, PAD_TriggerR(0));
}

void testStickAxis(int ax) {
	u8 error;
	i2c_write8(0x40, 0x03, 0x80, &error); //SX
	i2c_write8(0x40, 0x04, 0x80, &error); //SY
	i2c_write8(0x40, 0x05, 0x80, &error); //CX
	i2c_write8(0x40, 0x06, 0x80, &error); //CY

	bool failed = false;

	for (int i = 0; i < 8; i++) {
		int stickVal;
		i2c_write8(0x40, 0x03 + ax, i << 5, &error);
		padWait(NULL);
		switch (ax) {
			case 0:
				stickVal = PAD_StickX(0) + 128;
			break;
			case 1:
				stickVal = PAD_StickY(0) + 128;
			break;
			case 2:
				stickVal = PAD_SubStickX(0) + 128;
			break;
			case 3:
				stickVal = PAD_SubStickY(0) + 128;
			break;
		}
		printf("Sent 0x%02X. Got 0x%02X\n", i << 5, stickVal);

		if (abs(stickVal - (i << 5)) > 32) {
			failed = true;
		}
	}

	if (failed) {
		switch (ax) {
			case 0:
				errorList.push_back("Error on SX");
			break;
			case 1:
				errorList.push_back("Error on SY");
			break;
			case 2:
				errorList.push_back("Error on CX");
			break;
			case 3:
				errorList.push_back("Error on CY");
			break;
		}
	}
}

void startTest() {
	int held;
	u8 error;
	u8 mode;
	u16 version;
	char errString[512];
	u8 oldConfig[13];

	errorList.clear();

	//Write default values
	i2c_write8(0x40, 0x00, 0xFF, &error); //LATA
	i2c_write8(0x40, 0x01, 0xFF, &error); //LATB
	i2c_write8(0x40, 0x02, 0xFF, &error); //LATC
	i2c_write8(0x40, 0x03, 0x80, &error); //SX
	i2c_write8(0x40, 0x04, 0x80, &error); //SY
	i2c_write8(0x40, 0x05, 0x80, &error); //CX
	i2c_write8(0x40, 0x06, 0x80, &error); //CY
	padWait(&held);

	printf("Starting test\n");
	if (!GCPlus::unlock()) {
		printf("Couldn't unlock GC+\n");
		return;
	}
	if (!GCPlus::getFWVer(&version)) {
		printf("Couldn't get version\n");
		return;
	}
	if (!GCPlus::getMode(&mode)) {
		printf("Couldn't get mode\n");
		return;
	}
	//Now write default configuration
    u8 tempConfig[13] = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x02, 0x03, 0x00, 0x01, 0x00};
    GCPlus::readEEPROM(0x06, oldConfig, 13);
    GCPlus::writeEEPROM(0x06, tempConfig, 13);
    GCPlus::reset();

	u64 now = gettime();
	while (ticks_to_millisecs(gettime() - now) < 2000);

	printf("Running FW v%u.%u. Mode: %u\n", version >> 3, version & 0x0007, mode);

	printf("Testing A... "); //RA3
	i2c_write8(0x40, 0x00, 0xF7, &error);
	padWait(&held);
	if (held == PAD_BUTTON_A && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on A. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}
	PAD_ControlMotor(0, 1);

	printf("Testing B... "); //RA7
	i2c_write8(0x40, 0x00, 0x7F, &error);
	padWait(&held);
	if (held == PAD_BUTTON_B && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on B. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing X... "); //RA6
	i2c_write8(0x40, 0x00, 0xBF, &error);
	padWait(&held);
	if (held == PAD_BUTTON_X && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on X. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing Y... "); //RC0
	i2c_write8(0x40, 0x00, 0xFF, &error);
	i2c_write8(0x40, 0x02, 0xFE, &error);
	padWait(&held);
	if (held == PAD_BUTTON_Y && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on Y. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing Z... "); //RC1
	i2c_write8(0x40, 0x02, 0xFD, &error);
	padWait(&held);
	if (held == PAD_TRIGGER_Z && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on Z. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing R... "); //RC6
	i2c_write8(0x40, 0x02, 0xBF, &error);
	padWait(&held);
	if (!held && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && (PAD_TriggerR(0) > 200)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on R. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing L... "); //RC7
	i2c_write8(0x40, 0x02, 0x7F, &error);
	padWait(&held);
	if (!held && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerR(0) && (PAD_TriggerL(0) > 200)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on L. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing RT... "); //RC5
	i2c_write8(0x40, 0x02, 0xDF, &error);
	padWait(&held);
	if (held == PAD_TRIGGER_R && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on RT. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing LT... "); //RB0
	i2c_write8(0x40, 0x02, 0xFF, &error);
	i2c_write8(0x40, 0x01, 0xFE, &error);
	padWait(&held);
	if (held == PAD_TRIGGER_L && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on LT. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing START... "); //RC2
	i2c_write8(0x40, 0x01, 0xFF, &error);
	i2c_write8(0x40, 0x02, 0xFB, &error);
	padWait(&held);
	if (held == PAD_BUTTON_START && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on START. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing RIGHT... "); //RB4
	i2c_write8(0x40, 0x01, 0xEF, &error);
	i2c_write8(0x40, 0x02, 0xFF, &error);
	padWait(&held);
	if (held == PAD_BUTTON_RIGHT && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on RIGHT. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing LEFT... "); //RB5
	i2c_write8(0x40, 0x01, 0xDF, &error);
	padWait(&held);
	if (held == PAD_BUTTON_LEFT && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on LEFT. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing UP... "); //RB1
	i2c_write8(0x40, 0x01, 0xFD, &error);
	padWait(&held);
	if (held == PAD_BUTTON_UP && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on UP. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	printf("Testing DOWN... "); //RB3
	i2c_write8(0x40, 0x01, 0xF7, &error);
	padWait(&held);
	if (held == PAD_BUTTON_DOWN && !PAD_StickX(0) && !PAD_StickY(0) && !PAD_SubStickX(0) && !PAD_SubStickY(0) && !PAD_TriggerL(0) && !PAD_TriggerR(0)) {
		printf("OK\n");
	} else {
		printf("Error: ");
		printButtons(held);
		printf("\n");
		printButtonsToString(held, errString);
		sprintf(errString, "Error on DOWN. Got %s", held ? errString : "nothing");
		errorList.push_back(errString);
	}

	PAD_ControlMotor(0, 0);

	printf("Testing SX\n");
	testStickAxis(0);
	printf("Testing SY\n");
	testStickAxis(1);
	printf("Testing CX\n");
	testStickAxis(2);
	printf("Testing CY\n");
	testStickAxis(3);
	if (errorList.size() == 0) {
		printf("Test succeeded!\n");
	} else {
		printf("Test got the following errors:\n");
		for (auto & el : errorList) {
			printf("\t%s\n", el.c_str());
		}
	}

	//Restore config
    GCPlus::writeEEPROM(0x06, oldConfig, 13);
    GCPlus::reset();
}
