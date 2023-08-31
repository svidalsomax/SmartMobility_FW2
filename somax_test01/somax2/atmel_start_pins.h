/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAME51 has 14 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8
#define GPIO_PIN_FUNCTION_J 9
#define GPIO_PIN_FUNCTION_K 10
#define GPIO_PIN_FUNCTION_L 11
#define GPIO_PIN_FUNCTION_M 12
#define GPIO_PIN_FUNCTION_N 13

#define AN1 GPIO(GPIO_PORTA, 2)
#define AN2 GPIO(GPIO_PORTA, 3)
#define UART_BLE_TX GPIO(GPIO_PORTA, 4)
#define UART_BLE_RX GPIO(GPIO_PORTA, 5)
#define GPRS_nPWR GPIO(GPIO_PORTA, 7)
#define UART_GPRS_RX GPIO(GPIO_PORTA, 8)
#define UART_GPRS_TX GPIO(GPIO_PORTA, 9)
#define GPRS_NETSTAT GPIO(GPIO_PORTA, 10)
#define GPRS_RI GPIO(GPIO_PORTA, 11)
#define SPI_SCK GPIO(GPIO_PORTA, 12)
#define SPI_MOSI GPIO(GPIO_PORTA, 13)
#define SPI_CS0 GPIO(GPIO_PORTA, 14)
#define SPI_MISO GPIO(GPIO_PORTA, 15)
#define I2C_SCL GPIO(GPIO_PORTA, 16)
#define I2C_SDA GPIO(GPIO_PORTA, 17)
#define LED0 GPIO(GPIO_PORTA, 18)
#define LED1 GPIO(GPIO_PORTA, 19)
#define LED2 GPIO(GPIO_PORTA, 20)
#define USB_DN GPIO(GPIO_PORTA, 24)
#define USB_DP GPIO(GPIO_PORTA, 25)
#define INT_ACC GPIO(GPIO_PORTB, 2)
#define BLE_PIO1 GPIO(GPIO_PORTB, 4)
#define VSYS_SOURCE GPIO(GPIO_PORTB, 5)
#define GPRS_PG GPIO(GPIO_PORTB, 6)
#define GPRS_PEN GPIO(GPIO_PORTB, 7)
#define AN0 GPIO(GPIO_PORTB, 8)
#define BAT_STATUS GPIO(GPIO_PORTB, 9)
#define INT_CAN GPIO(GPIO_PORTB, 13)
#define CAN_TX GPIO(GPIO_PORTB, 14)
#define CAN_RX GPIO(GPIO_PORTB, 15)

#endif // ATMEL_START_PINS_H_INCLUDED
