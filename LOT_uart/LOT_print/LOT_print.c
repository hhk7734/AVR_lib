/**
 * @file LOT_print.c
 * @date 2018/11
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief print 함수를 통해 숫자를 송신
 */

#include "LOT_print.h"
#ifdef _LOT_PRINT_FLOAT_
#    include <math.h>
#endif

#define LOT_print_uint( buf_size, data, temp )       \
    char  buf[buf_size];                             \
    char *str = &buf[buf_size - 1];                  \
    *str      = '\0';                                \
    do                                               \
    {                                                \
        temp           = data / 10;                  \
        char remainder = ( data - temp * 10 ) + '0'; \
        *--str         = remainder;                  \
        data           = temp;                       \
    } while( data );                                 \
    LOT_print_str( str )

#ifdef _LOT_PRINT_NL_CR_

void LOT_newline( void )
{
#    ifdef _LOT_PRINT_NL_CR_
    LOT_print_str( "\n\r" );
#    endif
}
#endif

#ifdef _LOT_PRINT_8_BIT_
void LOT_print_u8( uint8_t data )
{
    uint8_t temp;
    LOT_print_uint( 4, data, temp );
}

void LOT_print_i8( int8_t data )
{
    if( data < 0 )
    {
        LOT_print_char( '-' );
        data = -data;
    }
    LOT_print_u8( ( uint8_t )data );
}
#endif    // _LOT_PRINT_8_BIT_

#ifdef _LOT_PRINT_16_BIT_
void LOT_print_u16( uint16_t data )
{
    uint16_t temp;
    LOT_print_uint( 6, data, temp );
}

void LOT_print_i16( int16_t data )
{
    if( data < 0 )
    {
        LOT_print_char( '-' );
        data = -data;
    }
    LOT_print_u16( ( uint16_t )data );
}
#endif    // _LOT_PRINT_16_BIT_

#if defined( _LOT_PRINT_32_BIT_ ) || defined( _LOT_PRINT_FLOAT_ )
void LOT_print_u32( uint32_t data )
{
    uint32_t temp;
    LOT_print_uint( 11, data, temp );
}
#endif    // _LOT_PRINT_32_BIT_ || _LOT_PRINT_FLOAT_

#ifdef _LOT_PRINT_32_BIT_
void LOT_print_i32( int32_t data )
{
    if( data < 0 )
    {
        LOT_print_char( '-' );
        data = -data;
    }
    LOT_print_u32( ( uint32_t )data );
}
#endif    // _LOT_PRINT_32_BIT_

#ifdef _LOT_PRINT_64_BIT_
void LOT_print_u64( uint64_t data )
{
    uint64_t temp;
    LOT_print_uint( 21, data, temp );
}

void LOT_print_i64( int64_t data )
{
    if( data < 0 )
    {
        LOT_print_char( '-' );
        data = -data;
    }
    LOT_print_u64( ( uint64_t )data );
}
#endif    // _LOT_PRINT_64_BIT_

#ifdef _LOT_PRINT_FLOAT_
void LOT_print_f( float data, uint8_t digits )
{
    if( isnan( data ) )
    {
        LOT_print_str( "nan" );
        return;
    }
    else if( isinf( data ) )
    {
        LOT_print_str( "inf" );
        return;
    }
    else if( data > 4294967040.0 )
    {
        LOT_print_str( "ovf" );
        return;
    }
    else if( data < -4294967040.0 )
    {
        LOT_print_str( "ovf" );
        return;
    }

    if( data < 0.0 )
    {
        LOT_print_char( '-' );
        data = -data;
    }

    float rounding = 0.5;
    for( uint8_t i = 0; i < digits; ++i ) { rounding /= 10.0; }
    data += rounding;

    uint32_t _int = ( uint32_t )data;
    LOT_print_u32( _int );
    LOT_print_char( '.' );

    float _dec = data - ( float )_int;

    while( digits-- )
    {
        _dec *= 10.0;
        uint8_t toPrint = ( uint8_t )( _dec );
        LOT_print_char( toPrint + '0' );
        _dec -= toPrint;
    }
}
#endif    // _LOT_PRINT_FLOAT_

/// @}