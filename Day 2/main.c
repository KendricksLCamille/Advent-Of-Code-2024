#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTO(name, data) typeof(data) name = data

enum Direction {BLANK, UP, DOWN};
#define MAX INT16_MAX
void part_1(FILE *file) {
    char str[MAX];

    int safe = 0;
    int unsafe = 0;
    while (fgets(str, MAX, file)) {
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
                break;
            }
            if (direction == BLANK) {
                direction = diff < 0 ? DOWN : UP;
            }
            else {
                const enum Direction temp_direction = diff < 0 ? DOWN : UP;
                if (temp_direction != direction) {
                    is_safe = false;
                    break;
                }
            }


            const int abs1 = abs(diff);
            is_safe = abs1 >= 1 && abs1 <= 3;
            first = second;
        }

        memset(str, 0, MAX);
        is_safe ? safe++ : unsafe++;
    }

    printf("Safe: %d\nUnsafe: %d\n", safe, unsafe);
}
int find_element_that_fails_thresholds(const int len, const int array[len]) {
    int output = -1;
    for (int i = 0; i < len; i++) {
        const int result = abs(array[i]);
        if (result == 0) goto on_fail_check;
        if (result < 1 || result > 3) goto on_fail_check;
        continue;

        on_fail_check:
        if (output!= -1) return -2; // More than one element passes thresholds
        output = i;
    }
    return output;
}

int find_any_element_that_changes_signs(const int len, const int array[len]) {
    int output = -1;
    for (int i = 1; i < len; i++) {
        const int result = array[i];
        if (result == 0) goto check_and_set_output;
        if (result < 0 && array[i - 1] >= 0 || result > 0 && array[i - 1] <= 0) goto check_and_set_output;

        continue;
        check_and_set_output:
        if (output != -1) return -2; // More than one element changes signs or passes thresholds
        output = i;
    }
    return output;
}

void add_and_move(const int len, int array[len], const int index) {
    if (index < 0) return;
    if (index >= len) return;
    const int new_len = len - 1;
    if (index == new_len - 1) return;

    if (index > 0) {
        array[index - 1] += array[index];
    }

    memcpy(&array[index], &array[index + 1], sizeof(int) * new_len - index);
}

/* [a,b,c,d]
     * [1,7,2,3]
     * ab = 6
     * bc = -5
     * cd = 1
     *
     * // if b is the issue try skipping a
     * bc = -5
     * cd = 1
     *
     * // if not fix try skipping c:
     * ab = 6
     * bd = -4
     *
     * // if still not fixed try skipping b:
     * ac = 1
     * cd = 1
     *
     * // else it is not safe
     *
     */
_Bool is_safe(const int len, int diff_array[len], const int index, int dampener_count) {
    if (dampener_count > 1) return false;
    int index_or_error = 0;

    typedef int (*find_func)(const int, const int[len]);
    const find_func funcList[2] = {find_element_that_fails_thresholds, find_any_element_that_changes_signs};
    for (int i = 0; i < 2; i++) {
        index_or_error = funcList[i](len, diff_array);
        if (index_or_error == -2) {
            return false;
        }

        if (index_or_error > -1) {
            const int diff_len = len - 1;

            for (int j = -1; j < 2; j++) {
                int diff_array2[len];
                memcpy(diff_array2, diff_array, sizeof(diff_array2));
                add_and_move(len, diff_array2, index_or_error + j);
                if (is_safe(diff_len, diff_array2, index, dampener_count + 1)) {
                    return true;
                }
            }

            return false;
        }
    }

    return true;
}

void part_2(FILE* file) {
    char str[MAX];
    int array[MAX];
    int safe = 0, unsafe = 0;


    int index = 0;
    while (fgets(str, MAX, file)) {
        char* str2 = strtok(str, " ");
        int count = 0;
        array[count++] = atoi(str2);

        while (true) {
            str2 = strtok(NULL, " ");
            if (!str2) {
                break;
            }
            array[count++] = atoi(str2);
        }


        const int diff_array_count = count - 1;
        int diff_array[diff_array_count]; // Yes. It is a VLA.

        // Calculate differences between consecutive elements
        for (int i = 0; i < diff_array_count; i++) {
            diff_array[i] = array[i] - array[i + 1];
        }

        if (is_safe(diff_array_count, diff_array, index, 0)) {
            safe++;
            printf("%d: Safe\n", index);
        }
        else {
            unsafe++;
            printf("%d: Unsafe\n", index);
        }
        index++;
    }



    printf("Safe: %d\nUnsafe: %d\n", safe, unsafe);
}

int main(void) {
    FILE* file = fopen("input.txt", "r+");
    if (!file){
        perror("File opening failed");
        return EXIT_FAILURE;
    }
    part_2(file);

    return 0;
}
