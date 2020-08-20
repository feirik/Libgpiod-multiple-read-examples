#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <gpiod.h>

// Number of gpiolines, should match ngpio for gpiochip#
#define NGPIO 16

int main(int argc, char **argv)
{
    int ret;

    // Printing libgpiod version in use
    printf("Using libgpiod version: %s\n", gpiod_version_string());

    // Use this notation if you want to access by gpiochip#
    //char *chipname = "gpiochip0";

    // Using the label name to access the gpiochip
    char *label = "mockup_gpio_chip";

    const char* consumer = "mockup_gpio_driver";

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

    // Create a libgpiod bulk item for storing multiple lines
    struct gpiod_line_bulk bulk;

    struct gpiod_chip *chip;
    struct gpiod_line *line;

    // Status flag
    int status;

    /* Gpio flag for setting active low or active high 
    ** (Whether 1 is high or 0 is low)
    */
    int flags;

    // Setting active to be high
    bool active_low = 0;
    unsigned int i;

    // Checking to see if selected NGPIO is higher than max possible lines
    if (NGPIO > GPIOD_LINE_BULK_MAX_LINES) 
    {
        errno = EINVAL;
        return -1;
    }
    
    // Opening chip by label name
    chip = gpiod_chip_open_lookup(label);
    if (!chip)
    {        
        return -1;
    }

    // Initializing the bulk struct to empty
    gpiod_line_bulk_init(&bulk);

    // Opening up the lines, updating each line and adding them to the bulk struct
    for (i = 0; i < NGPIO; i++)
    {
	line = gpiod_chip_get_line(chip, offset[i]);
	if (!line)
	{
	    gpiod_chip_close(chip);
	    return -1;
	}

	gpiod_line_bulk_add(&bulk, line);
    }

    // Setting active to be high from active_low variable
    flags = active_low ? GPIOD_LINE_REQUEST_FLAG_ACTIVE_LOW : 0;
    
    // Reserving gpio lines
    status = gpiod_line_request_bulk_input_flags(&bulk, consumer, flags);
    if (status < 0) 
    {
        gpiod_chip_close(chip);
	return -1;
    }

    // Ensuring values array is initialized to 0
    memset(values, 0, sizeof(*values) * NGPIO);
	
    // Getting values from the gpio lines
    status = gpiod_line_get_value_bulk(&bulk, values);

    gpiod_chip_close(chip);

    // Status is dependent on return values from libgpoid IOCTL calls
    ret = status;    

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
