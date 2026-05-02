#include "logic.h"
#define BUTTON(x, y, x_max, y_max, color, val, f) {(x), (y), (x_max), (y_max), (color), (val), (f)}
//Структура для кнопки
struct Button {
  uint16_t x; uint16_t y; uint16_t x_max; uint16_t y_max;
  uint16_t color; uint8_t value; void (*func)(uint16_t);
};
//Кнопки
// Ряд 1
struct Button b7 = BUTTON(8, 110, 58,  160, 0xFFFF, 7,  get_number);
struct Button b8 = BUTTON(66, 110, 116, 160, 0xFFFF, 8,  get_number);
struct Button b9 = BUTTON(124, 110, 174, 160, 0xFFFF, 9,  get_number);
struct Button bDiv = BUTTON(182, 110, 232, 160, 0xFD20, 14,  divide);

// Ряд 2
struct Button b4 = BUTTON(8, 168, 58,  218, 0xFFFF, 4,  get_number);
struct Button b5 = BUTTON(66, 168, 116, 218, 0xFFFF, 5,  get_number);
struct Button b6 = BUTTON(124, 168, 174, 218, 0xFFFF, 6,  get_number);
struct Button bMul = BUTTON(182, 168, 232, 218, 0xFD20, 13,  multip);

// Ряд 3
struct Button b1 = BUTTON(8, 226, 58,  276, 0xFFFF, 1,  get_number);
struct Button b2 = BUTTON(66, 226, 116, 276, 0xFFFF, 2,  get_number);
struct Button b3 = BUTTON(124, 226, 174, 276, 0xFFFF, 3,  get_number);
struct Button bMin = BUTTON(182, 226, 232, 276, 0xFD20, 11,  minus);

// Ряд 4 (0, =, +)
struct Button b0 = BUTTON(8, 284, 58,  315, 0xFFFF, 0,  get_number);
struct Button bEq = BUTTON(66, 284, 174, 315, 0x07E0, 12,  equal);   // Зеленая кнопка '='
struct Button bPls = BUTTON(182, 284, 232, 315, 0xFD20, 10,  plus);

// Массив указателей для удобного перебора
volatile struct Button* my_buttons[] = {
  &b7, &b8, &b9, &bDiv,
  &b4, &b5, &b6, &bMul,
  &b1, &b2, &b3, &bMin,
  &b0, &bEq, &bPls
};
//Функция обработки нажатой кнопки
void use_button(){
  for(uint8_t i = 0; i < 15; i++){  // 15 кнопок в массиве
      if(coordinates[0] >= my_buttons[i]->x && coordinates[0] <= my_buttons[i]->x_max &&
          coordinates[1] >= my_buttons[i]->y && coordinates[1] <= my_buttons[i]->y_max){
          my_buttons[i]->func(my_buttons[i]->value);
          return;
      }
  }
}