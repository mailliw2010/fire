/*
 * Copyright © 2001-2011 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 *
 * This library implements the Modbus protocol.
 * http://libmodbus.org/
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "config.h"

#include "dlt645.h"
#include "dlt645-private.h"
#include "globalFun.h"

/* Internal use */
#define MSG_LENGTH_UNDEFINED -1

/* Exported version */
const unsigned int libmodbus_version_major = LIBDLT645_VERSION_MAJOR;
const unsigned int libmodbus_version_minor = LIBDLT645_VERSION_MINOR;
const unsigned int libmodbus_version_micro = LIBDLT645_VERSION_MICRO;

/* Max between RTU and TCP max adu length (so TCP) */
#define MAX_MESSAGE_LENGTH 260

/* 3 steps are used to parse the query */
typedef enum {
    _STEP_FUNCTION,
    _STEP_META,
    _STEP_DATA
} _step_t;

const char *modbus_strerror(int errnum) {
    switch (errnum) {
    case EMBXILFUN:
        return "Illegal function";
    case EMBXILADD:
        return "Illegal data address";
    case EMBXILVAL:
        return "Illegal data value";
    case EMBXSFAIL:
        return "Slave device or server failure";
    case EMBXACK:
        return "Acknowledge";
    case EMBXSBUSY:
        return "Slave device or server is busy";
    case EMBXNACK:
        return "Negative acknowledge";
    case EMBXMEMPAR:
        return "Memory parity error";
    case EMBXGPATH:
        return "Gateway path unavailable";
    case EMBXGTAR:
        return "Target device failed to respond";
    case EMBBADCRC:
        return "Invalid CRC";
    case EMBBADDATA:
        return "Invalid data";
    case EMBBADEXC:
        return "Invalid exception code";
    case EMBMDATA:
        return "Too many data";
    case EMBBADSLAVE:
        return "Response not from requested slave";
    default:
        return strerror(errnum);
    }
}

void _error_print(dlt645_t *ctx, const char *context)
{
    if (ctx->debug) {
        fprintf(stderr, "ERROR %s", modbus_strerror(errno));
        if (context != NULL) {
            fprintf(stderr, ": %s\n", context);
        } else {
            fprintf(stderr, "\n");
        }
    }
}

static void _sleep_response_timeout(dlt645_t *ctx)
{
    /* Response timeout is always positive */
#ifdef _WIN32
    /* usleep doesn't exist on Windows */
    Sleep((ctx->response_timeout.tv_sec * 1000) +
          (ctx->response_timeout.tv_usec / 1000));
#else
    /* usleep source code */
    struct timespec request, remaining;
    request.tv_sec = ctx->response_timeout.tv_sec;
    request.tv_nsec = ((long int)ctx->response_timeout.tv_usec) * 1000;
    while (nanosleep(&request, &remaining) == -1 && errno == EINTR) {
        request = remaining;
    }
#endif
}

int modbus_flush(dlt645_t *ctx)
{
    int rc;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    rc = ctx->backend->flush(ctx);
    if (rc != -1 && ctx->debug) {
        /* Not all backends are able to return the number of bytes flushed */
        printf("Bytes flushed (%d)\n", rc);
    }
    return rc;
}

/* Computes the length of the expected response */
//static unsigned int compute_response_length_from_request(dlt645_t *ctx, uint8_t *req)
//{
//    int length;
//    const int offset = ctx->backend->header_length;

//    switch (req[offset]) {
//    case DLT645_FC_READ_COILS:
//    case DLT645_FC_READ_DISCRETE_INPUTS: {
//        /* Header + nb values (code from write_bits) */
//        int nb = (req[offset + 3] << 8) | req[offset + 4];
//        length = 2 + (nb / 8) + ((nb % 8) ? 1 : 0);
//    }
//        break;
//    case DLT645_FC_WRITE_AND_READ_REGISTERS:
//    case DLT645_FC_READ_HOLDING_REGISTERS:
//    case DLT645_FC_READ_INPUT_REGISTERS:
//        /* Header + 2 * nb values */
//        length = 2 + 2 * (req[offset + 3] << 8 | req[offset + 4]);
//        break;
//    case DLT645_FC_READ_EXCEPTION_STATUS:
//        length = 3;
//        break;
//    case DLT645_FC_REPORT_SLAVE_ID:
//        /* The response is device specific (the header provides the
//           length) */
//        return MSG_LENGTH_UNDEFINED;
//    case DLT645_FC_MASK_WRITE_REGISTER:
//        length = 7;
//        break;
//    default:
//        length = 5;
//    }

