#ifndef COMPLEX_CONDITIONS_H
#define COMPLEX_CONDITIONS_H

#include <string>

/**
 * Демонстрационная функция с сложным условием
 * A && (B || C)
 */
bool complexCondition(bool A, bool B, bool C);

/**
 * Функция с ошибкой в сложном условии, которую 
 * не обнаруживают DC тесты, но обнаруживают MC/DC тесты
 */
bool buggyFunction(bool A, bool B, bool C);

/**
 * Функция с правильным условием для сравнения
 */
bool correctFunction(bool A, bool B, bool C);

#endif