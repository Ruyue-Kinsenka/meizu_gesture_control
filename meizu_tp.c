#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int write_to_sysfs(const char *path, const char *value) {
    FILE *fp;
    fp = fopen(path, "w");
    if (fp == NULL) {
        return -1;
    }

    if (fprintf(fp, "%s", value) < 0) { 
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    const char *feature = argv[1];
    const char *value_str = argv[2];
    int value_int = atoi(value_str);
    const char *sysfs_path = NULL;
    char value_to_write[20];

    if (strcmp(feature, "dt2w") == 0) {
        sysfs_path = "/sys/class/meizu/tp/gesture_control";
        if (value_int == 1) {
            strcpy(value_to_write, "8100014");
        } else if (value_int == 0) {
            strcpy(value_to_write, "81000004");
        }
    } else if (strcmp(feature, "highreport") == 0) {
        sysfs_path = "/sys/class/meizu/main_tp/game_mode_node";
        if (value_int == 1) {
            strcpy(value_to_write, "1");
        } else if (value_int == 0) {
            strcpy(value_to_write, "0");
        }// For meizu 20PRO and meizu 21PRO
    } else if (strcmp(feature, "wireless") == 0) {
        sysfs_path = "/sys/class/meizu/wireless/reverse_chg_enable";
        if (value_int == 1) {
            strcpy(value_to_write, "1");
        } else if (value_int == 0) {
            strcpy(value_to_write, "0");
        }
    }
    if (write_to_sysfs(sysfs_path, value_to_write) != 0) {
        return 1;
    }
    return 0;
}