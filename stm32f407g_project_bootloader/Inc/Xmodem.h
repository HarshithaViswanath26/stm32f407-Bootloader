/*
 * Xmodem.h
 *
 *  Created on: 29-May-2026
 *      Author: 49157
 */

#ifndef XMODEM_H_
#define XMODEM_H_

#include "stdint.h"

#define SOH						0x01U      // start of header
#define EOT						0x04U	  // end of transmission
#define ACK						0x06U	  //
#define NAK						0x15U
#define ETB						0x17U	  // end of transmission block
#define CAN						0x18U	  // cancel
#define C						0x43U	  // ASCII C

#define PACKET_SIZE				128U
#define	PACKET_OVERHEAD			5U		  // SOH + block + ~block + CRC_H + CRC_L
#define FULL_PACKET_SIZE		133U

// other necessary flags
#define EOT_FLAG				0U
#define ACK_FLAG				1U
#define ERROR_FLAG				-1

typedef struct __attribute__((packed))
{
	uint8_t start;
	uint8_t blockNum;
	uint8_t blockNumInv;
	uint8_t data[PACKET_SIZE];
	uint8_t crcH;
	uint8_t crcL;
}Packet_Format_t;


int8_t Receive_Packet(Packet_Format_t* pHandle);
void Xmodem_Receive(void);

void USART_Initialize(void);
uint8_t CRC_Verify(uint8_t* data, uint8_t CRCH, uint8_t CRCL);


#endif /* XMODEM_H_ */
