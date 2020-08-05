/* Ahmed Abd-Allah, lab 3 */

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
    Pixmap pix;			/* Parent window pixmap for face */
    Window dwin, fwin;		/* Children window IDs */
    Pixmap dpix, fpix;		/* Pixmaps used in children windows */
    unsigned long black, white;	/* Pixel values */
    unsigned int i;		/* Counter */
    int x, y;			/* Temporary positions */
    int pix_x, pix_y;		/* Parent window pixmap coords */
    XEvent event;		/* Event received */
    XWindowAttributes topxwa;	/* Parent window attributes */
    XWindowAttributes chxwa;	/* Child window attributes */
    int done;			/* Loop variable */
    XGCValues xgcvalues;	/* GC values */
    GC gc;			/* Normal GC */
    GC thickgc;			/* GC for thick lines */

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
			      0, 0, 600, 600,
			      0, black, white);
    if (XGetWindowAttributes(dpy, win, &topxwa) == 0)
    {
	exit(0);
    }
    else
    {
	if (topxwa.width != 600 || topxwa.height != 600)
	{
	    fprintf(stderr, "%s: Wrong Window Size\n", argv[0]);
	    exit(0);
	}
    }
    XSelectInput(dpy, win, ExposureMask | ButtonPressMask );

    /* Allocate normal GC */
    xgcvalues.foreground = black;
    xgcvalues.background = white;
    gc = XCreateGC(dpy, win, GCForeground | GCBackground,
		   &xgcvalues);

    /* Allocate thick line GC */
    xgcvalues.foreground = black;
    xgcvalues.background = white;
    xgcvalues.line_width = 3;
    thickgc = XCreateGC(dpy, win, GCLineWidth | GCForeground | GCBackground,
		   &xgcvalues);
    
    pix = XCreatePixmap(dpy, win, 100, 100, DefaultDepth(dpy,screen));
    ClearPixmap(dpy, pix, 100, 100, white);
    XDrawArc(dpy, pix, gc, 0, 0, 100, 100, 0, 360*64);
    XFillArc(dpy, pix, thickgc, 20, 20, 10, 10, 0, 360*64);
    XFillArc(dpy, pix, thickgc, 70, 20, 10, 10, 0, 360*64);
    XDrawArc(dpy, pix, gc, 20, 70, 60, 10, 0, -180*64);
    XCopyArea(dpy, pix, win, gc, 0, 0, 100, 100, 0, 0);
    pix_x = pix_y = 0;

    XMapWindow(dpy, win);

    /* Draw center box */
    XFillRectangle(dpy, win, gc, 100, 100, 400, 400);

    /* Create first child */
    dwin = XCreateSimpleWindow(dpy, win, 0, 0, 100, 100, 1, black, white);

    /* Create pixmap to put in child 1 using DRAW */
    dpix = XCreatePixmap(dpy, dwin, 100, 100, DefaultDepth(dpy,screen));
    ClearPixmap(dpy, dpix, 100, 100, white);
    XDrawArc(dpy, dpix, gc, 10, 10, 10, 10, 0, 360*64);
    XDrawArc(dpy, dpix, gc, 80, 10, 10, 10, 0, 360*64);
    XDrawArc(dpy, dpix, gc, 10, 80, 80, 10, 0, -180*64);

    /* Copy pixmap into window, map */
    XCopyArea(dpy, dpix, dwin, gc, 0, 0, 100, 100, 0, 0);
    XMapWindow(dpy, dwin);

    /* Create second child */
    fwin = XCreateSimpleWindow(dpy, win, 0, 0, 100, 100, 1, black, white);

    /* Create pixmap to put in child 2 using FILL */
    fpix = XCreatePixmap(dpy, fwin, 100, 100, DefaultDepth(dpy,screen));
    ClearPixmap(dpy, fpix, 100, 100, white);
    XFillArc(dpy, fpix, thickgc, 10, 10, 10, 10, 0, 360*64);
    XFillArc(dpy, fpix, thickgc, 80, 10, 10, 10, 0, 360*64);
    XDrawArc(dpy, fpix, thickgc, 10, 80, 80, 10, 0, -180*64);

    /* Copy pixmap into window, map */
    XCopyArea(dpy, fpix, fwin, gc, 0, 0, 100, 100, 0, 0);
    XMapWindow(dpy, fwin);

    done = 0;
    while ( !done ) 
    {
	while ( !XEventsQueued(dpy, QueuedAfterReading) )
	{
	    XFlush(dpy);

	    /* Move first window slowly (mod 5) */
	    if (XGetWindowAttributes(dpy, dwin, &chxwa) == 0)
	    {
		exit(0);
	    }
	    if (!GetNewCoordinates(chxwa.x, chxwa.y, &x, &y, 5))
	    {
		done=1;
		break;
	    }
	    XMoveWindow(dpy, dwin, x, y);

	    /* Move second window faster (mod 10) */
	    if (XGetWindowAttributes(dpy, fwin, &chxwa) == 0)
	    {
		exit(0);
	    }
	    if (!GetNewCoordinates(chxwa.x, chxwa.y, &x, &y, 10))
	    {
		done=1;
		break;
	    }
	    XMoveWindow(dpy, fwin, x, y);

	    /* Clear drawn face */
	    XClearArea(dpy, win, pix_x, pix_y, 100, 100, True);
	    /* Move it fastest of all (mod 15) */
	    if (!GetNewCoordinates(pix_x, pix_y, &x, &y, 15))
	    {
		done=1;
		break;
	    }
	    pix_x = x;
	    pix_y = y;

	    /* Update all three faces */
	    XCopyArea(dpy, dpix, dwin, gc, 0, 0, 100, 100, 0, 0);
	    XCopyArea(dpy, fpix, fwin, gc, 0, 0, 100, 100, 0, 0);
	    XCopyArea(dpy, pix, win, gc, 0, 0, 100, 100, pix_x, pix_y);
	}
	if (!done)
	{
	    XNextEvent(dpy, &event);
	    switch ( event.type )
	    {
	      case Expose:
		if (event.xexpose.count != 0)
		    break;
		/* Draw center box */
		XFillRectangle(dpy, win, gc, 100, 100, 400, 400);
		break;
	      case ButtonPress:
		done = 1;
		break;
	      default:
		break;
	    }
	}
    }
    XFreeGC(dpy, gc);
    XFreeGC(dpy, thickgc);
    XFreePixmap(dpy, pix);
    XFreePixmap(dpy, dpix);
    XFreePixmap(dpy, fpix);
    XCloseDisplay(dpy);
    exit(1);
}

