#include <stdio.h>
#include <stdlib.h>
#include "../include/libusb/libusb.h"

using namespace std;

static void print_devs(libusb_device **devs){
    libusb_device *dev;
    int i = 0;
    int rc;
    while((dev = devs[i]) != NULL){
        libusb_device_descriptor desc;
        rc = libusb_get_device_descriptor( dev, &desc );
        if( rc != LIBUSB_SUCCESS ){
            printf(" libusb_get_device_descriptor() failed, errCode(%d)=%s\n", rc, libusb_error_name(rc));
            libusb_close( NULL );
        }
        printf("device: PID:0x%04X, VID:0x%04X\n", desc.idProduct, desc.idVendor);
        i++;
    }
}

int main(int argc, char *argv[]){

    printf("Hello LibUsb : Device list\n");

    libusb_device **devicelist = NULL;
    int rc = 0;
    ssize_t cnt;
    
    rc = libusb_init( NULL );
    if( rc != LIBUSB_SUCCESS ){
        printf("Initial failed! libusb_init() failed, errCode(%d)=%s\n", rc, libusb_error_name(rc));
        return rc;
    }

    cnt = libusb_get_device_list( NULL, &devicelist );
    if(cnt < 0){
        printf("there is no device to be listed\n");
        libusb_exit( NULL );
        return (int) cnt ;
    }

    print_devs( devicelist );

    libusb_free_device_list( devicelist, 1 );

    libusb_exit( NULL );

    return 0;
}