#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

    int main() {
        // Check if jq is available
    if (system("command -v jq > /dev/null") != 0) {
        printf("jq is not installed. Please install it before running this script.\n");
        return 1;
    }

    // Check if tar is available
    if (system("command -v tar > /dev/null") != 0) {
        printf("tar is not installed. Please install it before running this script.\n");
        return 1;
    }

    // Check if curl is available
    if (system("command -v curl > /dev/null") != 0) {
        printf("curl is not installed. Please install it before running this script.\n");
        return 1;
    }

    // Rest of your code
    printf("All required dependancies are available.\n");


    char homeDir[256];

    char *tempHomeDir = getenv("HOME");

    if (tempHomeDir == NULL) {
        printf("Unable to determine the user's home directory.\n");
        return 1;
    }

    strcpy(homeDir, tempHomeDir);
    
    char apiURL[] = "https://api.github.com/repos/GloriousEggroll/wine-ge-custom/releases/latest";
    char releaseInfo[] = "curl -sSL ";
    char cReleaseInfo[256];
    snprintf(cReleaseInfo, sizeof(cReleaseInfo), "%s%s", releaseInfo, apiURL);

    unlink("release.json");
    char fetchCommand[512];
    snprintf(fetchCommand, sizeof(fetchCommand), "%s > release.json", cReleaseInfo);
    int result = system(fetchCommand);

    if (result != 0) {
        printf("Error executing fetch command.\n");
        return 1;
    }

    FILE* fp = popen("jq -r '.tag_name' release.json", "r");
    char releaseTag[100];

    if (fp == NULL) {
        printf("Error opening pipe.\n");
        return 1;
    }

    if (fgets(releaseTag, sizeof(releaseTag), fp) != NULL) {
        releaseTag[strcspn(releaseTag, "\n")] = 0; // Remove the newline character, if present
    } else {
        printf("Error reading release tag.\n");
    }

    pclose(fp);

    // Create directories
    char ROLDdir[512];
    char downloadDir[512];
    char extDir[512];
    char fileName[512];
    snprintf(fileName, sizeof(fileName), "wine-lutris-%s-x86_64.tar.xz", releaseTag);
    snprintf(ROLDdir, sizeof(ROLDdir), "%s/.local/share/ROLD/", homeDir);
    snprintf(downloadDir, sizeof(downloadDir), "%s/.local/share/ROLD/WINE-GE", homeDir);
    snprintf(extDir, sizeof(extDir), "%s/.local/share/ROLD/WINE-GE-EXT", homeDir);

    // Use mkdir to create the directories only if they don't exist
    if (mkdir(ROLDdir, 0755) != 0 && errno != EEXIST) {
        perror("Error creating ROLD directory");
        return 1;
    }
    if (mkdir(downloadDir, 0755) != 0 && errno != EEXIST) {
        perror("Error creating download directory");
        return 1;
    }

    // Check if the archive is already downloaded
    char downloadFilePath[512];
    snprintf(downloadFilePath, sizeof(downloadFilePath), "%s/%s", downloadDir, fileName);
    struct stat downloadStat;
    if (stat(downloadFilePath, &downloadStat) == 0) {
        printf("Archive already downloaded. Skipping download.\n");
    } else {
        // Construct the correct download URL
        char downloadURL[512];
        snprintf(downloadURL, sizeof(downloadURL), "https://github.com/GloriousEggroll/wine-ge-custom/releases/download/%s/%s", releaseTag, fileName);

        // Download the archive directly into WINE-GE directory
        char downloadCommand[2048];
        snprintf(downloadCommand, sizeof(downloadCommand), "curl -# -L -o %s/%s %s", downloadDir, fileName, downloadURL);
        int downloadResult = system(downloadCommand);
        if (downloadResult != 0) {
            printf("Error executing download command.\n");
            return 1;
        }
    }

    // Check if the archive is already extracted
    char extractFilePath[512];
    snprintf(extractFilePath, sizeof(extractFilePath), "%s/lutris-%s-x86_64", extDir, releaseTag);
    struct stat extractStat;
    if (stat(extractFilePath, &extractStat) == 0) {
        printf("Archive already extracted. Skipping extraction.\n");
    } else {
        // Extract the downloaded archive into WINE-GE-EXT directory
        char extractCommand[2048];
        snprintf(extractCommand, sizeof(extractCommand), "tar -xf %s/%s -C %s", downloadDir, fileName, extDir);
        printf("Extracting....\n");
        int extractResult = system(extractCommand);
        if (extractResult != 0) {
            printf("Error executing extraction command.\n");
            return 1;
        }
    }

    printf("Download and extraction completed successfully.\n");

    return 0;
}
