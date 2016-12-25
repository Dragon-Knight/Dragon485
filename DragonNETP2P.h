/*
 *	DragonNETP2P.h
 *	Class of functions of the P2P network (Peer-to-peer network).
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#ifndef DragonNETP2P_h_
#define DragonNETP2P_h_

class DragonNETP2P : public DragonNETCore
{
	public:
	
		DragonNETP2P(UART &serial, uint8_t directionPin) : DragonNETCore(serial, directionPin)
		{
			this->_configByte = 0b10000000;
			
			return;
		}
		
		DragonNETP2P(UART &&serial, uint8_t directionPin) = delete;
		
		void Processing(uint32_t currentMicroTime = micros())
		{
			this->ReceivePackage(currentMicroTime);
			
			return;
		}
	
	protected:
	
	private:
	
};

#endif
