/*
 *	DragonNETCore.h
 *	Class of basic functions of the protocol.
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#ifndef DragonNETCore_h_
#define DragonNETCore_h_

class DragonNETCore
{
	using RXcallback_t = bool (*)(DragonNETPacket_t &request, DragonNETPacket_t &response);
	using ErrorCallback_t = void (*)(uint8_t errorType);
	
	public:
	
		void Begin(uint32_t baudRate, uint8_t address, bool receiveAll, RXcallback_t callback1, ErrorCallback_t callback2 = NULL)
		{
			pinMode(this->_directionPin, OUTPUT);
			digitalWrite(this->_directionPin, LOW);
			
			this->_serial->begin(baudRate);
			this->_myAddress = address;
			this->_receiveAll = receiveAll;
			this->_RXcallback = callback1;
			this->_ErrorCallback = callback2;
			
			return;
		}
	
	protected:
	
		DragonNETCore(UART &serial, uint8_t directionPin) : _serial(&serial), _directionPin(directionPin)
		{
			return;
		}
		
		void TransmitPackage(DragonNETPacket_t &package, uint32_t time = micros())	// ???
		{
			package.PutConfigByte(this->_configByte);
			package.PreparePackage();
			if(package.CheckPacket() == 0)
			{
				digitalWrite(this->_directionPin, HIGH);
				this->_serial->write(package.TakePacket2(), package.TakePacketLength());
				this->_serial->flush();
				digitalWrite(this->_directionPin, LOW);
			}
			package.Cleaning();
			
			//this->_lastTransmitTime = micros();
			
			return;
		}
		
		void ReceivePackage(uint32_t currentMicroTime)
		{
			if(this->_lastMicroTime + DRAGONNET_RECEIVETIMEOUT <= currentMicroTime)
			{
				this->_lastMicroTime = currentMicroTime;
				
				this->_requestPacket.Cleaning();
				
				while(this->_serial->available() > 0){ this->_serial->read(); }
			}
			
			bool isReceived = false;
			while(this->_serial->available() > 0)
			{
				if(this->_requestPacket.PutPacket1(this->_serial->read()) == false)
				{
					this->_requestPacket.Cleaning();
					
					if(this->_ErrorCallback != NULL)
					{
						this->_ErrorCallback(DRAGONNET_ERROR_OVERFLOW);
					}
				}
				
				isReceived = true;
			}
			
			if(isReceived == true)
			{
				uint8_t result = this->_requestPacket.CheckPacket();	// Проблема. Пока пакет не до конца собран, будут вызывается колбэки о нарушенной структуре пакета.
				if(result == 0)
				{
					if(this->_requestPacket.TakeToAddress() == this->_myAddress || this->_requestPacket.TakeToAddress() == DRAGONNET_BROADCASTADDRESS || this->_receiveAll == true)
					{
						byte rxconfig = this->_requestPacket.TakeConfigByte();
						
						if(rxconfig & DRAGONNET_SYSTEM_COMMAND)
						{
							// Парсер системных команд.
						}
						
						/*
						if(rxconfig & DRAGONNET_NEED_CONFIRM)
						{
							this->_responsePacket.Cleaning();
							this->_responsePacket.PutToAddress(this->_requestPacket.TakeFromAddress());
							this->_responsePacket.PutFromAddress(this->_myAddress);
							this->_configByte = this->_configByte | DRAGONNET_SYSTEM_COMMAND | DRAGONNET_CONFIRM_COMMAND;
							this->_responsePacket.PutData1('Y');
							this->TransmitPackage(this->_responsePacket);
						}
						*/
						
						if(this->_RXcallback != NULL)
						{
							this->_responsePacket.Cleaning();
							
							//this->_configByte = this->_configByte | this->_RXcallback(this->_requestPacket, this->_responsePacket);
							bool response = this->_RXcallback(this->_requestPacket, this->_responsePacket);
							
							if(response == true)
							{
								this->_responsePacket.PutFromAddress(this->_myAddress);
								this->TransmitPackage(this->_responsePacket);
							}
						}
					}
				}
				else
				{
					if(this->_ErrorCallback != NULL)
					{
						this->_ErrorCallback(DRAGONNET_ERROR_ + result);
					}
				}
				
				this->_lastMicroTime = currentMicroTime;
			}
			
			return;
		}
		
		byte _configByte;	// Может переделать...
	
	private:
	
		UART *_serial;
		DragonNETPacket_t _requestPacket;
		DragonNETPacket_t _responsePacket;
		RXcallback_t _RXcallback;
		ErrorCallback_t _ErrorCallback;
		uint32_t _lastMicroTime = 0;
		uint8_t _directionPin;
		uint8_t _myAddress;
		bool _receiveAll;
};

#endif
