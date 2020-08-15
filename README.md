### Background
The purpose of this repo is to show examples of how the Libgpiod library can be used to read multiple GPIO lines. The library is easy to use, but there are limited examples of how the library works in code.

In short the library provides a simple API for accessing GPIO chips through the new char device interface. Behind the scenes the library issues IOCTL calls on the char device GPIO chip to get GPIO information to user space.

The repo also includes GPIO mockup kernel modules for testing the library without having access to a physical GPIO chip. These modules are based on modules by [John Madieu](https://github.com/john-madieu/ldd/tree/master/chapter-15) and have been slightly modified under GPL to fix kernel warnings.

Code is tested with:
- Ubuntu 5.4
- Libgpiod 1.0

### Building and loading the kernel modules
Go to the mockup_gpio_chip folder and compile the modules.
```bash
# make
# sudo insmod mockup_gpio_ins.ko
# sudo insmod mockup_gpio_chip.ko
# sudo lsmod | grep mockup
```
If the modules are successfully, the kernel modules are ready to be used by Libgpiod!

### Building and using the libgpiod examples
Go to the libgpiod_examples folder and compile the programs.
```bash
# make
# sudo ./gpioReadMultipleCtxless.out
```
It is important that the program is run with **sudo privileges**, else the read operation will fail.
