#include <util/delay.h>
//Настройка SPI - дисплей
void spi_setup(void){
  SPCR = (1 << SPE); //Включаем модуль
  SPSR |= (1 << SPI2X); //Удваиваем скорость
  SPCR |= (1 << MSTR); //Режим Master
  //Настраиваем подключенные пины как выходы
  DDRH |= (1 << PH5) | (1 << PH6); //RST, D/C
  DDRB = (1 << PB2) | (1 << PB1) | (1 << PB4) | (1 << PB0); //MOSI, SCK, CS
  PORTB |= (1 << PB3); //Включаем подтяжку
  PORTB &= ~(1 << PB4); //Выбираем для работы именно дисплей
}
//Функция для передачи одного байта
void take_data(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1 << SPIF)));
}
//Вспомогательная функция для отправки команды
void lcd_cmd(uint8_t data){
  PORTH &= ~(1 << PH6); // D/C = 0 чтобы подать команду
  take_data(data);
}
//Вспомогательная функция для отправки данных
void lcd_data(uint8_t data){
  PORTH |= (1 << PH6); // D/C = 1 чтобы подать команду
  take_data(data);
}
//Функция для настройки дисплея на прием - отправка базовых необходимых команд
void display_setup(void){
  // 1. Сброс
  PORTH &= ~(1 << PH5); // RST = 0 (пин 8)
  _delay_ms(180);
  PORTH |= (1 << PH5);  // RST = 1
  _delay_ms(180);
  // 2. Software Reset
  lcd_cmd(0x01); 
  _delay_ms(180);
  // 3. Power Control A (Стандартные значения из даташита)
  lcd_cmd(0xCB); _delay_ms(180); lcd_data(0x39); _delay_ms(180); lcd_data(0x2C); _delay_ms(180); lcd_data(0x00); _delay_ms(180); lcd_data(0x34); _delay_ms(180); lcd_data(0x02);
  // 4. Driver Timing Control
  _delay_ms(180); lcd_cmd(0xE8); _delay_ms(180); lcd_data(0x85); _delay_ms(180); lcd_data(0x00); _delay_ms(180); lcd_data(0x78);
  // 5. Control Display Function
  _delay_ms(180); lcd_cmd(0xB6); _delay_ms(180); lcd_data(0x0A); _delay_ms(180); lcd_data(0xA2);
  // 6. Sleep Out (Проснуться)
  _delay_ms(180); lcd_cmd(0x11); 
  // 7. Pixel Format Set (
  _delay_ms(180); lcd_cmd(0x3A); _delay_ms(180); lcd_data(0x55); _delay_ms(180);
  // 8. Memory Access Control (Ориентация экрана)
  lcd_cmd(0x36); _delay_ms(180); lcd_data(0x48); _delay_ms(180); 
  // 9. Display ON (Включить картинку)
  lcd_cmd(0x29); _delay_ms(180); 
}