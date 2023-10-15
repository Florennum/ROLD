#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [--vinegar] [--download-ge] [--f-vinegar] [--patch-wayland]\n", argv[0]);
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
        }
    }

    if (f_vinegar_flag && download_ge_flag) {
        system("./download-ge");
        system("./vinegar/f-set-ge");
    }

    if (vinegar_flag && download_ge_flag) {
        system("./download-ge");
        system("./vinegar/set-ge");
    }

    if (vinegar_flag && patch_wayland_flag) {
        system("./vinegar/patch-wayland");
    }

    if (f_vinegar_flag && patch_wayland_flag) {
        system("./vinegar/f-patch-wayland");
    }

    if (f_grapejuice_flag && patch_wayland_flag) {
        system("./grapejuice/f-patch-wayland");
    }

    if (grapejuice_flag && patch_wayland_flag) {
        system("./grapejuice/patch-wayland");
    }

    if (f_grapejuice_flag && download_ge_flag) {
        system("./download-ge");
        system("./grapejuice/f-set-ge");
    }

    if (grapejuice_flag && download_ge_flag) {
        system("./download-ge");
        system("./grapejuice/set-ge");
    }

    return 0;
}
