#include "../include/complex_conditions.h"

bool complexCondition(bool A, bool B, bool C) {
    return A && (B || C);  // Классический пример сложного условия
}

bool buggyFunction(bool A, bool B, bool C) {
    // Ошибка в реализации: должно быть A && (B || C), 
    // но реализовано как (A && B) || C
    return (A && B) || C;
}

bool correctFunction(bool A, bool B, bool C) {
    // Правильная реализация
    return A && (B || C);
} 