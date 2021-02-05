/**
 * Finds extremes in a vector of numbers
 * 
 * Author: Donatas Pucinskas
 */

#include <stdio.h>

#define MAX_NUM 1000

void findExtremes(int *extreme_count, int extremes[], int number_count, int numbers[]) {
    int is_increasing = numbers[1] > numbers[0] ? 1 : 0;
    for (int i = 0; i < number_count - 1; ++i) {
        if (is_increasing && numbers[i+1] < numbers[i]) {
            is_increasing = 0;
            extremes[i] = 1;
            (*extreme_count)++;
        } else if (!is_increasing && numbers[i+1] > numbers[i]) {
            is_increasing = 1;
            extremes[i] = 1;
            (*extreme_count)++;
        }
    }
}

int findFirstExtreme(int number_count, int extremes[]) {
    for (int i = 0; i < number_count; ++i) {
        if (extremes[i]) {
            return i;
        }
    }
    return -1;
}

void findMaxDistance(int number_count, int extremes[], int *first_ind, int *second_ind, int *max_dist) {
    int prev_extreme_ind = findFirstExtreme(number_count, extremes);
    for (int i = prev_extreme_ind + 1; i < number_count; ++i) {
        if (extremes[i]) {
            int dist = i - prev_extreme_ind;
            if (dist > *max_dist) {
                *max_dist = dist;
                *first_ind = prev_extreme_ind;
                *second_ind= i;
            }
            prev_extreme_ind = i;
        }
    }
}

void calculateExtremes(int number_count, int numbers[], int *extreme_count, int extremes[], int *max_dist, int *first_ind, int *second_ind) {
    findExtremes(extreme_count, extremes, number_count, numbers);
    if (*extreme_count > 1) {
        findMaxDistance(number_count, extremes, first_ind, second_ind, max_dist);
    } else if (*extreme_count == 1) {
        *first_ind = findFirstExtreme(number_count, extremes);
    }
}

int main() {

    int number_count;
    int numbers[MAX_NUM];
    int extreme_count = 0;
    int extremes[MAX_NUM] = {0};        // 1 - extreme, 0 - not extreme
    int max_dist = 0;
    int first_ind = 0;
    int second_ind = 0;

    printf("Iveskite skaiciu kieki: ");
    scanf("%d", &number_count);
    printf("Iveskite sveikuju skaiciu seka: ");
    for (int i = 0; i < number_count; ++i) {
        scanf("%d", numbers + i);
    }

    calculateExtremes(number_count, numbers, &extreme_count, extremes, &max_dist, &first_ind, &second_ind);
    
    if (extreme_count == 0) {
        printf("Nera ekstremumu.");
    } else if (extreme_count == 1) {
        printf("Yra tik vienas ekstremumas indeksu: %d", first_ind);
    } else {
        printf("Didziausias atstumas tarp ekstremumu yra: %d\n", max_dist);
        printf("Ekstremumu indeksai yra %d ir %d", first_ind, second_ind);
    }
    
    return 0;
}