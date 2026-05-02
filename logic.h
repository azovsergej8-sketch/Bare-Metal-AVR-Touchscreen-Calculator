#include "stack.h"
#include "draw_functions.h"
volatile int buf_operations[SIZE_S];
volatile int buf_arguments[SIZE_S];
volatile struct Stack operations = STACK(0, 'o', true, buf_operations);
volatile struct Stack arguments = STACK(0, 'a', true, buf_arguments);
//Результат и его знак
volatile int result; volatile bool is_negative = false;
//Координаты курсора
volatile uint16_t x_pointer = 10; volatile uint16_t y_pointer = 40;
//Флаг разрешения вывода
volatile bool equal_use = false;
volatile bool is_operation = false; //Операция выбрана
int take_res_op(uint16_t i){
  int res;
  switch(operations.buffer[i]){
    case 1: res = arguments.buffer[i] + arguments.buffer[i+1]; break;
    case 3: res = arguments.buffer[i] - arguments.buffer[i+1]; break;
    case 4: res = arguments.buffer[i] * arguments.buffer[i+1]; break;
    case 5: res = arguments.buffer[i] / arguments.buffer[i+1]; break;
  }
  return res;
}
//Промежуточный результат
volatile int result_p;
//Функция для возврата цифры
void get_number(uint16_t i){
  if(equal_use){
    draw_rectangle(0, 40, 239, 100, 0x0000); //Закрашиваем строку вывода
    x_pointer = 10; y_pointer = 40;
    equal_use = false;
  }
  uint8_t of;
  if(is_operation){
    is_operation = false; result_p = i; of = 10;}
  else{
    result_p = result_p*10 + i; of = 5;}
  //Рисуем цифру и сдвигаем указатель по х
  draw_char(i, x_pointer, y_pointer, 0xFFFF); x_pointer += of;
}
//Функция для вычитания
void minus(uint16_t i){
  push(&arguments, result_p);
  push(&operations, 2); is_operation = true;
  //Рисуем символ и сдвигаем указатель по х
  draw_char(i, x_pointer, y_pointer, 0xFFFF); x_pointer += 10;
}
//Функция для умножения
void multip(uint16_t i){
  push(&arguments, result_p);
  push(&operations, 2); is_operation = true;
  //Рисуем символ и сдвигаем указатель по х
  draw_char(i, x_pointer, y_pointer, 0xFFFF); x_pointer += 10;
}
//Функция для деления
void divide(uint16_t i){
  push(&arguments, result_p);
  push(&operations, 2); is_operation = true;
  //Рисуем символ и сдвигаем указатель по х
  draw_char(i, x_pointer, y_pointer, 0xFFFF); x_pointer += 10;
}
//Функция для сложения
void plus(uint16_t i){
  push(&arguments, result_p);
  push(&operations, 2); is_operation = true;
  //Рисуем символ и сдвигаем указатель по х
  draw_char(i, x_pointer, y_pointer, 0xFFFF); x_pointer += 10;
}
//Функция для вывода результата
void equal(uint16_t i){
  push(&arguments, result_p);
  result_p = 0;
  if(arguments.position == 0){
    result = 0;  return;
  }
  if(arguments.position == 1){ result = pop(&arguments);  return;}
  uint16_t pos = 0;
  for(uint16_t k = 0; k < operations.position; k++){
    pos = 0;
    while(operations.buffer[pos] < 3){
      pos++;
    }
    result = take_res_op(pos); arguments.buffer[pos] = result;
    for(uint16_t p = pos+1; p < arguments.position - 1; p++){
      arguments.buffer[p] = arguments.buffer[p+1];
    }
    int rub = pop(&arguments);
    for(uint16_t p = pos; p < operations.position - 1; p++){
      operations.buffer[p] = operations.buffer[p+1];
    }
    rub = pop(&operations);
  }
  equal_use = true;
  for(int s = 0; s < SIZE_S; s++){ buf_operations[s] = 0; buf_operations[s] = 0;}
  //Определяем знак и вычисляем модуль
  if(result < 0){ is_negative = true;}
  if(result != 0) {
    uint16_t temp = result >> 15;
    result = (result ^ temp) - temp;
  }
  //Рисуем = и сдвигаем указатель по х
  draw_char(i, x_pointer, y_pointer, 0xFFFF); x_pointer += 10;
  //Рисуем -
  if(is_negative){ draw_char(12, x_pointer, y_pointer, 0xFFFF); x_pointer += 5;}
  uint16_t ten_degree = 10;
  while(result != (result % ten_degree)){ ten_degree *= 10;}
  while(ten_degree > 0){
    uint8_t res = (result / 10);
    result %= ten_degree; ten_degree / 10;
    draw_char(res, x_pointer, y_pointer, 0xFFFF); x_pointer += 5;
  }
}
