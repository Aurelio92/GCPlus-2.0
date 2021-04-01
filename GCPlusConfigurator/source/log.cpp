#include <stdio.h>
#include <stdarg.h>

void log(const char* fmt, ...) {
    FILE* fp = fopen("/log.txt", "r");
    if (fp) {
        fclose(fp);
        fp = fopen("/log.txt", "a");
    } else {
        fp = fopen("/log.txt", "w");
    }

    if (fp) {
        char buffer[256];
        va_list args;

        va_start(args, fmt);
        vsnprintf(buffer, 256, fmt, args);
        fprintf(fp, buffer);
        va_end(args);
        fclose(fp);
    }
}