/*
 * Copyright © 2001-2013 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#ifndef DLT645_H
#define DLT645_H

/* Add this for macros that defined unix flavor */
#if (defined(__unix__) || defined(unix)) && !defined(USG)
#include <sys/param.h>
#endif

#ifndef _MSC_VER
#include <stdint.h>
#else
#include "stdint.h"
#endif

#include "modbus-version.h"
#include "globalFun.h"

#if defined (_WIN32)
#pragma comment(lib,"ws2_32.lib")
#endif



#if defined(_MSC_VER)
# if defined(DLLBUILD)
/* define DLLBUILD when building the DLL */
#  define DLT645_API __declspec(dllexport)
# else
#  define DLT645_API __declspec(dllimport)
# endif
#else
# define DLT645_API
#endif

#ifdef  __cplusplus
# define DLT645_BEGIN_DECLS  extern "C" {
# define DLT645_END_DECLS    }
#else
# define DLT645_BEGIN_DECLS
# define DLT645_END_DECLS
#endif

DLT645_BEGIN_DECLS

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef ON
#define ON 1
#endif

/* Modbus function codes */
#define DLT645_FC_READ_COILS                0x01
#define DLT645_FC_READ_DISCRETE_INPUTS      0x02
#define DLT645_FC_READ_REGISTERS            0x11
#define DLT645_FC_READ_CONTINUE_REGISTERS   0x12
#define DLT645_FC_READ_ADDR                 0x13

#define DLT645_FC_WRITE_DATA                0x14
#define DLT645_FC_WRITE_ADDR                0x15
#define DLT645_FC_CHANGE_BAURD              0x17
#define DLT645_FC_CHANGE_PASSWD             0x18
#define DLT645_FC_DEV_WARN                  0x19
#define DLT645_FC_DEV_WARN_RSP              0x1A
#define DLT645_FC_EVENT_CLEAR               0x1B
#define DLT645_FC_DEV_CTRL                  0x1C


#define DLT645_BROADCAST_ADDRESS    0

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 1 page 12)
 * Quantity of Coils to read (2 bytes): 1 to 2000 (0x7D0)
 * (chapter 6 section 11 page 29)
 * Quantity of Coils to write (2 bytes): 1 to 1968 (0x7B0)
 */
#define DLT645_MAX_READ_BITS              2000
#define DLT645_MAX_WRITE_BITS             1968

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 3 page 15)
 * Quantity of Registers to read (2 bytes): 1 to 125 (0x7D)
 * (chapter 6 section 12 page 31)
 * Quantity of Registers to write (2 bytes) 1 to 123 (0x7B)
 * (chapter 6 section 17 page 38)
 * Quantity of Registers to write in R/W registers (2 bytes) 1 to 121 (0x79)
 */
#define DLT645_MAX_READ_REGISTERS          200    // no more than 200
#define DLT645_MAX_WRITE_REGISTERS         123
#define DLT645_MAX_WR_WRITE_REGISTERS      121
#define DLT645_MAX_WR_READ_REGISTERS       125

/* The size of the DLT645 PDU is limited by the size constraint inherited from
 * the first DLT645 implementation on Serial Line network (max. RS485 ADU = 256
 * bytes). Therefore, DLT645 PDU for serial line communication = 256 - Server
 * address (1 byte) - CRC (2 bytes) = 253 bytes.
 */
#define DLT645_MAX_PDU_LENGTH              253

/* Consequently:
 * - RTU DLT645 ADU = 253 bytes + Server address (1 byte) + CRC (2 bytes) = 256
 *   bytes.
 * - TCP DLT645 ADU = 253 bytes + MBAP (7 bytes) = 260 bytes.
 * so the maximum of both backend in 260 bytes. This size can used to allocate
 * an array of bytes to store responses and it will be compatible with the two
 * backends.
 */
#define DLT645_MAX_ADU_LENGTH              260

/* Random number to avoid errno conflicts */
#define DLT645_ENOBASE 112345678

/* Protocol exceptions */
enum {
    DLT645_EXCEPTION_ILLEGAL_FUNCTION = 0x01,
    DLT645_EXCEPTION_ILLEGAL_DATA_ADDRESS,
    DLT645_EXCEPTION_ILLEGAL_DATA_VALUE,
    DLT645_EXCEPTION_SLAVE_OR_SERVER_FAILURE,
    DLT645_EXCEPTION_ACKNOWLEDGE,
    DLT645_EXCEPTION_SLAVE_OR_SERVER_BUSY,
    DLT645_EXCEPTION_NEGATIVE_ACKNOWLEDGE,
    DLT645_EXCEPTION_MEMORY_PARITY,
    DLT645_EXCEPTION_NOT_DEFINED,
    DLT645_EXCEPTION_GATEWAY_PATH,
    DLT645_EXCEPTION_GATEWAY_TARGET,
    DLT645_EXCEPTION_MAX
};

