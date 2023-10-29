#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "download-ge.h"
#include "../homeDir/home-dir.h"
#include "../fetchTag/fetch-tag.h"
#include "../../colors/colors.h"

void installGE() {
    char home[256];
    homeDir(home);
    char releasetag[256];
    releaseTag(releasetag);

    // Create directories
    char ROLDdir[512];
    char downloadDir[512];
    char extDir[512];
    char fileName[512];
    snprintf(fileName, sizeof(fileName), "wine-lutris-%s-x86_64.tar.xz", releasetag);
    snprintf(ROLDdir, sizeof(ROLDdir), "%s/.local/share/ROLD/", home);
    snprintf(downloadDir, sizeof(downloadDir), "%s/WINE-GE", ROLDdir);
    snprintf(extDir, sizeof(extDir), "%s/WINE-GE-EXT", ROLDdir);

    if (mkdir(ROLDdir, 0755) != 0 && errno != EEXIST) {
        perror(RED "Error creating ROLD directory" RESET);
        exit(1); // Exit with an error code
    }
    if (mkdir(downloadDir, 0755) != 0 && errno != EEXIST) {
        perror(RED "Error creating download directory" RESET);
        exit(1); // Exit with an error code
    }
    if (mkdir(extDir, 0755) != 0 && errno != EEXIST) {
        perror(RED "Error creating extraction directory" RESET);
        exit(1); // Exit with an error code
    }

    // Check if the archive is already downloaded
    char downloadFilePath[512];
    snprintf(downloadFilePath, sizeof(downloadFilePath), "%s/%s", downloadDir, fileName);
    struct stat downloadStat;
    if (stat(downloadFilePath, &downloadStat) == 0) {
        printf(YELLOW "Archive already downloaded. Skipping download." RESET "\n");
    } else {
        // Construct the correct download URL
        char downloadURL[512];
        snprintf(downloadURL, sizeof(downloadURL), "https://github.com/GloriousEggroll/wine-ge-custom/releases/download/%s/%s", releasetag, fileName);

        // Download the archive directly into WINE-GE directory
        char downloadCommand[2048];
        snprintf(downloadCommand, sizeof(downloadCommand), "axel -a -o \"%s/%s\" \"%s\"", downloadDir, fileName, downloadURL);
        int downloadResult = system(downloadCommand);
        if (downloadResult != 0) {
            printf(RED "Error executing download command." RESET "\n");
            exit(1); // Exit with an error code
        }
    }

    // Check if the archive is already extracted
    char extractFilePath[512];
    snprintf(extractFilePath, sizeof(extractFilePath), "%s/lutris-%s-x86_64", extDir, releasetag);
    struct stat extractStat;
    if (stat(extractFilePath, &extractStat) == 0) {
        printf(YELLOW "Archive already extracted. Skipping extraction." RESET "\n");
    } else {
        // Extract the downloaded archive into WINE-GE-EXT directory
        char extractCommand[2048];
        snprintf(extractCommand, sizeof(extractCommand), "tar -xf %s/%s -C %s", downloadDir, fileName, extDir);
        printf("Extracting....\n");
        int extractResult = system(extractCommand);
        if (extractResult != 0) {
            printf(RED "Error executing extraction command." RESET "\n");
            exit(1); // Exit with an error code
        }
    }

    printf(GREEN "Download and extraction completed successfully." RESET "\n");
}
