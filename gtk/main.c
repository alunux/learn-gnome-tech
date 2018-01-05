#include "usbip_app.h"

int
main(int argc, char *argv[])
{   
    UsbipApp *app = usbip_app_new();

    return g_application_run(G_APPLICATION(app), argc, argv);
}