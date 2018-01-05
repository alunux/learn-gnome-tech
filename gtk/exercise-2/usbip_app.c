/*
 * Copyright (C) 2017 La Ode Muh. Fadlun Akbar <fadlun.net@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>

#include "usbip_app.h"
#include "usbip_app_win.h"

struct _UsbipApp {
    GtkApplication parent;
};

G_DEFINE_TYPE(UsbipApp, usbip_app, GTK_TYPE_APPLICATION)

static void
usbip_app_init(__attribute__((unused)) UsbipApp* self)
{
}

static void
usbip_app_activate(GApplication* self)
{
    UsbipAppWin* win = usbip_app_win_new(USBIP_APP(self));
    gtk_widget_show_all(GTK_WIDGET(win));
    gtk_window_present(GTK_WINDOW(win));
}

static void
usbip_app_class_init(UsbipAppClass* class)
{
    G_APPLICATION_CLASS(class)->activate = usbip_app_activate;
}

UsbipApp*
usbip_app_new(void)
{
    return g_object_new(USBIP_APP_TYPE,
                        "application-id", "org.alunux.usbipapp",
                        "flags", G_APPLICATION_FLAGS_NONE,
                        NULL);
}
