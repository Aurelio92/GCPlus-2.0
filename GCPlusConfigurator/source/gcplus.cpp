#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <ogc/lwp_watchdog.h>
#include <ogc/machine/processor.h>
#include <ogc/lwp_threads.h>
#include <unistd.h>
#include "gcplus.h"
#include "log.h"
#include "main.h"

#define EEPROM_PACKET_SIZE 4
#define FLASH_PACKET_SIZE 32
#define FLASH_BLOCK_SIZE 64
#define FLASH_BASE_ADDRESS 0x2000

namespace GCPlus {
    static bool gotPadAnswer = false;

    static void padCallback(s32 chan, u32 type) {
        gotPadAnswer = true;
    }

    static bool transferData(void *out, u32 out_len, void *in, u32 in_len) {
        #ifdef DEMOBUILD
        return true;
        #endif
        lockSIMutex();
        SI_DisablePolling(0xf0000000);
        while (SI_Busy());
        gotPadAnswer = false;
        SI_Transfer(0, out, out_len, in, in_len, padCallback, 0);
        u64 now = gettime();
        while ((ticks_to_millisecs(gettime() - now) < GCP_TRANSFER_DELAY) && !gotPadAnswer);
        now = gettime();
        while (ticks_to_microsecs(gettime() - now) < GCP_AFTERTRANSFER_DELAY);
        PAD_Reset(0xf0000000);
        unlockSIMutex();
        return gotPadAnswer;
    }

    bool isV1() {
        u8 cmd = 0x31;
        u8 answer[2];

        if (!transferData(&cmd, 1, answer, 2)) {
            return false;
        }

        if (answer[0] == 1 && answer[1] == 0)
            return true;

        return false;
    }

    bool isV2() {
        u16 version;

        if (!unlock())
            return false;

        if (!getHWVer(&version))
            return false;

        if (!lock())
            return false;

        if ((version >> 3) == 2)
            return true;

        return false;
    }

    bool unlock() {
        u8 cmd[5] = {GCP_CMD_LOCKUNLOCK, 0x47, 0x43, 0x2B, 0x32};
        u8 answer;
        if (!transferData(cmd, 5, &answer, 1)) {
            return false;
        }

        if (answer == GCP_ERR_NONE)
            return true;
        return false;
    }

    bool lock() {
        u8 cmd[5] = {GCP_CMD_LOCKUNLOCK, 0x00, 0x00, 0x00, 0x00};
        u8 answer;
        if (!transferData(cmd, 5, &answer, 1)) {
            return false;
        }

        if (answer == GCP_ERR_NONE)
            return true;
        return false;
    }

    bool getFWVer(u16* version) {
        u8 cmd = GCP_CMD_GETVER;
        u8 answer[5];
        if (!transferData(&cmd, 1, answer, 5)) {
            return false;
        }

        if (answer[0] == GCP_ERR_LOCKED)
            return false;

        *version = ((answer[2] << 8) | answer[1]);
        return true;
    }

    bool getHWVer(u16* version) {
        u8 cmd = GCP_CMD_GETVER;
        u8 answer[5];
        if (!transferData(&cmd, 1, answer, 5)) {
            return false;
        }

        if (answer[0] == GCP_ERR_LOCKED)
            return false;

        *version = ((answer[4] << 8) | answer[3]);
        return true;
    }

    bool getMode(u8* mode) {
        u8 cmd = GCP_CMD_GETVER;
        u8 answer[5];
        *mode = 0;
        if (!transferData(&cmd, 1, answer, 5)) {
            return false;
        }

        if (answer[0] == GCP_ERR_LOCKED)
            return false;

        *mode = answer[0];
        return true;
    }

    bool writeEEPROM(u8 addr, u8* data, u8 len) {
        u8* cmd = (u8*)malloc(3 + EEPROM_PACKET_SIZE);
        u8* test = (u8*)malloc(EEPROM_PACKET_SIZE);
        u8 answer;

        while (len) {
            bool success = false;
            u8 pLen = len > EEPROM_PACKET_SIZE ? EEPROM_PACKET_SIZE : len;

            cmd[0] = GCP_CMD_WRITEEEPROM;
            cmd[1] = addr;
            cmd[2] = 0x00;
            for (int i = 0; i < pLen; i++) {
                cmd[3 + i] = data[i];
            }
            for (int i = 0; i < 10 && !success; i++) {
                if (!transferData(cmd, 3 + pLen, &answer, 1)) {
                    free(cmd);
                    free(test);
                    return false;
                }
                readEEPROM(addr, test, pLen);
                if (!memcmp(test, data, pLen))
                    success = true;
            }

            if (answer != GCP_ERR_NONE || !success) {
                free(cmd);
                free(test);
                return false;
            }

            len -= pLen;
            addr += pLen;
            data += pLen;
        }

        free(cmd);
        free(test);

        return true;
    }

