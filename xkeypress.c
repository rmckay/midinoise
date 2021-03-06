//
// Created by rm on 30/12/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

Display *dis;
Window win;
XEvent report;

int main ()
{
    dis = XOpenDisplay (NULL);
    // XAutoRepeatOn(dis);
    win = XCreateSimpleWindow (dis, RootWindow (dis, 0), 1, 1, 430, 100,
                               0, BlackPixel (dis, 0), BlackPixel (dis, 0));
    XSelectInput (dis, win, KeyPressMask | KeyReleaseMask);
    XMapWindow (dis, win);
    XFlush (dis);
    XColor red, brown, blue, yellow, green;

    XVisualInfo vinfo;
    Colormap screen_colormap;
    XGCValues gcv = {0};
    XSetWindowAttributes attr = {0};
    gcv.foreground = 255;
    gcv.line_width = 1;
    gcv.line_style = LineSolid;
    gcv.cap_style = CapButt;
    gcv.join_style = JoinBevel;
    screen_colormap = DefaultColormap(dis, DefaultScreen(dis));

    XAllocNamedColor(dis, screen_colormap, "#ff0000", &red, &red);
    int screen = DefaultScreen(dis);

    GC gctx = XCreateGC(dis, win, 0, &gcv);
    XSetFillStyle(dis, gctx, FillSolid);

    XSetForeground(dis, gctx, WhitePixel(dis, screen));
    XSetBackground(dis, gctx, BlackPixel(dis, screen));
    XSetFillStyle(dis, gctx, FillSolid);
    //XSetLineAttributes(dis, gctx, 2, LineSolid, CapButt, JoinBevel);

    //XDrawPoint(dis, win, gctx, 5, 5);

    char whitekeys[] = "QWERTYUIOPZXCVBNMXXXXXXXXXXXXXXXX";
    char *w = &whitekeys[0];
    for(int i=0;i<17;i++) {
        int offset = i*25;
        XSetForeground(dis, gctx, WhitePixel(dis, screen));
        XFillRectangle(dis, win, gctx, offset, 0, 24, 100);
        XSetForeground(dis, gctx, BlackPixel(dis, screen));
        char buf[3] = {0};
        strncpy(buf, (w++), 1);
        XDrawString (dis, win, gctx, offset + 10, 80, buf, 1);
    }


    char blackkeys[] = "2356790SDFHJ";
    char *c = &blackkeys[0];
    for(int i=0;i<16;i++) {
        int pos = i%8;
        int offset = i*25;
        if ((pos!=2)&&(pos!=6))
        {
            XSetForeground(dis, gctx, BlackPixel(dis, screen));
            XFillRectangle(dis, win, gctx, offset + 18, 0, 12, 60);
            XSetForeground(dis, gctx, WhitePixel(dis, screen));
            char buf[3] = {0};
            strncpy(buf, (c++), 1);
            XDrawString (dis, win, gctx, offset + 21, 20, buf, 1);
        }
    }






   // XFlush(dis);

    XColor color;
   // attr.colormap = XCreateColormap(dis, DefaultRootWindow(dis), vinfo.visual, AllocNone);
    char orangeDark[] = "#FFFFFF";
   // XParseColor(dis, attr.colormap, orangeDark, &color);
   // XAllocColor(dis, attr.colormap, &color);




    //XDrawRectangle(dis, win, gctx, 0, 0, 50, 50);

    while (1)
    {
        //XDrawPoint(dis, win, gctx, 5, 5);
        XNextEvent (dis, &report);
        switch (report.type)
        {
            case KeyPress:
                fprintf (stdout, "key #%ld was pressed.\n",
                         (long) XLookupKeysym (&report.xkey, 0));
                break;
            case KeyRelease:
            {
                unsigned short is_retriggered = 0;

                if (XEventsQueued(dis, QueuedAfterReading))
                {
                    XEvent nev;
                    XPeekEvent(dis, &nev);

                    if (nev.type == KeyPress && nev.xkey.time == report.xkey.time &&
                        nev.xkey.keycode == report.xkey.keycode)
                    {
                        fprintf (stdout, "key #%ld was retriggered.\n",
                                 (long) XLookupKeysym (&nev.xkey, 0));

                        // delete retriggered KeyPress event
                        XNextEvent (dis, &report);
                        is_retriggered = 1;
                    }
                }

                if (!is_retriggered)
                    fprintf (stdout, "key #%ld was released.\n",
                             (long) XLookupKeysym (&report.xkey, 0));
            }
                break;
        }
    }

    return (0);
}