#define EMBXILFUN  (DLT645_ENOBASE + DLT645_EXCEPTION_ILLEGAL_FUNCTION)
#define EMBXILADD  (DLT645_ENOBASE + DLT645_EXCEPTION_ILLEGAL_DATA_ADDRESS)
#define EMBXILVAL  (DLT645_ENOBASE + DLT645_EXCEPTION_ILLEGAL_DATA_VALUE)
#define EMBXSFAIL  (DLT645_ENOBASE + DLT645_EXCEPTION_SLAVE_OR_SERVER_FAILURE)
#define EMBXACK    (DLT645_ENOBASE + DLT645_EXCEPTION_ACKNOWLEDGE)
#define EMBXSBUSY  (DLT645_ENOBASE + DLT645_EXCEPTION_SLAVE_OR_SERVER_BUSY)
#define EMBXNACK   (DLT645_ENOBASE + DLT645_EXCEPTION_NEGATIVE_ACKNOWLEDGE)
#define EMBXMEMPAR (DLT645_ENOBASE + DLT645_EXCEPTION_MEMORY_PARITY)
#define EMBXGPATH  (DLT645_ENOBASE + DLT645_EXCEPTION_GATEWAY_PATH)
#define EMBXGTAR   (DLT645_ENOBASE + DLT645_EXCEPTION_GATEWAY_TARGET)

/* Native libmodbus error codes */
#define EMBBADCRC  (EMBXGTAR + 1)
#define EMBBADDATA (EMBXGTAR + 2)
#define EMBBADEXC  (EMBXGTAR + 3)
#define EMBUNKEXC  (EMBXGTAR + 4)
#define EMBMDATA   (EMBXGTAR + 5)
#define EMBBADSLAVE (EMBXGTAR + 6)

extern const unsigned int libmodbus_version_major;
extern const unsigned int libmodbus_version_minor;
extern const unsigned int libmodbus_version_micro;


typedef struct _dlt645 dlt645_t;

typedef struct _dlt645_mapping_t {
    int nb_bits;
    int start_bits;
    int nb_input_bits;
    int start_input_bits;
    int nb_input_registers;
    int start_input_registers;
    int nb_registers;
    int start_registers;
    uint8_t *tab_bits;
    uint8_t *tab_input_bits;
    uint16_t *tab_input_registers;
    uint16_t *tab_registers;
} modbus_mapping_t;

typedef enum
{
    DLT645_ERROR_RECOVERY_NONE          = 0,
    DLT645_ERROR_RECOVERY_LINK          = (1<<1),
    DLT645_ERROR_RECOVERY_PROTOCOL      = (1<<2)
} modbus_error_recovery_mode;

DLT645_API int dlt645_set_slave(dlt645_t *ctx, const uint64_t slaveid);
DLT645_API int dlt645_set_password(dlt645_t *ctx, const uint32_t pass);
DLT645_API int dlt645_set_user(dlt645_t *ctx, const uint32_t user);
DLT645_API uint8_t* modbus_get_slave(dlt645_t* ctx);
DLT645_API int modbus_set_error_recovery(dlt645_t *ctx, modbus_error_recovery_mode error_recovery);
DLT645_API int modbus_set_socket(dlt645_t *ctx, int s);
DLT645_API int modbus_get_socket(dlt645_t *ctx);

DLT645_API int modbus_get_response_timeout(dlt645_t *ctx, uint32_t *to_sec, uint32_t *to_usec);
DLT645_API int modbus_set_response_timeout(dlt645_t *ctx, uint32_t to_sec, uint32_t to_usec);

DLT645_API int modbus_get_byte_timeout(dlt645_t *ctx, uint32_t *to_sec, uint32_t *to_usec);
DLT645_API int modbus_set_byte_timeout(dlt645_t *ctx, uint32_t to_sec, uint32_t to_usec);

DLT645_API int modbus_get_indication_timeout(dlt645_t *ctx, uint32_t *to_sec, uint32_t *to_usec);
DLT645_API int modbus_set_indication_timeout(dlt645_t *ctx, uint32_t to_sec, uint32_t to_usec);

DLT645_API int modbus_get_header_length(dlt645_t *ctx);

DLT645_API int dlt645_connect(dlt645_t *ctx);
DLT645_API void modbus_close(dlt645_t *ctx);

