#include <stdio.h>
#include "board.h"
#include "periph/gpio.h"
#include "xtimer.h"

#define VALID_KEY 0x36 // Валидный ключ 0312
#define KEY_LENGTH 0x04 // Длина валидного ключа

#define BUTTON1_CODE 0 // Коды кнопок
#define BUTTON2_CODE 1
#define BUTTON3_CODE 2
#define BUTTON4_CODE 3

volatile uint8_t currentKeylength = 0; // Счетчик нажатий
volatile uint8_t currentInput = 0; // Буфер последовтельности нажатий

int main() {
    gpio_init(PA_0, GPIO_IN);
    gpio_init(LED0_PIN, GPIO_OUT);
    /* Задача кейса */

    while (1) {
        // Опрашиваем состояние кнопок
        //==============================
        if (!buttonS1) {
            currentKeylength++; // Инкрементируем счетчик нажатий
            currentInput <<= 2;  // Сдвигаем последовательность кодов нажатий
            currentInput |= BUTTON1_CODE; // Записываем код очередного нажатия
        }
        if (!buttonS2) {
            currentKeylength++;
            currentInput <<= 2;
            currentInput |= BUTTON2_CODE;
        }
        if (!buttonS3) {
            currentKeylength++;
            currentInput <<= 2;
            currentInput |= BUTTON3_CODE;
        }
        if (!buttonS4) {
            currentKeylength++;
            currentInput <<= 2;
            currentInput |= BUTTON4_CODE;
        }
        //==============================
        printf("Current input: %X\n", currentInput);
        wait_us(500000);
        //currentKeylength = 0;
        // Контроллируем количество нажатий на кнопки и открываем дверь при валидном ключе

        if (currentKeylength >= 4) {
            currentKeylength = 0; // Обнуляем счетчик нажатий

            if (currentInput == VALID_KEY) { // Сравниваем набранную в буфер последовательность нажатий с ключом
                gpio_toggle(LED0_PIN);
                puts("CORRECT KEY!");
            }
            else {
                puts("Wrong key :^(");
            }
            currentInput = 0; // Очищаем буфер
        }
    }
}