//    return offset + length + ctx->backend->checksum_length;
//}

/* Sends a request/response */
static int send_msg(dlt645_t *ctx, uint8_t *msg, int msg_length)
{
    int rc;
    int i;
    uint8_t msg_total[32];

    msg_length = ctx->backend->send_msg_pre(msg, msg_length);  // add crc,

    /**add forehead 4 bytes**/
    memset(msg_total, FOREHEAD, 4);
    msg_length += 4;
    memcpy(msg_total+4, msg, (uint64_t)msg_length);

    // copy to global buffer
    memcpy((uint8_t *)UartMesg.sndUartData, msg_total, (unsigned long)msg_length);
    UartMesg.sndUartLen = msg_length;


    if (ctx->debug) {
        for (i = 0; i < msg_length; i++)
            printf("[%.2X]", msg_total[i]);
        printf("\n");
    }


    /* In recovery mode, the write command will be issued until to be
       successful! Disabled by default. */
    do {
        rc = ctx->backend->send(ctx, msg_total, msg_length);  //send to uart.
        if (rc == -1) {
            _error_print(ctx, NULL);
            if (ctx->error_recovery & DLT645_ERROR_RECOVERY_LINK) {
                int saved_errno = errno;

                if ((errno == EBADF || errno == ECONNRESET || errno == EPIPE)) {
                    modbus_close(ctx);
                    _sleep_response_timeout(ctx);
                    dlt645_connect(ctx);
                } else {
                    _sleep_response_timeout(ctx);
                    modbus_flush(ctx);
                }
                errno = saved_errno;
            }
        }
    } while ((ctx->error_recovery & DLT645_ERROR_RECOVERY_LINK) &&
             rc == -1);

    if (rc > 0 && rc != msg_length) {
        errno = EMBBADDATA;
        return -1;
    }

    return rc;
}

int modbus_send_raw_request(dlt645_t *ctx, const uint8_t *raw_req, int raw_req_length)
{
    sft_t sft;
    uint8_t req[MAX_MESSAGE_LENGTH];
    int req_length;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (raw_req_length < 2 || raw_req_length > (DLT645_MAX_PDU_LENGTH + 1)) {
        /* The raw request must contain function and slave at least and
           must not be longer than the maximum pdu length plus the slave
           address. */
        errno = EINVAL;
        return -1;
    }

    sft.slave = raw_req[0];
    sft.function = raw_req[1];
    /* The t_id is left to zero */
    sft.t_id = 0;
    /* This response function only set the header so it's convenient here */
    req_length = ctx->backend->build_response_basis(&sft, req);

    if (raw_req_length > 2) {
        /* Copy data after function code */
        memcpy(req + req_length, raw_req + 2, raw_req_length - 2);
        req_length += raw_req_length - 2;
    }

    return send_msg(ctx, req, req_length);
}

/*
 *  ---------- Request     Indication ----------
 *  | Client | ---------------------->| Server |
 *  ---------- Confirmation  Response ----------
 */

/* Computes the length to read after the function received */
static uint8_t compute_meta_length_after_function(int function,
                                                  msg_type_t msg_type)
{
    int length;

    if (msg_type == MSG_INDICATION) {


    } else {
        /* MSG_CONFIRMATION */

            length = 1;
        }


    return length;
}