DLT645_API void dlt645_free(dlt645_t *ctx);

DLT645_API int modbus_flush(dlt645_t *ctx);
DLT645_API int dlt645_set_debug(dlt645_t *ctx, int flag);

DLT645_API const char *modbus_strerror(int errnum);
DLT645_API int dlt645_read_registers(dlt645_t *ctx, uint32_t addr, void *dest, tRegType type);
DLT645_API int dlt645_write_register(dlt645_t *ctx, uint32_t addr, void *data, int len, tRegType type);
DLT645_API int modbus_report_slave_id(dlt645_t *ctx, int max_dest, uint8_t *dest);
DLT645_API modbus_mapping_t* modbus_mapping_new_start_address(
    unsigned int start_bits, unsigned int nb_bits,
    unsigned int start_input_bits, unsigned int nb_input_bits,
    unsigned int start_registers, unsigned int nb_registers,
    unsigned int start_input_registers, unsigned int nb_input_registers);

DLT645_API modbus_mapping_t* modbus_mapping_new(int nb_bits, int nb_input_bits,
                                                int nb_registers, int nb_input_registers);
DLT645_API void modbus_mapping_free(modbus_mapping_t *mb_mapping);

DLT645_API int modbus_send_raw_request(dlt645_t *ctx, const uint8_t *raw_req, int raw_req_length);

DLT645_API int modbus_receive(dlt645_t *ctx, uint8_t *req);

DLT645_API int modbus_receive_confirmation(dlt645_t *ctx, uint8_t *rsp);


DLT645_API int modbus_reply_exception(dlt645_t *ctx, const uint8_t *req,
                                      unsigned int exception_code);

/**
 * UTILS FUNCTIONS
 **/

#define DLT645_GET_HIGH_BYTE(data) (((data) >> 8) & 0xFF)
#define DLT645_GET_LOW_BYTE(data) ((data) & 0xFF)
#define DLT645_GET_INT64_FROM_INT16(tab_int16, index) \
    (((int64_t)tab_int16[(index)    ] << 48) + \
     ((int64_t)tab_int16[(index) + 1] << 32) + \
     ((int64_t)tab_int16[(index) + 2] << 16) + \
      (int64_t)tab_int16[(index) + 3])
#define DLT645_GET_INT32_FROM_INT16(tab_int16, index) ((tab_int16[(index)] << 16) + tab_int16[(index) + 1])
#define DLT645_GET_INT16_FROM_INT8(tab_int8, index) ((tab_int8[(index)] << 8) + tab_int8[(index) + 1])
#define DLT645_SET_INT16_TO_INT8(tab_int8, index, value) \
    do { \
        tab_int8[(index)] = (value) >> 8;  \
        tab_int8[(index) + 1] = (value) & 0xFF; \
    } while (0)
#define DLT645_SET_INT32_TO_INT16(tab_int16, index, value) \
    do { \
        tab_int16[(index)    ] = (value) >> 16; \
        tab_int16[(index) + 1] = (value); \
    } while (0)
#define DLT645_SET_INT64_TO_INT16(tab_int16, index, value) \
    do { \
        tab_int16[(index)    ] = (value) >> 48; \
        tab_int16[(index) + 1] = (value) >> 32; \
        tab_int16[(index) + 2] = (value) >> 16; \
        tab_int16[(index) + 3] = (value); \
    } while (0)

DLT645_API void modbus_set_bits_from_byte(uint8_t *dest, int idx, const uint8_t value);
DLT645_API void modbus_set_bits_from_bytes(uint8_t *dest, int idx, unsigned int nb_bits,
                                       const uint8_t *tab_byte);
DLT645_API uint8_t modbus_get_byte_from_bits(const uint8_t *src, int idx, unsigned int nb_bits);
DLT645_API float modbus_get_float(const uint16_t *src);
DLT645_API float modbus_get_float_abcd(const uint16_t *src);
DLT645_API float modbus_get_float_dcba(const uint16_t *src);
DLT645_API float modbus_get_float_badc(const uint16_t *src);
DLT645_API float modbus_get_float_cdab(const uint16_t *src);

DLT645_API void modbus_set_float(float f, uint16_t *dest);
DLT645_API void modbus_set_float_abcd(float f, uint16_t *dest);
DLT645_API void modbus_set_float_dcba(float f, uint16_t *dest);
DLT645_API void modbus_set_float_badc(float f, uint16_t *dest);
DLT645_API void modbus_set_float_cdab(float f, uint16_t *dest);

#include "modbus-tcp.h"
#include "dlt645-rtu.h"

DLT645_END_DECLS

#endif  /* DLT645_H */
