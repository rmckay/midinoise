//
// Created by rm on 30/12/20.
//

// USES KEYBOARD KEY TO RESIZE A WINDOW

// Compile : gcc -o go key_and_win.c -lX11

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    Display *d;
    Window window;
    XEvent event, ev;
    int s;

    /* open connection with the server */
    d = XOpenDisplay(NULL);
    if (d == NULL)
    {
        fprintf(stderr, "Cannot open d\n");
        exit(1);
    }

    s = DefaultScreen(d);

    /* create window */
    window = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 200, 200, 1,
                                 BlackPixel(d, s), BlackPixel(d, s));

    /* select kind of events we are interested in */
    XSelectInput(d, window, StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask );

    /* map (show) the window */
    XMapWindow(d, window);

    /* event loop */
    while (1)
    {
        XNextEvent(d, &event);

        /* keyboard events */
        if ((event.type == KeyPress)||(event.type == KeyRelease))
        {
            printf( "KeyPress: %x %d\n", event.xkey.keycode, event.type );

            if(event.xkey.keycode == 0x18)      // Resize on pressing Q as, key Q => 0x18
            {
                printf("Here in Q\n");

                int r = XResizeWindow(d, window, 100, 200);     // Resizing the window through Q keypress
                if(r==BadValue || r==BadWindow)
                    printf("Error in resizing\n");

                XNextEvent(d, &event);                          // To get ConfigureNotify event
                if(event.type == ConfigureNotify)
                    printf("Resized!\n");
                else
                    printf("Not resized\n");
                //XMapWindow(d, window);                            // Map the resized window   (not necessary)
            }
            /* exit on ESC key press */
            if ( event.xkey.keycode == 0x09 )
                break;
        }
    }

    /* close connection to server */
    XCloseDisplay(d);

    return 0;
}