/* Computes the length to read after the meta information (address, count, etc) */
static int compute_data_length_after_meta(dlt645_t *ctx, uint8_t *msg,
                                          msg_type_t msg_type)
{
    int function = msg[ctx->backend->header_length];
    int length;

    if (msg_type == MSG_INDICATION) {
        switch (function) {

        default:
            length = 0;
        }
    } else {
        /* MSG_CONFIRMATION */
        if (function <= 0x9C) {   // 0x11
            length = msg[ctx->backend->header_length + 1];
        } else {
            length = 0;
        }
    }

    length += ctx->backend->checksum_length;

    return length;
}


/* Waits a response from a modbus server or a request from a modbus client.
   This function blocks if there is no replies (3 timeouts).

   The function shall return the number of received characters and the received
   message in an array of uint8_t if successful. Otherwise it shall return -1
   and errno is set to one of the values defined below:
   - ECONNRESET
   - EMBBADDATA
   - EMBUNKEXC
   - ETIMEDOUT
   - read() or recv() error codes
*/

int _dlt645_receive_msg(dlt645_t *ctx, uint8_t *msg, msg_type_t msg_type)
{
    int rc;
    fd_set rset;
    struct timeval tv;
    struct timeval *p_tv;
    int length_to_read;
    int msg_length = 0;
    _step_t step;

    if (ctx->debug) {     //debug message
        if (msg_type == MSG_INDICATION) {
            printf("Waiting for an indication...\n");
        } else {
            printf("Waiting for a confirmation...\n");
        }
    }

    /* Add a file descriptor to the set */
    FD_ZERO(&rset);
    FD_SET(ctx->s, &rset);

    /* We need to analyse the message step by step.  At the first step, we want
     * to reach the function code because all packets contain this
     * information. */
    step = _STEP_FUNCTION;
                                                         // read the first 13 bytes
    length_to_read = ctx->backend->header_length + 1;   //header_length default is 12


    if (msg_type == MSG_INDICATION) {    // for server, default is blocking for receive
        /* Wait for a message, we don't know when the message will be
         * received */
        if (ctx->indication_timeout.tv_sec == 0 && ctx->indication_timeout.tv_usec == 0) {
            /* By default, the indication timeout isn't set */
            p_tv = NULL;
        } else {
            /* Wait for an indication (name of a received request by a server, see schema) */
            tv.tv_sec = ctx->indication_timeout.tv_sec;
            tv.tv_usec = ctx->indication_timeout.tv_usec;
            p_tv = &tv;
        }
    } else {   // for client, default is wait for response_timeout!
        tv.tv_sec = ctx->response_timeout.tv_sec;    
        tv.tv_usec = ctx->response_timeout.tv_usec;
        p_tv = &tv;
    }

    while (length_to_read != 0) {
        rc = ctx->backend->select(ctx, &rset, p_tv, length_to_read);
        if (rc == -1) {
            _error_print(ctx, "select");
            if (ctx->error_recovery & DLT645_ERROR_RECOVERY_LINK) {
                int saved_errno = errno;

                if (errno == ETIMEDOUT) {
                    _sleep_response_timeout(ctx);
                    modbus_flush(ctx);
                } else if (errno == EBADF) {
                    modbus_close(ctx);
                    dlt645_connect(ctx);
                }
                errno = saved_errno;
            }
            return -1;
        }

        rc = ctx->backend->recv(ctx, msg + msg_length, length_to_read);
        if (rc == 0) {      // no response, default is -1, change to return 0; by william 20200814
            errno = ECONNRESET;
            rc = 0;
        }

        if (rc == -1) {
            _error_print(ctx, "read");
            if ((ctx->error_recovery & DLT645_ERROR_RECOVERY_LINK) &&
                (errno == ECONNRESET || errno == ECONNREFUSED ||
                 errno == EBADF)) {
                int saved_errno = errno;
                modbus_close(ctx);
                dlt645_connect(ctx);
                /* Could be removed by previous calls */
                errno = saved_errno;
            }
            return -1;
        }



        /* Display the hex code of each character received */
        if (ctx->debug) {
            int i;
            for (i=0; i < rc; i++)
                printf("<%.2X>", msg[msg_length + i]);
        }

        /* Sums bytes received */
        msg_length += rc;
        /* Computes remaining bytes */
        length_to_read -= rc;

        if (length_to_read == 0) {   // when read over
            switch (step) {
            case _STEP_FUNCTION:
                /* Function code position */
                length_to_read = compute_meta_length_after_function(
                    msg[ctx->backend->header_length],
                    msg_type);
                if (length_to_read != 0) {
                    step = _STEP_META;
                    break;
                } /* else switches straight to the next step */
            case _STEP_META:
                length_to_read = compute_data_length_after_meta(
                    ctx, msg, msg_type);
                if ((msg_length + length_to_read) > (int)ctx->backend->max_adu_length) {
                    errno = EMBBADDATA;
                    _error_print(ctx, "too many data");
                    return -1;
                }
                step = _STEP_DATA;
                break;
            default:
                break;
            }
        }

        if (length_to_read > 0 &&
            (ctx->byte_timeout.tv_sec > 0 || ctx->byte_timeout.tv_usec > 0)) {
            /* If there is no character in the buffer, the allowed timeout
               interval between two consecutive bytes is defined by
               byte_timeout */
            tv.tv_sec = ctx->byte_timeout.tv_sec;
            tv.tv_usec = ctx->byte_timeout.tv_usec;
            p_tv = &tv;
        }
        /* else timeout isn't set again, the full response must be read before
           expiration of response timeout (for CONFIRMATION only) */
    }

    if (ctx->debug)
        printf("\n");

    // copy to global buffer
    memcpy((uint8_t *)UartMesg.recvUartData, msg+msg_length, (uint64_t)rc);
    UartMesg.recvUartLen = rc;   // recv length

    return ctx->backend->check_integrity(ctx, msg, msg_length);
}

