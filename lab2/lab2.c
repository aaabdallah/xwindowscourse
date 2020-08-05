/* Ahmed Abd-Allah, lab 2 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define DEAD		0
#define ALIVE 		1

main(argc,argv)
int argc;
char **argv;
{
    Display *dpy;		/* X server connection */
    int screen;			/* Screen ID */
    Window win;			/* Window ID */
    Window subwin[10];		/* Subwindows ID's */
    int subwin_status[10];	/* Subwindow statuses */
    unsigned long black, white;	/* Pixel values */
    unsigned int i;		/* Counter */
    int x, y;			/* Temporary position holders */
    XEvent event;		/* Event received */
    XWindowAttributes topxwa;	/* Parent window attributes */
    XWindowAttributes subxwa;	/* Subwindow attributes */
    int done;			/* Loop variable */

    /* Open the display using the $DISPLAY environment variable to locate
       the X server.  */
    if ((dpy = XOpenDisplay(NULL)) == NULL) {
	fprintf(stderr, "%s: can't open %s\n", argv[0], XDisplayName(NULL));
	exit(1);
    }

    screen = XDefaultScreen(dpy);
    black = BlackPixel(dpy, screen);
    white = WhitePixel(dpy, screen);

    win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
			      0, 0, 500, 500,
			      0, black, white);
    if (XGetWindowAttributes(dpy, win, &topxwa) == 0)
    {
	exit(0);
    }
    else
    {
	if (topxwa.width < 200 || topxwa.height < 200)
	{
	    fprintf(stderr, "%s: Window too small\n", argv[0]);
	    exit(0);
	}
    }

    XSelectInput(dpy, win, ExposureMask);

    XMapWindow(dpy, win);

    /* Create and map 10 subwindows */
    for ( i=0; i<10; i++ )
    {
	subwin[i] = XCreateSimpleWindow(dpy, win,
				    rand() % 500, 
				    rand() % 500 , 20, 20,
				    0, white, black);
	XMapWindow(dpy, subwin[i]);
	subwin_status[i] = ALIVE;
    }

    done = 0;
    while ( done < 10 ) 
    {
	while ( !XEventsQueued(dpy, QueuedAlready) && done < 10)
	{
	    XFlush(dpy);
	    for (i=0; i<10; i++)
	    {
		if (subwin_status[i] == DEAD)
		{
		    continue;
		}	
		if (XGetWindowAttributes(dpy, subwin[i], &subxwa) == 0)
		{
		    exit(0);
		}
		x = subxwa.x + (rand() % 5) - 2;
		y = subxwa.y + (rand() % 5) - 2;
		if ( x < 0 || y < 0 || (x+20) > topxwa.width || (y+20) > topxwa.height )
		{
		    XDestroyWindow(dpy, subwin[i]);
		    subwin_status[i] = DEAD;
		    done++;
		}
		else
		{
		    XMoveWindow(dpy, subwin[i], x, y);
		}
	    }
	}
	XNextEvent(dpy, &event);
    }
    XCloseDisplay(dpy);
    exit(1);
}
