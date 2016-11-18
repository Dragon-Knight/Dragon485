/*
 *	DragonNET.cpp
 *	Class of basic functions of the protocol.
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#include <Arduino.h>
#include "DragonNET.h"

void DragonNET::AttachRXCallback(RXcallback_t callback)
{
	this->_RXcallback = callback;
	
	return;
}

void DragonNET::AttachErrorCallback(ErrorCallback_t callback)
{
	this->_ErrorCallback = callback;
	
	return;
}

void DragonNET::TransmitPackage(uint8_t toAddress, byte *data, uint8_t dataLength)
{
	this->_TXBuffer[0] = toAddress;
	this->_TXBuffer[1] = this->_parameters.address;
	this->_TXBuffer[2] = this->_systemByte;
	this->_TXBuffer[3] = dataLength;
	for(uint8_t i = 0; i < dataLength; ++i)
	{
		this->_TXBuffer[4 + i] = data[i];
	}
	
	uint16_t crc = this->CRC16(this->_TXBuffer, (dataLength + 4));
	
	digitalWrite(this->_directionPin, HIGH);
	this->_serial->write(DRAGONNET_STARTBYTE);
	this->_serial->write(this->_TXBuffer, (dataLength + 4));
	this->_serial->write(highByte(crc));
	this->_serial->write(lowByte(crc));
	this->_serial->write(DRAGONNET_ENDBYTE);
	this->_serial->flush();
	digitalWrite(this->_directionPin, LOW);
	
	return;
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
				if(this->_RXBuffer[1] == this->_parameters.address || this->_RXBuffer[1] == DRAGONNET_BROADCASTADDRESS || this->_parameters.receiveAll == true)
				{
					if(this->_RXcallback != NULL)
					{
						this->_RXcallback(this->_RXBuffer[2], this->_RXBuffer[1], this->_RXBuffer + 6, this->_RXBufferIndex - 9);
					}
				}
				else
				{
					// Пакет предназначен не для этого устройства.
					if(this->_ErrorCallback != NULL)
					{
						this->_ErrorCallback(0xEE);
					}
				}
			}
			else
			{
				// Ошибка CRC.
				if(this->_ErrorCallback != NULL)
				{
					this->_ErrorCallback(DRAGONNET_ERROR_CRC);
				}
			}
		}
		
		this->_lastReceiveTime = currentTime;
	}
	
	return;
}

void DragonNET::SetSystemByte(uint8_t index, bool value)
{
	bitWrite(this->_systemByte, index, value);
	
	return;
}

bool DragonNET::GetSystemByte(uint8_t index)
{
	return bitRead(this->_systemByte, index);
}

void DragonNET::Begin()
{
	pinMode(this->_directionPin, OUTPUT);
	digitalWrite(this->_directionPin, LOW);
	
	this->ClearArray(this->_TXBuffer, sizeof(this->_TXBuffer));
	this->ClearArray(this->_RXBuffer, sizeof(this->_RXBuffer));
	
	return;
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
	
	return;
}
