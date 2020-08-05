# Lab specific definitions
lab_c = lab8.c
lab_o = lab8.o
lab = lab8

# Generic definitions
CC = gcc
#X11_HOME = /usr/cse02/X11R5
#MOTIF_HOME = /usr/cse02/motif
X11_HOME = /usr/usc/X11R5
MOTIF_HOME = /usr/usc/motif/default
CFLAGS = -I$(MOTIF_HOME)/include -I$(X11_HOME)/include 
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
