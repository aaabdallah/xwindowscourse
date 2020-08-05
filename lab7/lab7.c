/* Ahmed Abd-Allah, Lab 7 */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/Scale.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>

Widget form, value, reset, slider, topLevel;

void Reset(w, client_data, call_data)
Widget w;
XtPointer client_data, call_data;
{ 
    XmString text;

    XtVaSetValues(slider,
		  XmNvalue, 1,
		  NULL);
    text = XmStringCreateLocalized("Value: 1");
    XtVaSetValues(value, 
		  XmNlabelString, text,
		  NULL);
    XmStringFree(text);
}

void DisplayNewValue(w, client_data, call_data)
Widget w;
XtPointer client_data;
XmScaleCallbackStruct *call_data;
{
    char buff[80];
    XmString text;

    sprintf(buff, "Value: %d", call_data->value);
    text = XmStringCreateLocalized(buff);
    XtVaSetValues(value, 
		  XmNlabelString, text,
		  NULL);
    XmStringFree(text);
}

main(argc, argv)
int argc;
char **argv;
{
    XtAppContext app_context;
    XmString text;

    XtSetLanguageProc(NULL, (XtLanguageProc) NULL, NULL);

    topLevel = XtVaAppInitialize(
            &app_context,       /* Application context */
            "XLab7",            /* application class name */
            NULL, 0,            /* command line option list */
            &argc, argv,        /* command line args */
            NULL,               /* for missing app-defaults file */
            NULL);              /* terminate varargs list */

    form = XtVaCreateManagedWidget(
            "form",     /* widget name */
            xmFormWidgetClass,  /* widget class */
            topLevel,   /* parent widget*/
            NULL);  /* argument list*/

    text = XmStringCreateLocalized("Value: 1");

    value = XtVaCreateManagedWidget(
	    "value",  /* widget name   */
	    xmLabelWidgetClass,    /* widget class */
            form,   /* parent widget*/
	    XmNlabelString, text,
            NULL);  /* argument list*/

    XmStringFree(text);

    reset = XtVaCreateManagedWidget(
            "reset", /* widget name */
            xmPushButtonWidgetClass,    /* widget class */
            form,   /* parent widget*/
            NULL);  /* argument list*/

    slider = XtVaCreateManagedWidget(
            "slider", /* widget name */
            xmScaleWidgetClass,    /* widget class */
            form,   /* parent widget*/
            NULL);  /* argument list*/

    XtAddCallback(reset, XmNactivateCallback, Reset, 0);
    XtAddCallback(slider, XmNvalueChangedCallback, DisplayNewValue, 0);

    XtRealizeWidget(topLevel);

    XtAppMainLoop(app_context);
}
