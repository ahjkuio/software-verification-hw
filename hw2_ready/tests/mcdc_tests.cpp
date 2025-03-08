#include <iostream>
#include <cassert>
#include "../include/levenshtein.h"

void testMCDCCoverage() {
    // MC/DC тестирование для useOptimization
    // Условие: useOptimization && s1.empty()
    assert(levenshteinDistance("", "xyz", true, true) == 3);    // true && true
    assert(levenshteinDistance("abc", "xyz", true, true) != 5); // true && false
    assert(levenshteinDistance("", "xyz", false, true) == 3);   // false && true
    
    // Условие: useOptimization && s2.empty()
    assert(levenshteinDistance("abc", "", true, true) == 3);    // true && true
    assert(levenshteinDistance("abc", "xyz", true, true) != 0); // true && false
    assert(levenshteinDistance("abc", "", false, true) == 3);   // false && true
    
    // MC/DC тестирование для условия совпадения символов с оптимизацией
    // Условие: (s1[i-1] == s2[j-1]) && useDiagonalOptimization
    assert(levenshteinDistance("abc", "abc", true, true) == 0);  // true && true
    assert(levenshteinDistance("abc", "abd", true, true) == 1);  // false && true
    assert(levenshteinDistance("abc", "abc", true, false) == 0); // true && false
    
    // Различные комбинации параметров для сложных условий
    assert(levenshteinDistance("", "", true, true) == 0);
    assert(levenshteinDistance("", "", false, false) == 0);
    assert(levenshteinDistance("abc", "ab", true, true) == 1);
    assert(levenshteinDistance("abc", "ab", false, false) == 1);
    assert(levenshteinDistance("abc", "abz", true, false) == 1);
    
    // Тесты, демонстрирующие независимое влияние каждого атомарного условия в сложных условиях
    
    assert(levenshteinDistance("abc", "ab", true, true) == 1);
    assert(levenshteinDistance("abc", "ab", false, false) == 1);
    assert(levenshteinDistance("abc", "abz", true, false) == 1);
    
    // MC/DC для условия (useDiagonalOptimization || s1.length() > 10)
    assert(levenshteinDistance("abcdef", "abcdef", false, true) == 0);  // false || false, но совпадающие строки
    assert(levenshteinDistance("abcdefghi", "abcdefghi", false, false) == 0);  // false || false, но совпадающие строки
    assert(levenshteinDistance("abcdefghijklm", "abcdefghijklm", false, false) == 0);  // false || true (длина > 10)
    assert(levenshteinDistance("abcdefghijklm", "abcdefghijxlm", false, false) == 1);  // false || true (длина > 10)
    
    // Покрытие ветви с min3
    assert(levenshteinDistance("abcdef", "abcxef", false, false) == 1);  // символы совпадают, но не все
    
    std::cout << "MC/DC Coverage тесты пройдены успешно!" << std::endl;
}

int main() {
    testMCDCCoverage();
    return 0;
} 