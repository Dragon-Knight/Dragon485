# DragonNET
Arduino library for RS-485 network communication

#!!! IN DEVELOPING !!!
## Progress
Master-Slave protocol: ![Progress](http://progressed.io/bar/40)

P2P protocol: ![Progress](http://progressed.io/bar/10)



### Класс DragonNET_Packet
Класс предназначен для работы с пакетом данных, который упрощает работу с библиотекой в целом.

Таблица методов и их назначения:
| Метод | Описание | Примечание |
| --- | --- | --- |
| `void PutToAddress(uint8_t)` | Вставить адрес получателя | - |
| `uint8_t TakeToAddress()` | Взять адрес получателя | - |
| `void PutFromAddress(uint8_t)` | Вставить адрес отправителя | - |
| `uint8_t TakeFromAddress()` | Взять адрес отправителя | - |
| `void PutConfigByte(byte)` | Вставить байт настройки | - |
| `byte TakeConfigByte()` | Взять байт настройки | - |
| `bool PutData1(byte)` | Вставить данные побайтно | - |
| `byte TakeData1()` | Взять данные побайтно | - |
| `bool PutData2(byte*, uint8_t)` | Вставить данные по ссылке | - |
| `const byte *TakeData2()` | Взять данные по ссылке | - |
| `uint8_t TakeDataLength()` | Взять длину данных | - |
| `bool PutPacket1(byte)` | Вставить пакет побайтно | - |
| `byte TakePacket1()` | Взять пакет побайтно | - |
| `bool PutPacket2(byte*, uint8_t)` | Вставить пакет по ссылке | - |
| `const byte *TakePacket2()` | Взять пакет по ссылке | - |
| `uint8_t TakePacketLength()` | Взять длину пакета | - |
| `uint8_t CheckPacket()` | Проверка пакета, при приёме | Вернёт: `0` - Пакет корректный, `1` - Ошибка структуры пакета, `2` - Ошибка CRC16. |
| `void PreparePackage()` | Подготовка пакета перед отправкой | Системный метод |
| `uint16_t TakeCRC16()` | Взять CRC16 от пакета | - |
| `void Cleaning()` | Сбросить класс | - |
