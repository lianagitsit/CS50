/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dictionary.h"



// pointer to top of dictionary
node *dict_head_pointer;

unsigned int nodeCount;
unsigned int wordCount;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    //printf("Checking for %s in our dictionary...", word);
    int word_length;
    node *crawler;

    word_length = strlen(word);
        
    // set crawler to the head
    crawler = dict_head_pointer;
        
    for (int i = 0; i <= word_length; i++)
    {
        if (i == word_length && crawler->is_word == true)
        {
            //printf("and I found it!\n");
            return true;
        }
        else if (i == word_length)
        {
            //printf("But I got to the end of the word, did not find it\n");
            return false;
        }
        
        int char_index;
        // find alpha index of char
        if (word[i] == '\'')
        {
            char_index = 26;
        }
        else if (word[i] > 96)
        {
            char_index = word[i] - 'a';
        }
        else
        {
            char_index = word[i] - 'A';
        }
        if (char_index < 0 || char_index > 26)
        {
            printf("MASSIVE ERROR ABORT MISSION: letter is %c, index is %i...\n", word[i], char_index);
            return false;
        }
        
            
        // if crawler's current node's array (letters) at index position char_index is null:
        if (crawler->letters[char_index] == NULL)
        {
            //printf("Hit a null value, did not find it\n");
            return false;
        }
            
        // if not null, set crawler to the value stored in crawler's letter[i]
        else
        {
            crawler = crawler->letters[char_index];
        }
            
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    nodeCount = 1;
    wordCount = 0;
    // initialize our trie head; set is_word to false and each pointer in array to NULL
    dict_head_pointer = malloc(sizeof(node));
    dict_head_pointer->is_word = false;
    for (int i = 0; i < 27; i++)
    {
        dict_head_pointer->letters[i] = NULL;
    }

    // open the dictionary file
    FILE *dict_file_pointer = fopen(dictionary, "r");
    
    // return false if that failed
    if (dict_file_pointer == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    char word[LENGTH];
    node *crawler;
    int word_length;
    node *temp_new_node_pointer;
    node *swap;
    int char_index;
    
    while (fscanf(dict_file_pointer, "%s\n", word) != EOF)
    {
        wordCount++;
        //printf("word scanned: %s", word);
        // Add each character of word to the trie
        word_length = strlen(word);
        //printf("word length: %i", word_length);
        // set crawler to the head
        crawler = dict_head_pointer;
        
        for (int i = 0; i < word_length; i++)
        {
            // find alpha index of char, or set ' to be at index 26
            if(word[i] == '\'')
            {
                char_index = 26;
            }
            else
            {
                char_index = word[i] - 'a';
            }
            
            // if crawler's current node's array (letters) at index position char_index is null:
            if (crawler->letters[char_index] == NULL)
            {
                // malloc new node and temporarily store its pointer
                // ** try crawler->letters[char_index] = malloc(sizeof(node));
                temp_new_node_pointer = malloc(sizeof(node));
                nodeCount++;
                
                //initialize new node
                temp_new_node_pointer->is_word = false;
                for (int j = 0; j < 27; j++)
                {
                    temp_new_node_pointer->letters[j] = NULL;
                }
                
                // set crawler's letter[char_index] pointer to new node's pointer
                crawler->letters[char_index] = temp_new_node_pointer;
                
                // set crawler to new node
                crawler = temp_new_node_pointer;
            }
            
            // if not null, set crawler to the value store in crawler's letter[i]
            else
            {
                swap = crawler->letters[char_index];
                crawler = swap;
            }
            
            // if this is the last letter of the dictionary word:
            if (i == (word_length - 1))
            {
                // set is_word to true
                crawler->is_word = true;
            }
        }
    }
    fclose(dict_file_pointer);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return wordCount;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */

bool unloadrecur(node *crawler)
{
    
    for (int i = 0; i < 27; i++)
    {
        if (crawler->letters[i] != NULL)
        {
            unloadrecur(crawler->letters[i]);
        }
    }
    
    free(crawler);
    return true;
    
    // base case
    /*if (crawler = dict_head_pointer)
    {
        free(dict_head_pointer);
        return true;
    }
    
    // TODO: recursive case
    else
    {
        unload()
    }
    
    return false;*/
}

bool unload(void)
{
    unloadrecur(dict_head_pointer);
    return true;
}