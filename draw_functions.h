#include "spi.h"
const uint8_t font[16][7] = {
    // '0'
    {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},
    // '1'
    {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E},
    // '2'
    {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F},
    // '3'
    {0x0E, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0E},
    // '4'
    {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02},
    // '5'
    {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E},
    // '6'
    {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E},
    // '7'
    {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08},
    // '8'
    {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E},
    // '9'
    {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C},
    // '+'
    {0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00},
    // '-'
    {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00},
    // '='
    {0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00},
    // '*'
    {0x00, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x00},
    // '/'
    {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00}
};

//Функция для закрашивания пикселя
void draw_pixel(uint16_t x, uint16_t y, uint16_t color){
  PORTH &= ~(1 << PH6); // D/C = 0 чтобы подать команду
  take_data(0x2A); //Команда для установки границ по абсциссе
  PORTH |= (1 << PH6); // D/C = 1 чтобы подать данные
  take_data((x >> 8) & 0xFF); //Передаем старший байт левой границы
  take_data(x & 0xFF); //Передаем младший байт
  take_data((x >> 8) & 0xFF); //Передаем старший байт правой границы
  take_data(x & 0xFF); //Передаем младший байт
  PORTH &= ~(1 << PH6); // D/C = 0 чтобы подать команду
  take_data(0x2B); //Команда для установки границ по ординате
  PORTH |= (1 << PH6); // D/C = 1 чтобы подать данные
  take_data((y >> 8) & 0xFF); //Передаем старший байт нижней границы
  take_data(y & 0xFF); //Передаем младший байт
  take_data((y >> 8) & 0xFF); //Передаем старший байт верхней границы
  take_data(y & 0xFF); //Передаем младший байт
  PORTH &= ~(1 << PH6); // D/C = 0 чтобы подать команду
  take_data(0x2C); //Команда для непрерывной записи в GRAM
  PORTH |= (1 << PH6); // D/C = 1 чтобы подать данные
  take_data(color >> 8);  take_data(color & 0xFF); //Цвет
}

//Функция для отрисовки 8 симметричных точек
void draw_circle_points(int16_t cx, int16_t cy, int16_t x, int16_t y, uint16_t color) {
  draw_pixel(cx + x, cy + y, color);
  draw_pixel(cx - x, cy + y, color);
  draw_pixel(cx + x, cy - y, color);
  draw_pixel(cx - x, cy - y, color);
  draw_pixel(cx + y, cy + x, color);
  draw_pixel(cx - y, cy + x, color);
  draw_pixel(cx + y, cy - x, color);
  draw_pixel(cx - y, cy - x, color);
}

//Вспомогательная функция для подготовки к выводу на дисплей
void prepare_to_draw(uint16_t x_min, uint16_t y_min, uint16_t x_max, uint16_t y_max){
  // 1. Обозначаем границы прямоугольника
  // 1.1 Устанавливаем границы по абсциссе
  PORTH &= ~(1 << PH6); // D/C = 0 чтобы подать команду
  take_data(0x2A); //Команда для установки границ по абсциссе
  PORTH |= (1 << PH6); // D/C = 1 чтобы подать данные
  take_data((x_min >> 8) & 0xFF); //Передаем старший байт левой границы
  take_data(x_min & 0xFF); //Передаем младший байт
  take_data((x_max >> 8) & 0xFF); //Передаем старший байт правой границы
  take_data(x_max & 0xFF); //Передаем младший байт
  // 1.2 Устанавливаем границы по ординате
  PORTH &= ~(1 << PH6); // D/C = 0 чтобы подать команду
  take_data(0x2B); //Команда для установки границ по ординате
  PORTH |= (1 << PH6); // D/C = 1 чтобы подать данные
  take_data((y_min >> 8) & 0xFF); //Передаем старший байт нижней границы
  take_data(y_min & 0xFF); //Передаем младший байт
  take_data((y_max >> 8) & 0xFF); //Передаем старший байт верхней границы
  take_data(y_max & 0xFF); //Передаем младший байт
  // 2. Закрашиваем прямоугольник
  PORTH &= ~(1 << PH6); // D/C = 0 чтобы подать команду
  take_data(0x2C); //Команда для непрерывной записи в GRAM
  PORTH |= (1 << PH6); // D/C = 1 чтобы подать данные
}

//Функция для вывода прямоугольника
void draw_rectangle(uint16_t x_min, uint16_t y_min, uint16_t x_max, uint16_t y_max, uint16_t color){
  //Закрашиваем
  for(int i = 0; i < ((x_max - x_min + 1)*(y_max - y_min + 1)); i++){
    take_data(color >> 8);  take_data(color & 0xFF); //Цвет
  }
}
//Функция для вывода круга
void draw_circle(int16_t cx, int16_t cy, uint16_t r, uint16_t color) {
  int16_t x = 0;            // текущая координата X (начинаем с 0)
  int16_t y = r;            // текущая координата Y (начинаем с радиуса)
  int16_t d = 1 - r;        // начальное значение параметра решения (ошибка)
  draw_circle_points(cx, cy, x, y, color);
  // Основной цикл: идём по X от 0 до тех пор, пока x <= y
  while (x < y) {
    x++;   // увеличиваем x на 1
    // Проверяем, нужно ли уменьшить y
    // Если d < 0, то ошибка показывает, что текущий y всё ещё подходит
    // Если d >= 0, то нужно уменьшить y на 1 и скорректировать ошибку
    if (d < 0) {
      d += 2 * x + 1;   // коррекция ошибки для случая, когда y не меняется
    } else {
      y--;
      d += 2 * (x - y) + 1; // коррекция ошибки для случая, когда y уменьшается
    }
    // Отрисовываем 8 симметричных точек для новых (x, y)
    draw_circle_points(cx, cy, x, y, color);
  }
}
//Функция для вывода элемента на экран
void draw_char(uint8_t pos, uint16_t x, uint16_t y, uint16_t color){
  uint8_t byt;
  prepare_to_draw(x, y, x + 9, y + 13);
  for(int row = 0; row < 7; row++){
    byt = font[pos][row];
    for(uint8_t sub_row = 0; sub_row < 2; sub_row++){
      for(uint8_t col = 0; col < 5; col++){
        uint16_t draw_color = (((byt >> (4 - col)) & 1)) ? color : 0x0000;
        for(uint8_t sub_col = 0; sub_col < 2; sub_col++){
          take_data(draw_color >> 8);  take_data(draw_color & 0xFF); //Цвет
        }
     }
    }
  }
}