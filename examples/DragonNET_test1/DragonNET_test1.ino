/*
 *	DragonNET_test1.ino
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

// Если раскоментировать, то компилится, но не работает.
// А вообще очень странно: Если serial->begin в конструкторе, то не работает HWserial. Если serial->begin в моём методе Begin, то не работает SWSerial.
//#define DRAGONNET_USE_SOFTWARESERIAL
#include "DragonNET.h"


#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
	DragonNET_Master bus(Serial, 13);
#else
	SoftwareSerial mySerial(2, 3);
	DragonNET_Master bus(mySerial, 13);
	
	//DragonNET_Master bus(SoftwareSerial(2, 3), 13);	// Вот так не скомпилируется из-за третьего конструктора, который delete
#endif


void setup()
{
	bus.Begin(115200, 0x00, true);
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
