# Lab specific definitions
lab_c = lab2.c
lab_o = lab2.o
lab = lab2

# Generic definitions
CC = gcc
INCLUDE = /usr/cse02/Share/X11R5/include
CFLAGS = -I$(INCLUDE)
LIBS = -lX11 -lX

all: $(lab)

$(lab): $(lab_o)
	$(CC) $(LDFLAGS) $(lab_o) -o $(lab) $(LIBS)

# Create the object files (-c option for gcc)
$(lab_o): $(lab_c)
	$(CC) $(CFLAGS) -c $(lab_c) -o $(lab_o)

clean:
	rm $(lab) $(lab_o)
