#include "usb-desc.h"

int
main(void)
{   
    UsbDesc *fd_toshiba;
    gchar *name, *idvendor, *idproduct, *manufacturer, *busid, *node_addr;
    gboolean state;

    fd_toshiba = g_object_new(USB_TYPE_DESC,
                              "name", "Toshiba Transmemory",
                              "id-vendor", "0930",
                              "id-product", "6544",
                              "manufacturer", "TOSHIBA",
                              "busid", "1-2",
                              "node-addr", "192.168.122.184",
                              "state", TRUE,
                              NULL);

    g_object_get(fd_toshiba,
                 "name", &name,
                 "id-vendor", &idvendor,
                 "id-product", &idproduct,
                 "manufacturer", &manufacturer,
                 "busid", &busid,
                 "node-addr", &node_addr,
                 "state", &state,
                 NULL);

    g_print("Use g_object_new with initialized value\n");
    g_print("=======================================\n\n");
    g_print("Testing: Get Value (Copied)\n\n"
            "%s\n"
            "idVendor: %s\n"
            "idProduct: %s\n"
            "Manufacturer: %s\n"
            "BUSID: %s\n"
            "Node: %s\n"
            "State: %s\n"
            "\n",
            name,
            idvendor,
            idproduct,
            manufacturer,
            busid,
            node_addr,
            (state == TRUE) ? "Attachable" : "Detachable");

    g_free(name);
    g_free(idvendor);
    g_free(idproduct);
    g_free(manufacturer);
    g_free(busid);
    g_free(node_addr);

    g_print("Testing: Direct access (Getter)\n\n"
            "%s\n"
            "idVendor: %s\n"
            "idProduct: %s\n"
            "Manufacturer: %s\n"
            "BUSID: %s\n"
            "Node: %s\n"
            "State: %s\n"
            "\n",
            usb_desc_get_name(fd_toshiba),
            usb_desc_get_idvendor(fd_toshiba),
            usb_desc_get_idproduct(fd_toshiba),
            usb_desc_get_manufacturer(fd_toshiba),
            usb_desc_get_busid(fd_toshiba),
            usb_desc_get_node_addr(fd_toshiba),
            (usb_desc_get_state(fd_toshiba) == TRUE) ? "Attachable" : "Detachable");

    g_object_unref(fd_toshiba);

    //////////////////////////////////////////////////////////////////////////////////////

    fd_toshiba = g_object_new(USB_TYPE_DESC, NULL);

    usb_desc_set_name(fd_toshiba, "Toshiba Transmemory");
    usb_desc_set_idvendor(fd_toshiba, "0930");
    usb_desc_set_idproduct(fd_toshiba, "6544");
    usb_desc_set_manufacturer(fd_toshiba, "TOSHIBA");
    usb_desc_set_busid(fd_toshiba, "1-2");
    usb_desc_set_node_addr(fd_toshiba, "192.168.122.184");
    usb_desc_set_state(fd_toshiba, TRUE);

    g_print("Use g_object_new with empty fields\n");
    g_print("==================================\n\n");
    g_print("Testing: Direct access (Setter + Getter)\n\n"
            "%s\n"
            "idVendor: %s\n"
            "idProduct: %s\n"
            "Manufacturer: %s\n"
            "BUSID: %s\n"
            "Node: %s\n"
            "State: %s\n"
            "\n",
            usb_desc_get_name(fd_toshiba),
            usb_desc_get_idvendor(fd_toshiba),
            usb_desc_get_idproduct(fd_toshiba),
            usb_desc_get_manufacturer(fd_toshiba),
            usb_desc_get_busid(fd_toshiba),
            usb_desc_get_node_addr(fd_toshiba),
            (usb_desc_get_state(fd_toshiba) == TRUE) ? "Attachable" : "Detachable");

    g_object_get(fd_toshiba,
                 "name", &name,
                 "id-vendor", &idvendor,
                 "id-product", &idproduct,
                 "manufacturer", &manufacturer,
                 "busid", &busid,
                 "node-addr", &node_addr,
                 "state", &state,
                 NULL);

    g_print("Testing: Get Value (Copied)\n\n"
            "%s\n"
            "idVendor: %s\n"
            "idProduct: %s\n"
            "Manufacturer: %s\n"
            "BUSID: %s\n"
            "Node: %s\n"
            "State: %s\n"
            "\n",
            name,
            idvendor,
            idproduct,
            manufacturer,
            busid,
            node_addr,
            (state == TRUE) ? "Attachable" : "Detachable");

    g_free(name);
    g_free(idvendor);
    g_free(idproduct);
    g_free(manufacturer);
    g_free(busid);
    g_free(node_addr);
    
    g_object_unref(fd_toshiba);
    return 0;
}