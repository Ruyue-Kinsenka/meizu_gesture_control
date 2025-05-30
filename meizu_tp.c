#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h> 

int write_uint32_le_to_sysfs(const char *path, uint32_t value) {
    FILE *fp;
    fp = fopen(path, "wb");
    if (fp == NULL) {
        perror("Error opening file for binary write");
        return -1;
    }

    uint8_t bytes[4];
    bytes[0] = (value >> 0) & 0xFF;  
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF; 

    size_t written = fwrite(bytes, sizeof(uint8_t), 4, fp);
    if (written != 4) {
        perror("Error writing bytes to file");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

int write_string_to_sysfs(const char *path, const char *value) {
    FILE *fp;
    fp = fopen(path, "w"); 
    if (fp == NULL) {
        perror("Error opening file for string write");
        return -1;
    }

    if (fprintf(fp, "%s", value) < 0) {
        perror("Error writing string to file");
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <feature> <0|1>\n", argv[0]);
        return 1;
    }

    const char *feature = argv[1];
    const char *value_str = argv[2];
    int value_int = atoi(value_str); 
    const char *sysfs_path = NULL;

    int result = -1;

    if (strcmp(feature, "dt2w") == 0) {
        sysfs_path = "/sys/class/meizu/tp/gesture_control";
        uint32_t value_to_write_uint32;

        if (value_int == 1) { // 开启 DT2W，8100014
            value_to_write_uint32 = 0x81000014;
        } else if (value_int == 0) { // 关闭 DT2W，8100004
            value_to_write_uint32 = 0x81000004;
        } else {
            fprintf(stderr, "Invalid value for dt2w: %s. Use 0 or 1.\n", value_str);
            return 1;
        }
        result = write_uint32_le_to_sysfs(sysfs_path, value_to_write_uint32);

    } else if (strcmp(feature, "highreport") == 0) {
        sysfs_path = "/sys/class/meizu/main_tp/game_mode_node";
        char value_to_write_str[2]; // "0" or "1"
        if (value_int == 1) {
            strcpy(value_to_write_str, "1");
        } else if (value_int == 0) {
            strcpy(value_to_write_str, "0");
        } else {
            return 1;
        }
        result = write_string_to_sysfs(sysfs_path, value_to_write_str);

    } else if (strcmp(feature, "wireless") == 0) {
        sysfs_path = "/sys/class/meizu/wireless/reverse_chg_enable";
        char value_to_write_str[2]; // "0" or "1"
        if (value_int == 1) {
            strcpy(value_to_write_str, "1");
        } else if (value_int == 0) {
            strcpy(value_to_write_str, "0");
        } else {
            return 1;
        }
        result = write_string_to_sysfs(sysfs_path, value_to_write_str);
    } else {
        fprintf(stderr, "Unknown feature: %s\n", feature);
        return 1;
    }

    if (result != 0) {
        fprintf(stderr, "Failed to write to sysfs path: %s\n", sysfs_path ? sysfs_path : "NULL");
        return 1;
    }

    printf("Successfully set %s to %d for path %s\n", feature, value_int, sysfs_path);
    return 0;
}