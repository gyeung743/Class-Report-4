#include "blinker_core.h"
#include <stdint.h>

BlinkerCore::BlinkerCore(uint32_t core_base_addr) {
    base_addr = core_base_addr;
}

BlinkerCore::~BlinkerCore() {
}

void BlinkerCore::set_interval(uint16_t ms) {
    io_write((uintptr_t)base_addr, 0, (uint32_t)ms);
}