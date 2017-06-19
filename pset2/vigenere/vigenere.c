#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main (int argc, string argv[])
{
    // get the keyword as a command-line argument composed entirely of alpha chars
    // if prog is executed without args, with more than 1 arg, or with an arg containing non-alpha chars, return 1 and print error
    
    if (argc != 2)
    {
        printf("KEEP OUT\n");
        return 1;
    }
    
    string key = argv[1];
    int key_length = strlen(key);
    
    for (int i = 0; i < key_length; i++)
    {
        if (isalpha(key[i]) == false)
        {
            printf("KEEP OUT\n");
            return 1;
        }
        
        key[i] = toupper(key[i]);
    }
    
    // if valid arg, prompt user for string of plaintext (p)
    printf("plaintext: ");
    string message = get_string();
    //printf("%s\n", message);
    
    // encrpyt p using cipher (k)
    
    int nonalpha_chars = 0;
    
    printf("ciphertext: ");
    
    // iterate through characters in message
    for (int x = 0, message_length = strlen(message); x < message_length; x++)
    {
        // find key position
        int key_position = (x - nonalpha_chars) % key_length;
        //printf("Key position: %i ", key_position);

        //find key value
        int key_value = key[key_position] - 65;
        //printf("Key value: %i ", key_value);

        // shift the message character by the key value
        if (isupper(message[x]))
        {
            message[x] = ((message[x] - 65 + key_value) % 26) + 65;
        }
        else if (islower(message[x]))
        {
            message[x] = ((message[x] - 97 + key_value) % 26) + 97;

        }
        printf("%c", message[x]);
        
        if (isalpha(message[x]) == false)
        {
            nonalpha_chars++;
        }
    }
    
    printf("\n");
    
    // print ciphertext
    return 0;
}