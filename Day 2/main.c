#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTO(name, data) typeof(data) name = data

enum Direction {BLANK, UP, DOWN};

int main(void) {
    FILE* file = fopen("input.txt", "r+");
    if (!file){
        perror("File opening failed");
        return EXIT_FAILURE;
    }
    const int max = INT16_MAX;
    char str[max];

    int safe = 0;
    int unsafe = 0;
    while (fgets(str, max, file)) {
        enum Direction direction = BLANK;
        char* first_str = strtok(str, " ");
        int first = atoi(first_str);
        _Bool is_safe = true;

        while (is_safe) {
            char* second_str = strtok(NULL, " ");
            if (!second_str) break;
            int second = atoi(second_str);

            int diff = second - first;
            if (diff == 0) {
                is_safe = false;
                continue;
            }
            if (direction == BLANK) {
                direction = diff < 0 ? DOWN : UP;
            }
            else {
                const enum Direction temp_direction = diff < 0 ? DOWN : UP;
                if (temp_direction != direction) {
                    is_safe = false;
                    continue;
                }
            }


            const int abs1 = abs(diff);
            is_safe = abs1 >= 1 && abs1 <= 3;
            first = second;
        }

        memset(str, 0, max);
        is_safe ? safe++ : unsafe++;
    }

    printf("Safe: %d\nUnsafe: %d\n", safe, unsafe);


    printf("Hello, World!\n");
    return 0;
}
