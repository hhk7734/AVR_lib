/**
 * @file LOT_print.h
 * @date 2018/11
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief print 함수를 통해 숫자를 송신
 */

#ifndef _LOT_PRINT_H_
#define _LOT_PRINT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

#include "LOT_uart0.h"
#define LOT_print_char( x ) LOT_uart0_write( x )
#define LOT_print_str( x ) LOT_uart0_print_str( x )

#define _LOT_PRINT_NL_CR_
#define _LOT_PRINT_8_BIT_
#define _LOT_PRINT_16_BIT_
#define _LOT_PRINT_32_BIT_
#define _LOT_PRINT_64_BIT_
#define _LOT_PRINT_FLOAT_

#ifdef _LOT_PRINT_NL_CR_
void LOT_print_newline( void );
#endif    // _LOT_PRINT_NL_CR_
#ifdef _LOT_PRINT_8_BIT_
void LOT_print_u8( uint8_t data );
void LOT_print_i8( int8_t data );
#endif    // _LOT_PRINT_8_BIT_
#ifdef _LOT_PRINT_16_BIT_
void LOT_print_u16( uint16_t data );
void LOT_print_i16( int16_t data );
#endif    // _LOT_PRINT_16_BIT_
#ifdef _LOT_PRINT_32_BIT_
void LOT_print_u32( uint32_t data );
void LOT_print_i32( int32_t data );
#endif    // _LOT_PRINT_32_BIT_
#ifdef _LOT_PRINT_64_BIT_
void LOT_print_u64( uint64_t data );
void LOT_print_i64( int64_t data );
#endif    // _LOT_PRINT_64_BIT_
#ifdef _LOT_PRINT_FLOAT_
void LOT_print_f( float data, uint8_t digits );
#endif    // _LOT_PRINT_FLOAT_

#ifdef __cplusplus
}
#endif
#endif    // _LOT_PRINT_H_