int GetNewCoordinates(int oldx, int oldy, int *newx, int *newy, 
		      unsigned int maxincrement)
{
    if (oldx==0 && oldy<500)
    {
	*newx = 0;
	*newy = oldy + rand() % maxincrement;
	if (*newy > 500)
	    *newy = 500;
    }
    else if (oldx<500 && oldy==500)
    {
	*newy = 500;
	*newx = oldx + rand() % maxincrement;
	if (*newx > 500)
	    *newx = 500;
    }
    else if (oldx==500 && oldy>0)
    {
	*newx = 500;
	*newy = oldy - rand() % maxincrement;
	if (*newy < 0)
	    *newy = 0;
    }
    else if (oldx>0 && oldy==0)
    {
	*newy = 0;
	*newx = oldx - rand() % maxincrement;
	if (*newx <= 0)
	{
	    *newx = 0;
	    return 0;
	}
    }
    return 1;
}

int ClearPixmap(Display *dpy,Pixmap pix, unsigned int width, 
		unsigned int height, unsigned long color)
{
    GC gc;
    XGCValues xgcvalues;

    xgcvalues.foreground = color;
    gc = XCreateGC(dpy, pix, GCForeground, &xgcvalues);
    XFillRectangle(dpy, pix, gc, 0, 0, width, height);
    XFreeGC( dpy, gc );
    return 1;
}