/* Receive the request from a modbus master */
int modbus_receive(dlt645_t *ctx, uint8_t *req)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->backend->receive(ctx, req);
}

/* Receives the confirmation.

   The function shall store the read response in rsp and return the number of
   values (bits or words). Otherwise, its shall return -1 and errno is set.

   The function doesn't check the confirmation is the expected response to the
   initial request.
*/
int modbus_receive_confirmation(dlt645_t *ctx, uint8_t *rsp)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return _dlt645_receive_msg(ctx, rsp, MSG_CONFIRMATION);
}

static int check_confirmation(dlt645_t *ctx, uint8_t *req,
                              uint8_t *rsp, int rsp_length)
{
    int rc;
    const int offset = ctx->backend->header_length;
    const int function = rsp[offset];
    int data_len = rsp_length - offset - 2 - 2 - 4;// 2:=funccode+lencode;2:=crc+endframe;4:=DI0-DI3

    if (ctx->backend->pre_check_confirmation) {   // no parameter?check weather function exist.
        rc = ctx->backend->pre_check_confirmation(ctx, req, rsp, rsp_length); // check response id, if success, return 0
        if (rc == -1) {
            if (ctx->error_recovery & DLT645_ERROR_RECOVERY_PROTOCOL) {
                _sleep_response_timeout(ctx);
                modbus_flush(ctx);
            }
            return -1;
        }
    }


    /* Exception code */    // response func code <=0x9C
    if (function >= 0x9C) {
        if (rsp_length == (offset + 2 + (int)ctx->backend->checksum_length) &&
            req[offset] == (rsp[offset] - 0x80)) {
            /* Valid exception code received */

            int exception_code = rsp[offset + 1];
            if (exception_code < DLT645_EXCEPTION_MAX) {
                errno = DLT645_ENOBASE + exception_code;
            } else {
                errno = EMBBADEXC;
            }
            _error_print(ctx, NULL);
            return -1;
        } else {
            errno = EMBBADEXC;
            _error_print(ctx, NULL);
            return -1;
        }
    }else {
        /* Check function code */
        if (function != (req[offset] | 0x80)) {
            if (ctx->debug) {
                fprintf(stderr,
                        "Received function not corresponding to the request (0x%X vs 0x%X)\n",
                        function, req[offset]);
            }
            if (ctx->error_recovery & DLT645_ERROR_RECOVERY_PROTOCOL) {
                _sleep_response_timeout(ctx);
                modbus_flush(ctx);
            }
            errno = EMBBADDATA;
            return -1;
        }


        /* Check the number of values is corresponding to the request */
        if((rsp[offset+1] - 4) != (data_len)){
            printf("Check ERROR: LEN\n");
            rc = -1;
        }

        if((rsp[offset+1] - 4) == 1){
            printf("Check ERROR: Receive Err Mesg!\n");
            rc = -1;

        }


        /* Check the DI0-DI3 */
        for(int i = 0; i < data_len+4; i++) // transfer all data area by sub 0x33, 4:=DI0-DI3
        {
            rsp[offset+1+1+i] -= 0x33;
        }
        if(memcmp(req+offset+2, rsp+offset+2, 4) != 0){
            printf("Check ERROR: DI0-DI3\n");
            return -1;

        }

    }

    return data_len;
}

