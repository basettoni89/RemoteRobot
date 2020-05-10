#include "Transmission.h"

Transmission::Transmission(uint8_t fl, uint8_t bl, uint8_t fr, uint8_t br, uint16_t max, RemoteDebug* debug) {
    _fl = fl;
    _bl = bl;
    _fr = fr;
    _br = br;

    _max = max;

    _debug = debug;
}

void Transmission::Init() {
    pinMode(_fl, OUTPUT);
    pinMode(_bl, OUTPUT);
    pinMode(_fr, OUTPUT);
    pinMode(_br, OUTPUT);
}

void Transmission::Move(int8_t dir, int8_t turn) {
    int left = dir + turn;
    left = left > 100 ? 100 : left;
    left = left < -100 ? -100 : left;

    int right = dir - turn;
    right = right > 100 ? 100 : right;
    right = right < -100 ? -100 : right;

    if(_debug->isActive(_debug->DEBUG))
        _debug->printf("%d %d\n", left, right);

    if(left > 0) {
        analogWrite(_fl, (_max / 100.0f) * left);
        analogWrite(_bl, 0);
    }else if(left < 0) {
        analogWrite(_fl, 0);
        analogWrite(_bl, (_max / 100.0f) * -left);       
    } else {
        analogWrite(_fl, 0);
        analogWrite(_bl, 0);
    }

    if(right > 0) {
        analogWrite(_fr, (_max / 100.0f) * right);
        analogWrite(_br, 0);
    }else if(right < 0) {
        analogWrite(_fr, 0);
        analogWrite(_br, (_max / 100.0f) * -right);       
    } else {
        analogWrite(_fr, 0);
        analogWrite(_br, 0);
    }
}