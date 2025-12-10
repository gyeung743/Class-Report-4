#include "chu_init.h"
#include "gpio_cores.h"
#include "blinker_core.h"

int main() {
    uint32_t slot4_base = get_slot_addr(BRIDGE_BASE, S4_USER);

    // Instantiate cores for 4 LEDs
    BlinkerCore led0(slot4_base + 0x00);
    BlinkerCore led1(slot4_base + 0x10);
    BlinkerCore led2(slot4_base + 0x20);
    BlinkerCore led3(slot4_base + 0x30);

    // Set intervals
    led0.set_interval(100);
    led1.set_interval(250);
    led2.set_interval(500);
    led3.set_interval(1000);

    return 0;
}