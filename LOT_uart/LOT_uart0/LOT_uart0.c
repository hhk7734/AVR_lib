/**
 * @file LOT_uart0.c
 * @date 2017/03
 * @date 2018/09
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief UART 통신
 */

#include "LOT_uart0.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#ifdef _LOT_UART0_FLOAT_
#    include <math.h>
#endif    // _LOT_UART0_FLOAT_

#define LOT_UCSR0A UCSR0A
#define LOT_UCSR0B UCSR0B
#define LOT_UCSR0C UCSR0C
#define LOT_UBRR0L UBRR0L
#define LOT_UBRR0H UBRR0H
#define LOT_UBRR0 UBRR0
#define LOT_UDR0 UDR0

volatile static uint8_t tx_buf_head                   = 0;
volatile static uint8_t tx_buf_tail                   = 0;
volatile static uint8_t tx_buf[LOT_UART0_TX_BUF_SIZE] = { 0 };
volatile static uint8_t rx_buf_head                   = 0;
volatile static uint8_t rx_buf_tail                   = 0;
volatile static uint8_t rx_buf[LOT_UART0_RX_BUF_SIZE] = { 0 };

ISR( USART_RX_vect )
{
    rx_buf[rx_buf_head] = LOT_UDR0;
    rx_buf_head         = ( rx_buf_head + 1 ) % LOT_UART0_RX_BUF_SIZE;
    if( rx_buf_head == rx_buf_tail ) { rx_buf_tail = ( rx_buf_tail + 1 ) % LOT_UART0_RX_BUF_SIZE; }
}

ISR( USART_UDRE_vect )
{
    LOT_UDR0    = tx_buf[tx_buf_tail];
    tx_buf_tail = ( tx_buf_tail + 1 ) % LOT_UART0_TX_BUF_SIZE;
    LOT_UCSR0A |= _BV( TXC0 );
    if( tx_buf_head == tx_buf_tail ) { LOT_UCSR0B &= ~_BV( UDRIE0 ); }
}

void LOT_uart0_begin( const uint32_t baud_rate )
{
    LOT_UCSR0A = 0;
    LOT_UCSR0B = _BV( RXCIE0 ) | _BV( RXEN0 ) | _BV( TXEN0 );
    LOT_UCSR0C = _BV( UCSZ01 ) | _BV( UCSZ00 );
    LOT_UBRR0  = ( uint16_t )( ( F_CPU / baud_rate / 16 ) - 1 );
    sei();
}

uint8_t LOT_uart0_read( void )
{
    if( rx_buf_head == rx_buf_tail )
        return 0;
    else
    {
        uint8_t buf = rx_buf[rx_buf_tail];
        rx_buf_tail = ( rx_buf_tail + 1 ) % LOT_UART0_RX_BUF_SIZE;
        return buf;
    }
}

void LOT_uart0_write( const uint8_t data )
{
    if( ( tx_buf_head == tx_buf_tail ) && ( LOT_UCSR0A & _BV( UDRE0 ) ) )
    {
        LOT_UDR0 = data;
        LOT_UCSR0A |= _BV( TXC0 );
        return;
    }
    tx_buf[tx_buf_head] = data;
    tx_buf_head         = ( tx_buf_head + 1 ) % LOT_UART0_TX_BUF_SIZE;
    while( tx_buf_head == tx_buf_tail ) {}
    LOT_UCSR0B |= _BV( UDRIE0 );
}

void LOT_uart0_print_str( const char *str )
{
    while( *str ) { LOT_uart0_write( *str++ ); }
}

uint8_t LOT_uart0_available( void )
{
    return ( LOT_UART0_RX_BUF_SIZE + rx_buf_head - rx_buf_tail ) % LOT_UART0_RX_BUF_SIZE;
}

void LOT_uart0_flush( void )
{
    rx_buf_head = 0;
    rx_buf_tail = 0;
    rx_buf[0]   = 0;
}
