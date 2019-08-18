/**
 * @file LOT_tc2_time.c
 * @date 2017/03
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief TC2, 전원 인가 후 시간
 */

#include "LOT_tc2_time.h"

#include <avr/interrupt.h>
#include <avr/io.h>

volatile static uint32_t tc2_isr_count  = 0;
volatile static uint32_t tc2_isr_millis = 0;
volatile static uint8_t  tc2_remainder  = 0;

ISR( TIMER2_OVF_vect )
{
    ++tc2_isr_count;
    ++tc2_isr_millis;
    tc2_remainder += 3;
    if( tc2_remainder > 124 )
    {
        ++tc2_isr_millis;
        tc2_remainder -= 125;
    }
}

void LOT_tc2_time_setup( void )
{
    TCCR2A = 0;
#if F_CPU == 8000000UL
    TCCR2B = _BV( CS21 ) | _BV( CS20 );
#elif F_CPU == 16000000UL
    TCCR2B = _BV( CS22 );
#endif
    TIMSK2 = _BV( TOIE2 );
    sei();
}

uint32_t LOT_micros( void )
{
    uint16_t tcnt;
    uint32_t count;
    cli();
    count = tc2_isr_count;
    tcnt  = TCNT2;
    if( ( TIFR2 & _BV( TOV2 ) ) && tcnt < 32 ) { ++count; }
    sei();
    return ( count << 10 ) + ( tcnt << 2 );
}

uint32_t LOT_millis( void ) { return tc2_isr_millis; }