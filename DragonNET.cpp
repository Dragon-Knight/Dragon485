/*
 *	DragonNET.cpp
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#include <Arduino.h>
#include "DragonNET.h"

void DragonNET::Begin(uint32_t baudRate, uint8_t address, bool receiveAll)
{
	// Для вызова метода begin, который не виртуальный, восстанавливаем информацию о типе
	if(_isHWSerial) {
		// XXX Можно изменить список аргументов на тот, который требуется
		static_cast<HardwareSerial*>(_serial)->begin(baudRate, SERIAL_8N2);
	} else {
		// XXX Можно изменить список аргументов на тот, который требуется
		static_cast<SoftwareSerial*>(_serial)->begin(baudRate);
	}


	this->_serial->write(0x33);	// Отладка.
	
	this->_address = address;
	this->_receiveAll = receiveAll;
	
	pinMode(this->_directionPin, OUTPUT);
	digitalWrite(this->_directionPin, LOW);
	
	this->ClearArray(this->_TXBuffer, sizeof(this->_TXBuffer));
	this->ClearArray(this->_RXBuffer, sizeof(this->_RXBuffer));
}

void DragonNET::AttachRXCallback(RXcallback_t callback)
{
	this->_RXcallback = callback;
}

void DragonNET::AttachErrorCallback(ErrorCallback_t callback)
{
	this->_ErrorCallback = callback;
}

/*
	Пакет:
		[0] - Стартовый байт.
		[1] - Адрес получателя.		]
		[2] - Адрес отправителя.	]
		[3] - Параметр 1.			]
		[4] - Параметр 2.			]
		[5] - Длинна данных.		]
		[6+n] - Данные.				]
		[7+n] - CRC16 H.
		[8+n] - CRC16 L.
		[9+n] - Стоповый байт.
*/

void DragonNET::TransmitPackage(uint8_t toAddress, byte *data, uint8_t dataLength)
{
	this->_TXBuffer[0] = toAddress;
	this->_TXBuffer[1] = this->_address;
	this->_TXBuffer[2] = this->_parameter[0];
	this->_TXBuffer[3] = this->_parameter[1];
	this->_TXBuffer[4] = dataLength;
	for(uint8_t i = 0; i < dataLength; ++i)
	{
		this->_TXBuffer[5+i] = data[i];
	}
	
	uint16_t crc = this->CRC16(this->_TXBuffer, (dataLength + 5));
	
	digitalWrite(this->_directionPin, HIGH);
	this->_serial->write(DRAGONNET_STARTBYTE);
	this->_serial->write(this->_TXBuffer, (dataLength + 5));
	this->_serial->write(highByte(crc));
	this->_serial->write(lowByte(crc));
	this->_serial->write(DRAGONNET_ENDBYTE);
	this->_serial->flush();
	digitalWrite(this->_directionPin, LOW);
}

void DragonNET::ReceivePackage()
{
	uint32_t currentTime = millis();
	
	if(this->_lastReceiveTime + DRAGONNET_RECEIVETIMEOUT <= currentTime)
	{
		this->_lastReceiveTime = currentTime;
		
		while(this->_serial->available() > 0)
		{
			this->_serial->read();
		}
		
		this->ClearArray(this->_RXBuffer, sizeof(this->_RXBuffer));
		this->_RXBufferIndex = 0;
	}
	
	bool isReceived = false;
	while(this->_serial->available() > 0)
	{
		if(this->_RXBufferIndex == sizeof(this->_RXBuffer))
		{
			// Если буфер переполнен, то....
			break;
		}
		
		this->_RXBuffer[this->_RXBufferIndex++] = this->_serial->read();
		isReceived = true;
	}
	
	if(isReceived == true)
	{
		if(this->_RXBuffer[0] == DRAGONNET_STARTBYTE && this->_RXBuffer[(this->_RXBufferIndex - 1)] == DRAGONNET_ENDBYTE)
		{
			uint16_t crc = this->CRC16(this->_RXBuffer + 1, (this->_RXBufferIndex - 4));
			
			if(this->_RXBuffer[(this->_RXBufferIndex - 3)] == highByte(crc) && this->_RXBuffer[(this->_RXBufferIndex - 2)] == lowByte(crc))
			{
				if(this->_RXBuffer[1] == this->_address || this->_RXBuffer[1] == DRAGONNET_BROADCASTADDRESS || this->_receiveAll == true)
				{
					this->_RXcallback(this->_RXBuffer[2], this->_RXBuffer[1], this->_RXBuffer + 6, this->_RXBufferIndex - 9);
				}
				else
				{
					this->_serial->write(0xE1);
					// Пакет предназначен не для этого устройства.
				}
			}
			else
			{
				this->_serial->write(0xE2);
				// Ошибка CRC.
			}
		}
		
		this->_lastReceiveTime = currentTime;
	}
}

void DragonNET::SetParameter(uint8_t index, bool value)
{
	(index > 7) ?
		bitWrite(this->_parameter[1], index % 8, value) :
		bitWrite(this->_parameter[0], index % 8, value);
}

bool DragonNET::GetParameter(uint8_t index)
{
	return (index > 7) ?
		bitRead(this->_parameter[1], index % 8) :
		bitRead(this->_parameter[0], index % 8);
}


uint16_t DragonNET::CRC16(byte *array, uint8_t length)
{
	uint16_t crc = 0xFFFF;
	uint8_t i;
	
	while(length--)
	{
		crc ^= *array++ << 8;
		for(i = 0; i < 8; i++)
		{
			crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
		}
	}
	
	return crc;
}

void DragonNET::ClearArray(byte *array, uint8_t length)
{
	memset(array, 0x00, length);
}
