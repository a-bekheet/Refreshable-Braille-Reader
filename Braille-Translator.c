#include <stdio.h>
#include <string.h>
#include <time.h>

#define BRAILLE_SIZE 6

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

    // Braille pattern for space
    const char *braille_space = "000000";

    // Braille pattern for period
    const char *braille_period = "010011";

    // Braille pattern for comma
    const char *braille_comma = "010000";

    // Braille pattern for question mark
    const char *braille_question = "011001";

    // Braille pattern for exclamation mark
    const char *braille_exclamation = "011010";

void print_braille(const char *input) {
    for (int i = 0; i < strlen(input); i++) {
        char c = input[i];
        if (c >= 'a' && c <= 'z') {
            printf("%s ", braille_patterns[c - 'a']);
        } else {
            printf("000000 "); // Non-alphabet characters are represented as empty Braille cells
        }
    }
    printf("\n");
}

void simulate_outputs(const char *input) {
    for (int i = 0; i < strlen(input); i++) {
        char c = input[i];
        if (c >= 'a' && c <= 'z') {
            const char *pattern = braille_patterns[c - 'a'];
            for (int j = 0; j < BRAILLE_SIZE; j++) {
                if (pattern[j] == '1') {
                    printf("Output %d: ON\n", j + 1);
                } else {
                    printf("Output %d: OFF\n", j + 1);
                }
            }
        } else {
            for (int j = 0; j < BRAILLE_SIZE; j++) {
                printf("Output %d: OFF\n", j + 1);
            }
        }
        printf("\n");
    }
}

void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[8]; // 7 characters + null terminator
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character if present
        printf("Processing: %s\n", buffer);
        print_braille(buffer);
        simulate_outputs(buffer);
    }

    fclose(file);
}

int main() {
    char input[100];
    printf("Enter a string: ");
    int choice;
    printf("Choose input method:\n");
    printf("1. Enter text\n");
    printf("2. Read from file\n");
    printf("Enter choice (1 or 2): ");
    scanf("%d", &choice);
    getchar(); // Consume the newline character left by scanf

    if (choice == 1) {
        printf("Enter a string: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character

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
    } else if (choice == 2) {
        char filename[100];
        printf("Enter filename: ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0'; // Remove newline character

        clock_t start_time, end_time;
        double total_time;

        start_time = clock();
        process_file(filename);
        end_time = clock();

        total_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC * 1000;

        printf("Total processing time for file: %.2f ms\n", total_time);
    } else {
        printf("Invalid choice.\n");
        return 1; // Exit with error code
    }

    return 0;
}
