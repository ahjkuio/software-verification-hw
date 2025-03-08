#include <iostream>
#include <cassert>
#include "../include/complex_conditions.h"

void testDecisionCoverage() {
    // DC тесты для buggyFunction
    assert(buggyFunction(true, true, false) == true);   // A=T, B=T, C=F -> true
    assert(buggyFunction(false, true, false) == false); // A=F, B=T, C=F -> false
    
    std::cout << "Decision Coverage тесты пройдены успешно!" << std::endl;
}

void testMCDCCoverage() {
    // MC/DC тесты для buggyFunction
    assert(buggyFunction(true, true, false) == true);   // A=T, B=T, C=F -> true
    assert(buggyFunction(false, true, false) == false); // A=F, B=T, C=F -> false
    
    std::cout << "Проверка критического случая, где проявляется ошибка:" << std::endl;
    
    // Тест, где функции дают разный результат
    bool A = false, B = false, C = true;
    
    bool correctResult = correctFunction(A, B, C);
    bool buggyResult = buggyFunction(A, B, C);
    
    std::cout << "A=" << A << ", B=" << B << ", C=" << C << std::endl;
    std::cout << "- Корректная функция A&&(B||C): " << correctResult << std::endl;
    std::cout << "- Ошибочная функция (A&&B)||C: " << buggyResult << std::endl;
    
    if (correctResult == buggyResult) {
        std::cout << "MC/DC тесты не выявили отличия для этого случая." << std::endl;
    } else {
        std::cout << "MC/DC тесты обнаружили ошибку в buggyFunction, которую не обнаружили DC тесты!" << std::endl;
    }
}

int main() {
    std::cout << "-- ПРОВЕРКА РАЗНИЦЫ МЕЖДУ DC И MC/DC ТЕСТАМИ --" << std::endl;
    
    std::cout << "\n1. Тестирование с Decision Coverage (DC):" << std::endl;
    testDecisionCoverage();
    
    std::cout << "\n2. Тестирование с MC/DC Coverage:" << std::endl;
    testMCDCCoverage();
    
    std::cout << "\n-- ВЫВОД --" << std::endl;
    std::cout << "DC тесты не нашли ошибку в сложном условии." << std::endl;
    std::cout << "MC/DC тесты обнаружили ошибку, потому что проверяют каждое условие отдельно." << std::endl;
    std::cout << "Это показывает, что MC/DC лучше, чем DC." << std::endl;
    
    return 0;
} 