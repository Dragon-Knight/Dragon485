/*
 *	DragonNET.h
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#ifndef DragonNET_h_
#define DragonNET_h_

#include <Arduino.h>
#include "HardwareSerial.h"
#include "SoftwareSerial.h"

#define DRAGONNET_STARTBYTE			0x5B
#define DRAGONNET_ENDBYTE			0x5D
#define DRAGONNET_BROADCASTADDRESS	0xFF
#define DRAGONNET_RECEIVETIMEOUT	10
#define DRAGONNET_BUFFERSIZE		64


class DragonNET
{
	public:
		DragonNET(HardwareSerial &serial, uint8_t directionPin) :
			_serial(&serial),
			_directionPin(directionPin),
			_isHWSerial(true)
		{}
		DragonNET(SoftwareSerial &serial, uint8_t directionPin) :
			_serial(&serial),
			_directionPin(directionPin)
		{}
		
		void Begin(uint32_t baudRate, uint8_t address, bool receiveAll);
		void AttachRXCallback(void (*callback)(uint8_t fromAddress, uint8_t toAddress, byte *data, uint8_t dataLength));
		void AttachErrorCallback(void (*callback)(uint8_t errorType));
		void TransmitPackage(uint8_t toAddress, byte *data, uint8_t dataLength);
		void ReceivePackage();
		void SetParameter(uint8_t index, bool value);
		bool GetParameter(uint8_t index);
		
	private:
		uint16_t CRC16(byte *array, uint8_t length);
		void ClearArray(byte *array, uint8_t length);
		
		void (*_RXcallback)(uint8_t fromAddress, uint8_t toAddress, byte *data, uint8_t dataLength);
		void (*_ErrorCallback)(uint8_t errorType);

		Stream *_serial;
		
		byte _TXBuffer[DRAGONNET_BUFFERSIZE + 5];
		byte _RXBuffer[DRAGONNET_BUFFERSIZE + 9];
		uint32_t _lastReceiveTime = 0;
		uint8_t _RXBufferIndex = 0;
		uint8_t _directionPin;
		uint8_t _address;
		byte _parameter[2];
		bool _receiveAll;

		// Is _serial instance of HardwareSerial
		bool _isHWSerial = false;
};

class DragonNET_Master : public DragonNET
{
public:
	DragonNET_Master(HardwareSerial &serial, uint8_t directionPin) : DragonNET(serial, directionPin) {}
	DragonNET_Master(SoftwareSerial &serial, uint8_t directionPin) : DragonNET(serial, directionPin) {}
	DragonNET_Master(SoftwareSerial &&serial, uint8_t directionPin) = delete;
};

class DragonNET_Slave : public DragonNET
{
public:
	DragonNET_Slave(HardwareSerial &serial, uint8_t directionPin) : DragonNET(serial, directionPin) {}
	DragonNET_Slave(SoftwareSerial &serial, uint8_t directionPin) : DragonNET(serial, directionPin) {}
	DragonNET_Slave(SoftwareSerial &&serial, uint8_t directionPin) = delete;
};

class DragonNET_P2P : public DragonNET
{
public:
	DragonNET_P2P(HardwareSerial &serial, uint8_t directionPin) : DragonNET(serial, directionPin) {}
	DragonNET_P2P(SoftwareSerial &serial, uint8_t directionPin) : DragonNET(serial, directionPin) {}
	DragonNET_P2P(SoftwareSerial &&serial, uint8_t directionPin) = delete;
};

#endif
