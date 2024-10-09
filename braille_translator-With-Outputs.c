#include <stdio.h>
#include <string.h>
#include <time.h>

#define BRAILLE_SIZE 6
#define MAX_INPUT_LENGTH 1000

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

void print_braille(const char *input) {
    for (int i = 0; i < strlen(input); i++) {
        char c = input[i];
        if (c >= 'a' && c <= 'z') {
            printf("%s ", braille_patterns[c - 'a']);
        } else if (c >= '0' && c <= '9') {
            printf("%s ", braille_numbers[c - '0']);
        } else if (c == ' ') {
            printf("%s ", braille_space);
        } else if (c == '.') {
            printf("%s ", braille_period);
        } else if (c == ',') {
            printf("%s ", braille_comma);
        } else if (c == '?') {
            printf("%s ", braille_question);
        } else if (c == '!') {
            printf("%s ", braille_exclamation);
        } else {
            printf("%s ", braille_space); // Unknown characters represented as space
        }
    }
    printf("\n");
}

void simulate_outputs(const char *input) {
    for (int i = 0; i < strlen(input); i++) {
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

        for (int j = 0; j < BRAILLE_SIZE; j++) {
            printf("Output %d: %s\n", j + 1, pattern[j] == '1' ? "ON" : "OFF");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_string>\n", argv[0]);
        return 1;
    }

    char *input = argv[1];
    if (strlen(input) > MAX_INPUT_LENGTH) {
        printf("Input too long. Maximum length is %d characters.\n", MAX_INPUT_LENGTH);
        return 1;
    }

    printf("Processing: %s\n", input);
    printf("Braille translation: ");
    print_braille(input);

    clock_t start_time, end_time;
    double total_time;

    start_time = clock();
    simulate_outputs(input);
    end_time = clock();

    total_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC * 1000;

    printf("Total processing time: %.2f ms\n", total_time);
    printf("Average time per character: %.2f ms\n", total_time / strlen(input));

    return 0;
}