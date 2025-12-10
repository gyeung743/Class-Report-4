#include "chu_init.h"
#include "gpio_cores.h"
#include "spi_core.h"

// ADXL362 Register Addresses
#define DEVID_AD 0x00
#define DEVID_PART 0x02
#define POWER_CTL 0x2D
#define XDATA8 0x08
#define YDATA8 0x09
#define ZDATA8 0x0A

// Commands
#define RD_CMD 0x0B
#define WR_CMD 0x0A

// Threshold for tilt detection
// 1G is about 64. Use ~0.5G for detection.
#define THRESHOLD 30

uint8_t read_adxl_reg(SpiCore *spi, uint8_t reg) {
    uint8_t data;
    spi->assert_ss(0);
    spi->transfer(RD_CMD);
    spi->transfer(reg);
    data = spi->transfer(0x00);
    spi->deassert_ss(0);
    return data;
}

void write_adxl_reg(SpiCore *spi, uint8_t reg, uint8_t data) {
    spi->assert_ss(0);
    spi->transfer(WR_CMD);
    spi->transfer(reg);
    spi->transfer(data);
    spi->deassert_ss(0);
}

int main() {
    // Instantiate Cores
    // SPI in Slot 9
    SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));
    // LEDs are GPO in Slot 2
    GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));

    // SPI
    // 1 MHz, Mode 0
    spi.set_freq(1000000); 
    spi.set_mode(0, 0);

    // Enable Measurement Mode
    write_adxl_reg(&spi, POWER_CTL, 0x02);
    sleep_ms(100);

    while (1) {
        // Read X and Y Data
        int8_t x = (int8_t)read_adxl_reg(&spi, XDATA8);
        int8_t y = (int8_t)read_adxl_reg(&spi, YDATA8);
        
        int abs_x = (x < 0) ? -x : x;
        int abs_y = (y < 0) ? -y : y;

        int orientation = -1; // -1 = flat
        
        if (abs_y > abs_x && abs_y > THRESHOLD) {
            // Y-axis is vertical
            if (y > 0) {
                orientation = 0; // 0 degrees to LED 0
            } else {
                orientation = 2; // 180 degrees to LED 2
            }
        } 
        else if (abs_x > abs_y && abs_x > THRESHOLD) {
            // X-axis is vertical
            if (x > 0) {
                orientation = 1; // 90 degrees to LED 1
            } else {
                orientation = 3; // 270 degrees to LED 3
            }
        }

        // Update LEDs
        uint32_t led_pattern = 0;
        
        if (orientation != -1) {
            // Set bit corresponding to orientation (0, 1, 2, or 3)
            led_pattern = (1 << orientation);
        }
        
        led.write(led_pattern);

        sleep_ms(200); // Wait 0.2s before next check
    }

    return 0;
}