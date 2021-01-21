#include <F2837xD_Device.h>
#include <F28x_Project.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "UART_Driver.h"

static inline void uart_init_gpio() {
    EALLOW;

    GpioCtrlRegs.GPEPUD.bit.GPIO139 = 1; // Disable pull-up for GPIO139 (SCIRXDC)
    GpioCtrlRegs.GPBPUD.bit.GPIO56 = 1; // Disable pull-up for GPIO56 (SCITXDC)

    GpioCtrlRegs.GPEQSEL1.bit.GPIO139 = 3; // Asynch input GPIO32 (SCIRXDC)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch output GPIO33 (SCITXDC)

    GpioCtrlRegs.GPEDIR.bit.GPIO139 = 0; // Set GPIO139 as input (SCIRXDC)
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1; // Set GPIO56 as output (SCITXDC)

    GpioCtrlRegs.GPEGMUX1.bit.GPIO139 = 1; // Configure GPIO139 for SCIRXDC operation
    GpioCtrlRegs.GPEMUX1.bit.GPIO139 = 2;

    GpioCtrlRegs.GPBGMUX2.bit.GPIO56 = 1; // Configure GPIO56 for SCITXDC operation
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 2;
}

static inline void uart_init_fifo() {
    EALLOW;

    // Setup TX FIFO
    ScicRegs.SCIFFTX.bit.SCIFFENA = 1; // Enable SCI FIFO
    ScicRegs.SCIFFTX.bit.TXFIFORESET = 0; // Reset FIFO pointer and hold reset
    ScicRegs.SCIFFTX.bit.TXFIFORESET = 1; // Enable TX FIFO from reset
    ScicRegs.SCIFFTX.bit.TXFFINTCLR = 1; // Clear TXFFINT flag
    ScicRegs.SCIFFTX.bit.TXFFIENA = 0; // Disable TX FIFO interrupt
    ScicRegs.SCIFFTX.bit.TXFFIL = 0; // FIFO interrupt level bits

    // Setup RX FIFO
    ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1; // Clear RXFFOVF flag
    ScicRegs.SCIFFRX.bit.RXFIFORESET = 0; // Reset FIFO pointer and hold reset
    ScicRegs.SCIFFRX.bit.RXFIFORESET = 1; // Enable RX FIFO from reset
    ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1; // Clear RXFFINT flag
    ScicRegs.SCIFFRX.bit.RXFFIENA = 0; // Disable RX FIFO interrupt
    ScicRegs.SCIFFRX.bit.RXFFIL = 0; // FIFO interrupt level bits

    // Setup FIFO
    ScicRegs.SCIFFCT.all = 0;
}

static inline void set_uart_clock(float lsp_clk_mhz, uint32_t baud_rate) {
    EALLOW;

    // BRR = LSPCLK / (SCI Asynchronous Baud * 8) - 1

    float32 approx_brr = (lsp_clk_mhz * 1000000) / (baud_rate * 8) - 1;

    uint16_t brr = (uint16_t)lroundf(approx_brr);

    ScicRegs.SCIHBAUD.bit.BAUD = (brr & 0xFF00) >> 8;
    ScicRegs.SCILBAUD.bit.BAUD = (brr & 0x00FF);
}

void uart_init(uint32_t baud_rate) {

    uart_init_gpio();

    EALLOW;

    // Enable clock for SCI C
    CpuSysRegs.PCLKCR7.bit.SCI_C = 1;

    ScicRegs.SCICTL1.bit.SWRESET = 0; // Reset SCI C

    ScicRegs.SCICCR.bit.STOPBITS = 0; // 1 stop bit
    ScicRegs.SCICCR.bit.PARITY = 0; // Odd parity
    ScicRegs.SCICCR.bit.PARITYENA = 0; // Disable parity bit
    ScicRegs.SCICCR.bit.LOOPBKENA = 0; // Loopback disabled
    ScicRegs.SCICCR.bit.ADDRIDLE_MODE = 0; // Idle-line mode protocol
    ScicRegs.SCICCR.bit.SCICHAR = 7; // 8 data bits

    ScicRegs.SCICTL1.bit.RXERRINTENA = 0; // Disable RX error interrupt
    ScicRegs.SCICTL1.bit.TXWAKE = 0; // Disable wake
    ScicRegs.SCICTL1.bit.SLEEP = 0; // Disable sleep
    ScicRegs.SCICTL1.bit.TXENA = 1; // Enable TX
    ScicRegs.SCICTL1.bit.RXENA = 1; // Enable TX

    ScicRegs.SCICTL2.bit.RXBKINTENA = 0; // Disable RX interrupt
    ScicRegs.SCICTL2.bit.TXINTENA = 0; // Disable TX interrupt

    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0; // LSPCLK = 200 MHz
    set_uart_clock(200, baud_rate);

    uart_init_fifo();

    ScicRegs.SCICTL1.bit.SWRESET = 1;  // Enable SCI from reset
}

void uart_send(uint16_t data) {
    // wait until the FIFO has space
    while (ScicRegs.SCIFFTX.bit.TXFFST == 15);
    ScicRegs.SCITXBUF.all = data;
}

void uart_send_string(uint16_t* address) {
    while (*address != 0) {
        uart_send(*address);
        address++;
    }
}

void uart_send_array(uint16_t* address, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        uart_send(address[i]);
    }
}

void uart_send_float(float data) {
    uint16_t buff[sizeof(float)];

    memcpy(buff, &data, sizeof(float));

    uart_send((buff[1] & 0xFF00) >> 8);
    uart_send(buff[1] & 0xFF);
    uart_send((buff[0] & 0xFF00) >> 8);
    uart_send(buff[0] & 0xFF);
}

void uart_send_float_array(float* address, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        uart_send_float(address[i]);
    }
}

uint16_t uart_recv_block() {
  while (!((ScicRegs.SCIRXST.bit.RXRDY != 0) || (ScicRegs.SCIFFRX.bit.RXFFST > 0)));
  return ScicRegs.SCIRXBUF.bit.SAR;
}

// Expecting a float in big-endian format
// Python Serialization Example: serial.write(struct.pack('>f', 3.14))
float uart_recv_float_block() {
    float ret;
    uint16_t buff[sizeof(float)];

    buff[1] = uart_recv_block() << 8;
    buff[1] |= uart_recv_block() & 0xFF;
    buff[0] = uart_recv_block() << 8;
    buff[0] |= uart_recv_block() & 0xFF;
    memcpy(&ret, buff, sizeof(float));
    return ret;
}

uint16_t uart_recv() {
  if ((ScicRegs.SCIRXST.bit.RXRDY != 0) || (ScicRegs.SCIFFRX.bit.RXFFST > 0)) {
    return ScicRegs.SCIRXBUF.bit.SAR;
  } else return 0;
}
