#include <iostream>
#include <cassert>
#include "../include/levenshtein.h"

void testStatementCoverage() {
    // Проверка базовых случаев
    assert(levenshteinDistance("", "", true, true) == 0);
    assert(levenshteinDistance("a", "", true, true) == 1);
    assert(levenshteinDistance("", "a", true, true) == 1);
    assert(levenshteinDistance("a", "a", true, true) == 0);
    assert(levenshteinDistance("abc", "abc", true, true) == 0);
    
    // Проверка операций вставки/удаления/замены
    assert(levenshteinDistance("abc", "ab", true, true) == 1);  // удаление
    assert(levenshteinDistance("ab", "abc", true, true) == 1);  // вставка
    assert(levenshteinDistance("abc", "abd", true, true) == 1); // замена
    
    // Проверка более сложных случаев
    assert(levenshteinDistance("kitten", "sitting", true, true) == 3);
    
    // Тест для ветки с совпадающими символами,но useDiagonalOptimization=false и длина строки ≤ 10
    assert(levenshteinDistance("abcdef", "abcxef", false, false) == 1);
    
    // Тест с обычными строками
    assert(levenshteinDistance("cat", "cat", true, true) == 0);
    assert(levenshteinDistance("cat", "bat", true, true) == 1);
    assert(levenshteinDistance("sunday", "saturday", true, true) == 3);
    
    // Тест на случай совпадения символов с отключенной диагональной оптимизацией
    assert(levenshteinDistance("cat", "cat", true, false) == 0);
    
    // Проверка работы функции min3
    assert(min3(1, 2, 3) == 1);
    assert(min3(3, 1, 2) == 1);
    assert(min3(2, 3, 1) == 1);
    
    std::cout << "Statement Coverage тесты пройдены успешно!" << std::endl;
}

int main() {
    testStatementCoverage();
    return 0;
} 