/**
 * Encodes text from one file to another.
 * E.g., "aaa bbuvvvvo" is encoded to "a$3 b$2uv$4o"
 * 
 * Author: Donatas Pucinskas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256
#define MAX_DIGITS 5

// opens input and output files, on error prints error message and returns 1
int prepareFiles(FILE **in_file, FILE **out_file, char *argv[]) {
    *in_file = fopen(argv[1], "r");
    if (*in_file == NULL) {
        printf("Input file could not be opened.\n");
        return 1;
    }
    *out_file = fopen(argv[2], "w");
    if (*out_file == NULL) {
        printf("Output file could not be created.\n");
        return 1;
    }
    return 0;
}

// encodes source string to dest string
void encode(char *source, char *dest) {
    int index = 0;
    char number_str[MAX_DIGITS];
    
    for (int i = 0; source[i] != 0; ++i) {
        // copy character
        dest[index++] = source[i];

        if (source[i] != ' ') {
            // count consecutive characters
            int count = 1;
            while (source[i] == source[i+1]) {
                ++count;
                ++i;
            }
            // copy number of characters
            if (count > 1) {
                dest[index++] = '$';
                sprintf(number_str, "%d", count);
                for (int j = 0; number_str[j] != 0; ++j, ++index) {
                    dest[index] = number_str[j];
                }
            }
        }
    }
    dest[index] = '\0';
}

// encodes all text from input file to output file
void convertText(FILE *in_file, FILE *out_file) {
    char *buffer = malloc(MAX_BUFFER_SIZE * sizeof(char));
    char *encoded_str = malloc(MAX_BUFFER_SIZE * 2 * sizeof(char));

    while (fgets(buffer, MAX_BUFFER_SIZE, in_file)) {
        encode(buffer, encoded_str);
        fputs(encoded_str, out_file);

        // discard line ending if line is longer than maximum line length
        if (strlen(buffer) == MAX_BUFFER_SIZE - 1) {
            char ch;
            fscanf(in_file, "%*[^\n]%c", &ch);
            fputc('\n', out_file);
        }
    }

    free(encoded_str);
    free(buffer);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Expected 2 arguments: input file name and output file name.\n");
        return 1;
    }
    
    FILE *in_file;
    FILE *out_file;

    if (prepareFiles(&in_file, &out_file, argv) == 1) {
        return 1;
    }

    convertText(in_file, out_file);

    fclose(in_file);
    fclose(out_file);

    return 0;
}