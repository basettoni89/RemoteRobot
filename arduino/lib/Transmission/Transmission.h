#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <Arduino.h>

class Transmission
{
public:
    typedef enum {
        Stop = 0,
        Forward = 1,
        Back = -1
    } Direction;

    typedef enum {
        None = 0,
        Right = 1,
        Left = -1
    } Turn;

    Transmission(uint8_t fl, uint8_t bl, uint8_t fr, uint8_t br, uint16_t max = 1024);

    void Init();

    ///Set movement
    ///\param[in] d Direction movement
    ///\param[in] power Movement percentage (0-100)
    void Move(Direction d, Turn t, uint8_t power);

private:
    uint8_t     _fl;
    uint8_t     _bl;
    uint8_t     _fr;
    uint8_t     _br;

    uint16_t    _max;
};

#endif