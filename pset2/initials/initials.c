#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main (void)
{
    //ask user's name
    string name;
    name = get_string();
    
    //check if string
    if (name != NULL) {
        
        // find first letter and capitalize it if needed
       printf("%c", toupper(name[0]));
        
        //iterate through rest of name
        for (int i = 0, n = strlen(name); i < n; i++)
        {
            //find space
            if (name[i] == ' ')
            {
                //access the char after the space and capitalize it if needed
                printf("%c", toupper(name[i + 1]));
            }
        }
        //print line break
        printf("\n");    
    }
}