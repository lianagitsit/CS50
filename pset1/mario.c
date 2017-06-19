#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int maxRows;
    do 
    {
        printf("Height: \n");
        maxRows = get_int();
    }
    while(maxRows < 0 || maxRows > 23);
    
        for (int index = 0; index < maxRows; index++)
        {
            // print spaces
            for (int spaces = 0; spaces < (maxRows - (index + 1)); spaces++)
            {
                printf(" ");
            }
            // print hashes
            for (int hashes = 0; hashes <= (index + 1); hashes++)
            {
                printf("#");
            }
            // print new line
             printf("\n");
        }
}