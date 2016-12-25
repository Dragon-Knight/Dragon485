/*
 *	DragonNETSlave.h
 *	Class of functions of a slave device.
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#ifndef DragonNETSlave_h_
#define DragonNETSlave_h_

class DragonNETSlave : public DragonNETCore
{
	public:
	
		DragonNETSlave(UART &serial, uint8_t directionPin) : DragonNETCore(serial, directionPin)
		{
			this->_configByte = 0b00000000;
			
			return;
		}
		
		DragonNETSlave(UART &&serial, uint8_t directionPin) = delete;
		
		void Processing(uint32_t currentMicroTime = micros())
		{
			this->ReceivePackage(currentMicroTime);
			
			return;
		}
	
	protected:
	
	private:
	
};

#endif
