#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

void uart_init(uint32_t baud_rate);

void uart_send(uint16_t data);

void uart_send_string(uint16_t* address);

void uart_send_array(uint16_t* address, uint16_t size);

void uart_send_float(float data);

void uart_send_float_array(float* address, uint16_t size);

uint16_t uart_recv_block();

float uart_recv_float_block();

uint16_t uart_recv();




#endif // UART_DRIVER_H_
