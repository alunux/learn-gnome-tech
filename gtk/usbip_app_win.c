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
#include "usb_desc.h"

struct _UsbipAppWin {
    GtkApplicationWindow parent;
};

typedef struct _UsbipAppWinPrivate UsbipAppWinPrivate;

struct _UsbipAppWinPrivate {
    GtkWidget *win_scroll;
    GtkWidget *list_dev;
    GtkWidget *button_add_dev;
    GList     *devs;
};

G_DEFINE_TYPE_WITH_PRIVATE(UsbipAppWin,
                           usbip_app_win,
                           GTK_TYPE_APPLICATION_WINDOW)

static void
usbip_app_win_init(UsbipAppWin *self)
{
    gtk_widget_init_template(GTK_WIDGET(self));
}

static void
usbip_state_changed(GtkWidget *usb_state, __attribute__((unused)) gpointer data)
{
    if (!g_strcmp0(gtk_button_get_label(GTK_BUTTON(usb_state)), "Attach")) {
        gtk_button_set_label(GTK_BUTTON(usb_state), "Detach");
    } else {
        gtk_button_set_label(GTK_BUTTON(usb_state), "Attach");
    }
}

static GtkWidget*
create_usbip_entry(UsbDesc *fd_toshiba)
{   
    gchar *name, *idvendor, *idproduct, *manufacturer, *busid, *node_addr;
    gboolean state;

    GtkBuilder *usb_entry_builder;
    GtkWidget *usb_entry_row;
    GtkWidget *usb_entry_box;
    GtkWidget *usb_info;
    GtkWidget *usb_state;

    g_object_get(fd_toshiba,
                 "name", &name,
                 "id-vendor", &idvendor,
                 "id-product", &idproduct,
                 "manufacturer", &manufacturer,
                 "busid", &busid,
                 "node-addr", &node_addr,
                 "state", &state,
                 NULL);

    gchar* devs_desc =
      g_strdup_printf("<b>%s</b>\nidVendor: %s\nidProduct: %s\nManufacturer: "
                      "%s\nBUSID: %s\nNode: %s\n",
                      name,
                      idvendor,
                      idproduct,
                      manufacturer,
                      busid,
                      node_addr);

    usb_entry_row = gtk_list_box_row_new ();
    gtk_list_box_row_set_activatable (GTK_LIST_BOX_ROW (usb_entry_row), FALSE);
    usb_entry_builder = gtk_builder_new_from_resource ("/org/alunux/usbipapp/usbip-entry.ui");
    
    usb_info = GTK_WIDGET(gtk_builder_get_object(usb_entry_builder, "label"));
    gtk_label_set_markup(GTK_LABEL(usb_info), devs_desc);
    
    usb_state = GTK_WIDGET(gtk_builder_get_object(usb_entry_builder, "button"));
    gtk_button_set_label(GTK_BUTTON(usb_state), (state == TRUE) ? "Attach" : "Detach");
    g_signal_connect (usb_state, "clicked", G_CALLBACK (usbip_state_changed), NULL);
    
    usb_entry_box = GTK_WIDGET(gtk_builder_get_object(usb_entry_builder, "box"));
    gtk_container_add (GTK_CONTAINER (usb_entry_row), usb_entry_box);

    gtk_widget_show_all (usb_entry_row);

    g_free(name);
    g_free(idvendor);
    g_free(idproduct);
    g_free(manufacturer);
    g_free(busid);
    g_free(node_addr);
    g_free(devs_desc);
    g_clear_object (&usb_entry_builder);

    return usb_entry_row;
}

static void
usbip_app_win_append_list(UsbipAppWin *app)
{
    UsbipAppWin *win = USBIP_APP_WIN(gtk_widget_get_toplevel(GTK_WIDGET(app)));
    UsbipAppWinPrivate *self = usbip_app_win_get_instance_private(win);
    
    UsbDesc *fd_toshiba = g_object_new(USB_TYPE_DESC,
                                       "name", "Toshiba Transmemory",
                                       "id-vendor", "0930",
                                       "id-product", "6544",
                                       "manufacturer", "TOSHIBA",
                                       "busid", "1-2",
                                       "node-addr", "192.168.122.184",
                                       "state", TRUE,
                                       NULL);

    GtkWidget *usb_entry_row =  create_usbip_entry(fd_toshiba);
    g_object_unref(fd_toshiba);

    gtk_container_add (GTK_CONTAINER (self->list_dev), usb_entry_row);
}

static void
usbip_app_win_finalize(GObject* obj)
{
    UsbipAppWin *win = USBIP_APP_WIN(obj);
    UsbipAppWinPrivate *priv = usbip_app_win_get_instance_private(win);

    g_list_free(priv->devs);

    G_OBJECT_CLASS(usbip_app_win_parent_class)->finalize(obj);
}

static void
usbip_app_win_class_init(UsbipAppWinClass* class)
{
    G_OBJECT_CLASS(class)->finalize = usbip_app_win_finalize;

    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/org/alunux/usbipapp/window.ui");
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), UsbipAppWin, win_scroll);
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), UsbipAppWin, button_add_dev);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), usbip_app_win_append_list);
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), UsbipAppWin, list_dev);
}

UsbipAppWin*
usbip_app_win_new(UsbipApp* self)
{
    return g_object_new(USBIP_APP_WIN_TYPE, "application", self, NULL);
}
