/*
 * Copyright © 2001-2011 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#ifndef DLT645_RTU_H_
#define DLT645_RTU_H_

#include "dlt645.h"

DLT645_BEGIN_DECLS

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
 */

#define DLT645_RTU_MAX_ADU_LENGTH  200    // 200 for dlt645

DLT645_API dlt645_t* dlt645_new_rtu(const char *device, int baud, char parity,
                                    int data_bit, int stop_bit);

#define DLT645_RTU_RS232 0
#define DLT645_RTU_RS485 1

DLT645_API int modbus_rtu_set_serial_mode(dlt645_t *ctx, int mode);
DLT645_API int modbus_rtu_get_serial_mode(dlt645_t *ctx);

#define DLT645_RTU_RTS_NONE   0
#define DLT645_RTU_RTS_UP     1
#define DLT645_RTU_RTS_DOWN   2

DLT645_API int modbus_rtu_set_rts(dlt645_t *ctx, int mode);
DLT645_API int modbus_rtu_get_rts(dlt645_t *ctx);

DLT645_API int modbus_rtu_set_custom_rts(dlt645_t *ctx, void (*set_rts) (dlt645_t *ctx, int on));

DLT645_API int modbus_rtu_set_rts_delay(dlt645_t *ctx, int us);
DLT645_API int modbus_rtu_get_rts_delay(dlt645_t *ctx);

DLT645_END_DECLS


#endif /* DLT645_RTU_H */
