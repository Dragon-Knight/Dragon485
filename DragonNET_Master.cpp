/*
 *	DragonNET_Master.cpp
 *	Class of functions of a master device.
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#include <Arduino.h>
#include "DragonNET.h"

void DragonNET_Master::Processing()
{
	uint32_t currentMicroTime = micros();
	
	this->Processing(currentMicroTime);
	
	return;
}

void DragonNET_Master::Processing(uint32_t currentMicroTime)
{
	this->ReceivePackage(currentMicroTime);
	
	return;
}

void DragonNET_Master::initialization()
{
	this->_configByte = 0b01000000;
	
	return;
}
