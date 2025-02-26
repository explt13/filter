#include "helpers.h"
#include "math.h"
#include "stdio.h"

typedef uint8_t BYTE;
int check_in_scope(int value, int min, int max);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = (int) round(
                (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, half = width / 2; j < half; j++)
        {
            int backward_index = width - 1 - j;
            RGBTRIPLE temp = {image[i][j].rgbtBlue, image[i][j].rgbtGreen, image[i][j].rgbtRed};
            image[i][j] =
                (RGBTRIPLE){image[i][backward_index].rgbtBlue, image[i][backward_index].rgbtGreen,
                            image[i][backward_index].rgbtRed};
            image[i][backward_index] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sum_blue = 0;
            int sum_green = 0;
            int sum_red = 0;
            int q = 0;

            for (int di = -1; di < 2; di++)
            {
                for (int dj = -1; dj < 2; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sum_blue += image[i + di][j + dj].rgbtBlue;
                        sum_green += image[i + di][j + dj].rgbtGreen;
                        sum_red += image[i + di][j + dj].rgbtRed;
                        q += 1;
                    }
                }
            }
            BYTE avg_blue = (BYTE) round((float) sum_blue / q);
            BYTE avg_green = (BYTE) round((float) sum_green / q);
            BYTE avg_red = (BYTE) round((float) sum_red / q);
            copy[i][j] = (RGBTRIPLE){avg_blue, avg_green, avg_red};
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    int GX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int GY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int gxb, gxg, gxr, gyb, gyg, gyr;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            gxb = gxg = gxr = gyb = gyg = gyr = 0;
            for (int di = -1; di < 2; di++)
            {
                for (int dj = -1; dj < 2; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni < 0 || ni > height - 1)
                    {
                        continue;
                    }
                    else if (nj < 0 || nj > width - 1)
                    {
                        continue;
                    }

                    gxb += image[ni][nj].rgbtBlue * GX[di + 1][dj + 1];
                    gxg += image[ni][nj].rgbtGreen * GX[di + 1][dj + 1];
                    gxr += image[ni][nj].rgbtRed * GX[di + 1][dj + 1];

                    gyb += image[ni][nj].rgbtBlue * GY[di + 1][dj + 1];
                    gyg += image[ni][nj].rgbtGreen * GY[di + 1][dj + 1];
                    gyr += image[ni][nj].rgbtRed * GY[di + 1][dj + 1];
                }
            }

            int blue = fmin(round(sqrt(gxb * gxb + gyb * gyb)), 255);
            int green = fmin(round(sqrt(gxg * gxg + gyg * gyg)), 255);
            int red = fmin(round(sqrt(gxr * gxr + gyr * gyr)), 255);
            copy[i][j] = (RGBTRIPLE){blue, green, red};
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}

int check_in_scope(int value, int min, int max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    else
    {
        return value;
    }
}
