/*
	
*/

#include <Arduino.h>
#include "DragonNET.h"

//#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
DragonNET_Master::DragonNET_Master(HardwareSerial &serial, uint8_t directionPin, uint32_t baudRate) :
	_serial(&serial),
	_directionPin(directionPin)
{
	
	serial.begin(baudRate);
}
//#else
//DragonNET_Master::DragonNET_Master(uint8_t RXPin, uint8_t TXPin, uint8_t directionPin) : _serial(&SoftwareSerial(RXPin, TXPin))
DragonNET_Master::DragonNET_Master(SoftwareSerial &serial, uint8_t directionPin, uint32_t baudRate) :
	_serial(&serial),
	_directionPin(directionPin)
{
	serial.begin(baudRate);
}
//#endif

void DragonNET_Master::Begin(uint8_t address, bool receiveAll)
{
	//this->_serial->begin(baudRate);
	this->_address = address;
	this->_receiveAll = receiveAll;
	
	pinMode(this->_directionPin, OUTPUT);
	digitalWrite(this->_directionPin, LOW);
	
	this->ClearArray(this->_TXBuffer, sizeof(this->_TXBuffer));
	this->ClearArray(this->_RXBuffer, sizeof(this->_RXBuffer));
	
	return;
}

//void DragonNET_Master::AttachRXCallback(void (*callback)(uint8_t fromAddress, byte *data, uint8_t dataLength))
void DragonNET_Master::AttachRXCallback(void (*callback)(byte *data, uint8_t dataLength))
{
	this->_callback = callback;
	
	return;
}

//void DragonNET_Master::Send



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

void DragonNET_Master::TransmitPackage(uint8_t toAddress, byte *data, uint8_t dataLength)
{
	/*
	uint8_t i;
	
	this->ClearArray(this->_TXBuffer, sizeof(this->_TXBuffer));
	
	this->_TXBuffer[0] = DRAGONNET_STARTBYTE;
	this->_TXBuffer[1] = toAddress;
	this->_TXBuffer[2] = this->_address;
	this->_TXBuffer[3] = this->_parameter[0];
	this->_TXBuffer[4] = this->_parameter[1];
	this->_TXBuffer[5] = dataLength;
	for(i = 0; i < dataLength; ++i)
	{
		this->_TXBuffer[6+i] = data[i];
	}
	this->_TXBuffer[8+i] = DRAGONNET_ENDBYTE;
	
	uint16_t crc = this->CRC16(this->_TXBuffer, (dataLength + 9));
	
	this->_TXBuffer[6+i] = highByte(crc);
	this->_TXBuffer[7+i] = lowByte(crc);
	
	digitalWrite(this->_directionPin, HIGH);
	this->_serial->write(this->_TXBuffer, (dataLength + 9));
	digitalWrite(this->_directionPin, LOW);
	*/
	

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
	digitalWrite(this->_directionPin, LOW);

	
	return;
}

void DragonNET_Master::ReceivePackage()
{
	uint32_t currentTime = millis();
	
	if(this->_lastReceiveTime + 100 <= currentTime)
	{
		this->_lastReceiveTime = currentTime;
		
		this->ClearArray(this->_RXBuffer, sizeof(this->_RXBuffer));
		this->_RXBufferIndex = 0;
		//this->_serial->write(0xEE); // Два раза вызывается?
	}
	
	bool isReceived = false;
	while(this->_serial->available() > 0)
	{
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
					//this->_callback(this->_RXBuffer[2], this->_RXBuffer, this->_RXBufferIndex);
					this->_callback(this->_RXBuffer, this->_RXBufferIndex);
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
	}
	
	return;
}


uint16_t DragonNET_Master::CRC16(byte *data, uint8_t length)
{
	uint16_t crc = 0xFFFF;
	uint8_t i;
	
	while(length--)
	{
		crc ^= *data++ << 8;
		for(i = 0; i < 8; i++)
		{
			crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
		}
	}
	
	return crc;
}

void DragonNET_Master::ClearArray(byte *array, uint8_t length)
{
	// А можно ли тут использовать sizeof() в замен length?
	memset(array, 0x00, length);
	
	return;
}
