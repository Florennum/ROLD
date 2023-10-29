#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "../../../common/homeDir/home-dir.h"
#include "../../../common/fetchTag/fetch-tag.h"
#include "../../../colors/colors.h"
#include "f-set-ge.h"

void fgjsetGE() {
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
    if (stat(folderName, &extractStat) != 0) {
        printf(RED "Oh no, extracted archive not found!\n" RESET);
        exit(1); // Exit with an error code
    }

    char configFile[256];
    snprintf(configFile, sizeof(configFile), "%s/.var/app/net.brinkervii.grapejuice/config/brinkervii/grapejuice/user_settings.json", home);

    // Update 'wine_home' and 'default_wine_home' values
    char updateWineHomeCommand[512];
    snprintf(updateWineHomeCommand, sizeof(updateWineHomeCommand), "sed -i '/\"wine_home\":/c\\    \"wine_home\": \"%s\",' %s", folderName, configFile);
    int updateWineHomeResult = system(updateWineHomeCommand);

    char updateDefaultWineHomeCommand[512];
    snprintf(updateDefaultWineHomeCommand, sizeof(updateDefaultWineHomeCommand), "sed -i '/\"default_wine_home\":/c\\    \"default_wine_home\": \"%s\",' %s", folderName, configFile);
    int updateDefaultWineHomeResult = system(updateDefaultWineHomeCommand);

    if (updateWineHomeResult != 0 || updateDefaultWineHomeResult != 0) {
        printf(RED "Error updating wine_home and default_wine_home values!\n" RESET);
        exit(1); // Exit with an error code
    } else {
        printf(GREEN "Successfully updated wine_home and default_wine_home!\n" RESET);
        exit(0); // Exit with a success code
    }
}
