#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../common/homeDir/home-dir.h"
#include "../../../colors/colors.h"
#include <unistd.h> // Include for the exit function

void vpatchwayland() {
    char home[256];
    homeDir(home);

    // Check if gamescope is available
    if (system("command -v gamescope > /dev/null") != 0) {
        printf(RED "gamescope is not installed. Please install it before running this script.\n" RESET);
        exit(1); // Exit with an error code
    }

    char configFile[256];
    snprintf(configFile, sizeof(configFile), "%s/.config/vinegar/config.toml", home);

    // Construct the launcher argument with the nice level included
    int height, width, unFocused, fps;

    printf(MAGENTA "To patch the Wayland cursor locking, we use gamescope here, so make sure it's actually installed.\n" RESET);
    printf(MAGENTA "This will currently overwrite your launcher.\n" RESET);
    printf(MAGENTA "Unfocused FPS caps your FPS while the game is unfocused.\n" RESET);

    printf(MAGENTA "Type your screen height: " RESET);
    scanf("%d", &height);
    printf(MAGENTA "Type your screen width: " RESET);
    scanf("%d", &width);
    printf(MAGENTA "Type what you want to limit your FPS to (put a big number for unlimited): " RESET);
    scanf("%d", &fps);
    printf(MAGENTA "Type what you want your unfocused FPS to be set to: " RESET);
    scanf("%d", &unFocused);

    char newLauncherARG[512];
    snprintf(newLauncherARG, sizeof(newLauncherARG), "gamescope -h %d -w %d -W %d -H %d -r %d -o %d -f --force-grab-cursor --", height, width, height, width, fps, unFocused);

    // Use the system command to check if the launcher already exists in the config file
    char checkCommand[512];
    snprintf(checkCommand, sizeof(checkCommand), "grep -q -E 'launcher[[:space:]]*=' %s", configFile);
    int checkResult = system(checkCommand);

    // If the check command failed, it means 'launcher' is not found; create it under [player]
    if (checkResult != 0) {
        char addLauncherCommand[512];
        snprintf(addLauncherCommand, sizeof(addLauncherCommand), "sed -i '/^\\[player\\]/a launcher = \"%s\"' %s", newLauncherARG, configFile);
        int addLauncherResult = system(addLauncherCommand);

        if (addLauncherResult != 0) {
            printf(RED "Error adding launcher to the config file!\n" RESET);
            exit(1); // Exit with an error code
        } else {
            printf(GREEN "Added launcher under [player] section.\n" RESET);
        }
    } else {
        // 'launcher' already exists; update the value
        char sedCommand[512];
        snprintf(sedCommand, sizeof(sedCommand), "sed -i '/^launcher/c\\launcher = \"%s\"' %s", newLauncherARG, configFile);
        int sedResult = system(sedCommand);

        if (sedResult != 0) {
            printf(RED "Error updating launcher in the config file!\n" RESET);
            exit(1); // Exit with an error code
        } else {
            printf(GREEN "Successfully updated launcher!\n" RESET);
        }
    }

    exit(0); // Exit with a success code
}