static int response_io_status(uint8_t *tab_io_status,
                              int address, int nb,
                              uint8_t *rsp, int offset)
{
    int shift = 0;
    /* Instead of byte (not allowed in Win32) */
    int one_byte = 0;
    int i;

    for (i = address; i < address + nb; i++) {
        one_byte |= tab_io_status[i] << shift;
        if (shift == 7) {
            /* Byte is full */
            rsp[offset++] = one_byte;
            one_byte = shift = 0;
        } else {
            shift++;
        }
    }

    if (shift != 0)
        rsp[offset++] = one_byte;

    return offset;
}

/* Build the exception response */
static int response_exception(dlt645_t *ctx, sft_t *sft,
                              int exception_code, uint8_t *rsp,
                              unsigned int to_flush,
                              const char* template, ...)
{
    int rsp_length;

    /* Print debug message */
    if (ctx->debug) {
        va_list ap;

        va_start(ap, template);
        vfprintf(stderr, template, ap);
        va_end(ap);
    }

    /* Flush if required */
    if (to_flush) {
        _sleep_response_timeout(ctx);
        modbus_flush(ctx);
    }

    /* Build exception response */
    sft->function = sft->function + 0x80;
    rsp_length = ctx->backend->build_response_basis(sft, rsp);
    rsp[rsp_length++] = exception_code;

    return rsp_length;
}

/* Send a response to the received request.
   Analyses the request and constructs a response.

   If an error occurs, this function construct the response
   accordingly.
*/


int modbus_reply_exception(dlt645_t *ctx, const uint8_t *req,
                           unsigned int exception_code)
{
    int offset;
    int slave;
    int function;
    uint8_t rsp[MAX_MESSAGE_LENGTH];
    int rsp_length;
    int dummy_length = 99;
    sft_t sft;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    offset = ctx->backend->header_length;
    slave = req[offset - 1];
    function = req[offset];

    sft.slave = slave;
    sft.function = function + 0x80;
    sft.t_id = ctx->backend->prepare_response_tid(req, &dummy_length);
    rsp_length = ctx->backend->build_response_basis(&sft, rsp);

    /* Positive exception code */
    if (exception_code < DLT645_EXCEPTION_MAX) {
        rsp[rsp_length++] = exception_code;
        return send_msg(ctx, rsp, rsp_length);
    } else {
        errno = EINVAL;
        return -1;
    }
}



