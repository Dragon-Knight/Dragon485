/*
 *	DragonNETParser.h
 *	Parser System Command
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#ifndef DragonNETParser_h_
#define DragonNETParser_h_

void DragonNETParser(DragonNETPacket_t &package)
{
	byte command = package.TakeData1();
	
	switch(command)
	{
		case 0x01:
		{
			break;
		}
	}
	
	
	return;
}

#endif
