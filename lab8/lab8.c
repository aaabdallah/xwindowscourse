/* Ahmed Abd-Allah, Lab 8 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/Scale.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/DrawingA.h>

#define TIMEOUT 50
Widget form, value, start, stop, quit, canvas, erase, slider, topLevel;
GC gc;
XGCValues xgcvalues;
Display *dpy;
int started = 0;
int screen, faces, position[5] = {0, 0, 0, 0, 0};
XtIntervalId timeout;
XtAppContext app_context;

void drawface(Window win, int x, int y)
{
    XFillArc(dpy, win, gc, x+3, y+3, 3, 3, 0, 360*64);
    XFillArc(dpy, win, gc, x+9, y+3, 3, 3, 0, 360*64);
    XDrawArc(dpy, win, gc, x+3, y+7, 9, 6, 0, -180*64);
    XDrawArc(dpy, win, gc, x, y, 15, 15, 0, 360*64);
}

void drawfaces(client_data, wid)
XtPointer client_data;
XtIntervalId *wid;
{
    Widget *wptr = (Widget *) client_data;
    Dimension height, width;
    int i;

    XtVaGetValues(*wptr, XmNheight, &height, 
		  XmNwidth, &width, NULL);
    XClearWindow(dpy, XtWindow(*wptr));

    for (i=0; i<faces; i++)
    {
	position[i] = (position[i] + 5) % ((int) height);
	drawface(XtWindow(*wptr), 
		 ((i+1) * width / (faces+1)) - 7, position[i]);
    }
    timeout = XtAppAddTimeOut(app_context, TIMEOUT, 
			      drawfaces, client_data);
}

void StartFaces(w, client_data, call_data)
Widget w;
XtPointer client_data, call_data;
{
    if (!started)
    {
	timeout = XtAppAddTimeOut(app_context, TIMEOUT, 
				  drawfaces, client_data);
	started = 1;
    }
}

void StopFaces(w, client_data, call_data)
Widget w;
XtPointer client_data, call_data;
{
    if (started)
    {
	XtRemoveTimeOut(timeout);
	started = 0;
    }
}

void Erase(w, client_data, call_data)
Widget w;
XtPointer client_data, call_data;
{ 
    Widget *wptr = (Widget *) client_data;
    XmString text;
    int i;
    Dimension width, height;

    if (started)
    {
	XtRemoveTimeOut(timeout);
	started = 0;
    }

    XtVaSetValues(slider,
		  XmNvalue, 1,
		  NULL);
    faces = 1;
    text = XmStringCreateLocalized("Number of Objects: 1");
    XtVaSetValues(value, 
		  XmNlabelString, text,
		  NULL);
    XmStringFree(text);

    for (i=0; i<5; i++)
    {
	position[i] = 0;
    }
    XtVaGetValues(*wptr, XmNheight, &height, 
		  XmNwidth, &width, NULL);
    XClearWindow(dpy, XtWindow(*wptr));
    drawface(XtWindow(*wptr), (width / 2) - 7, 0);
}

void Quit(w, client_data, call_data)
Widget w;
XtPointer client_data, call_data;
{ 
    exit(0);
}

void ExposeCanvas(w, client_data, call_data)
Widget w;
XtPointer client_data, call_data;
{
    int i;
    Dimension height, width;

    XtVaGetValues(w, XmNheight, &height, 
		  XmNwidth, &width, NULL);
    XClearWindow(dpy, XtWindow(w));
    for (i=0; i<faces; i++)
    {
	drawface(XtWindow(w), 
		 ((i+1) * width / (faces+1)) - 7, position[i]);
    }
}

void UpdateFaces(w, client_data, call_data)
Widget w;
XtPointer client_data;
XmScaleCallbackStruct *call_data;
{
    Widget *wptr = (Widget *) client_data;
    int i;
    Dimension height, width;
    char buff[80];
    XmString text;

    sprintf(buff, "Number of Objects: %d", call_data->value);
    faces = call_data->value;
    for (i=4; i>faces-1; i--)
	position[i] = 0;
    text = XmStringCreateLocalized(buff);
    XtVaSetValues(value, 
		  XmNlabelString, text,
		  NULL);
    XmStringFree(text);

    XtVaGetValues(*wptr, XmNheight, &height, 
		  XmNwidth, &width, NULL);
    XClearWindow(dpy, XtWindow(*wptr));
    for (i=0; i<faces; i++)
    {
	drawface(XtWindow(*wptr), 
		 ((i+1) * width / (faces+1)) - 7, position[i]);
    }
}

void DisplayNewValue(w, client_data, call_data)
Widget w;
XtPointer client_data;
XmScaleCallbackStruct *call_data;
{
    char buff[80];
    XmString text;
    int i;

    sprintf(buff, "Number of Objects: %d", call_data->value);
    faces = call_data->value;
    text = XmStringCreateLocalized(buff);
    XtVaSetValues(value, 
		  XmNlabelString, text,
		  NULL);
    XmStringFree(text);

    for (i=4; i>faces-1; i--)
	position[i] = 0;
}

main(argc, argv)
int argc;
char **argv;
{
    XmString text;
    unsigned long black, white;

    XtSetLanguageProc(NULL, (XtLanguageProc) NULL, NULL);

    topLevel = XtVaAppInitialize(
            &app_context,       /* Application context */
            "XLab8",            /* application class name */
            NULL, 0,            /* command line option list */
            &argc, argv,        /* command line args */
            NULL,               /* for missing app-defaults file */
            NULL);              /* terminate varargs list */

    form = XtVaCreateManagedWidget(
            "form",     /* widget name */
            xmFormWidgetClass,  /* widget class */
            topLevel,   /* parent widget*/
            NULL);  /* argument list*/

    text = XmStringCreateLocalized("Number of Objects: 1");

    value = XtVaCreateManagedWidget(
	    "value",  /* widget name   */
	    xmLabelWidgetClass,    /* widget class */
            form,   /* parent widget*/
	    XmNlabelString, text,
            NULL);  /* argument list*/

    XmStringFree(text);

    canvas = XtVaCreateManagedWidget(
            "canvas", /* widget name */
            xmDrawingAreaWidgetClass,    /* widget class */
            form,   /* parent widget*/
            NULL);  /* argument list*/
    start = XtVaCreateManagedWidget(
            "start", /* widget name */
            xmPushButtonWidgetClass,    /* widget class */
            form,   /* parent widget*/
            NULL);  /* argument list*/
    stop = XtVaCreateManagedWidget(
            "stop", /* widget name */
            xmPushButtonWidgetClass,    /* widget class */
            form,   /* parent widget*/
            NULL);  /* argument list*/
    erase = XtVaCreateManagedWidget(
            "erase", /* widget name */
            xmPushButtonWidgetClass,    /* widget class */
            form,   /* parent widget*/
            NULL);  /* argument list*/
    quit = XtVaCreateManagedWidget(
            "quit", /* widget name */
            xmPushButtonWidgetClass,    /* widget class */
            form,   /* parent widget*/
            NULL);  /* argument list*/

    slider = XtVaCreateManagedWidget(
            "slider", /* widget name */
            xmScaleWidgetClass,    /* widget class */
            form,   /* parent widget*/
            NULL);  /* argument list*/

    XtAddCallback(canvas, XmNexposeCallback, ExposeCanvas, 0);
    XtAddCallback(stop, XmNactivateCallback, StopFaces, canvas);
    XtAddCallback(start, XmNactivateCallback, StartFaces, canvas);
    XtAddCallback(erase, XmNactivateCallback, Erase, canvas);
    XtAddCallback(quit, XmNactivateCallback, Quit, 0);
    XtAddCallback(slider, XmNdragCallback, DisplayNewValue, 0);
    XtAddCallback(slider, XmNvalueChangedCallback, UpdateFaces, canvas);

    faces = 1;
    dpy = XtDisplay(canvas);
    screen = XDefaultScreen(dpy);
    black = BlackPixel(dpy, screen);
    white = WhitePixel(dpy, screen);
    xgcvalues.foreground = black;
    xgcvalues.background = white;
    gc = XtGetGC(canvas, GCForeground | GCBackground, &xgcvalues);

    XtRealizeWidget(topLevel);

    XtAppMainLoop(app_context);
}
