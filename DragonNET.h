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
		[0]    - Стартовый байт.
		[*1]   - Адрес получателя.
		[*2]   - Адрес отправителя.
		[*3]   - Системный байт.
		  |- [7] - Флаг типа сети, 0 - Ведущий-Ведомые, 1 - P2P.
		  |- [6] - Флаг мастер устройства, 0 - Ведомый, 1 - Ведущий.
		  |- [5] - Флаг системной команды, 0 - Общая, 1 - Системная.
		  |- [4] - Резерв
		  |- [3] - Резерв
		  |- [2] - Флаг запроса с ожиданием ответа, 0 - Ответ не нужен, 1 - Ответ нужен.
		  |- [1] - Флаг подтверждения получения пакета, 0 - Подтверждение не нужно, 1 - Подтверждение нужно, (Не может быть установлен на подтверждающий пакет).
		  |- [0] - Резерв
		[*4]   - Длинна данных.
		[*5+n] - Данные.
		[6+n]  - CRC16 H.
		[7+n]  - CRC16 L.
		[8+n]  - Стоповый байт.
	Символом '*' указаны фрагменты данных, участвующие в подсчёте CRC16.
*/

/*
	Конструкторы и метод Begin реализованы в H файле по причине того, что только так можно реализовать универсальный класс,
	который может работать как с HardwareSerial так и с SoftwareSerial.
	Есть другие идеи? жду предложений :)
*/

#ifndef DragonNET_h_
#define DragonNET_h_

#include <Arduino.h>
#include "DragonNET_Packet.h"
#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
	#define UART	HardwareSerial
	#include <HardwareSerial.h>
#else
	#define UART	SoftwareSerial
	#include <SoftwareSerial.h>
#endif

/*
#if F_CPU == 16000000
#elif F_CPU == 8000000
#elif F_CPU == 20000000
#else
#error This version supports only 20, 16 and 8MHz processors
#endif
*/

#define DRAGONNET_BROADCASTADDRESS	0xFF
#define DRAGONNET_RECEIVETIMEOUT	10000		// мкс
#define DRAGONNET_BUFFERSIZE		64

#define DRAGONNET_ERROR_			0xE0		// База ошибок.
#define DRAGONNET_ERROR_STRUCTURE	0xE1		// Нарушена структура пакета.
#define DRAGONNET_ERROR_CRC			0xE2		// Нарушена контрольная сумма.
#define DRAGONNET_ERROR_OVERFLOW	0xEF		// Переполнение RX буфера.

using DragonNETPacket = DragonNET_Packet<DRAGONNET_BUFFERSIZE>;

class DragonNET
{
	using RXcallback_t = void (*)(DragonNETPacket &request, DragonNETPacket &response);
	using ErrorCallback_t = void (*)(uint8_t errorType);
	
	public:
		void Begin(uint32_t baudRate, uint8_t address, bool receiveAll){ this->_serial->begin(baudRate); this->_myAddress = address; this->_receiveAll = receiveAll; this->Begin(); return; }
		void AttachRXCallback(RXcallback_t callback);
		void AttachErrorCallback(ErrorCallback_t callback);
		void TransmitPackage(DragonNETPacket &package);
		void ReceivePackage(uint32_t currentMicroTime);
		void SetSystemByte(uint8_t index, bool value);
		bool GetSystemByte(uint8_t index);
	protected:
		DragonNET(UART &serial, uint8_t directionPin) : _serial(&serial), _directionPin(directionPin){ return; }
	private:
		void Begin();
		
		UART *_serial;
		DragonNETPacket _requestPacket;
		DragonNETPacket _responsePacket;
		RXcallback_t _RXcallback;
		ErrorCallback_t _ErrorCallback;
		uint32_t _lastMicroTime = 0;
		uint8_t _directionPin;
		uint8_t _myAddress;
		bool _receiveAll;
		byte _systemByte;
};

class DragonNET_Master : public DragonNET
{
	public:
		DragonNET_Master(UART &serial, uint8_t directionPin) : DragonNET(serial, directionPin){ return; }
		DragonNET_Master(UART &&serial, uint8_t directionPin) = delete;
};

class DragonNET_Slave : public DragonNET
{
	public:
		DragonNET_Slave(UART &serial, uint8_t directionPin) : DragonNET(serial, directionPin){ return; }
		DragonNET_Slave(UART &&serial, uint8_t directionPin) = delete;
		
		void Processing();
};

class DragonNET_P2P : public DragonNET
{
	public:
		DragonNET_P2P(UART &serial, uint8_t directionPin) : DragonNET(serial, directionPin){ return; }
		DragonNET_P2P(UART &&serial, uint8_t directionPin) = delete;
};

#endif
