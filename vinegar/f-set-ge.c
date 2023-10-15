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
    printf("All required dependencies are available.\n");

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

    FILE *fp = popen("jq -r '.tag_name' release.json", "r");
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


    char folderName[512];
    char extGE[512];
    snprintf(extGE, sizeof(extGE), "%s/.local/share/ROLD/WINE-GE-EXT", homeDir);
    snprintf(folderName, sizeof(folderName), "%s/lutris-%s-x86_64", extGE, releaseTag);

    char patchF[512];
    char patchF1[512];
    snprintf(patchF, sizeof(patchF), "flatpak override --user --filesystem=%s",folderName);
    snprintf(patchF1, sizeof(patchF1), "system(%s)",patchF);
    system(patchF1);

    // Check if the archive is extracted
    struct stat extractStat;
    if (stat(folderName, &extractStat) == 0) {
        printf("The extracted file archive seems to be fine.....\n");
    } else {
        printf("Oh no, extracted archive not found!\n");
        return 1;
    }

    char configFile[256];
    snprintf(configFile, sizeof(configFile), "%s/.var/app/io.github.vinegarhq.Vinegar/config", homeDir);

    char newWineroot[512];
    snprintf(newWineroot, sizeof(newWineroot), "wineroot = \"%s\"", folderName);

    // Use the system command to check if the 'wineroot' already exists in the config file
    char checkCommand[512];
    snprintf(checkCommand, sizeof(checkCommand), "grep -q -E 'wineroot[[:space:]]*=' %s", configFile);
    int checkResult = system(checkCommand);

    // If the check command failed, it means 'wineroot' is not found; create it under [player]
    if (checkResult != 0) {
        char addWinerootCommand[512];
        snprintf(addWinerootCommand, sizeof(addWinerootCommand), "sed -i '1i%s' %s", newWineroot, configFile);
        int addWinerootResult = system(addWinerootCommand);

        if (addWinerootResult != 0) {
            printf("Error adding wineroot to the config file!\n");
            return 1;
        } else {
            printf("Added wineroot");
        }
    } else {
        // 'wineroot' already exists; update the value
        char sedCommand[512];
        snprintf(sedCommand, sizeof(sedCommand), "sed -i '/^wineroot/c\\wineroot = \"%s\"' %s", folderName, configFile);
        int sedResult = system(sedCommand);

        if (sedResult != 0) {
            printf("Error updating wineroot in the config file!\n");
            return 1;
        } else {
            printf("Successfully updated wineroot!\n");
        }
    }

    pclose(fp);

    return 0;
}
