#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <Arduino.h>
#include <RemoteDebug.h>

class Transmission
{
public:
    Transmission(uint8_t fl, uint8_t bl, uint8_t fr, uint8_t br, uint16_t max, RemoteDebug* debug);

    void Init();

    ///Set movement
    ///\param[in] d Direction movement
    ///\param[in] power Movement percentage (0-100)
    void Move(int8_t dir, int8_t turn);

private:
    uint8_t     _fl;
    uint8_t     _bl;
    uint8_t     _fr;
    uint8_t     _br;

    uint16_t    _max;

    RemoteDebug* _debug;
};

#endif