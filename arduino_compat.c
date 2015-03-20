#include "stm32f4_discovery.h"

void digitalWrite(const uint8_t pin, const uint8_t val) {
  // WARNING WARNING the port and pins are totally wrong
  if(val){
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
  }
  else {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
  }
}

uint8_t digitalRead(const uint8_t pin) {
  // WARNING WARNING the port and pins are totally wrong
  return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13);
}
