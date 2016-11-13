/*
	
*/

#ifndef DragonNET_h_
#define DragonNET_h_

#include <Arduino.h>
#include "HardwareSerial.h"
#include "SoftwareSerial.h"

#define DRAGONNET_STARTBYTE			0x5B
#define DRAGONNET_ENDBYTE			0x5D
#define DRAGONNET_BROADCASTADDRESS	0xFF

class DragonNET_Master
{
	public:
		/*
		#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
		DragonNET_Master(HardwareSerial &serial, uint8_t directionPin);
		#else
		DragonNET_Master(uint8_t RXPin, uint8_t TXPin, uint8_t directionPin);
		#endif
		*/
		DragonNET_Master(HardwareSerial &serial, uint8_t directionPin, uint32_t baudRate);
		DragonNET_Master(SoftwareSerial &serial, uint8_t directionPin, uint32_t baudRate);
		DragonNET_Master(SoftwareSerial &&serial, uint8_t directionPin, uint32_t baudRate) = delete;
		

		void Begin(uint8_t address, bool receiveAll);
		//void AttachRXCallback(void (*callback)(uint8_t fromAddress, byte *data, uint8_t dataLength));
		void AttachRXCallback(void (*callback)(byte *data, uint8_t dataLength));
		void TransmitPackage(uint8_t toAddress, byte *data, uint8_t dataLength);
		void ReceivePackage();
	protected:
		
	private:
		uint16_t CRC16(byte *data, uint8_t length);
		void ClearArray(byte *array, uint8_t length);
		
		/*
		#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
		HardwareSerial *_serial;
		#else
		SoftwareSerial *_serial;
		#endif
		*/
		Stream *_serial;
		
		byte _TXBuffer[64 + 7];
		byte _RXBuffer[64 + 7];
		uint8_t _RXBufferIndex = 0;;
		//void (*_callback)(uint8_t fromAddress, byte *data, uint8_t dataLength);
		void (*_callback)(byte *data, uint8_t dataLength);
		uint8_t _directionPin;
		uint8_t _address;
		bool _receiveAll;
		byte _parameter[2];
		uint32_t _lastReceiveTime = 0;;
};

class DragonNET_Slave
{
	public:
		DragonNET_Slave();
	protected:
	private:
};

class DragonNET_P2P
{
	public:
		DragonNET_P2P();
	protected:
	private:
};

#endif
