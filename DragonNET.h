/*
 *	DragonNET.h
 *	RS-485 network communication protocol.
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

/*
	Пакет:
		[0] - Стартовый байт.
		[1] - Адрес получателя.		|
		[2] - Адрес отправителя.	|
		[3] - Системный байт.		|
		[4] - Длинна данных.		|
		[5+n] - Данные.				|
		[6+n] - CRC16 H.
		[7+n] - CRC16 L.
		[8+n] - Стоповый байт.
	Символом '|' указаны фрагменты данных, участвующие в подсчёте CRC16.
*/

/*
	Конструкторы и метод Begin реализованы в H файле по причине того, что только так можно реализовать универсальный класс,
	который может работать как с HardwareSerial так и с SoftwareSerial.
	Есть другие идеи? жду предложений :)
*/

#ifndef DragonNET_h_
#define DragonNET_h_

#include <Arduino.h>
#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
	#define SERIAL	HardwareSerial
	#include "HardwareSerial.h"
#else
	#define SERIAL	SoftwareSerial
	#include "SoftwareSerial.h"
#endif

#define DRAGONNET_STARTBYTE			0x5B
#define DRAGONNET_ENDBYTE			0x5D
#define DRAGONNET_BROADCASTADDRESS	0xFF
#define DRAGONNET_RECEIVETIMEOUT	10
#define DRAGONNET_BUFFERSIZE		64

#define DRAGONNET_ERROR_CRC			0xE0

struct DragonNETParameters_t
{
	uint8_t address;
	char name[16];
	bool receiveAll;
};

class DragonNET
{
	using RXcallback_t = void (*)(uint8_t fromAddress, uint8_t toAddress, byte *data, uint8_t dataLength);
	using ErrorCallback_t = void (*)(uint8_t errorType);
	
	public:
		void Begin(uint32_t baudRate, DragonNETParameters_t parameters){ this->_serial->begin(baudRate); this->_parameters = parameters; this->Begin(); return; }
		void AttachRXCallback(RXcallback_t callback);
		void AttachErrorCallback(ErrorCallback_t callback);
		void TransmitPackage(uint8_t toAddress, byte *data, uint8_t dataLength);
		void ReceivePackage();
		void SetSystemByte(uint8_t index, bool value);
		bool GetSystemByte(uint8_t index);
	protected:
		DragonNET(SERIAL &serial, uint8_t directionPin) : _serial(&serial), _directionPin(directionPin){ return; }
	private:
		void Begin();
		uint16_t CRC16(byte *array, uint8_t length);
		void ClearArray(byte *array, uint8_t length);
		
		SERIAL *_serial;
		RXcallback_t _RXcallback;
		ErrorCallback_t _ErrorCallback;
		DragonNETParameters_t _parameters;
		byte _TXBuffer[DRAGONNET_BUFFERSIZE + 4];
		byte _RXBuffer[DRAGONNET_BUFFERSIZE + 8];
		uint32_t _lastReceiveTime = 0;
		uint8_t _RXBufferIndex = 0;
		uint8_t _directionPin;
		byte _systemByte;
};

class DragonNET_Master : public DragonNET
{
	public:
		DragonNET_Master(SERIAL &serial, uint8_t directionPin) : DragonNET(serial, directionPin){ return; }
		DragonNET_Master(SERIAL &&serial, uint8_t directionPin) = delete;
};

class DragonNET_Slave : public DragonNET
{
	public:
		DragonNET_Slave(SERIAL &serial, uint8_t directionPin) : DragonNET(serial, directionPin){ return; }
		DragonNET_Slave(SERIAL &&serial, uint8_t directionPin) = delete;
};

class DragonNET_P2P : public DragonNET
{
	public:
		DragonNET_P2P(SERIAL &serial, uint8_t directionPin) : DragonNET(serial, directionPin){ return; }
		DragonNET_P2P(SERIAL &&serial, uint8_t directionPin) = delete;
};

#endif