/* Reads the data from a remove device and put that data into an array */
// return number of datas
static int read_registers(dlt645_t *ctx, int function, uint32_t addr, void *dest, tRegType type)
{
    int rc;
    int req_length;
    uint8_t req[_MIN_REQ_LENGTH];
    uint8_t rsp[MAX_MESSAGE_LENGTH];
    uint8_t data[32];


    /**build and translate by add 0x33**/
    req_length = ctx->backend->build_request_basis(ctx, function, req);
    req_length = ctx->backend->build_request_read(ctx, addr, req, type);  //req_length=14

    /*****For Time Test ***********/
//    struct timeval start, end;
//    gettimeofday(&start, NULL);

    rc = send_msg(ctx, req, req_length);
    if (rc > 0) {

        rc = _dlt645_receive_msg(ctx, rsp, MSG_CONFIRMATION);   // blocking wait for, return length
        if (rc == -1)
            return -1;

//        gettimeofday(&end, NULL);
//        printf("$$$modbus_read_registers elapsed: %ld secs, %ld usecs\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
         /*****For Time Test***********/

        rc = check_confirmation(ctx, req, rsp, rc);   // check slaveid, function code, return data_len/-1;not include DI0-DI3
        if (rc == -1)
            return -1;

        int offset = (int)ctx->backend->header_length;
        memcpy(data, rsp+offset+6, (uint64_t)rc);    // put meta data to data[];6=function + 4DI + len

        /************************* according to different type**********************************/


        }
        if(type == ReadDataOne){
            if(rc > 4){
                fprintf(stderr,
                        "ERROR Too many registers received :%d\n",rc);
                errno = EMBMDATA;
                return -1;
            memcpy(dest, data, (uint64_t)rc);
            reverse_buffer(dest, 4);

        }else if(type == ReadDataSerial){
            // reverse data
                memcpy(dest, data, (uint64_t)rc);
                reverse_buffer(dest, rc);
        }
        else if(type == ReadDataMulti) {   // type = RegMulti   0x01 0x02 0x01 0x03 0x01 0x04

            for(int i = 0; i < rc/2; i++){
                *((uint32_t*)dest+i) = data[i << 1]  | data[1+(i << 1)] << 8;
            }

        }

    }

    return rc;
}

/* Reads the holding registers of remote device and put the data into an
   array */
int dlt645_read_registers(dlt645_t *ctx, uint32_t addr, void *dest, tRegType type)
{
    int status;

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }
    status = read_registers(ctx, DLT645_FC_READ_REGISTERS,
                            addr, dest, type);  // original data;


    return status;
}


/* Write a value to the specified register of the remote device.
   Used by write_bit and write_register */
static int write_single(dlt645_t *ctx, int function, uint32_t addr,
                        void *data, int len, tRegType type)
{
    int rc;
    int req_length;
    uint8_t req[_MIN_REQ_LENGTH];

    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    req_length = ctx->backend->build_request_basis(ctx, function, req);    //put data to req
    req_length = ctx->backend->build_request_write(ctx, addr, data, len, req, type);

    rc = send_msg(ctx, req, req_length);    // send arrayof req , length to serial!

    if (rc > 0) {    // if send successfully
        /* Used by write_bit and write_register */
        uint8_t rsp[MAX_MESSAGE_LENGTH];

        rc = _dlt645_receive_msg(ctx, rsp, MSG_CONFIRMATION);
        if (rc == -1)    // if no recv, -1 ; crc:-1
            return -1;

        rc = check_confirmation(ctx, req, rsp, rc);  //check crc, length, function

        //    if check error, we want to return -2;
        if(rc == -1)  // add by william 20200814
            return -2;
    }

    return rc;
}

/* Writes a value in one register of the remote device */
int dlt645_write_register(dlt645_t *ctx, uint32_t addr, void *data, int len, tRegType type)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return write_single(ctx, DLT645_FC_DEV_CTRL, addr, data, len, type);
}


void _dlt645_init_common(dlt645_t *ctx)
{
    /* Slave initialized to NULL, socket initialized to -1 */
    memset(ctx->slaveid, 0, 6);
    ctx->s = -1;

    ctx->debug = FALSE;
    ctx->error_recovery = DLT645_ERROR_RECOVERY_NONE;

    ctx->response_timeout.tv_sec = 0;
//    ctx->response_timeout.tv_usec = _RESPONSE_TIMEOUT;  changto following code unit:ms, xcd 20201012
    ctx->response_timeout.tv_usec = ResponseTimeOut*1000;

    ctx->byte_timeout.tv_sec = 0;
    ctx->byte_timeout.tv_usec = _BYTE_TIMEOUT;

    ctx->indication_timeout.tv_sec = 0;
    ctx->indication_timeout.tv_usec = 0;
}

