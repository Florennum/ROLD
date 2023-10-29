#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dependcheck.h"
#include "../rold/colors/colors.h"

void dependcheck() {
    int missingDependencies = 0;  // Counter for missing dependencies

    // Check if jq is available
    if (system("command -v jq > /dev/null") != 0) {
        printf(RED "jq is not installed. Please install it before running this script.\n" RESET);
        missingDependencies++;
    }

    // Check if tar is available
    if (system("command -v tar > /dev/null") != 0) {
        printf(YELLOW "tar is not installed. Please install it before running this script.\n" RESET);
        missingDependencies++;
    }

    // Check if curl is available
    if (system("command -v curl > /dev/null") != 0) {
        printf(RED "curl is not installed. Please install it before running this script.\n" RESET);
        missingDependencies++;
    }

    // Check if axel is available
    if (system("command -v axel > /dev/null") != 0) {
        printf(RED "axel is not installed. Please install it before running this script.\n" RESET);
        missingDependencies++;
    }

    // Check if sed is available
    if (system("command -v sed > /dev/null") != 0) {
        printf(RED "sed is not installed. Please install it before running this script.\n" RESET);
        missingDependencies++;
    }

    if (missingDependencies > 0) {
        printf(RED "Total missing dependencies: %d\n" RESET, missingDependencies);
        exit(1);  // Exit the program with a non-zero status code
    } else {
        printf(GREEN "All required dependencies are available.\n" RESET);
    }
}
