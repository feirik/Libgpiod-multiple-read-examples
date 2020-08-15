#include <stdio.h>
#include <unistd.h>
#include <gpiod.h>

// Number of gpiolines, should match ngpio for gpiochip#
#define NGPIO 16

int main(int argc, char **argv)
{
    // Printing libgpiod version in use
    printf("Using libgpiod version: %s\n", gpiod_version_string());

    // Use this notation if you want to access by gpiochip#
    //char *chipname = "gpiochip0";

    // Using the label name to access the gpiochip
    char *label = "mockup_gpio_chip";

    // Offset array to select which gpio lines to read
    unsigned int offset[NGPIO];

    // Values array to store the read values
    int values[NGPIO];

    // Accessing offsets 0-15, base is at -1 for the mockup_gpio_chip
    for(int i = 0; i < NGPIO; i++)
    {
        offset[i] = i;
    }

    // Initalizing values to 0
    for(int i = 0; i < NGPIO; i++)
    {
        values[i] = 0;
    }

    // Using contextless libgpiod API to read multiple vales from label
    int ret = gpiod_ctxless_get_value_multiple(label, 
                                               offset, 
                                               values, 
                                               NGPIO, 0, "Mockup_gpio_driver");
    // Libgpiod error check
    if(ret == -1)
    {
        printf("Failed reading gpio lines for %s\n", label);
    }
    else
    {   
        // Success, printing the read values
        for(int i = 0; i < NGPIO; i++)
        {
            printf("Values at loc %i is %i\n", i, values[i]);
        }
    }

    printf("Ended program\n");

    return 0;
}
