/**
 * @file LOT_spi0.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief
 */

#include "LOT_spi0.h"

LOT_spi0::LOT_spi0()
    : error_count( 0 )
{
}

void LOT_spi0::setup( LOT_spi_data_order_typedef        data_order,
                      LOT_spi_clk_idle_state_typedef    clk_idle,
                      LOT_spi_clk_sampling_edge_typedef clk_sampling_edge,
                      LOT_spi_clk_divider_typedef       clk_divider )
{
    /// MOSI output
    /// MISO input
    /// SCK output
#if defined( __AVR_ATmega328P__ )
    DDRB |= _BV( DDB3 );
    DDRB &= ~_BV( DDB4 );
    DDRB |= _BV( DDB5 );
#endif

    SPCR = _BV( SPE ) | _BV( MSTR );

    if( data_order == LOT_SPI_LSB_FIRST ) { SPCR |= _BV( DORD ); }

    if( clk_idle == LOT_SPI_CLK_IDLE_HIGH ) { SPCR |= _BV( CPOL ); }

    if( clk_sampling_edge == LOT_SPI_CLK_SAMPLING_2_EDGE ) { SPCR |= _BV( CPHA ); }

    SPSR &= ~_BV( SPI2X );
    switch( clk_divider )
    {
        case LOT_SPI_SCK_DIV_2: SPSR |= _BV( SPI2X );
        case LOT_SPI_SCK_DIV_4: break;

        case LOT_SPI_SCK_DIV_8: SPSR |= _BV( SPI2X );
        case LOT_SPI_SCK_DIV_16: SPCR |= _BV( SPR0 ); break;

        case LOT_SPI_SCK_DIV_32: SPSR |= _BV( SPI2X );
        case LOT_SPI_SCK_DIV_64: SPCR |= _BV( SPR1 ); break;

        case LOT_SPI_SCK_DIV_128: SPCR |= _BV( SPR1 ) | _BV( SPR0 ); break;
    }
}

LOT_status_typedef
    LOT_spi0::transmit( const uint8_t register_address, const uint8_t *data, uint8_t size )
{
    if( transmit( register_address ) == LOT_OK )
    {
        for( ; size > 0; --size ) { transmit( *data ); }
        return LOT_OK;
    }
    else
    {
        error();
        return LOT_ERROR;
    }
}

LOT_status_typedef LOT_spi0::transmit( const uint8_t register_address, const uint8_t data )
{
    transmit( register_address );
    return transmit( data );
}

LOT_status_typedef LOT_spi0::receive( const uint8_t register_address, uint8_t *data, uint8_t size )
{
    if( transmit( register_address | LOT_SPI0_READ_MASK ) == LOT_OK )
    {
        for( ; size > 0; --size )
        {
            transmit( 0 );
            *data++ = SPDR;
        }
        return LOT_OK;
    }
    else
    {
        error();
        return LOT_ERROR;
    }
}

uint8_t LOT_spi0::receive( const uint8_t register_address )
{
    transmit( register_address | LOT_SPI0_READ_MASK );
    transmit( 0 );
    return SPDR;
}

LOT_status_typedef LOT_spi0::transceive( uint8_t *data, uint8_t size )
{
    if( transmit( *data ) == LOT_OK )
    {
        for( ; size > 1; --size )
        {
            *data++ = SPDR;
            transmit( *data );
        }
        *data = SPDR;
        return LOT_OK;
    }
    else
    {
        error();
        return LOT_ERROR;
    }
}

void LOT_spi0::error( void )
{
    ++error_count;
    if( error_count > 10 )
    {
        for( ;; ) {}
    }
}

LOT_spi0 spi0;