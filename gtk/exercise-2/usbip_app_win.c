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

#include <json-c/json.h>
#include <gtk/gtk.h>

#include "usbip_app.h"
#include "usbip_app_win.h"
#include "usb_desc.h"
#include "usb_get_list.h"

struct _UsbipAppWin {
    GtkApplicationWindow parent;
};

typedef struct _UsbipAppWinPrivate UsbipAppWinPrivate;

struct _UsbipAppWinPrivate {
    GtkWidget *win_scroll;
    GtkWidget *list_dev;
    GtkWidget *button_add_dev;
};

G_DEFINE_TYPE_WITH_PRIVATE(UsbipAppWin,
                           usbip_app_win,
                           GTK_TYPE_APPLICATION_WINDOW)

static GtkWidget*
usbip_app_win_empty(void)
{
    gchar* no_mess = g_strdup("<span size=\"x-large\">There are no USB devices "
                              "found\nin your area . . .</span>");

    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), no_mess);
    g_free(no_mess);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_widget_show(label);

    return label;
}

static void
usbip_app_win_init(UsbipAppWin *app)
{
    UsbipAppWin *win = USBIP_APP_WIN(gtk_widget_get_toplevel(GTK_WIDGET(app)));
    UsbipAppWinPrivate *self = usbip_app_win_get_instance_private(win);

    gtk_widget_init_template(GTK_WIDGET(app));
    gtk_list_box_set_placeholder(GTK_LIST_BOX(self->list_dev), usbip_app_win_empty());
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
    g_autofree gchar *name = NULL;
    g_autofree gchar *idvendor = NULL;
    g_autofree gchar *idproduct = NULL;
    g_autofree gchar *manufacturer = NULL;
    g_autofree gchar *busid = NULL;
    g_autofree gchar *node_addr = NULL;
    g_autofree gchar *devs_desc = NULL;
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

    devs_desc =
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
    g_clear_object (&usb_entry_builder);

    return usb_entry_row;
}

static const gchar*
query_usb_id(json_object* root, const gchar* key)
{
    json_object* ret_val;

    if (json_object_object_get_ex(root, key, &ret_val))
        return json_object_get_string(ret_val);

    return NULL;
}

static void
usbip_app_win_append_list(UsbipAppWin *app)
{
    UsbipAppWin *win = USBIP_APP_WIN(gtk_widget_get_toplevel(GTK_WIDGET(app)));
    UsbipAppWinPrivate *self = usbip_app_win_get_instance_private(win);

    json_object *usb_list, *iter;
    usb_list = get_devices();

    if (!json_object_object_length(usb_list)) {
        goto done;
    }

    json_object_object_foreach(usb_list, nodes, devices)
    {
        (void)(nodes);
        if (json_object_get_type(devices) == json_type_array) {
            for (int i = 0; i < json_object_array_length(devices); i++) {
                iter = json_object_array_get_idx(devices, i);

                g_autoptr(UsbDesc) fd_toshiba = NULL;
                fd_toshiba = g_object_new(USB_TYPE_DESC,
                                         "name", query_usb_id(iter, "product"),
                                         "id-vendor", query_usb_id(iter, "idVendor"),
                                         "id-product", query_usb_id(iter, "idProduct"),
                                         "manufacturer", query_usb_id(iter, "manufact"),
                                         "busid", query_usb_id(iter, "busid"),
                                         "node-addr", "127.0.0.1",
                                         "state", TRUE,
                                         NULL);

                GtkWidget *usb_entry_row =  create_usbip_entry(fd_toshiba);
                gtk_container_add (GTK_CONTAINER (self->list_dev), usb_entry_row);
            }
        }
    }
done:
    json_object_put(usb_list);
}

static void
usbip_app_win_finalize(GObject* obj)
{
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
