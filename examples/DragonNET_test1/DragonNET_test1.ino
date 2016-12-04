/*
 *	DragonNET_test1.ino
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

// 0x5B 0x01 0x00 0x00 0x0F 0x48 0x65 0x6C 0x6C 0x6F 0x2C 0x20 0x57 0x6F 0x72 0x6C 0x64 0x21 0x21 0x21 0x64 0xD2 0x5D

//#define MASTER
//#define DRAGONNET_USE_SOFTWARESERIAL
#include "DragonNET.h"

#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
	#if defined(MASTER)
		DragonNET_Master bus(Serial, 13);
	#else
		DragonNET_Slave bus(Serial, 13);
	#endif
#else
	SoftwareSerial mySerial(2, 3);
	#if defined(MASTER)
		DragonNET_Master bus(mySerial, 13);
	#else
		DragonNET_Slave bus(mySerial, 13);
	#endif
	//DragonNET_Master bus(SoftwareSerial(2, 3), 13);	// Вот так не скомпилируется из-за третьего конструктора, который delete
#endif

DragonNETPacket Packet;

/*
DragonNETParameters_t param = 
{
	0x00,
	"My cool dev",
	true
};
*/

void setup()
{
	//bus.Begin(115200, param);
	bus.Begin(115200, 0x00, false);
	bus.AttachRXCallback(OnReceive);
	bus.AttachErrorCallback(OnError);
	
	byte data[] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', '!', '!'};
	
	Packet.PutData2(data, sizeof(data));
	Packet.PutToAddress(0x01);
	
	bus.TransmitPackage(Packet);
	
	return;
}

void loop()
{
	bus.Processing();
	
	return;
}

void OnReceive(DragonNETPacket &request, DragonNETPacket &response)
{
	response.PutToAddress(45);
	response.PutFromAddress(46);
	//response.PutConfigByte(99);
	response.PutData1(0x50);
	response.PutData1(0x50);
	response.PutData1(0x50);
	response.PutData1(0x50);
	response.PutData1(0x50);
	
	Serial.write(request.TakeData2(), request.TakeDataLength());
	
	/*
		fromAddress - Адрес отправителя.
		toAddress - Адрес получателя.
		data - Данные.
		dataLength - Длинна данных.
	*/
	
	//byte data1[] = {'O', 'K'};
	//bus.TransmitPackage(0x01, data1, sizeof(data1));
	
	return;
}

void OnError(uint8_t errorType)
{
	//byte data1[] = {'E', 'E', 'E', errorType, 'E', 'E', 'E'};
	//bus.TransmitPackage(0x01, data1, sizeof(data1));
	
	return;
}
