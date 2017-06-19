/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int beg = 0;
    int end = n - 1;
    int mid = (beg + end) / 2;
    
    // TODO: implement a searching algorithm
    while (true)
    {
        // ensures user input is positive
        if (value < 0)
        {
            return false;
        }
        
        // goes to middle value of array and checks against value
        if (value == values[mid])
        {
            return true;
        }
        
        // if value is greater than the middle value, shifts search to the right
        else if (values[mid] < value)
        {
            beg = mid + 1;   // increments search start by one since mid value has already been checked
            mid = (end + beg) / 2;  // end value stays the same
        }
        
        // if value is less than the middle value, shifts search to the left
        else if (values[mid] > value)
        {
            end = mid - 1;  // decrements search start by one since mid value has already been checked
            mid = (end + beg) / 2;  // beginning value stays the same
        }
        
        // if there is nothing in the array left to search and the value has not been found, returns false
        if (beg > end)
        {
            return false;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    // iterate through numbers in the array
    for (int i = 0; i < n; i++)
    {
        int sort_slot = i; //index position of sorting slot
        int current_smallest = i; //index position of the current smallest number
        
        // holding the i'th number, loop through the array comparing each number to the i'th
        for (int j = i; j < n; j++)
        {
            // if a number is smaller than the i'th number, then that number becomes the i'th (smallest)
            if (values[j] < values[current_smallest])
            {
                current_smallest = j;
            }
        }
        
        // swap the actual smallest number with the number in the sorting slot position
        int holster = values[sort_slot];
        values[sort_slot] = values[current_smallest];
        values[current_smallest] = holster;
    }
    printf("Printing test results!\n");
    for (int a = 0; a < n; a++)
    {
        printf("Array index %i: %i\n", a, values[a]);
    }
    return;
}
