# Lab specific definitions
lab_c = lab6.c
lab_o = lab6.o
lab = lab6

# Generic definitions
CC = gcc
X11_HOME = /usr/usc/X11R5
MOTIF_HOME = /usr/usc/motif/default
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
