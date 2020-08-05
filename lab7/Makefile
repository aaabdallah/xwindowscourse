# Lab specific definitions
lab_c = lab7.c
lab_o = lab7.o
lab = lab7

# Generic definitions
CC = gcc
X11_HOME = /usr/cse02/X11R5
MOTIF_HOME = /usr/cse02/motif
CFLAGS = -I$(X11_HOME)/include -I$(MOTIF_HOME)/include
# LDFLAGS = 
LIBS = -L$(MOTIF_HOME)/lib -L$(X11_HOME)/lib -lXm -lXt -lX11

all: $(lab)

$(lab): $(lab_o)
	$(CC) $(LDFLAGS) $(lab_o) -o $(lab) $(LIBS)

# Create the object files (-c option for gcc)
$(lab_o): $(lab_c)
	$(CC) $(CFLAGS) -c $(lab_c) -o $(lab_o)

clean:
	rm $(lab) $(lab_o)
