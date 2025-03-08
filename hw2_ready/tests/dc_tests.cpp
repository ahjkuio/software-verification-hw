#include <iostream>
#include <cassert>
#include "../include/levenshtein.h"

void testDecisionCoverage() {
    // Тест if ((useOptimization && s1.empty()) || (useOptimization && s2.empty()))
    assert(levenshteinDistance("", "xyz", true, true) == 3);  // первая часть true
    assert(levenshteinDistance("xyz", "", true, true) == 3);  // вторая часть true
    assert(levenshteinDistance("xyz", "abc", true, true) == 3); // обе части false
    
    // Тест useOptimization = true/false
    assert(levenshteinDistance("abc", "", true, true) == 3);
    assert(levenshteinDistance("abc", "", false, true) == 3);
    
    // Тест useDiagonalOptimization = true/false
    assert(levenshteinDistance("cat", "cat", true, true) == 0);
    assert(levenshteinDistance("cat", "cat", true, false) == 0);
    
    // Тест условия s1 == s2 (true/false)
    assert(levenshteinDistance("test", "test", true, true) == 0);
    assert(levenshteinDistance("test", "text", true, true) == 1);
    
    // Тест условия s1[i-1] == s2[j-1] (true/false)
    assert(levenshteinDistance("abc", "abc", false, false) == 0);
    assert(levenshteinDistance("abc", "abd", false, false) == 1);
    
    // Тест сложного условия (useDiagonalOptimization || s1.length() > 10)
    // true через первое условие
    assert(levenshteinDistance("short", "short", false, true) == 0);
    // true через второе условие
    assert(levenshteinDistance("verylongstring", "verylongstring", false, false) == 0);
    // false для обоих условий
    assert(levenshteinDistance("short", "short", false, false) == 0);
    
    // Тестирование min3 для разных ветвей
    assert(min3(1, 2, 3) == 1);  // первый минимальный
    assert(min3(3, 1, 2) == 1);  // второй минимальный
    assert(min3(3, 2, 1) == 1);  // третий минимальный
    
    std::cout << "Decision Coverage тесты пройдены успешно!" << std::endl;
}

int main() {
    testDecisionCoverage();
    return 0;
} 