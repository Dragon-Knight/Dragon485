/*
 *	DragonNET_Packet.h
 *  Class data packet.
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

#ifndef DragonNET_Packet_h_
#define DragonNET_Packet_h_

#if defined(ARDUINO_ARCH_AVR)
	#include <util/crc16.h>
#else
	uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data)
	{
		data ^= crc & 0xff;
		data ^= data << 4;
		
		return ((((uint16_t)data << 8) | ((crc >> 8) & 0xff)) ^ (uint8_t)(data >> 4) ^ ((uint16_t)data << 3));
	}
#endif

template <uint8_t _maxDataLength>
class DragonNET_Packet
{
	public:
		/*
			Конструктор.
		*/
		DragonNET_Packet()
		{
			this->Cleaning();
			
			return;
		}
		
		/*
			Деструктор.
		*/
		~DragonNET_Packet()
		{
			return;
		}
		
		/*
			Вставить адрес получателя.
		*/
		void PutToAddress(uint8_t address)
		{
			this->_buffer[1] = address;
			
			return;
		}
		
		/*
			Получить адрес получателя.
		*/
		uint8_t TakeToAddress()
		{
			return this->_buffer[1];
		}
		
		/*
			Вставить адрес отправителя.
		*/
		void PutFromAddress(uint8_t address)
		{
			this->_buffer[2] = address;
			
			return;
		}
		
		/*
			Получить адрес отправителя.
		*/
		uint8_t TakeFromAddress()
		{
			return this->_buffer[2];
		}
		
		/*
			Вставить байт настройки.
		*/
		void PutConfigByte(byte config)
		{
			this->_buffer[3] = config;
			
			return;
		}
		
		/*
			Получить байт настройки.
		*/
		byte TakeConfigByte()
		{
			return this->_buffer[3];
		}
		
		/*
			Вставить данные побайтно.
		*/
		bool PutData(byte data)
		{
			bool result = false;
			
			if(this->_dataLength < _maxDataLength)
			{
				this->_buffer[5 + this->_dataLength++] = data;
				this->_buffer[4] = this->_dataLength;
				
				result = true;
			}
			
			return result;
		}
		
		/*
			Получить данные побайтно.
		*/
		byte TakeData()
		{
			byte result = 0x00;
			
			if(this->_takeDataIndex < this->_buffer[4])
			{
				result = this->_buffer[5 + this->_takeDataIndex++];
			}
			
			return result;
		}
		
		/*
			Вставить данные по ссылке.
		*/
		bool PutData(byte *data, uint8_t length)
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
		
		/*
			Получить данные по ссылке.
		*/
		byte *TakeData()
		{
			return &this->_buffer + 5;
		}
		
		/*
			Получить длину данных.
		*/
		uint8_t TakeDataLength()
		{
			return this->_buffer[4];
		}
		
		/*
			Вставить пакет побайтно.
		*/
		bool PutPacket(byte packet)
		{
			bool result = false;
			
			if(this->putPacketIndex <= (_maxDataLength + 8))
			{
				this->_buffer[this->putPacketIndex++] = packet;
				
				result = true;
			}
			
			return result;
		}
		
		/*
			Получить пакет побайтно.
		*/
		byte TakePacket()
		{
			byte result = 0x00;
			
			if(this->_takePacketIndex < (this->_buffer[4] + 8))
			{
				result = this->_buffer[this->_takePacketIndex++];
			}
			
			return result;
		}
		
		/*
			Вставить пакет по ссылке.
		*/
		bool PutPacket(byte *packet, uint8_t length)
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
		
		/*
			Получить пакет по ссылке.
		*/
		byte *TakePacket()
		{
			return &this->_buffer;
		}
		
		/*
			Получить длину пакета.
		*/
		uint8_t TakePacketLength()
		{
			return (this->_buffer[4] + 8);
		}
		
		/*
			Проверить пакет на валидность (Стартовый и топовый байт, длинна данных и фактическая длинна данных, CRC16).
		*/
		bool IsValidPacket()
		{
			bool result = true;
			
			if(this->_buffer[0] != 0x5B || this->_buffer[this->_buffer[4] + 3] != 0x5D)
			{
				// Нету стартового или стопового байта или нарушена длинна данных (указанная длинна одна а фактическая другая).
				result = false;
			}
			
			uint16_t crc = this->TakeCRC16();
			if(this->_buffer[this->_buffer[4] + 1] != highByte(crc) || this->_buffer[this->_buffer[4] + 2] != lowByte(crc))
			{
				// Нарушена CRC пакета.
				result = false;
			}
			
			return result;
		}
		
		/*
			Посчитать CRC16.
		*/
		uint16_t TakeCRC16()
		{
			uint16_t result = 65535;
			
			for(uint8_t i = 1; i < (this->_buffer[4] + 4); ++i)
			{
				result = _crc_ccitt_update(result, this->_buffer[i]);
			}
			
			return result;
		}
		
		/*
			Почистить поля класса.
		*/
		void Cleaning()
		{
			memset(this->_buffer, 0x00, (_maxDataLength + 8));
			this->_buffer[0] = 0x5B;
			this->_buffer[_maxDataLength + 7] = 0x5D;
			this->_dataLength = 0;
			this->_takeDataIndex = 0;
			this->_takePacketIndex = 0;
			this->_crc16 = 65535;
			
			return;
		}
	protected:
		
	private:
		byte _buffer[_maxDataLength + 8];
		uint8_t _dataLength;
		uint8_t _takeDataIndex;
		uint8_t _takePacketIndex;
		uint16_t _crc16;
};

#endif

/*
	Пакет:
		[0]    - Стартовый байт.
		[*1]   - Адрес получателя.
		[*2]   - Адрес отправителя.
		[*3]   - Системный байт.
		  |- [7] - Флаг типа сети, 0 - Ведущий-Ведомые, 1 - P2P.
		  |- [6] - Флаг мастер устройства, 0 - Ведомый, 1 - Ведущий.
		  |- [5] - Флаг системной команды, 0 - Общая, 1 - Системная.
		  |- [4] - Резерв
		  |- [3] - Резерв
		  |- [2] - Флаг запроса с ожиданием ответа, 0 - Ответ не нужен, 1 - Ответ нужен.
		  |- [1] - Флаг подтверждения получения пакета, 0 - Подтверждение не нужно, 1 - Подтверждение нужно, (Не может быть установлен на подтверждающий пакет).
		  |- [0] - Резерв
		[*4]   - Длинна данных.
		[*5+n] - Данные.
		[6+n]  - CRC16 H.
		[7+n]  - CRC16 L.
		[8+n]  - Стоповый байт.
	Символом '*' указаны фрагменты данных, участвующие в подсчёте CRC16.
*/

/*
	Нужные методы:
	*1)  Вставить \ Взять адрес получателя.
	*2)  Вставить \ Взять адрес отправителя.
	*3)  Вставить \ Взять байт настройки.
	*4)  Вставить \ Взять данные побайтно.
	*5)  Вставить \ Взять данные по ссылке.
	*6)  Взять длину данных.
	*7)  Вставить \ Взять пакет побайтно.
	*8)  Вставить \ Взять пакет по ссылке.
	*9)  Взять длину пакета.
	*10) Проверить пакет на валидность (Стартовый и топовый байт, длинна данных и фактическая длинна данных, CRC16).
	
	Не реализовано:
		Логика проверки CRC16 при вставке готового пакета (приём пакета), а так-же вставка CRC16 в пакет, при сборе пакета (отправка пакета)

*/
