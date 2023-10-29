#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../colors/colors.h"

void homeDir(char* homeDir) {
    char *tempHomeDir = getenv("HOME");

    if (tempHomeDir == NULL) {
        printf(RED "Unable to determine the user's home directory.\n" RESET);
        exit(1); // Exit with an error code
    }

    strcpy(homeDir, tempHomeDir);
}
