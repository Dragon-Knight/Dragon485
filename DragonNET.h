/*
 *	DragonNET.h
 *	RS-485 network communication protocol.
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@coauthor	Valeriy V Dmitriev aka valmat <ufabiz@gmail.com>
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/DragonNET
 */

/*
	Пакет:
		[0]    - Стартовый байт.
		[*1]   - Адрес получателя.
		[*2]   - Адрес отправителя.
		[*3]   - Системный байт.
		  |- [7] - Флаг типа сети, 0 - Ведущий-Ведомые, 1 - P2P.
		  |- [6] - Флаг мастер устройства, 0 - Ведомый, 1 - Ведущий.
		  |- [5] - Флаг системной команды, 0 - Общая, 1 - Системная.
		  |- [4] - Резерв.
		  |- [3] - Резерв.
		  |- [2] - Флаг запроса с ожиданием ответа, 0 - Ответ не нужен, 1 - Ответ нужен.
		  |- [1] - Флаг подтверждения получения пакета, 0 - Подтверждение не нужно, 1 - Подтверждение нужно, (Не может быть установлен на подтверждающий пакет).
		  |- [0] - Резерв.
		[*4]   - Длинна данных.
		[*5+n] - Данные.
		[6+n]  - CRC16 H.
		[7+n]  - CRC16 L.
		[8+n]  - Стоповый байт.
	Символом '*' указаны фрагменты данных, участвующие в подсчёте CRC16.
*/

/*
	Весь код написан исключительно в h файлах по причине того, что не знаю как сделать класс, который сможет принимать как HardwareSerial так и SoftwareSerial,
	а статический полиморфизм для меня как китайский язык...
	Если есть идеи, то добро пожаловать :)
	Задача: полная работоспособность на обоих UART.
	Причём некоторые микроконтроллеры не имеют на борту HardwareSerial.
*/

#ifndef DragonNET_h_
#define DragonNET_h_

#include <Arduino.h>
#include "DragonNETPacket.h"

#if !defined(DRAGONNET_USE_SOFTWARESERIAL)
	#define UART	HardwareSerial
	#include <HardwareSerial.h>
#else
	#define UART	SoftwareSerial
	#include <SoftwareSerial.h>
#endif

#define DRAGONNET_BROADCASTADDRESS	0xFF
#define DRAGONNET_RECEIVETIMEOUT	10000		// мкс
#define DRAGONNET_BUFFERSIZE		64


#define DRAGONNET_SYSTEM_COMMAND	0x20		// Флаг системной команды.

#define DRAGONNET_NEED_ANSWER		0x04		// Пакет требует ответ.
#define DRAGONNET_NEED_CONFIRM		0x02		// Пакет требует подтверждения получения.

#define DRAGONNET_ERROR_			0xE0		// База ошибок.
#define DRAGONNET_ERROR_STRUCTURE	0xE1		// Нарушена структура пакета.
#define DRAGONNET_ERROR_CRC			0xE2		// Нарушена контрольная сумма.
#define DRAGONNET_ERROR_OVERFLOW	0xEF		// Переполнение RX буфера.

using DragonNETPacket_t = DragonNETPacket<DRAGONNET_BUFFERSIZE>;

#include "DragonNETCore.h"
#include "DragonNETMaster.h"
#include "DragonNETSlave.h"
#include "DragonNETP2P.h"

#endif
