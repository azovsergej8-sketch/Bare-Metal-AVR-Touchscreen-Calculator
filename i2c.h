#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
//Настройка I2C - сенсор
void i2c_setup(void){
  //Выставляем 400кГц
  TWBR = 12; TWSR &= ~(1 << TWPS1); TWSR &= ~(1 << TWPS0);
}
//Глобальный флаг отсутствия касаний
volatile bool is_no_touch = true;
//Глобальная переменная для хранения координат касания
volatile uint32_t coordinates[2] = {0, 0};
//Глобальный флаг готовности координат касания
volatile bool ready_coordinates = false;
//Счетчик операции чтения
uint8_t count = 0;
volatile uint8_t x_11_4 = 0; volatile uint8_t x_3_0 = 0; volatile uint8_t y_11_4 = 0; volatile uint8_t y_3_0 = 0;
ISR(TWI_vect){
  uint8_t status = TWSR & 0xF8;
  switch(status){
    default: //Старт
      TWCR = (1 << TWIE) | (1 << TWSTA) | (1 << TWEN) | (1 << TWINT); break;
    case 0x08:
      TWDR = (0x38 << 1) | 0;
      TWCR = (1 << TWIE) | (1 << TWEN) | (1 << TWINT); break;
    case 0x18:
      TWDR = 0x02; //Регистр касаний
      TWCR = (1 << TWIE) | (1 << TWEN) | (1 << TWINT); break;
    case 0x28:
      //4. Меняем направление при помощи повторного старта
      TWCR = (1 << TWIE) | (1 << TWSTA) | (1 << TWEN) | (1 << TWINT); break;//Повторно сбрасываем флаг
    case 0x10:
      TWDR = (0x38 << 1) | 1; TWCR = (1 << TWIE) | (1 << TWEN) | (1 << TWINT); break;
    case 0x40:
      TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWEN) | (1 << TWINT); break;
    case 0x50:
      switch(count){
        case 0: //Чтение регистра касаний
          TWCR = (1 << TWEA);
          if(TWDR == 0x00){ //Проверка на наличие нажатий
          is_no_touch = true;
          TWCR = (1 << TWIE) | (1<<TWSTO)|(1<<TWEN)|(1<<TWINT);
          }
          //Чтение старших битов и полубитов координат
          else{ count++; TWCR = (1 << TWEN) | (1 << TWINT);}
          break;
        case 1:
          x_11_4 = TWDR; count ++;//Считываем значение регистра данных 
          TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWEN) | (1 << TWINT); break;
        case 2:
          x_3_0 = TWDR; count ++;//Считываем значение регистра данных
          TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWEN) | (1 << TWINT); break;
        case 3:
          y_11_4 = TWDR; count ++;//Считываем значение регистра данных 
          TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWEN) | (1 << TWINT); break;
        case 4:
          y_3_0 = TWDR; count++;//Считываем значение регистра данных
          //Записываем координаты путем объединения битов при помощи маски
          coordinates[0] = ( ((x_11_4 & 0x0F) << 8) | x_3_0); coordinates[1] = ( ((y_11_4 & 0x0F) << 8) | y_3_0); count = 0;
          TWCR = (1 << TWEN) | (1 << TWINT); break;
      }
      break;
    case 0x58:
        is_no_touch = false;
        //11. Генерация сигнала стоп
        TWCR = (1 << TWIE) | (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
  }
}