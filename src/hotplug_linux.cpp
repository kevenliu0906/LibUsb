#include <stdio.h>
#include <stdlib.h>
#include "../include/libusb/libusb.h"   // lib-usb library

using namespace std;

libusb_device_handle *device_handle = NULL;
int done = 0;


//************ this sample code is workable for Ubuntu 22.04 version ************
// NOTE: till 2023/12/21, lib-usb hotplug feature does NOT support on Windows system.
// reference: https://github.com/libusb/libusb/issues/86

// this code support detected the specific device(pid, vid) with hot-plug event
// if you want to detected any usb-device, you could assign pid & vid with 'LIBUSB_HOTPLUG_MATCH_ANY' value


static int hotplug_callback(libusb_context *ctx, libusb_device *device, libusb_hotplug_event event, void *user_data){

    libusb_device_descriptor desc;
    int rc;

    rc = libusb_get_device_descriptor( device, &desc );
    if( rc != LIBUSB_SUCCESS ){
        printf("fail to get device descriptor, libusb_get_device_descriptor() failed, errCode(%d):%s\n", rc, libusb_error_name(rc));
    }

    printf("hotplug event detected !!, PID:0x%04X, VID:0x%04X", 
            desc.idProduct,
            desc.idVendor);

    if( event ==  LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED ){
        printf(" >>>>> event (ARRIVE)\n");
        if( device_handle ){
            libusb_close( device_handle );
            device_handle = NULL;
        }

        rc = libusb_open( device, &device_handle );
        if( rc != LIBUSB_SUCCESS ){
            printf("Could NOT open usb device, libusb_open() failed, errCode(%d):%s, pid:0x%x, vid:0x%x\n", rc, libusb_error_name(rc), desc.idProduct, desc.idVendor);
        }
    }
    else if( event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT ){
        printf(" >>>>> event (LEFT)\n");
        if( device_handle ){
            libusb_close( device_handle );
            device_handle = NULL;
        }
    }
    else{
        printf(" >>>>> event (Unhandle event)\n");
        return 1;
    }

    done++;

    return 0;
}


int main(int argc, char *argv[]){

    printf("Hello LibUsb : hot-plug detection\n");

    libusb_hotplug_callback_handle callback_handle;
    int product_id, vendor_id, class_id;
    int rc;

    product_id = (argc > 1) ? strtol(argv[1], NULL, 0) : 0x16E4/*LIBUSB_HOTPLUG_MATCH_ANY*/;   // kingstone pluse fire gaming mouse /* PID:0x16E4, VID:0x0951 */
    vendor_id = (argc > 2) ? strtol(argv[2], NULL, 0) : 0x0951/*LIBUSB_HOTPLUG_MATCH_ANY*/;   // kingstone pluse fire gaming mouse /* PID:0x16E4, VID:0x0951 */
    class_id = (argc > 3) ? strtol(argv[3], NULL, 0) : LIBUSB_HOTPLUG_MATCH_ANY;

    printf("<kd> pid:0x%04X, vid:0x%04X, class_id:0x%X\n", product_id, vendor_id, class_id);

    rc = libusb_init( NULL );
    if( rc != LIBUSB_SUCCESS ){
        printf("Initial failed! libusb_init() failed, errCode(%d)=%s\n", rc, libusb_error_name(rc));
        return 1;
    }

    // test if the loaded library supports a given capability - libusb_has_capability() API is available
    rc = libusb_has_capability( LIBUSB_CAP_HAS_CAPABILITY );
    if( rc == 0 ){
        printf("the libusb_has_capability() API does not available!\n");
        libusb_exit( NULL );
        return 1;
    }

    rc = libusb_has_capability( LIBUSB_CAP_HAS_HOTPLUG );
    if( rc == 0 ){
        printf("hotplug capability does NOT supported on this platform!\n");
        libusb_exit( NULL );
        return 1;
    }

    rc = libusb_hotplug_register_callback( NULL, 
                                           LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
                                           0,
                                           vendor_id,
                                           product_id,
                                           class_id,
                                           hotplug_callback,
                                           NULL,
                                           &callback_handle );
    if( rc != LIBUSB_SUCCESS ){
        printf("Error registering callback, libusb_hotplug_register_callback() failed, errCode(%d)=%s\n", rc, libusb_error_name(rc));
        libusb_exit( NULL );
        return 1;
    }

    while( done != 2 ){
        rc = libusb_handle_events_completed(NULL, NULL);
        if( rc != LIBUSB_SUCCESS ){
            printf("libusb_handle_events_completed() failed, errCode(%d):%s\n", rc, libusb_error_name(rc));
        }
    }

    if( device_handle ){
        libusb_close( device_handle );
        device_handle = NULL;
    }

    libusb_exit( NULL );

    printf("lib-usb hot-plug event detection completely!\n");

    return 0;
}