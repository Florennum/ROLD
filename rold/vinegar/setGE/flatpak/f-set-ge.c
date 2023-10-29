#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "../../../common/homeDir/home-dir.h"
#include "../../../common/fetchTag/fetch-tag.h"
#include "../../../colors/colors.h"

void vfsetGE() {
    char home[256];
    homeDir(home);
    char releasetag[256];
    releaseTag(releasetag);

    char folderName[512];
    char extGE[512];
    snprintf(extGE, sizeof(extGE), "%s/.local/share/ROLD/WINE-GE-EXT", home);
    snprintf(folderName, sizeof(folderName), "%s/lutris-%s-x86_64", extGE, releasetag);

    char patchF[512];
    char patchF1[512];
    snprintf(patchF, sizeof(patchF), "flatpak override --user --filesystem=%s", folderName);
    snprintf(patchF1, sizeof(patchF1), "system(%s)", patchF);
    system(patchF1);

    // Check if the archive is extracted
    struct stat extractStat;
    if (stat(folderName, &extractStat) == 0) {
        printf(GREEN "The extracted archive seems to be fine.....\n" RESET);
    } else {
        if (errno == ENOENT) {
            printf(RED "Oh no, extracted archive not found!\n" RESET);
        } else {
            perror(RED "Error checking extracted archive status" RESET);
        }
        exit(1); // Exit with an error code
    }

    char configFile[256];
    snprintf(configFile, sizeof(configFile), "%s/.var/app/io.github.vinegarhq.Vinegar/config", home);

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
            printf(RED "Error adding wineroot to the config file!\n" RESET);
            exit(1); // Exit with an error code
        } else {
            printf(GREEN "Added wineroot\n" RESET);
        }
    } else {
        // 'wineroot' already exists; update the value
        char sedCommand[512];
        snprintf(sedCommand, sizeof(sedCommand), "sed -i '/^wineroot/c\\wineroot = \"%s\"' %s", folderName, configFile);
        int sedResult = system(sedCommand);

        if (sedResult != 0) {
            printf(RED "Error updating wineroot in the config file!\n" RESET);
            exit(1); // Exit with an error code
        } else {
            printf(GREEN "Successfully updated wineroot!\n" RESET);
        }
    }

    exit(0); // Exit with a success code
}
