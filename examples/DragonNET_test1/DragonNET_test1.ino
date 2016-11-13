

//#define DRAGONNET_USE_SOFTWARESERIAL		// Если раскоментировать, то непонятные ошибки. Почему?
#include "DragonNET.h"



#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
DragonNET_Master bus(Serial, 13, 115200);
#else
SoftwareSerial serial(2,3);
DragonNET_Master bus(serial, 13, 115200);

// Вот так не скомпилируется из-за третьего конструктора, который delete
//DragonNET_Master bus(SoftwareSerial(2,3), 13, 115200);

#endif









void setup()
{
	//Serial.begin(115200);
	//Serial.setTimeout(25);
	
	bus.Begin(0x00, true);
	bus.AttachRXCallback(OnReceive);
	
	byte data[] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
	bus.TransmitPackage(0x01, data, sizeof(data));
	
	return;
}

void loop()
{
	bus.ReceivePackage();
	
	return;
}

void OnReceive(uint8_t fromAddress, uint8_t toAddress, byte *data, uint8_t dataLength)
{
	/*
		fromAddress - Адрес отправителя.
		toAddress - Адрес получателя.
		data - Данные.
		dataLength - Длинна данных.
	*/
	
	byte data1[] = {'O', 'K'};
	bus.TransmitPackage(0x01, data1, sizeof(data1));
	
	return;
}
