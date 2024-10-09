#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // For sleep function on Unix systems

#define BRAILLE_SIZE 6
#define CHARS_PER_LINE 7

// Braille patterns for lowercase letters a-z
const char *braille_patterns[26] = {
    "100000", "101000", "110000", "110100", "100100", // a-e
    "111000", "111100", "101100", "011000", "011100", // f-j
    "100010", "101010", "110010", "110110", "100110", // k-o
    "111010", "111110", "101110", "011010", "011110", // p-t
    "100011", "101011", "011101", "110011", "110111", // u-y
    "100111"  // z
};

// Braille patterns for numbers 0-9
const char *braille_numbers[10] = {
    "010110", "100000", "101000", "110000", "110100", // 0-4
    "100100", "111000", "111100", "101100", "011000"  // 5-9
};

// Other Braille patterns
const char *braille_space = "000000";
const char *braille_period = "010011";
const char *braille_comma = "010000";
const char *braille_question = "011001";
const char *braille_exclamation = "011010";

// Function to print a line of Braille characters side by side
void print_braille_line(const char patterns[][BRAILLE_SIZE + 1], int count) {
    // Print top dots
    printf(" ");
    for (int i = 0; i < count; i++) {
        printf("%s ", patterns[i][0] == '1' ? "⠂" : " ");
        printf("%s  ", patterns[i][1] == '1' ? "⠂" : " ");
    }
    printf("\n");

    // Print middle dots
    printf(" ");
    for (int i = 0; i < count; i++) {
        printf("%s ", patterns[i][2] == '1' ? "⠂" : " ");
        printf("%s  ", patterns[i][3] == '1' ? "⠂" : " ");
    }
    printf("\n");

    // Print bottom dots
    printf(" ");
    for (int i = 0; i < count; i++) {
        printf("%s ", patterns[i][4] == '1' ? "⠂" : " ");
        printf("%s  ", patterns[i][5] == '1' ? "⠂" : " ");
    }
    printf("\n\n");
}

void translate_to_braille(const char *input) {
    int length = strlen(input);
    char patterns[CHARS_PER_LINE][BRAILLE_SIZE + 1];
    int pattern_count = 0;

    for (int i = 0; i < length; i++) {
        char c = input[i];
        const char *pattern;

        if (c >= 'a' && c <= 'z') {
            pattern = braille_patterns[c - 'a'];
        } else if (c >= '0' && c <= '9') {
            pattern = braille_numbers[c - '0'];
        } else if (c == ' ') {
            pattern = braille_space;
        } else if (c == '.') {
            pattern = braille_period;
        } else if (c == ',') {
            pattern = braille_comma;
        } else if (c == '?') {
            pattern = braille_question;
        } else if (c == '!') {
            pattern = braille_exclamation;
        } else {
            pattern = braille_space; // Unknown characters represented as space
        }

        strcpy(patterns[pattern_count], pattern);
        pattern_count++;

        if (pattern_count == CHARS_PER_LINE || i == length - 1) {
            print_braille_line(patterns, pattern_count);
            pattern_count = 0;
            sleep(1); // Wait for 1 second
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_string>\n", argv[0]);
        return 1;
    }

    char *input = argv[1];
    printf("Input: %s\n\n", input);

    clock_t start_time, end_time;
    double total_time;

    start_time = clock();
    translate_to_braille(input);
    end_time = clock();

    total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;

    printf("Total processing time: %.2f ms\n", total_time);
    printf("Average time per character: %.2f ms\n", total_time / strlen(input));

    return 0;
}