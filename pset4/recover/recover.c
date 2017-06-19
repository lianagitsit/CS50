#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile \n");
        return 1;
    }
    
    char* infile = argv[1];
    
    // open memory card file 
    FILE *file = fopen(infile, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    // create buffer array
    unsigned char buffer[512];
    int imagesFound = 0;
    FILE *outptr;
    char filename[7];
    int bytes_read = 0;
    
    // read 512 B from file into buffer
    while(1 > 0)
    {
        bytes_read = fread(&buffer, 1, 512, file);
        if (bytes_read != 512)
        {
            break;
        }
        
        // check for 4B match
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0 && imagesFound > 0)
            {
                fclose(outptr);
                //printf("Closing file %i...", imagesFound);
            }

        
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
            {
                sprintf(filename, "%03i.jpg", imagesFound);
                outptr = fopen(filename, "w");
                imagesFound++;
            }
        
        if (imagesFound > 0)
        {
            fwrite(&buffer, 512, 1, outptr);
        }
    } //close while loop
    
    //fwrite(&buffer, bytes_read, 1, outptr);
    
    fclose(outptr);
    return 0;

}