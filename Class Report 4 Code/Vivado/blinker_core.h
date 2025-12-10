#ifndef _BLINKER_CORE_H_INCLUDED
#define _BLINKER_CORE_H_INCLUDED

#include "chu_io_rw.h"
#include <stdint.h>

class BlinkerCore {
public:
    BlinkerCore(uint32_t core_base_addr);
    ~BlinkerCore();

    void set_interval(uint16_t ms);

private:
    uint32_t base_addr;
};

#endif