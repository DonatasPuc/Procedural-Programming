/* Author: Donatas Pucinskas
 *
 * Program counts words in a given file. 
 * Words contain alphanumeric characters.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_FILE_NAME_LENGTH 100
#define MAX_WORD_COUNT 500
#define MAX_BUFF_SZ 1000
#define MIN_NUM_OF_WORDS 5

int findWordIndex(int *word_count, char arr[][MAX_BUFF_SZ], char word[]);
void parseWord(int *curr_ind, char buff[], int buff_length, char word[]);
void appendWord(int *word_count, char words[][MAX_BUFF_SZ], char word[]);
void countWords(int *word_count, char words[][MAX_BUFF_SZ], char buff[], int freq[]);
void sortByFrequency(int word_count, char words[][MAX_BUFF_SZ], int freq[]);
void printWordsAndFreq(int word_count, char words[][MAX_BUFF_SZ], int freq[], int last_freq);

int main() {

    // data
    int word_count = 0;
    int *wc_ptr = &word_count;

    char buffer[MAX_BUFF_SZ];
    char words[MAX_WORD_COUNT][MAX_BUFF_SZ];
    int frequency[MAX_WORD_COUNT] = {0};

    char file_name[MAX_FILE_NAME_LENGTH];

    printf("\nPrograma suskaiciuoja zodziu dazni faile \n"
    "(zodziai yra sudaryti is mazuju, didziuju raidziu ir skaitmenu).\n"
    "\nIveskite failo varda: ");
    scanf("%100s", file_name);
    
    FILE *fp = fopen(file_name, "r");

    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            countWords(wc_ptr, words, buffer, frequency);
        }
    } else {
        printf("Failo nepavyko atidaryti.\n");
        return 1;
    }

    sortByFrequency(word_count, words, frequency);

    int last_freq = word_count > MIN_NUM_OF_WORDS ? frequency[MIN_NUM_OF_WORDS - 1] : 0;
    printWordsAndFreq(word_count, words, frequency, last_freq);

    return 0;
}

void countWords(int *word_count, char words[][MAX_BUFF_SZ], char buff[], int freq[]) {
    int buff_length = strlen(buff);

    for (int i = 0; i < buff_length; ++i) {
        char curr_w[MAX_BUFF_SZ] = {0};
        // jeigu zodis prasidejo
        if (isalnum(buff[i])) {
            parseWord(&i, buff, buff_length, curr_w);
            int pos = findWordIndex(word_count, words, curr_w);
            if (pos == -1) {
                appendWord(word_count, words, curr_w);
                freq[*word_count] = 1;
            } else {  
                ++freq[pos];
            }
        }
    }
}

int findWordIndex(int *word_count, char arr[][MAX_BUFF_SZ], char word[]) {
    for (int i = 0; i < *word_count; ++i) {
        if (strcmp(arr[i], word) == 0) {
            return i;
        }
    }
    return -1;
}

void parseWord(int *curr_ind, char buff[], int buff_length, char word[]) {
    for (int i = 0; isalnum(buff[*curr_ind]) && *curr_ind < buff_length; ++i, ++(*curr_ind)) {
        word[i] = buff[*curr_ind];
    }
}

void appendWord(int *word_count, char words[][MAX_BUFF_SZ], char word[]) {
    strcpy(words[*word_count], word);
    *word_count += 1;
}

void sortByFrequency(int word_count, char words[][MAX_BUFF_SZ], int freq[]) {
    for (int i = 0; i < word_count; ++i) {
        for (int j = i + 1; j < word_count; ++j) {
            if (freq[i] < freq[j]) {
                int temp = freq[i];
                freq[i] = freq[j];
                freq[j] = temp;
                char str[MAX_BUFF_SZ];
                strcpy(str, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], str);
            }
        }
    }
}

void printWordsAndFreq(int word_count, char words[][MAX_BUFF_SZ], int freq[], int last_freq) {
    printf("Zodziai ir ju dazniai:\n");
    for (int i = 0; i < word_count && freq[i] >= last_freq; ++i) {
        printf("%-20s daznis: %d\n", words[i], freq[i]);
    }
}