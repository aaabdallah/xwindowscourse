/* Ahmed Abd-Allah, partial lab 4: the sun/moon does not move */

#include <stdio.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

unsigned long GetColor(Display *dpy, int screen, Colormap cmap, char *cstring);

#define MIN_WIDTH	300
#define MIN_HEIGHT	300
#define COLORS		6
#define SHADES		6	/* Will only use 5 shades however */
#define DAY		0
#define NIGHT		1

main(argc,argv)
int argc;
char **argv;
{
    Display *dpy;		/* X server connection */
    int screen;			/* Screen ID */
    Window win;			/* Window ID */
    Window clockwin;		/* Window for displaying time */
    Window colorwin[COLORS];	/* Windows for choosing colors */
    Window shadewin[SHADES];	/* Windows for choosing shades (use 5 only) */
    Window sunmoonwin;		/* Window for displaying moon and sun */
    Pixmap pix[SHADES];		/* Pixmaps for shades */
    Pixmap newpixmap;
    unsigned long black, white;	/* Pixel values */
    unsigned long c[COLORS];	/* Pixel values for choice of colors */
    unsigned int i;		/* Counter */
    int x, y;			/* Temporary positions */
    XEvent event;		/* Event received */
    XWindowAttributes topxwa;	/* Parent window attributes */
    XWindowAttributes clockxwa;	/* Clock window attributes */
    XWindowAttributes shadexwa;	/* Shade window attributes */
    int done;			/* Loop variable */
    Colormap cmap;		/* Colormap */
    XGCValues xgcvalues;	/* GC values */
    GC gc;			/* Generic GC */
    GC sungc, groundgc;		/* Sun, Moon, and (day) ground GC's */
    GC stipplegc;		/* GC for drawing in stipples */
    time_t t;			/* Time as long int */
    char timestr[6], ot[6];	/* Time in terms of a string */
    char datestr[11], od[11];	/* Date in terms of a string */
    int dn;			/* boolean for day or night */
    XPoint pts[6];		/* Points for the polygon */
    char *fontname = "*fixed*bold*15*"; /* Font */
    XFontStruct *fs;

    /* Open the display using the $DISPLAY environment variable to locate
       the X server.  */
    if ((dpy = XOpenDisplay(NULL)) == NULL) {
	fprintf(stderr, "%s: can't open %s\n", argv[0], XDisplayName(NULL));
	exit(1);
    }

    /*XSynchronize(dpy, True);*/

    screen = XDefaultScreen(dpy);
    black = BlackPixel(dpy, screen);
    white = WhitePixel(dpy, screen);
    cmap = DefaultColormap(dpy, screen);

    c[0] = GetColor(dpy, screen, cmap, "red");
    c[1] = GetColor(dpy, screen, cmap, "green");
    c[2] = GetColor(dpy, screen, cmap, "blue");
    c[3] = GetColor(dpy, screen, cmap, "cyan");
    c[4] = GetColor(dpy, screen, cmap, "magenta");
    c[5] = GetColor(dpy, screen, cmap, "yellow");

    fs = XLoadQueryFont(dpy, fontname);
    win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
			      0, 0, MIN_WIDTH, MIN_HEIGHT,
			      0, black, white);
    if (XGetWindowAttributes(dpy, win, &topxwa) == 0)
    {
	exit(0);
    }
    else
    {
	if (topxwa.width < MIN_WIDTH || topxwa.height < MIN_HEIGHT)
	{
	    fprintf(stderr, "%s: Window too small\n", argv[0]);
	    exit(0);
	}
    }
    XSelectInput(dpy, win, ExposureMask | ButtonPressMask | 
		 StructureNotifyMask);

    dn = get_time(timestr, datestr);
    od[0] = ot[0] = '\0';

    /* Allocate normal GC */
    xgcvalues.foreground = black;
    xgcvalues.background = white;
    xgcvalues.font = fs->fid;
    gc = XCreateGC(dpy, win, GCForeground | GCBackground | GCFont,
		   &xgcvalues);


    clockwin = XCreateSimpleWindow(dpy, win, 10, 10,
				   2 * topxwa.width / 3, 
				   topxwa.height - 20, 
				   2, black, white);
    XMapWindow(dpy, clockwin);

    if (XGetWindowAttributes(dpy, win, &clockxwa) == 0)
    {
	exit(0);
    }
    sunmoonwin = XCreateSimpleWindow(dpy, clockwin, 10, 10,
				     clockxwa.width / 6,
				     clockxwa.height / 6,
				     0, black, white);
    XMapWindow(dpy, sunmoonwin);

    for (i=0; i<3; i++)
    {
	colorwin[i] = XCreateSimpleWindow(dpy, win,
					  (2*topxwa.width/3) + 20 +
					      (i*topxwa.width/12), 
					  10,
					  topxwa.width/12,
					  topxwa.height/12,
					  1, black, c[i]);
	XMapWindow(dpy, colorwin[i]);
	colorwin[i+3] = XCreateSimpleWindow(dpy, win,
					    (2*topxwa.width/3) + 20 +
					       (i*topxwa.width/12),
					    10 + topxwa.height/12,
					    topxwa.width/12,
					    topxwa.height/12,
					    1, black, c[i+3]);
	XMapWindow(dpy, colorwin[i+3]);
    }
    shadewin[0] = XCreateSimpleWindow(dpy, win,
				      (2*topxwa.width/3) + 20,
				      10 + topxwa.height/2,
				      topxwa.width/12,
				      topxwa.height/12,
				      1, black, white);
    XMapWindow(dpy, shadewin[0]);
    shadewin[1] = XCreateSimpleWindow(dpy, win,
				      (2*topxwa.width/3) + 20 +
				      (topxwa.width/12), 
				      10 + topxwa.height/2,
				      topxwa.width/12,
				      topxwa.height/12,
				      1, black, white);
    XMapWindow(dpy, shadewin[1]);
    shadewin[2] = XCreateSimpleWindow(dpy, win,
				      (2*topxwa.width/3) + 20 +
				      (2*topxwa.width/12), 
				      10 + topxwa.height/2,
				      topxwa.width/12,
				      topxwa.height/12,
				      1, black, white);
    XMapWindow(dpy, shadewin[2]);
    shadewin[3] = XCreateSimpleWindow(dpy, win,
				      (2*topxwa.width/3) + 20,
				      10 + 7*topxwa.height/12,
				      topxwa.width/12,
				      topxwa.height/12,
				      1, black, white);
    XMapWindow(dpy, shadewin[3]);
    shadewin[5] = XCreateSimpleWindow(dpy, win,
				      (2*topxwa.width/3) + 20 +
				      (2*topxwa.width/12), 
				      10 + 7*topxwa.height/12,
				      topxwa.width/12,
				      topxwa.height/12,
				      1, black, black);
    XMapWindow(dpy, shadewin[5]);

    /* Create a 25% shaded stipple */
    pix[0] = XCreatePixmap(dpy, win, 2, 2, 1);
    ClearPixmap(dpy, pix[0], 2, 2, white);
    xgcvalues.foreground = black;
    xgcvalues.background = white;
    stipplegc = XCreateGC(dpy, pix[0], 
			  GCBackground | GCForeground,
			  &xgcvalues);
    XDrawPoint(dpy, pix[0], stipplegc, 0, 0);

    /* Create a 50% shaded stipple */
    pix[1] = XCreatePixmap(dpy, win, 2, 2, 1);
    ClearPixmap(dpy, pix[1], 2, 2, white);
    XDrawPoint(dpy, pix[1], stipplegc, 0, 0);
    XDrawPoint(dpy, pix[1], stipplegc, 1, 1);

    /* Create a 75% shaded stipple */
    pix[2] = XCreatePixmap(dpy, win, 2, 2, 1);
    ClearPixmap(dpy, pix[2], 2, 2, white);
    XDrawPoint(dpy, pix[2], stipplegc, 0, 0);
    XDrawPoint(dpy, pix[2], stipplegc, 0, 1);
    XDrawPoint(dpy, pix[2], stipplegc, 1, 1);

    /* Allocate sun GC */
    xgcvalues.background = c[5];
    xgcvalues.foreground = black;
    xgcvalues.fill_style = FillOpaqueStippled;
    xgcvalues.stipple = pix[0];
    sungc = XCreateGC(dpy, win, 
		      GCForeground | GCBackground | GCFillStyle | GCStipple,
		      &xgcvalues);

    /* Allocate ground GC */
    xgcvalues.foreground = c[1];
    xgcvalues.background = black;
    xgcvalues.fill_style = FillOpaqueStippled;
    xgcvalues.stipple = pix[2];
    groundgc = XCreateGC(dpy, win, 
			 GCForeground | GCBackground | GCFillStyle | GCStipple,
			 &xgcvalues);

    XMapWindow(dpy, win);

    done = 0;
    while ( !done ) 
    {
	while ( !XEventsQueued(dpy, QueuedAfterReading) )
	{
	    XFlush(dpy);
	    strcpy(ot, timestr);
	    strcpy(od, datestr);
	    dn = get_time(timestr, datestr);
	    if (dn == DAY)
	    {
		XSetWindowBackground(dpy, clockwin, c[3]);
		XSetWindowBackground(dpy, sunmoonwin, c[3]);
	    }
	    else
	    {
		XSetWindowBackground(dpy, clockwin, c[2]);
		XSetWindowBackground(dpy, sunmoonwin, c[2]);
	    }
	    if ( strcmp(timestr, ot) || strcmp(datestr, od) )
	    {
		/* Clear Ground */
		XFillPolygon(dpy, clockwin, groundgc, pts, 6, 
			     Nonconvex, CoordModeOrigin);

		/* Display Time */
		xgcvalues.foreground = white;
		xgcvalues.fill_style = FillSolid;
		XChangeGC(dpy, gc,
			  GCForeground | GCFillStyle,
			  &xgcvalues);
		XDrawString(dpy, clockwin, gc,
			    1 * clockxwa.width / 3,
			    7 * clockxwa.height / 8,
			    timestr, strlen(timestr) );
		XDrawString(dpy, clockwin, gc,
			    1 * clockxwa.width / 3,
			    (7 * clockxwa.height / 8) +
			    fs->max_bounds.ascent +
			    fs->max_bounds.descent,
			    datestr, strlen(datestr) );
	    }
	}
	if (!done)
	{
	    XNextEvent(dpy, &event);
	    switch ( event.type )
	    {
	      case Expose:
		if (event.xexpose.count != 0)
		    break;
		/* draw sun/moon and ground */
		if ( dn == DAY )
		{
		    xgcvalues.background = c[5];
		    xgcvalues.foreground = black;
		    xgcvalues.fill_style = FillOpaqueStippled;
		    xgcvalues.stipple = pix[0];
		    XChangeGC(dpy, sungc,
			      GCForeground | GCBackground | 
			      GCFillStyle | GCStipple,
			      &xgcvalues);
		    XFillArc(dpy, sunmoonwin, 
			     sungc, 0, 0, 
			     clockxwa.width / 6,
			     clockxwa.height / 6,
			     0, 360*64);
		    xgcvalues.background = c[1];
		    xgcvalues.foreground = black;
		    xgcvalues.fill_style = FillOpaqueStippled;
		    xgcvalues.stipple = pix[2];
		    XChangeGC(dpy, groundgc,
			      GCForeground | GCBackground | 
			      GCFillStyle | GCStipple,
			      &xgcvalues);
		    XFillPolygon(dpy, clockwin, groundgc, pts, 6, 
				 Nonconvex, CoordModeOrigin);
		}
		else
		{
		    xgcvalues.foreground = white;
		    xgcvalues.fill_style = FillSolid;
		    XChangeGC(dpy, sungc,
			      GCForeground | GCFillStyle,
			      &xgcvalues);
		    XFillArc(dpy, sunmoonwin, 
			     sungc, 0, 0, 
			     clockxwa.width / 6,
			     clockxwa.height / 6,
			     0, 360*64);
		    xgcvalues.foreground = black;
		    xgcvalues.fill_style = FillSolid;
		    XChangeGC(dpy, groundgc,
			      GCForeground | GCFillStyle,
			      &xgcvalues);
		    XFillPolygon(dpy, clockwin, groundgc, pts, 6, 
				 Nonconvex, CoordModeOrigin);
		}

		/* Draw shades */
		for (i=0; i<3; i++)
		{
		    if (XGetWindowAttributes(dpy, shadewin[i], &shadexwa) == 0)
		    {
			exit(0);
		    }
		    xgcvalues.foreground = black;
		    xgcvalues.background = white;
		    xgcvalues.fill_style = FillOpaqueStippled;
		    xgcvalues.stipple = pix[i];
		    XChangeGC(dpy, gc,
			      GCForeground | GCBackground | 
			      GCFillStyle | GCStipple,
			      &xgcvalues);
		    XFillRectangle(dpy, shadewin[i], gc, 0, 0,
				   shadexwa.width, shadexwa.height);
		}

		/* Display Time */
		xgcvalues.foreground = white;
		xgcvalues.fill_style = FillSolid;
		XChangeGC(dpy, gc,
			  GCForeground | GCFillStyle,
			  &xgcvalues);
		XDrawString(dpy, clockwin, gc,
			    1 * clockxwa.width / 3,
			    7 * clockxwa.height / 8,
			    timestr, strlen(timestr) );
		XDrawString(dpy, clockwin, gc,
			    1 * clockxwa.width / 3,
			    (7 * clockxwa.height / 8) +
			    fs->max_bounds.ascent +
			    fs->max_bounds.descent,
			    datestr, strlen(datestr) );
		break;
	      case ButtonPress:
		done = 1;
		break;
	      case ConfigureNotify:
		topxwa.width = event.xconfigure.width;
		topxwa.height = event.xconfigure.height;
		if (topxwa.width < MIN_WIDTH || topxwa.height < MIN_HEIGHT)
		{
		    fprintf(stderr, "%s: Window too small\n", argv[0]);
		    exit(0);
		}
		XResizeWindow(dpy, clockwin, 
			      2 * topxwa.width / 3, topxwa.height - 20);
		if (XGetWindowAttributes(dpy, clockwin, &clockxwa) == 0)
		{
		    exit(0);
		}
		pts[0].x = 0;
		pts[0].y = clockxwa.height;
		pts[1].x = 0;
		pts[1].y = 5 * clockxwa.height / 6;
		pts[2].x = clockxwa.width / 3;
		pts[2].y = 3 * clockxwa.height / 4;
		pts[3].x = 2 * clockxwa.width / 3;
		pts[3].y = 5 * clockxwa.height / 6;
		pts[4].x = clockxwa.width;
		pts[4].y = 3 * clockxwa.height / 4;
		pts[5].x = clockxwa.width;
		pts[5].y = clockxwa.height;

		XMoveResizeWindow(dpy, sunmoonwin,
				  10, 10,
				  clockxwa.width / 6,
				  clockxwa.height / 6);
		for (i=0; i<3; i++)
		{
		    XMoveResizeWindow(dpy, colorwin[i], 
				      (2*topxwa.width/3) + 20 +
				         (i*topxwa.width/12), 
				      10,
				      topxwa.width/12,
				      topxwa.height/12);
		    XMoveResizeWindow(dpy, colorwin[i+3], 
				      (2*topxwa.width/3) + 20 +
				         (i*topxwa.width/12), 
				      10 + topxwa.height/12,
				      topxwa.width/12,
				      topxwa.height/12);
		}
		XMoveResizeWindow(dpy, shadewin[0],
				      (2*topxwa.width/3) + 20,
				      10 + topxwa.height/2,
				      topxwa.width/12,
				      topxwa.height/12);
		XMoveResizeWindow(dpy, shadewin[1],
				      (2*topxwa.width/3) + 20 +
				      (topxwa.width/12), 
				      10 + topxwa.height/2,
				      topxwa.width/12,
				      topxwa.height/12);
		XMoveResizeWindow(dpy, shadewin[2],
				      (2*topxwa.width/3) + 20 +
				      (2*topxwa.width/12), 
				      10 + topxwa.height/2,
				      topxwa.width/12,
				      topxwa.height/12);
		XMoveResizeWindow(dpy, shadewin[3],
				      (2*topxwa.width/3) + 20,
				      10 + 7*topxwa.height/12,
				      topxwa.width/12,
				      topxwa.height/12);
		XMoveResizeWindow(dpy, shadewin[5],
				      (2*topxwa.width/3) + 20 +
				      (2*topxwa.width/12), 
				      10 + 7*topxwa.height/12,
				      topxwa.width/12,
				      topxwa.height/12);
		break;
	      default:
		break;
	    }
	}
    }
    XFreeGC(dpy, gc);
    XFreeGC(dpy, sungc);
    XFreeGC(dpy, groundgc);
    XCloseDisplay(dpy);
    exit(1);
}

unsigned long GetColor(Display *dpy, int screen, Colormap cmap, char *cstring)
{
    XColor actual, exact;

    if ( !XAllocNamedColor(dpy, cmap, cstring, &actual, &exact) )
	return WhitePixel(dpy, screen);
    else
	return actual.pixel;
}


int ClearPixmap(Display *dpy,Pixmap pix, unsigned int width, 
		unsigned int height, unsigned long color)
{
    GC gc;
    XGCValues xgcvalues;

    xgcvalues.foreground = color;
    gc = XCreateGC(dpy, pix, GCForeground, &xgcvalues);
    XFillRectangle(dpy, pix, gc, 0, 0, width, height);
    XFreeGC(dpy, gc);
    return 1;
}

int get_time(char *timestr, char *datestr)
{
    char buf[26];
    time_t t;
    int hour;

    t = time(&t);
    strcpy(buf, ctime(&t));
    strncpy(timestr, &buf[11], 5);
    timestr[5] = '\0';
    strncpy(datestr, buf, 10);
    datestr[10] = '\0';
    hour = (timestr[0]-'0') * 10 + timestr[1] - '0';

    if ( (hour <= 5) || (hour >=18) )
    {
	return NIGHT;
    }
    else
    {
	return DAY;
    }
}
