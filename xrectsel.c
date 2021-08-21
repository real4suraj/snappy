#include <X11/Xlib.h>
//#include <X11/Xresource.h>
#include <X11/cursorfont.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  Display *dpy;
  XEvent ev;
  Cursor c;

  GC sel_gc;
  XGCValues sel_gv;

  int x, y, i;
  int start_x, start_y, width, height;

  if (!(dpy = XOpenDisplay(NULL))) {
    fprintf(stderr, "Could not open display %s", getenv("DISPLAY"));
  }

  c = XCreateFontCursor(dpy, XC_cross);
  XDefineCursor(dpy, DefaultRootWindow(dpy), c);

  XGrabButton(dpy, 1, 0, DefaultRootWindow(dpy), True, ButtonPressMask,
              GrabModeSync, GrabModeAsync, None, None);

  sel_gv.function = GXinvert;
  sel_gv.subwindow_mode = IncludeInferiors;
  sel_gv.line_width = 1;
  sel_gc = XCreateGC(dpy, DefaultRootWindow(dpy),
                     GCFunction | GCSubwindowMode | GCLineWidth, &sel_gv);

  for (;;) {
    XNextEvent(dpy, &ev);

    if (ev.type == ButtonPress) {
      /* Grab pointer so we receive ButtonRelease */
      XGrabPointer(dpy, DefaultRootWindow(dpy), True,
                   PointerMotionMask | ButtonReleaseMask, GrabModeAsync,
                   GrabModeAsync, None, c, CurrentTime);
      x = start_x = ev.xbutton.x_root;
      y = start_y = ev.xbutton.y_root;

      width = height = 0;

      // XAllowEvents(dpy, ReplayPointer, CurrentTime); /* Allow other apps to
      // process this message */
    } else if (ev.type == MotionNotify) {
      while (XCheckTypedEvent(dpy, MotionNotify, &ev))
        ;

      XDrawRectangle(dpy, DefaultRootWindow(dpy), sel_gc, x, y, width,
                     height); /* Clear Rectangle */

      width = ev.xbutton.x_root - start_x;
      height = ev.xbutton.y_root - start_y;

      /* Ugliness to make width/height positive and put the start positions
       * in the right place so we can draw backwards basically. */
      if (width < 0) {
        width = abs(width);
        x = ev.xbutton.x_root;
      } else {
        x = start_x;
      }
      if (height < 0) {
        height = abs(height);
        y = ev.xbutton.y_root;
      } else {
        y = start_y;
      }

      XDrawRectangle(dpy, DefaultRootWindow(dpy), sel_gc, x, y, width,
                     height); /* Draw Rectangle */
    } else if (ev.type == ButtonRelease) {
      XUngrabPointer(dpy, CurrentTime);

      XDrawRectangle(dpy, DefaultRootWindow(dpy), sel_gc, x, y, width,
                     height); /* Clear Rectangle */

      XSync(dpy,
            1); /* Needed to Ungrab Pointer NOW otherwise we continue receiving
                   MotionNotify events and the previous line is useless */

      printf("-video_size %ix%i -i :0.0+%i,%i\n", width, height, x, y);
      break;
    }
  }

  XFreeCursor(dpy, c);

  c = XCreateFontCursor(dpy, XC_left_ptr);
  XDefineCursor(dpy, DefaultRootWindow(dpy), c);
  XFreeCursor(dpy, c);

  XFreeGC(dpy, sel_gc);

  XCloseDisplay(dpy);

  return 0;
}
