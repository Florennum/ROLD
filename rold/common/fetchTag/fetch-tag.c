#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../colors/colors.h"

void releaseTag(char* releaseTag) {

    // Fetch release information from GitHub API
    char apiURL[] = "https://api.github.com/repos/GloriousEggroll/wine-ge-custom/releases/latest";
    char releaseInfo[512];
    snprintf(releaseInfo, sizeof(releaseInfo), "curl -sSL %s | jq -r '.tag_name'", apiURL);

    FILE* fp = popen(releaseInfo, "r");
    if (fp == NULL) {
        printf(RED "Error opening pipe.\n" RESET);
        exit(1); // Exit with an error code
    }

    if (fgets(releaseTag, 100, fp) != NULL) {
        releaseTag[strcspn(releaseTag, "\n")] = 0; // Remove the newline character, if present
        printf(GREEN "Release Tag: %s\n" RESET, releaseTag);
    } else {
        printf(RED "Error reading release tag.\n" RESET);
        pclose(fp); // Close the file pointer
        exit(1); // Exit with an error code
    }

    if (pclose(fp) != 0) {
        printf(RED "Error closing the pipe.\n" RESET);
        exit(1); // Exit with an error code
    }
}
