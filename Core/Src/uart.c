/*
 * uart.c
 *
 *  Created on: Mar 11, 2024
 *      Author: kccistc
 */

#include "uart.h"
#include <stdio.h>

UART_HandleTypeDef *myHuart;

#define rxBufferMax 255

int rxBufferGp;			// get pointer (read)
int rxBufferPp;			// put pointer (write)
uint8_t rxBuffer[rxBufferMax];
uint8_t rxChar;

// init device
void initUart(UART_HandleTypeDef *inHuart) {
	myHuart = inHuart;
	HAL_UART_Receive_IT(myHuart, &rxChar, 1);	//	문자 하나 수신할 때마다 HAL_UART_RxCpltCallback호출
}

// process received charactor
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	rxBuffer[rxBufferPp++] = rxChar;						//buffer가 꽉 찰 때까지 반복 됨
	rxBufferPp %= rxBufferMax;
	HAL_UART_Receive_IT(myHuart, &rxChar, 1);
}

// get charactor from buffer
uint8_t getChar() {
	uint8_t result;
	if (rxBufferGp == rxBufferPp)	return 0;	// Gp == Pp : 버퍼가 비어있다는 뜻
	result = rxBuffer[rxBufferGp++];				//
	rxBufferGp %= rxBufferMax;
	return result;
}



// Binary data transmit
void binaryTransmit (protocol_t inData) {				// 이 구조체의 데이터를 txBuffer로 transmit
	uint8_t txBuffer[] = {STX, 0, 0, 0, 0, 0, 0, 0, ETX};
	// data copy
	// memcpy(&txBuffer[1], &inData, 6);
	// (copy destination, source address, 옮길 데이터 개수(uint8 : 1개 uint32 : 4개))
	txBuffer[1] = inData.id | 0x80;
	txBuffer[2] = inData.command | 0x80;
	txBuffer[3] = inData.data | 0x80;
	txBuffer[4] = (inData.data >> 7) | 0x80;
	txBuffer[5] = (inData.data >> 14) | 0x80;
	txBuffer[6] = (inData.data >> 21) | 0x80;
	//CRC calculate
	for(int i = 0; i < 7; i++)
		txBuffer[7] += txBuffer[i];
	// transmit
	HAL_UART_Transmit(myHuart, txBuffer, sizeof(txBuffer), 10);
}

int _write(int file, char *p, int len) {
	HAL_UART_Transmit(myHuart, p, len, 10); // lcd, uart, ...
	return len;
}
