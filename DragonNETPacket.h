/*
 *	DragonNETPacket.h
 *	Class data packet.
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#ifndef DragonNETPacket_h_
#define DragonNETPacket_h_

#if defined(ARDUINO_ARCH_AVR)
	#include <util/crc16.h>
#else
	namespace
	{
		inline uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data)
		{
			data ^= crc & 0xff;
			data ^= data << 4;
			
			return ((((uint16_t)data << 8) | ((crc >> 8) & 0xff)) ^ (uint8_t)(data >> 4) ^ ((uint16_t)data << 3));
		}
	}
#endif

template <uint8_t _maxDataLength>
class DragonNETPacket
{
	public:
		DragonNETPacket()
		{
			this->Cleaning();
			
			return;
		}
		
		~DragonNETPacket() = default;
		
		// Copy & Move constructors.
		DragonNETPacket(const DragonNETPacket&) = default;
		DragonNETPacket(DragonNETPacket &&) = default;
		
		// Copy & Move assigments.
		DragonNETPacket& operator=(const DragonNETPacket &) = default;
		DragonNETPacket& operator=(DragonNETPacket &&) = default;
		
		void PutToAddress(uint8_t address)
		{
			this->_buffer[1] = address;
			
			return;
		}
		
		uint8_t TakeToAddress() const
		{
			return this->_buffer[1];
		}
		
		void PutFromAddress(uint8_t address)
		{
			this->_buffer[2] = address;
			
			return;
		}
		
		uint8_t TakeFromAddress() const
		{
			return this->_buffer[2];
		}
		
		void PutConfigByte(byte config)
		{
			this->_buffer[3] = config;
			
			return;
		}
		
		byte TakeConfigByte() const
		{
			return this->_buffer[3];
		}
		
		bool PutData1(byte data)
		{
			bool result = false;
			
			if(this->_putDataIndex < _maxDataLength)
			{
				this->_buffer[5 + this->_putDataIndex++] = data;
				this->_buffer[4] = this->_putDataIndex;
				
				result = true;
			}
			
			return result;
		}
		
		byte TakeData1()
		{
			byte result = 0x00;
			
			if(this->_takeDataIndex < this->_buffer[4])
			{
				result = this->_buffer[5 + this->_takeDataIndex++];
			}
			
			return result;
		}
		
		bool PutData2(byte *data, uint8_t length)
		{
			bool result = false;
			
			if(length <= _maxDataLength)
			{
				for(uint8_t i = 0; i < length; ++i)
				{
					this->_buffer[5 + i] = data[i];
				}
				this->_buffer[4] = length;
				
				result = true;
			}
			
			return result;
		}
		
		const byte *TakeData2() const
		{
			return this->_buffer + 5;
		}
		
		uint8_t TakeDataLength() const
		{
			return this->_buffer[4];
		}
		
		bool PutPacket1(byte packet)
		{
			bool result = false;
			
			if(this->_putPacketIndex <= (_maxDataLength + 8))
			{
				this->_buffer[this->_putPacketIndex++] = packet;
				
				result = true;
			}
			
			return result;
		}
		
		byte TakePacket1()
		{
			byte result = 0x00;
			
			if(this->_takePacketIndex < (this->_buffer[4] + 8))
			{
				result = this->_buffer[this->_takePacketIndex++];
			}
			
			return result;
		}
		
		bool PutPacket2(byte *packet, uint8_t length)
		{
			bool result = false;
			
			if(length <= (_maxDataLength + 8))
			{
				for(uint8_t i = 0; i < length; ++i)
				{
					this->_buffer[i] = packet[i];
				}
				
				result = true;
			}
			
			return result;
		}
		
		const byte *TakePacket2() const
		{
			return this->_buffer;
		}
		
		uint8_t TakePacketLength() const
		{
			return (this->_buffer[4] + 8);
		}
		
		uint8_t CheckPacket() const
		{
			uint8_t result = 0;
			
			if(this->_buffer[0] != 0x5B || this->_buffer[this->_buffer[4] + 7] != 0x5D)
			{
				result = 1;
			}
			else
			{
				uint16_t crc = this->TakeCRC16();
				if(this->_buffer[this->_buffer[4] + 5] != highByte(crc) || this->_buffer[this->_buffer[4] + 6] != lowByte(crc))
				{
					result = 2;
				}
			}
			
			return result;
		}
		
		void PreparePackage()
		{
			uint16_t crc = this->TakeCRC16();
			
			this->_buffer[this->_buffer[4] + 5] = highByte(crc);
			this->_buffer[this->_buffer[4] + 6] = lowByte(crc);
			this->_buffer[this->_buffer[4] + 7] = 0x5D;
			
			return;
		}
		
		uint16_t TakeCRC16() const
		{
			uint16_t result = 65535;
			
			for(uint8_t i = 1; i < (this->_buffer[4] + 4); ++i)
			{
				result = _crc_ccitt_update(result, this->_buffer[i]);
			}
			
			return result;
		}
		
		void Cleaning()
		{
			memset(this->_buffer, 0x00, (_maxDataLength + 8));
			this->_buffer[0] = 0x5B;
			this->_putDataIndex = 0;
			this->_takeDataIndex = 0;
			this->_putPacketIndex = 0;
			this->_takePacketIndex = 0;
			
			return;
		}
	protected:
		
	private:
		byte _buffer[_maxDataLength + 8];
		uint8_t _putDataIndex;
		uint8_t _takeDataIndex;
		uint8_t _putPacketIndex;
		uint8_t _takePacketIndex;
};

#endif
