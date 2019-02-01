#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void process_path(int *);
void get_coords(int *, int *);
void click(Display *);
void move_mouse(Display *, int, int);
void release(Display *);
