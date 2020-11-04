/*
 * Copyright © 2010-2012 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#ifndef DLT645_PRIVATE_H
#define DLT645_PRIVATE_H

#ifndef _MSC_VER
# include <stdint.h>
# include <sys/time.h>
#else
# include "stdint.h"
# include <time.h>
typedef int ssize_t;
#endif
#include <sys/types.h>
#include "config.h"

#include "dlt645.h"
#include "dlt645-rtu-private.h"

DLT645_BEGIN_DECLS // declear as c when meet cplusplus

#define FOREHEAD    0xFE
#define FRAME_HEAD   0x68
#define FRAME_END   0x16

/* It's not really the minimal length (the real one is report slave ID
 * in RTU (4 bytes)) but it's a convenient size to use in RTU or TCP
 * communications to read many values or write a single one.
 * Maximum between :
 * - HEADER_LENGTH_TCP (7) + function (1) + address (2) + number (2)
 * - HEADER_LENGTH_RTU (1) + function (1) + address (2) + number (2) + CRC (2)
 */
#define _MIN_REQ_LENGTH 20

#define _REPORT_SLAVE_ID 180

#define _DLT645_EXCEPTION_RSP_LENGTH 5

/* Timeouts in microsecond(us) (0.5 s) */
#define _RESPONSE_TIMEOUT    500000
#define _BYTE_TIMEOUT        500000

typedef enum {
    _DLT645_BACKEND_TYPE_RTU=0,
    _DLT645_BACKEND_TYPE_TCP
} modbus_backend_type_t;

/*
 *  ---------- Request     Indication ----------
 *  | Client | ---------------------->| Server |
 *  ---------- Confirmation  Response ----------
 */
typedef enum {
    /* Request message on the server side */
    MSG_INDICATION,
    /* Request message on the client side */
    MSG_CONFIRMATION
} msg_type_t;

/* This structure reduces the number of params in functions and so
 * optimizes the speed of execution (~ 37%). */
typedef struct _sft {
    int slave;
    int function;
    int t_id;
} sft_t;

typedef struct _dlt645_backend {
    unsigned int backend_type;
    unsigned int header_length;    // this is the index of function code,header means before function code
    unsigned int checksum_length;
    unsigned int max_adu_length;
    int (*set_slave) (dlt645_t *ctx, const uint64_t slaveid);
    int (*set_password) (dlt645_t *ctx, const uint32_t pass);
    int (*set_user) (dlt645_t *ctx, const uint32_t user);
    int (*build_request_basis) (dlt645_t *ctx, int function, uint8_t *req);
    int (*build_request_read) (dlt645_t *ctx, uint32_t addr, uint8_t *req, tRegType type);
    int (*build_request_write) (dlt645_t *ctx, uint32_t addr, const uint8_t *data,
                                int len, uint8_t *req, tRegType type);
    int (*build_response_basis) (sft_t *sft, uint8_t *rsp);
    int (*prepare_response_tid) (const uint8_t *req, int *req_length);
    int (*send_msg_pre) (uint8_t *req, int req_length);
    ssize_t (*send) (dlt645_t *ctx, const uint8_t *req, int req_length);
    int (*receive) (dlt645_t *ctx, uint8_t *req);
    ssize_t (*recv) (dlt645_t *ctx, uint8_t *rsp, int rsp_length);
    int (*check_integrity) (dlt645_t *ctx, uint8_t *msg,
                            const int msg_length);
    int (*pre_check_confirmation) (dlt645_t *ctx, const uint8_t *req,
                                   const uint8_t *rsp, int rsp_length);
    int (*connect) (dlt645_t *ctx);
    void (*close) (dlt645_t *ctx);
    int (*flush) (dlt645_t *ctx);
    int (*select) (dlt645_t *ctx, fd_set *rset, struct timeval *tv, int msg_length);
    void (*free) (dlt645_t *ctx);
} modbus_backend_t;

struct _dlt645 {
    /* Slave address */
    uint64_t slaveid;    // low byte is in low address. ex: {0x00, 0x01), = 0100
    uint32_t password;   // highest is level
    uint32_t user;
    /* Socket or file descriptor */
    int s;
    int debug;   // for debug: TRUE:Open
    int error_recovery;
    struct timeval response_timeout;
    struct timeval byte_timeout;
    struct timeval indication_timeout;
    const modbus_backend_t *backend;
    dlt645_rtu_t *backend_data;
};

void _dlt645_init_common(dlt645_t *ctx);
void _error_print(dlt645_t *ctx, const char *context);
int _dlt645_receive_msg(dlt645_t *ctx, uint8_t *msg, msg_type_t msg_type);

#ifndef HAVE_STRLCPY
size_t strlcpy(char *dest, const char *src, size_t dest_size);
#endif

DLT645_END_DECLS

#endif  /* DLT645_PRIVATE_H */
