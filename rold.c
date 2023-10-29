#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./rold/common/installGE/download-ge.h"
#include "./rold/vinegar/setGE/source/set-ge.h"
#include "./rold/vinegar/setGE/flatpak/f-set-ge.h"
#include "./rold/grapejuice/setGE/source/set-ge.h"
#include "./rold/grapejuice/setGE/flatpak/f-set-ge.h"
#include "./rold/vinegar/patchWayland/source/patch-wayland.h"
#include "./rold/vinegar/patchWayland/flatpak/f-patch-wayland.h"
#include "./dependcheck/dependcheck.h"

void printUsage() {
    printf("Usage: program [options]\n");
    printf("Options:\n");
    printf("  --vinegar         Perform actions related to Vinegar\n");
    printf("  --download-ge     Perform actions related to downloading GE\n");
    printf("  --f-vinegar       Perform actions related to Flatpak Vinegar\n");
    printf("  --patch-wayland   Perform actions related to patching Wayland\n");
    printf("  --grapejuice      Perform actions related to Grapejuice\n");
    printf("  --f-grapejuice    Perform actions related to Flatpak Grapejuice\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No options specified. Use -h or --help to see available options.\n");
        return 1;
    }

    int f_vinegar_flag = 0;
    int vinegar_flag = 0;
    int download_ge_flag = 0;
    int patch_wayland_flag = 0;
    int grapejuice_flag = 0;
    int f_grapejuice_flag = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--f-vinegar") == 0) {
            f_vinegar_flag = 1;
        } else if (strcmp(argv[i], "--download-ge") == 0) {
            download_ge_flag = 1;
        } else if (strcmp(argv[i], "--vinegar") == 0) {
            vinegar_flag = 1;
        } else if (strcmp(argv[i], "--patch-wayland") == 0) {
            patch_wayland_flag = 1;
        } else if (strcmp(argv[i], "--grapejuice") == 0) {
            grapejuice_flag = 1;
        } else if (strcmp(argv[i], "--f-grapejuice") == 0) {
            f_grapejuice_flag = 1;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printUsage();
            return 0;
        }
    }

    // Call the dependcheck function to ensure dependencies are met
    dependcheck();

    // Perform actions based on command line flags
    if (f_vinegar_flag && download_ge_flag) {
        installGE();
        vfsetGE();
    }

    if (vinegar_flag && download_ge_flag) {
        installGE();
        vsetGE();
    }

    if (vinegar_flag && patch_wayland_flag) {
        vpatchwayland();
    }

    if (f_vinegar_flag && patch_wayland_flag) {
        fvpatchwayland();
    }

    // Handle other cases as needed

    return 0;
}
