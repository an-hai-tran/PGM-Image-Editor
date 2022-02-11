#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*function declarations*/
unsigned char* transpose(unsigned char *array, int rows, int cols)
{
    /*declare new array*/
    int i, j;
    unsigned char *tempArray = NULL;
    tempArray = (unsigned char *)malloc((rows * cols) * sizeof(unsigned char));

    /*transposing*/
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            tempArray[j * rows + i] = array[i * cols + j];
        }
    }

    /*return new array*/
    free(array);
    return tempArray;
}

void flipVertical(unsigned char *array, int rows, int cols)
{
    int i, x, y;
    unsigned char temp;

    /*swapping a[i][j] and a[rows - i - 1][j]*/
    for (i = 0; i < cols; i++)
    {
       x = 0;
       y = rows - 1;
       while (x < y)
       {
           temp = array[x * cols + i];
           array[x * cols + i] = array[y * cols + i];
           array[y * cols + i] = temp;
           
           x++;
           y--;
       } 
    }
}

void flipHorizontal(unsigned char *array, int rows, int cols)
{
    int i, x, y;
    unsigned char temp;

    /*swapping a[i][j] and a[i][cols - j - 1]*/
    for (i = 0; i < rows; i++)
    {
       x = 0;
       y = cols - 1;
       while (x < y)
       {
           temp = array[i * cols + x];
           array[i * cols + x] = array[i * cols + y];
           array[i * cols + y] = temp;
           
           x++;
           y--;
       } 
    }
}

unsigned char average(unsigned char *array, int i, int j, int rows, int cols)
{
    int total = 0;
    int count = 0;
    unsigned char average;
    
    /*check 8 cases of neighbor*/
    if (i-1 >= 0 && j-1 >= 0)
    {
        total+= (int)array[(i-1)*cols + j-1];
        count++;
    }
    if (i-1 >= 0)
    {
        total+= (int)array[(i-1)*cols + j];
        count++;
    }
    if (i -1 >= 0 && j +1 < cols)
    {
        total+= (int)array[(i-1)*cols + j+1];
        count++;
    }
    if (j-1 >= 0)
    {
        total+= (int)array[i*cols + j-1];
        count++;
    }
    if (j+1 < cols)
    {
        total+= (int)array[i*cols + j+1];
        count++;
    }
    if (i+1 < rows)
    {
        total+= (int)array[(i+1)*cols + j];
        count++;
    }
    if (i+1 < rows && j-1 >= 0)
    {
        total+= (int)array[(i+1)*cols + j-1];
        count++;
    }
    if (i+1 < rows && j +1 < cols)
    {
        total+= (int)array[(i+1)*cols + j+1];
        count++;
    }
    
    total += (int)array[i * cols + j];
    count++;
    average = (unsigned char) (total/count);
    return average;
}

int main(int argc, char *argv[])
{
    
    /*declare variables*/
    char line[150];
    FILE *fp;
    int option;
    int i, j, k, temp, rows, cols;
    unsigned char tempData;

    /*create pixel array*/
    unsigned char *pixel = NULL; 
    unsigned char *tempPixel = NULL;

    /*get rid of argc warning messages*/
    (void)argc;

    /*open file and scan the first 4 information*/
    fp = fopen(argv[1], "rb");
    fscanf(fp, "%s\n", line);
    fscanf(fp, "%d", &cols);
    fscanf(fp, "%*[ ]");
    fscanf(fp, "%d\n", &rows); /*default is width columns and height rows*/
    fscanf(fp, "%s\n", line);

    /*initialize pixel and tempPixel*/
    pixel = (unsigned char *)malloc((rows * cols) * sizeof(unsigned char));
    
    /*read the pixel value of the file into the array*/
    for (i = 0; i < rows; i++) /*h rows*/
    {
        for (j = 0; j < cols; j++) /*w columns*/
        {
            fscanf(fp, "%c", &pixel[i * cols + j]);
        }
    }

    /*close the file*/
    fclose(fp);

    /*options menu*/
    do
    {
        printf("Options menu: \n");
        printf(" 1. Rotate Left (counter-clockwise)\n");
        printf(" 2. Rotate Right (clockwise)\n");
        printf(" 3. Flip Vertical\n");
        printf(" 4. Flip Horizontal\n");
        printf(" 5. Convert to Monochrome (black and white)\n");
        printf(" 6. Negative (invert image)\n");
        printf(" 7. Blur\n");
        printf(" 8. Darken\n");
        printf(" 9. Sort Each Row (ascending)\n");
        printf("10. Save Changes and Quit\n");
        printf("\nYour choice? ");
        scanf("%d", &option);
        
        if (option == 1)
        {
            tempPixel = transpose(pixel, rows, cols);  

            temp = rows;
            rows = cols;
            cols = temp;

            pixel = tempPixel;
            flipVertical(pixel, rows, cols);
        }
        else if (option == 2)
        {
            tempPixel = transpose(pixel, rows, cols);   

            temp = rows;
            rows = cols;
            cols = temp;

            pixel = tempPixel;
            flipHorizontal(pixel, rows, cols);
        }
        else if (option == 3)
        {
            flipVertical(pixel, rows, cols);
        }
        else if (option == 4)
        {
            flipHorizontal(pixel, rows, cols);
        }
        else if (option == 5)
        {
            for (i = 0; i < rows; i++)
            {
                for (j = 0; j < cols; j++)
                {
                    if ((pixel[i * cols + j] & 0xC0) != 0 )
                    {
                        pixel[i * cols + j] = 0xFF;
                    }
                    else
                    {
                        pixel[i * cols + j] = 0x00;
                    }
                }
            }
        }
        else if (option == 6)
        {
            for (i = 0; i < rows; i++)
            {
                for (j = 0; j < cols; j++)
                {
                    pixel[i * cols + j] = 255 - pixel[i * cols + j];
                }
            }
        }
        else if (option == 7)
        {
            tempPixel = pixel;
            for (i = 0; i < rows; i++)
            {
                for (j = 0; j < cols; j++)
                {
                    pixel[i * cols + j] = average(tempPixel, i, j, rows, cols);
                }
            }
        }
        else if (option == 8)
        {
            for (i = 0; i < rows; i++)
            {
                for (j = 0; j < cols; j++)
                {
                    pixel[i * cols + j] = pixel[i * cols + j] >> 1;
                }
            }
        }
        else if (option == 9)
        {
            for (i = 0; i < rows; i++)
            {
                for (j = 0; j < cols; j++)
                {
                    for (k = 0; k < cols - j - 1; k++)
                    {
                        if (pixel[i * cols + k] > pixel[i * cols + k + 1])
                        {
                            tempData = pixel[i * cols + k];
                            pixel[i * cols + k] = pixel[i * cols + k + 1];
                            pixel[i * cols + k + 1] = tempData;
                        }
                    }
                }
            }
        }
    }
    while (option != 10);

    /*save the new image*/

    /*open output.pgm and write the first 4 lines*/
    fp = fopen("output.pgm","wb");
    fprintf(fp, "P5\n");
    fprintf(fp, "%d %d\n", cols, rows);
    fprintf(fp, "255\n");

    /*printing the pixel value of the array into the file*/
    for (i = 0; i < rows; i++) /*w rows*/
    {
        for (j = 0; j < cols; j++) /*h columns*/
        {
            fprintf(fp, "%c", pixel[i * cols + j]);
        }
    }

    /*close the file*/
    fclose(fp);
    free(pixel);
    return 0;
}
