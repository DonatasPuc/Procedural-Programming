/* 
 * Author: Donatas Pučinskas
 *
 * Checks for 3 consecutive zeros
 */

#include <stdio.h>

int main() {
    int num = 0;
    int zero_count = 0;   /* Number of consecutive zeros */
    int zero_flag = 0;    /* Flag set when 3 zeros in a row are entered */

    printf("Enter an integer: ");
    scanf("%d", &num);

    while (num <= 1000 && num >= -1000)
    {
        /* Count how many consecutive zeros have been entered */
        if (num == 0)
            zero_count++;
        else
            zero_count = 0;

        /* Check if 3 consecutive 0 have been entered */
        if (zero_count == 3)
            zero_flag = 1;
        
        printf("Enter an integer: ");
        scanf("%d", &num);
    }
    
    /* Output after input doesn't satisfy conditions */
    printf("\nYou entered a number higher than 1000 or lower than -1000\n");
    if (zero_flag)
        printf("3 zeros in a row have been entered.");

    return 0;
}