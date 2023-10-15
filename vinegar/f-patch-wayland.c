#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Check if gamescope is available
    if (system("command -v flatpak run org.freedesktop.Platform.VulkanLayer.gamescope > /dev/null") != 0) {
        printf("gamescope is not installed. Please install it before running this script.\n");
        printf("Please install it with `flatpak install flatpak run org.freedesktop.Platform.VulkanLayer.gamescope`");
        return 1;
    }
    
    // Check if sed is available
    if (system("command -v sed > /dev/null") != 0) {
        printf("sed is not installed. Please install it before running this script.\n");
        return 1;
    }


    char homeDir[256];

    char *tempHomeDir = getenv("HOME");

    if (tempHomeDir == NULL) {
        printf("Unable to determine the user's home directory.\n");
        return 1;
    }

    strcpy(homeDir, tempHomeDir);

    char configFile[256];
    snprintf(configFile, sizeof(configFile), "%s/.var/app/io.github.vinegarhq.Vinegar/config", homeDir);

    // Construct the launcher argument with the nice level included
    int height, width, unFocused, fps, nice;
    
    printf("To patch the Wayland cursor locking, we use gamescope here, so make sure it's actually installed.\n");
    printf("This will currently overwrite your launcher.\n");
    printf("Unfocused FPS caps your FPS while the game is unfocused.\n");
    printf("Nice levels are the kernel's way of doing priority. The lower the number, the better! (-10 is better than 10)\n");
    printf("It's recommended to set the nice levels between -20 and -10 for the best performance and stability, however, you can do whatever you want.\n");

    printf("Set a nice level: ");
    scanf("%d", &nice);
    printf("Type your screen height: ");
    scanf("%d", &height);
    printf("Type your screen width: ");
    scanf("%d", &width);
    printf("Type what you want to limit your FPS to (put a big number for unlimited): ");
    scanf("%d", &fps);
    printf("Type what you want your unfocused FPS to be set to: ");
    scanf("%d", &unFocused);
    
    char newLauncherARG[512];
    snprintf(newLauncherARG, sizeof(newLauncherARG), "nice -n %d gamescope -h %d -w %d -W %d -H %d -r %d -o %d -f --force-grab-cursor --", nice, height, width, height, width, fps, unFocused);

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
            printf("Error adding launcher to the config file!\n");
            return 1;
        } else {
            printf("Added launcher under [player] section.\n");
        }
    } else {
        // 'launcher' already exists; update the value
        char sedCommand[512];
        snprintf(sedCommand, sizeof(sedCommand), "sed -i '/^launcher/c\\launcher = \"%s\"' %s", newLauncherARG, configFile);
        int sedResult = system(sedCommand);

        if (sedResult != 0) {
            printf("Error updating launcher in the config file!\n");
            return 1;
        } else {
            printf("Successfully updated launcher!\n");
        }
    }

    return 0;
}
