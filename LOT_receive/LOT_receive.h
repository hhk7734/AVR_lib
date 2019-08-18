/**
 * @file LOT_receive.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief 문자열 수신
 */

#ifndef _LOT_RECEIVE_H_
#define _LOT_RECEIVE_H_

#include <stdint.h>

class LOT_receive {
public:
    inline LOT_receive() {}

    /**
     * @brief 수신 버퍼 읽기 가상 함수
     * @param uint8_t *data 읽은 데이터
     * @param uint16_t max_size 읽어들일 데이터 최대 개수
     * @return uint16_t 읽은 데이터 개수
     */
    virtual uint16_t receive_basic( uint8_t *data, uint16_t max_size ) = 0;

    /**
     * @brief 수신 버퍼에 있는 데이터 수 확인 가상 함수
     * @return uint8_t 버퍼에 있는 데이터 수
     */
    virtual uint16_t receive_size( void ) = 0;

    /**
     * @brief 수신 버퍼 읽기 가상 함수
     * @return 가장 오래된 데이터
     */
    virtual uint8_t receive_basic( void ) = 0;

    inline uint8_t receive( uint8_t *data, uint8_t max_size )
    {
        return receive_basic( data, max_size );
    }
    inline uint8_t receive( void ) { return receive_basic(); }

    /// @todo 문자열 찾기 함수 만들기

protected:
private:
};

#endif    // _LOT_RECEIVE_H_
