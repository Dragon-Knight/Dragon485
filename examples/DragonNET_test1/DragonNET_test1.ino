

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


// Если возможно, то предпочтительнее первый вариант, в котором из data будет обрезано всё, кроме самих данных, не создавая новый массив.
//void OnReceive(uint8_t fromAddress, byte *data, uint8_t dataLength)
void OnReceive(byte *data, uint8_t dataLength)
{
	/*
	Содержимое data:
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
	
	byte data1[] = {'O', 'K'};
	bus.TransmitPackage(0x01, data1, sizeof(data1));
}
