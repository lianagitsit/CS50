#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// get key from command line argument
int main (int argc, string argv[])
{
    string plain; 
    int k;
    
    if (argc == 2) 
    {
        // turn key into integer
        k = atoi(argv[1]);
        k %= 26;
        
        // prompt for plaintext
        printf("plaintext: ");
        plain = get_string();
        
        printf("ciphertext: ");
        
        // for each character in plaintext:
        for (int i = 0, n = strlen(plain); i < n; i++)
        {
            // checks if char is a letter
            if (isalpha(plain[i]))
            {
                // shift plaintext char by key
                int ascii_cipher = plain[i] + k;
                
                // wraparound z - a (preserve lowercase)
                if (ascii_cipher > 122)
                {
                    ascii_cipher -= 26;
                }
                
                // wraparound Z - A (preserve uppercase)
                if (ascii_cipher > 90 && plain[i] < 90)
                {
                    ascii_cipher -= 26;
                }
                
                // print ciphertext
                printf("%c", ascii_cipher);
            }
            else
            {
                printf("%c", plain[i]);
            }
        }
    }
    else
    {
        // if no argument or more than one argument is entered:
        printf("Error: key must be one integer.\n");
        return 1;
    }
    
    printf("\n");

    return 0;
}

