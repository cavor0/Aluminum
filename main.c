#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/* * ALUMINUM PROGRAMMING LANGUAGE 
 * Core v0.1 - Universal Edition (Linux, macOS, Windows)
 * Developed by: Scott (Muhammed Ali Akbaş)
 */

#define MAX_ATOMS 256
#define VERSION "0.1"

typedef enum {
    AL_SUCCESS = 0,
    AL_ERR_UNKNOWN_CMD,
    AL_ERR_BOUNDS,
    AL_ERR_SEALED,
    AL_ERR_ARGS
} AL_STATUS;

int atoms[MAX_ATOMS] = {0};
bool seals[MAX_ATOMS] = {false};

void al_log(AL_STATUS status, const char* detail) {
    if (status == AL_SUCCESS) return;
    printf("\n[AL-CORE ERROR %d]: ", status);
    switch (status) {
        case AL_ERR_UNKNOWN_CMD: printf("Undefined command: '%s'\n", detail); break;
        case AL_ERR_BOUNDS:      printf("Atom out of bounds (0-%d).\n", MAX_ATOMS-1); break;
        case AL_ERR_SEALED:      printf("Seal violation! Atom '%s' is locked.\n", detail); break;
        case AL_ERR_ARGS:        printf("Invalid arguments.\n"); break;
        default:                 printf("Critical failure.\n");
    }
}

AL_STATUS al_execute(char* line) {
    char cmd[16] = {0};
    int addr = -1, val = 0;
    
    if (line[0] == '/' || line[0] == '\n' || line[0] == '\r' || line[0] == '\0') return AL_SUCCESS;

    int parsed = sscanf(line, "%s %d %d", cmd, &addr, &val);
    if (parsed < 1) return AL_SUCCESS;

    if (strcmp(cmd, "DEFINE") == 0) {
        if (parsed < 3) return AL_ERR_ARGS;
        if (addr < 0 || addr >= MAX_ATOMS) return AL_ERR_BOUNDS;
        if (seals[addr]) return AL_ERR_SEALED;
        atoms[addr] = val;
    }
    else if (strcmp(cmd, "SEAL") == 0) {
        if (parsed < 2) return AL_ERR_ARGS;
        if (addr < 0 || addr >= MAX_ATOMS) return AL_ERR_BOUNDS;
        seals[addr] = true;
        printf("[SYSTEM] Atom %d sealed.\n", addr);
    }
    else if (strcmp(cmd, "PULSE") == 0) {
        if (parsed < 2) return AL_ERR_ARGS;
        if (addr < 0 || addr >= MAX_ATOMS) return AL_ERR_BOUNDS;
        printf(">>> PULSE [%d]: %d\n", addr, atoms[addr]);
    }
    else if (strcmp(cmd, "CORE") == 0) {
        printf("Aluminum Core [%s] | Author: Scott | Status: Online\n", VERSION);
    }
    else if (strcmp(cmd, "EXIT") == 0) {
        exit(0);
    }
    else {
        return AL_ERR_UNKNOWN_CMD;
    }
    return AL_SUCCESS;
}

int main(int argc, char *argv[]) {
    char buffer[1024];

    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            printf("Error: Could not open %s\n", argv[1]);
            return 1;
        }
        while (fgets(buffer, sizeof(buffer), file)) {
            AL_STATUS res = al_execute(buffer);
            if (res != AL_SUCCESS) {
                char cmd_err[16]; sscanf(buffer, "%s", cmd_err);
                al_log(res, cmd_err);
            }
        }
        fclose(file);
        return 0;
    }

    printf("Aluminum Core [%s] by Scott\nType 'EXIT' to quit.\n\n", VERSION);
    while (true) {
        printf("al> ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        AL_STATUS result = al_execute(buffer);
        if (result != AL_SUCCESS) {
            char cmd_err[16]; sscanf(buffer, "%s", cmd_err);
            al_log(result, cmd_err);
        }
    }
    return 0;
}
