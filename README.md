# DragonNET
Arduino library for RS-485 network communication

#!!! IN DEVELOPING !!!
## Progress
Master-Slave protocol: ![Progress](http://progressed.io/bar/40)

P2P protocol: ![Progress](http://progressed.io/bar/10)



### Класс DragonNET_Packet
Класс предназначен для работы с пакетом данных, который упрощает работу с библиотекой в целом.

| Метод | Описание | Тип |
| --- | --- | :---: |
| `void PutToAddress(uint8_t)` | Вставить адрес получателя | Пользовательский |
| `uint8_t TakeToAddress()` | Взять адрес получателя | Пользовательский |
| `void PutFromAddress(uint8_t)` | Вставить адрес отправителя | Пользовательский |
| `uint8_t TakeFromAddress()` | Взять адрес отправителя | Пользовательский |
| `void PutConfigByte(byte)` | Вставить байт настройки | Системный |
| `byte TakeConfigByte()` | Взять байт настройки | Системный |
| `bool PutData1(byte)` | Вставить данные побайтно | Пользовательский / Системный |
| `byte TakeData1()` | Взять данные побайтно | Пользовательский / Системный |
| `bool PutData2(byte*, uint8_t)` | Вставить данные по ссылке | Пользовательский / Системный |
| `const byte *TakeData2()` | Взять данные по ссылке | Пользовательский / Системный |
| `uint8_t TakeDataLength()` | Взять длину данных | Пользовательский / Системный |
| `bool PutPacket1(byte)` | Вставить пакет побайтно | Пользовательский / Системный |
| `byte TakePacket1()` | Взять пакет побайтно | Пользовательский / Системный |
| `bool PutPacket2(byte*, uint8_t)` | Вставить пакет по ссылке | Пользовательский / Системный |
| `const byte *TakePacket2()` | Взять пакет по ссылке | Пользовательский / Системный |
| `uint8_t TakePacketLength()` | Взять длину пакета | Пользовательский / Системный |
| `uint8_t CheckPacket()` | Проверка пакета, при приёме. Вернёт:<br> `0` - Пакет корректный<br> `1` - Ошибка структуры пакета<br> `2` - Ошибка CRC16. | Пользовательский / Системный |
| `void PreparePackage()` | Подготовка пакета перед отправкой | Системный |
| `uint16_t TakeCRC16()` | Взять CRC16 от пакета | Пользовательский / Системный |
| `void Cleaning()` | Сбросить класс | Пользовательский / Системный |
