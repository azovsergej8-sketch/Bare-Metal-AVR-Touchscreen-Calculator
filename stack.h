#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#define SIZE_S 50
#define STACK(position, type, is_empty, buffer) {(position), (type), (is_empty), (buffer)}
struct Stack{
  uint16_t position;
  char type;
  bool is_empty;
  int* buffer;
};
  int pop(struct Stack* obj){
    if(obj->position == 0){
      obj->is_empty = true; return 0;
    }
    return obj->buffer[--obj->position];
  }
  void push(struct Stack* obj, int c){
    if(obj->position == SIZE_S){return;}
    if(obj->is_empty){ obj->is_empty = false;}
    obj->buffer[obj->position++] = c;
  }