#include "Transmission.h"

Transmission::Transmission(uint8_t fl, uint8_t bl, uint8_t fr, uint8_t br, uint16_t max) {
    _fl = fl;
    _bl = bl;
    _fr = fr;
    _br = br;

    _max = max;
}

void Transmission::Init() {
    pinMode(_fl, OUTPUT);
    pinMode(_bl, OUTPUT);
    pinMode(_fr, OUTPUT);
    pinMode(_br, OUTPUT);
}

void Transmission::Move(Direction d, Turn t, uint8_t power) {
    int v = (_max / 100.0f) * power;

    switch (d)
    {
    case Direction::Forward:
        analogWrite(_bl, 0);
        analogWrite(_br, 0);
        analogWrite(_fl, v);
        analogWrite(_fr, v);
        break;
    case Direction::Back:
        analogWrite(_bl, v);
        analogWrite(_br, v);
        analogWrite(_fl, 0);
        analogWrite(_fr, 0);
        break;
    }
}