/* Define the slave number */
int dlt645_set_slave(dlt645_t *ctx, const uint64_t slaveid)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->backend->set_slave(ctx, slaveid);
}


int dlt645_set_password(dlt645_t *ctx, const uint32_t pass)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->backend->set_slave(ctx, pass);

}
int dlt645_set_user(dlt645_t *ctx, const uint32_t user)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->backend->set_slave(ctx, user);

}



uint8_t* modbus_get_slave(dlt645_t *ctx)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return NULL;
    }

    return ctx->slaveid;
}

int modbus_set_error_recovery(dlt645_t *ctx,
                              modbus_error_recovery_mode error_recovery)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* The type of modbus_error_recovery_mode is unsigned enum */
    ctx->error_recovery = (uint8_t) error_recovery;
    return 0;
}

int modbus_set_socket(dlt645_t *ctx, int s)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    ctx->s = s;
    return 0;
}

int modbus_get_socket(dlt645_t *ctx)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->s;
}

/* Get the timeout interval used to wait for a response */
int modbus_get_response_timeout(dlt645_t *ctx, uint32_t *to_sec, uint32_t *to_usec)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    *to_sec = ctx->response_timeout.tv_sec;
    *to_usec = ctx->response_timeout.tv_usec;
    return 0;
}

int modbus_set_response_timeout(dlt645_t *ctx, uint32_t to_sec, uint32_t to_usec)
{
    if (ctx == NULL ||
        (to_sec == 0 && to_usec == 0) || to_usec > 999999) {
        errno = EINVAL;
        return -1;
    }

    ctx->response_timeout.tv_sec = to_sec;
    ctx->response_timeout.tv_usec = to_usec;
    return 0;
}

/* Get the timeout interval between two consecutive bytes of a message */
int modbus_get_byte_timeout(dlt645_t *ctx, uint32_t *to_sec, uint32_t *to_usec)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    *to_sec = ctx->byte_timeout.tv_sec;
    *to_usec = ctx->byte_timeout.tv_usec;
    return 0;
}

int modbus_set_byte_timeout(dlt645_t *ctx, uint32_t to_sec, uint32_t to_usec)
{
    /* Byte timeout can be disabled when both values are zero */
    if (ctx == NULL || to_usec > 999999) {
        errno = EINVAL;
        return -1;
    }

    ctx->byte_timeout.tv_sec = to_sec;
    ctx->byte_timeout.tv_usec = to_usec;
    return 0;
}

/* Get the timeout interval used by the server to wait for an indication from a client */
int modbus_get_indication_timeout(dlt645_t *ctx, uint32_t *to_sec, uint32_t *to_usec)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    *to_sec = ctx->indication_timeout.tv_sec;
    *to_usec = ctx->indication_timeout.tv_usec;
    return 0;
}

int modbus_set_indication_timeout(dlt645_t *ctx, uint32_t to_sec, uint32_t to_usec)
{
    /* Indication timeout can be disabled when both values are zero */
    if (ctx == NULL || to_usec > 999999) {
        errno = EINVAL;
        return -1;
    }

    ctx->indication_timeout.tv_sec = to_sec;
    ctx->indication_timeout.tv_usec = to_usec;
    return 0;
}

int modbus_get_header_length(dlt645_t *ctx)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->backend->header_length;
}

int dlt645_connect(dlt645_t *ctx)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ctx->backend->connect(ctx);
}

void modbus_close(dlt645_t *ctx)
{
    if (ctx == NULL)
        return;

    ctx->backend->close(ctx);
}

void dlt645_free(dlt645_t *ctx)
{
    if (ctx == NULL)
        return;

    ctx->backend->free(ctx);
}

int dlt645_set_debug(dlt645_t *ctx, int flag)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    ctx->debug = flag;
    return 0;
}

