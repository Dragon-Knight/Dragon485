/*
 *	DragonNETMaster.h
 *	Class of functions of a master device.
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#ifndef DragonNETMaster_h_
#define DragonNETMaster_h_

class DragonNETMaster : public DragonNETCore
{
	public:
	
		DragonNETMaster(UART &serial, uint8_t directionPin) : DragonNETCore(serial, directionPin)
		{
			this->_configByte = 0b01000000;
			
			return;
		}
		
		DragonNETMaster(UART &&serial, uint8_t directionPin) = delete;
		
		void Processing(uint32_t currentMicroTime = micros())
		{
			this->ReceivePackage(currentMicroTime);
			
			return;
		}
		
		void Send(DragonNETPacket_t packet)
		{
			this->TransmitPackage(packet);
			
			return;
		}
	
	protected:
	
	private:
	
};

#endif