    bool readEEPROM(u8 addr, u8* output, u8 len) {
        u8 answer[EEPROM_PACKET_SIZE + 1];
        u8 cmd[4] = {GCP_CMD_READEEPROM, addr, 0x00, len};

        while (len) {
            u8 pLen = len > EEPROM_PACKET_SIZE ? EEPROM_PACKET_SIZE : len;
            cmd[3] = pLen;
            if (!transferData(cmd, 4, answer, pLen + 1)) {
                return false;
            }

            if (answer[0] == GCP_ERR_NONE) {
                /*for (int i = 0; i < pLen + 1; ++i) {
                    printf("%02 X", answer[i]);
                }*/
                memcpy(output, &answer[1], pLen);
                output += pLen;
                len -= pLen;
                addr += pLen;
                cmd[1] = addr;
            } else {
                return false;
            }
        }

        return true;
    }

    bool reset() {
        u8 cmd = GCP_CMD_RESET;

        if (!transferData(&cmd, 1, NULL, 0)) {
            return false;
        }

        return true;
    }

    bool bootBootloader() {
        u8 cmd = GCP_CMD_BOOTBL;
        u8 answer;

        if (!transferData(&cmd, 1, &answer, 1)) {
            return false;
        }

        return true;
    }

    bool resetIDX() {
        u8 cmd = GCP_CMD_RESETIDX;
        u8 answer;

        if (!transferData(&cmd, 1, &answer, 1)) {
            return false;
        }

        if (answer == GCP_ERR_NONE)
            return true;
        return false;
    }

    bool fillBuffer(u8* data, u8 len, u8* error) {
        u8* cmd = (u8*)malloc(1 + len);
        u8 answer;

        cmd[0] = GCP_CMD_FILLBUFFER;
        for (int i = 0; i < len; i++) {
            cmd[1 + i] = data[i];
        }

        if (!transferData(cmd, 1 + len, &answer, 1)) {
            *error = 1;
            free(cmd);
            return false;
        }

        free(cmd);

        *error = answer;
        if (answer == GCP_ERR_NONE)
            return true;
        return false;
    }

    bool readBuffer(u8* output, u8 len) {
        u8 cmd[2] = {GCP_CMD_READBUFFER, len};
        u8* answer = (u8*)malloc(len + 1);

        if (!transferData(&cmd, 1, answer, len + 1)) {
            free(answer);
            return false;
        }

        if (answer[0] == GCP_ERR_NONE) {
            memcpy(output, &answer[1], len);
            free(answer);
            return true;
        }

        free(answer);
        return false;
    }

    bool writeFlash(u16 addr) {
        u8 cmd[3] = {GCP_CMD_WRITEFLASH, (u8)(addr & 0xFF), (u8)((addr >> 8) & 0xFF)};
        u8 answer;

        if (!transferData(cmd, 3, &answer, 1)) {
            return false;
        }

        if (answer == GCP_ERR_NONE)
            return true;
        return false;
    }

    bool readFlash(u16 addr) {
        u8 cmd[3] = {GCP_CMD_READFLASH, (u8)(addr & 0xFF), (u8)((addr >> 8) & 0xFF)};
        u8 answer;

        if (!transferData(cmd, 3, &answer, 1)) {
            return false;
        }

        if (answer == GCP_ERR_NONE)
            return true;
        return false;
    }

    bool bootPayload() {
        u8 cmd = GCP_CMD_BOOTPAYLOAD;
        u8 answer;

        if (!transferData(&cmd, 1, &answer, 1)) {
            return false;
        }

        return true;
    }

    bool flashPayload(u8* payload, u32 payloadSize, mutex_t mutex, float* progress) {
        u8 buffer[FLASH_PACKET_SIZE];
        u32 packetIndex = 0;
        u16 address = FLASH_BASE_ADDRESS;
        u32 totalSize = payloadSize;
        u8 error;

        //Make sure flash buffer index is reset
        if (!resetIDX()) {
            return false;
        }

        while (payloadSize) {
            //Fill packet
            for (int i = 0; (i < FLASH_PACKET_SIZE) && payloadSize; i++) {
               buffer[i] = *payload++;
               payloadSize--;
            }
            packetIndex++;
            if (!fillBuffer(buffer, FLASH_PACKET_SIZE, &error)) {
                return false;
            }

            //Flash the internal buffer if a whole block has been sent
            //Also force the flashing if we got to the end of the payload
            if ((packetIndex == (FLASH_BLOCK_SIZE / FLASH_PACKET_SIZE)) || !payloadSize) {
                packetIndex = 0;
                if (!writeFlash(address)) {
                    *progress = (float)packetIndex + 0.004;
                    return false;
                }
                if (!resetIDX()) {
                    return false;
                }

                address += FLASH_BLOCK_SIZE;
            }

            #ifdef DEMOBUILD
            {
                u64 now = gettime();
                while (ticks_to_millisecs(gettime() - now) < 10);
            }
            #endif

            LWP_MutexLock(mutex);
            *progress = 100.0f * (float)(totalSize - payloadSize) / totalSize;
            LWP_MutexUnlock(mutex);
            usleep(100);
        }

        return true;
    }
};
