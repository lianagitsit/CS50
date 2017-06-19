#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("minutes: ");
    int i = get_int();
    
    int j = i * 192;
    
    int k = j / 16;
    printf("bottles: %i\n", k);
}