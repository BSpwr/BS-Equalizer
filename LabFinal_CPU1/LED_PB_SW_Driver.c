#include <stdbool.h>
#include <F2837xD_Device.h>
#include <LED_PB_SW_Driver.h>

void init_led_pb_sw_gpio() {
    // GPIO

    EALLOW;

    // Lower byte of GPAGMUX1 set to 0x0000 (GPIO0-7)
    // Upper byte of GPAGMUX1 and with 0x0F00 (GPIO8-11, 14-15)
    GpioCtrlRegs.GPAGMUX1.all = GpioCtrlRegs.GPAGMUX1.all & 0x0F000000;
    // Lower byte of GPAGMUX2 and with 0xFFFC (GPIO16)
    GpioCtrlRegs.GPAGMUX2.all = GpioCtrlRegs.GPAGMUX2.all & 0xFFFFFFFC;
    // Lower byte of GPAMUX1 set to 0x0000 (GPIO0-7)
    // Upper byte of GPAMUX1 and with 0x0F00 (GPIO8-11, 14-15)
    GpioCtrlRegs.GPAMUX1.all = GpioCtrlRegs.GPAMUX1.all & 0x0F000000;
    // Lower byte of GPAMUX2 and with 0xFFFC (GPIO16)
    GpioCtrlRegs.GPAMUX2.all = GpioCtrlRegs.GPAMUX2.all & 0xFFFFFFFC;

    // 16-14, 3-0, set to 0 (input)
    // 11-4 set to 1 (output)
    GpioCtrlRegs.GPADIR.all = (GpioCtrlRegs.GPADIR.all & 0xFFFE3FF0) | 0x00000FF0;

    // Setup GPAPUD to enable pull-up for input GPIO3-0, 16-14
    // Disable pull-up for output GPIO11-4
    GpioCtrlRegs.GPAPUD.all = (GpioCtrlRegs.GPAPUD.all & 0xFFFE3FF0) | 0x00000FF0;

    // LEDs off by default (active-low)
    GpioDataRegs.GPASET.all = 0x0FF0;
}

// Only bits 7-0 of the output matter
unsigned char read_led_values(void) {
    return (~GpioDataRegs.GPADAT.all & 0x0FF0) >> 4;

}

// Valid input led_num must be in range 7..0
bool read_led_value(unsigned char led_num) {
    return (~GpioDataRegs.GPADAT.all & (1 << (led_num + 4))) >> (led_num + 4);
}

// Valid input led_num must be in range 7..0
void set_led_value(unsigned char led_num, bool val) {

    // LEDS are active-low, set should clear bit
    if (val) {
        GpioDataRegs.GPACLEAR.all = (1 << (led_num + 4));
    }
    // clear should set bit
    else {
        GpioDataRegs.GPASET.all = (1 << (led_num + 4));
    }
}

// Only bits 7-0 of the input matter
void set_led_values(unsigned char bits) {
    // LEDs are active-low
    bits = ~bits;
    GpioDataRegs.GPADAT.all = (GpioDataRegs.GPADAT.all | ((((Uint32)bits) & 0x00FF) << 4)) & ((((Uint32)bits) << 4) | 0xFFFFF00F);
}

// BIT2 -> PB3, BIT1 -> PB2, BIT0 -> PB1
unsigned char read_push_buttons(void) {
    // push buttons are active-low
    return (~GpioDataRegs.GPADAT.all & 0x0001C000) >> 14;
}

// BIT3 -> DIP4, BIT2 -> DIP3, BIT1 -> DIP2, BIT0 -> DIP1
unsigned char read_dip_switches(void) {
    return GpioDataRegs.GPADAT.all & 0xF;
}
