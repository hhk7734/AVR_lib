/**
 * @file LOT_spi0.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief
 */

#ifndef _LOT_SPI0_H_
#define _LOT_SPI0_H_

#include <avr/io.h>

#if !defined( __AVR_ATmega328P__ )
#    warning "Untested device"
#endif

#define LOT_SPI0_TIME_OUT 255

/// 레지스터의 값을 읽어올 때, 레지스터 주소에 마스크를 설정해야 하는 경우가 많음
#define LOT_SPI0_READ_MASK 0x80

/// SPCR, SPSR, SPDR
/// DDRB

/// SPCR
/// SPIE, SPE, DORD, MSTR, CPOL, CPHA, SPR1, SPR0

/// SPSR
/// SPIF, WCOL, SPI2X

#ifndef _LOT_STATUS_TYPEDEF_
#    define _LOT_STATUS_TYPEDEF_
typedef enum
{
    LOT_OK = 0x00,
    LOT_ERROR,
    LOT_BUSY,
    LOT_TIMEOUT
} LOT_status_typedef;
#endif

typedef enum
{
    LOT_SPI_SCK_DIV_2 = 0x00,
    LOT_SPI_SCK_DIV_4,
    LOT_SPI_SCK_DIV_8,
    LOT_SPI_SCK_DIV_16,
    LOT_SPI_SCK_DIV_32,
    LOT_SPI_SCK_DIV_64,
    LOT_SPI_SCK_DIV_128
} LOT_spi_clk_divider_typedef;

typedef enum
{
    LOT_SPI_MSB_FIRST = 0x00,
    LOT_SPI_LSB_FIRST
} LOT_spi_data_order_typedef;

typedef enum
{
    LOT_SPI_CLK_IDLE_LOW = 0x00,
    LOT_SPI_CLK_IDLE_HIGH
} LOT_spi_clk_idle_state_typedef;

typedef enum
{
    LOT_SPI_CLK_SAMPLING_1_EDGE = 0x00,
    LOT_SPI_CLK_SAMPLING_2_EDGE
} LOT_spi_clk_sampling_edge_typedef;

class LOT_spi0 {
public:
    LOT_spi0();

    /**
     * @brief 통신 설정
     * @param LOT_spi_data_order_typedef data_order MSB, LSB 중 선택
     * @param LOT_spi_clk_idle_state_typedef clk_idle LOW, HIGH 중 선택
     * @param LOT_spi_clk_sampling_edge_typedef clk_sampling_edge 1 edge, 2 edge 중 선택
     * @param LOT_spi_clk_divider_typedef clk_divider 2, 4, 8, 16, 32, 64, 128 중 선택
     */
    void        setup( LOT_spi_data_order_typedef        data_order,
                       LOT_spi_clk_idle_state_typedef    clk_idle,
                       LOT_spi_clk_sampling_edge_typedef clk_sampling_edge,
                       LOT_spi_clk_divider_typedef       clk_divider );
    inline void setup( LOT_spi_clk_divider_typedef clk_divider )
    {
        setup( LOT_SPI_MSB_FIRST, LOT_SPI_CLK_IDLE_LOW, LOT_SPI_CLK_SAMPLING_1_EDGE, clk_divider );
    }

    /**
     * @brief 데이터 송신
     * @param uint8_t data
     * @return LOT_status_typedef
     */
    LOT_status_typedef transmit( const uint8_t data );

    /**
     * @brief 레지스터에 데이터 쓰기, 연속 쓰기가 가능한 경우 연속적인 레지스터에 접근 가능
     * @param uint8_t register_address
     * @param uint8_t *data
     * @param uint8_t size
     * @return LOT_status_typedef
     */
    LOT_status_typedef
        transmit( const uint8_t register_address, const uint8_t *data, uint8_t size );

    /**
     * @brief 레지스터에 데이터 쓰기
     * @param uint8_t register_address
     * @param uint8_t data
     * @return LOT_status_typedef
     */
    LOT_status_typedef transmit( const uint8_t register_address, const uint8_t data );

    /**
     * @brief 레지스터의 데이터 읽기, 연속 읽기가 가능한 경우 연속적인 레지스터에 접근 가능
     * @param uint8_t register_address
     * @param uint8_t *data
     * @param uint8_t size
     * @return LOT_status_typedef
     */
    LOT_status_typedef receive( const uint8_t register_address, uint8_t *data, uint8_t size );

    /**
     * @brief 레지스터의 데이터 읽기
     * @param uint8_t register_address
     * @return uint8_t 수신 데이터
     */
    uint8_t receive( const uint8_t register_address );

    /**
     * @brief 데이터 송수신
     * @param uint8_t *data 입력 데이터를 송신하고, 수신 데이터를 저장
     * @param uint8_t size
     * @return LOT_status_typedef
     */
    LOT_status_typedef transceive( uint8_t *data, uint8_t size );

    /**
     * @brief 데이터 송수신
     * @param uint8_t data 송신 데이터
     * @return uint8_t uint8_t 수신 데이터
     */
    uint8_t transceive( const uint8_t data );

protected:
private:
    uint8_t error_count;

    /**
     * @brief 통신 에러가 발생한 경우 처리하는 함수
     */
    void __attribute__( ( weak ) ) error( void );
};

inline LOT_status_typedef LOT_spi0::transmit( const uint8_t data )
{
    SPDR = data;
#if LOT_SPI0_TIME_OUT <= 0xFF
    uint8_t count = LOT_SPI0_TIME_OUT;
#else
    uint16_t count = LOT_SPI0_TIME_OUT;
#endif
    while( ( SPSR & _BV( SPIF ) ) == 0 )
    {
        --count;
        if( count == 0 ) { return LOT_ERROR; }
    }
    return LOT_OK;
}

inline uint8_t LOT_spi0::transceive( const uint8_t data )
{
    transmit( data );
    return SPDR;
}

extern LOT_spi0 spi0;
#endif    // _LOT_SPI0_H_
