#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define EULER_E 2.7182818284
#define ASCII_GREYSCALE " .:-=+*#%@"

char greyscale_mapper(float value)
{
    if (value <= 0.f)
    {
        return ASCII_GREYSCALE[0];
    }
    return ASCII_GREYSCALE[(int)(value*10.f)];
}

float radial_function(float x, float y, float radial_width, float radial_height)
{
    return 1 - (radial_width * x * x + radial_height * y * y);
}

float activation_function(float value, float activation_offset, float activation_steepness)
{
    return 1.f/(1 + pow(EULER_E, -1.f*(activation_steepness*value + activation_offset)));
}

float** generate_falloff_map(
    int width, int height, 
    float activation_offset, float activation_steepness,
    float radiual_width, float radial_height)
{
    float** map = malloc(height * sizeof(float*)); 
    for (int i = 0; i < height; i++)
    {
        map[i] = malloc(width * sizeof(float)); 
        for (int j = 0; j < width; j++)
        {
            float x = (float)(2 * j - width + 1) / width;
            float y = (float)(2 * i - height + 1) / height;

            float radial_value = radial_function(x, y, radiual_width, radial_height);
            float activated_value = activation_function(radial_value, activation_offset, activation_steepness);
            map[i][j] = activated_value;
        }
    }
    return map;
}
float generate_random_float(float min, float max)
{
    return (float)rand() / RAND_MAX * (max - min) + min;
}

/*
 * The top level array here corresponds to the rows of the map, that's why we'll iterate over the height first.
*/
float**  generate_random_map(int width, int height)
{
    float** map = malloc(height * sizeof(float*)); 
    for (int i = 0; i < height; i++)
    {
        map[i] = malloc(width * sizeof(float)); 
        for (int j = 0; j < width; j++)
        {
            map[i][j] = generate_random_float(-1.f, 1.f); 
        }
    }
    return map;
}

float clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float** clamped_matrix_addition(float** matrix1, float** matrix2, int width, int height, float min, float max)
{
    float** result = malloc(height * sizeof(float*)); 
    for (int i = 0; i < height; i++)
    {
        result[i] = malloc(width * sizeof(float)); 
        for (int j = 0; j < width; j++)
        {
            result[i][j] = clamp(matrix1[i][j] + matrix2[i][j], min, max);
        }
    }
    return result;
}

float** elementwise_multiplication(float** matrix1, float** matrix2, int width, int height)
{
    float** result = malloc(height * sizeof(float*)); 
    for (int i = 0; i < height; i++)
    {
        result[i] = malloc(width * sizeof(float)); 
        for (int j = 0; j < width; j++)
        {
            result[i][j] = matrix1[i][j] * matrix2[i][j];
        }
    }
    return result;
}

// done: falloff map created
// todo: change random map to go from 0 to 1 instead of -1 to 1.
int main(void)
{
    printf("PacGen!\n");
    printf("This is the ASCII greyscale: %s\n", ASCII_GREYSCALE);
    
    unsigned int seed = (unsigned int)time(NULL);
    srand(seed);

    int width = 70;
    int height = 35;
    printf("Width: %d, Height: %d, Seed: %d\n", width, height, seed);
    
    float activation_offset = -5.f;
    float activation_steepness = 10.f;
    float radiual_width = 1.f;
    float radiual_height = 1.f;

    float ** map = generate_falloff_map(width, height, activation_offset, activation_steepness, radiual_width, radiual_height);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%c", greyscale_mapper(map[i][j]));
        }
        printf("\n");
    }
    // free memory used by map
    for (int i = 0; i < height; i++)
    {
        free(map[i]);
    }
    free(map);
    return 0;
}