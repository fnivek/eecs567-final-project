#ifndef USB_H
#define USB_H

#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/cm3/scb.h>

#include "systick_handler.h"

/*
 *			Definitions
 */

 typedef struct {
	struct usb_cdc_header_descriptor header;
	struct usb_cdc_call_management_descriptor call_mgmt;
	struct usb_cdc_acm_descriptor acm;
	struct usb_cdc_union_descriptor cdc_union;
} __attribute__((packed)) FunctionalDescriptor;

// USB OTG setup
const struct usb_device_descriptor dev;
/*
 * This notification endpoint isn't implemented. According to CDC spec it's
 * optional, but its absence causes a NULL pointer dereference in the
 * Linux cdc_acm driver.
 */
extern const struct usb_endpoint_descriptor comm_endp[];
extern const struct usb_endpoint_descriptor data_endp[];
const FunctionalDescriptor cdcacm_functional_descriptors;
extern const struct usb_interface_descriptor comm_iface[];
extern const struct usb_interface_descriptor data_iface[];
extern const struct usb_interface ifaces[];
const struct usb_config_descriptor config;

#define NUM_USB_STRINGS 3
extern const char * usb_strings[NUM_USB_STRINGS];
/* Buffer to be used for control requests. */
uint8_t usbd_control_buffer[128];

/*
 * 		Functions
 */
int CDCACMControlRequest(usbd_device *usbd_dev,
	struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
	void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req));
void CDCACMDataRxCb(usbd_device *usbd_dev, uint8_t ep);
void CDCACMSetConfig(usbd_device *usbd_dev, uint16_t wValue);
void UsbWrite(const void* buf, uint16_t len);
void SetupUsb(void);
void UsbPoll(void);
void UsbWriteString(const char* buf);
int8_t is_usb_ready(void);

#endif
