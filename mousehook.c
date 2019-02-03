#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void process_path(int *);
void get_coords(int *x, int *y);
void click(Display *dpy);
void move_mouse(Display *dpy, int x, int y);
void release(Display *dpy);

struct node_coordinates {
	int x;
	int y;
};

struct node_coordinates grid_nc[16] = {
	{120, 385},
	{195, 385},
	{270, 385},
	{345, 385},

	{120, 460},
	{195, 460},
	{270, 460},
	{345, 460},

	{120, 535},
	{195, 535},
	{270, 535},
	{345, 535},

	{120, 610},
	{195, 610},
	{270, 610},
	{345, 610},
};

void process_path(int *path)
{
    	Display *dpy = XOpenDisplay(NULL);

        printf("%d ", path[0]);

	int i = 0;
	move_mouse(dpy, 
		grid_nc[path[i] - 1].x, grid_nc[path[i] - 1].y);

	click(dpy);

	++i;

	while (path[i] != 0) {
	move_mouse(dpy, 
		grid_nc[path[i] - 1].x, grid_nc[path[i] - 1].y);

		printf("%d ", path[i++]);
	}

	release(dpy);


	XCloseDisplay(dpy);
}

void get_coords(int *x, int *y)
{
    	Display *dpy = XOpenDisplay(NULL);
	XEvent event;
    	int button;
    	Window root= XDefaultRootWindow(dpy);

    	XGrabPointer(dpy, root, False, ButtonPressMask, 
	GrabModeAsync, GrabModeAsync, None, None, CurrentTime);

    	XSelectInput(dpy, root, ButtonPressMask) ;

	XNextEvent(dpy, &event);
	switch(event.type){
	case ButtonPress:
		switch(event.xbutton.button){
		default:
		*x = event.xbutton.x;
		*y = event.xbutton.y;
		button=Button1;
		break;
		}
		break;
	    default:
		break;
	}

	if(button == Button1)
		printf("click at %d %d \n", *x, *y);

	XCloseDisplay(dpy);
}

void move_mouse(Display *dpy, int x, int y)
{
	Window root = XRootWindow(dpy, 0);
    	XWarpPointer(dpy, None, root, 0, 0, 0, 0, x, y);
	XFlush(dpy);
	usleep(250000);
}

void click(Display *dpy) 
{
	XEvent event;
	memset(&event, 0, sizeof(event));
	event.xbutton.button = Button1;
	event.xbutton.same_screen = True;
	event.xbutton.subwindow = DefaultRootWindow(dpy);

	while (event.xbutton.subwindow) {
	      event.xbutton.window = event.xbutton.subwindow;

	      XQueryPointer(dpy, event.xbutton.window,
		     &event.xbutton.root, &event.xbutton.subwindow,
		     &event.xbutton.x_root, &event.xbutton.y_root,
		     &event.xbutton.x, &event.xbutton.y,
		     &event.xbutton.state);
	}

	  event.type = ButtonPress;
	  if (XSendEvent(dpy, PointerWindow, True, ButtonPressMask, &event) == 0)
	    fprintf(stderr, "Error to send the event!\n");

	  XFlush(dpy);
	  usleep(500000);
}

void release(Display *dpy)
{
	XEvent event;
	memset(&event, 0, sizeof(event));
	event.xbutton.button = Button1;
	event.xbutton.same_screen = True;
	event.xbutton.subwindow = DefaultRootWindow(dpy);

	while (event.xbutton.subwindow) {
	      event.xbutton.window = event.xbutton.subwindow;

	      XQueryPointer(dpy, event.xbutton.window,
		     &event.xbutton.root, &event.xbutton.subwindow,
		     &event.xbutton.x_root, &event.xbutton.y_root,
		     &event.xbutton.x, &event.xbutton.y,
		     &event.xbutton.state);
	}
	  event.type = ButtonRelease;
	  if (XSendEvent(dpy, PointerWindow, True, ButtonReleaseMask, &event) == 0)
	    fprintf (stderr, "Error to send the event!\n");

	  XFlush (dpy);
	  usleep(500000);
}
