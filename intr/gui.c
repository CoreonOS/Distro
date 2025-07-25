#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    Display *display;
    Window window;
    XEvent event;
    int screen;

    // Abre la conexión con el servidor X
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "No se pudo abrir la pantalla\n");
        exit(1);
    }

    screen = DefaultScreen(display);

    // Crea la ventana
    window = XCreateSimpleWindow(display,
                                 RootWindow(display, screen),
                                 100, 100, 400, 300, 1,
                                 BlackPixel(display, screen),
                                 WhitePixel(display, screen));

    // Selecciona los eventos que la ventana recibirá
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // Muestra la ventana
    XMapWindow(display, window);

    // Loop de eventos
    for (;;) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            XFillRectangle(display, window,
                           DefaultGC(display, screen),
                           20, 20, 100, 50);
        }
        if (event.type == KeyPress)
            break;
    }

    XCloseDisplay(display);
    return 0;
}

