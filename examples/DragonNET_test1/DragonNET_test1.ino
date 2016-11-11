

//#define DRAGONNET_USE_SOFTWARESERIAL
#include "DragonNET.h"



#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
DragonNET_Master bus(Serial, 5);
#else
DragonNET_Master bus(2, 3, 5);
#endif







void setup()
{
	//Serial.begin(115200);
	//Serial.setTimeout(25);
	
	bus.Begin(0x00, 115200);
	bus.AttachRXCallback(OnReceive);
	
	return;
}

void loop()
{	
	return;
}


void OnReceive(uint8_t fromAddress, byte *data, uint8_t dataLength)
{
	
}
