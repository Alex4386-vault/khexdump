#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* num2KHex(char num) {
    char *str, *target = NULL;;

    if (num < 10) {
        // since atoi is NOT a standard.
        // making C99 compliant

        str = malloc(sizeof(char) * 2);
        str[0] = 0x30 + num;
        str[1] = 0x00;

        return str;
    } else {        
        // in case of character set going crazy
        switch (num) {
            case 10:
                target = "ㄱ";
                break;
            case 11:
                target = "ㄴ";
                break;
            case 12:
                target = "ㄷ";
                break;
            case 13:
                target = "ㄹ";
                break;
            case 14:
                target = "ㅁ";
                break;
            case 15:
                target = "ㅂ";
                break;
        }

        int len = strlen(target) + 1;
        str = malloc(sizeof(char) * (len + 1));

        strcpy(str, target);
    }

    return str;
} 

char *byte2KHex(char byte) {
    char *str = NULL;

    char firstHalf = (byte & 0b11110000) >> 4;
    char secondHalf = (byte & 0b00001111);

    char *firstHalfHex = num2KHex(firstHalf);
    char *secondHalfHex = num2KHex(secondHalf);

    int length = strlen(firstHalfHex);
    length += strlen(secondHalfHex);
    length -= 1;

    str = malloc(sizeof(char) * length);
    strcpy(str, firstHalfHex);
    strcat(str, secondHalfHex);

    free(firstHalfHex);
    free(secondHalfHex);

    return str;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("사용법: %s [내보낼 자료 이름]\n", argv[0]);
        return 0;
    } else if (argc == 0) {
        return 0;        
    }

    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) return 1;

    while (!feof(fp)) {
        long loc = ftell(fp);
        for (int i = sizeof(loc) - 1; i >= 0; i--) {
            char *target = (char *) &loc;
            printf("%s", byte2KHex(*(target+i)));
        }
        printf(" ");

        char *data = malloc(sizeof(char) * 16);
        int bytes = fread(data, sizeof(char), 16, fp);

        for (int j = 0; j < ((bytes + (bytes % 2)) / 2); j++) {
            for (int sub = 1; sub >= 0; sub--) {
                int i = (j * 2) + sub;
                char byte = *(data+i);
                if (i >= bytes) byte = 0;

                printf("%s", byte2KHex(byte));
            }

            printf(" ");
        }
        printf("\n");

        free(data);
    }

    long loc = ftell(fp);
    for (int i = sizeof(loc) - 1; i >= 0; i--) {
        char *target = (char *) &loc;
        printf("%s", byte2KHex(*(target+i)));
    }
    printf("\n");

    fclose(fp);
}
