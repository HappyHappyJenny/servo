/*
 * uart.h
 *
 *  Created on: Mar 11, 2024
 *      Author: kccistc
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

#define STX 0x02
#define ETX 0x03

typedef struct {
	uint8_t id;
	uint8_t command;
	uint32_t data;
} protocol_t;

void initUart(UART_HandleTypeDef *inHuart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
uint8_t getChar();
void binaryTransmit (protocol_t inData);
int _write(int file, char *p, int len);
#endif /* INC_UART_H_ */
