PROJ=gpioReadMultipleCtxless.out
PROJ_DETAILED=gpioReadMultipleDetailed.out
CC=cc
SRC=gpioReadMultipleCtxless.c
SRC_DETAILED=gpioReadMultipleDetailed.c
LIBS=-lgpiod
CFLAGS=-D CONSUMER=\"$(PROJ)\"

all:
	$(CC) $(SRC) $(LIBS) $(FLAGS) -o $(PROJ)
	$(CC) $(SRC_DETAILED) $(LIBS) $(FLAGS) -o $(PROJ_DETAILED)

clean:
	rm $(PROJ) $(PROJ_DETAILED) 
