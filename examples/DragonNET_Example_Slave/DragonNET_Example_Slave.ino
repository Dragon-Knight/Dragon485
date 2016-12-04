/*
 *	DragonNET_Example_Slave.ino
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */


#define DRAGONNET_USE_SOFTWARESERIAL
#include "DragonNET.h"

#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
	DragonNET_Slave bus(Serial, 13);
#else
	SoftwareSerial mySerial(12, 3);
	DragonNET_Slave bus(mySerial, 13);
#endif

void setup()
{
	Serial.begin(115200);
	
	bus.Begin(115200, 0x02, false);
	bus.AttachRXCallback(OnReceive);
	bus.AttachErrorCallback(OnError);
	
	return;
}

void loop()
{
	bus.Processing();
	
	return;
}

uint8_t OnReceive(DragonNETPacket &request, DragonNETPacket &response)
{
	bool result = false;
	
	if(request.TakeData1() == '5' && request.TakeData1() == '5' && request.TakeData1() == '5')
	{
		response.PutToAddress(request.TakeFromAddress());
		response.PutFromAddress(request.TakeToAddress());
		response.PutData1(0x50);
		response.PutData1(0x50);
		response.PutData1(0x50);
		response.PutData1(0x50);
		response.PutData1(0x50);
		
		result = true;
	}
	
	Serial.write(request.TakeData2(), request.TakeDataLength());
	
	return result;
}

void OnError(uint8_t errorType)
{
	Serial.write(">");
	Serial.write(errorType);
	Serial.write("<");
	
	return;
}
