#include "../include/levenshtein.h"
#include <vector>
#include <string>
#include <algorithm>

int min3(int a, int b, int c) {
    return std::min(std::min(a, b), c);
}

unsigned int levenshteinDistance(const std::string& s1, const std::string& s2, 
                                 bool useOptimization, bool useDiagonalOptimization) {
    // Сложное условие для оптимизации: (useOptimization && s1.empty()) || (useOptimization && s2.empty())
    if ((useOptimization && s1.empty()) || (useOptimization && s2.empty())) {
        return std::max(s1.length(), s2.length());
    }
    
    // Простая оптимизация для идентичных строк
    if (s1 == s2) {
        return 0;
    }
    
    const size_t m = s1.length();
    const size_t n = s2.length();
    
    std::vector<std::vector<unsigned int>> dp(m + 1, std::vector<unsigned int>(n + 1, 0));
    
    // Инициализация базовых случаев
    for (size_t i = 0; i <= m; ++i) {
        dp[i][0] = i;
    }
    
    for (size_t j = 0; j <= n; ++j) {
        dp[0][j] = j;
    }
    
    // Основной алгоритм
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            // Сложное условие для оптимизации совпадающих символов
            if (s1[i-1] == s2[j-1]) {
                // Еще одно сложное условие для диагональной оптимизации
                if (useDiagonalOptimization || s1.length() > 10) {
                    dp[i][j] = dp[i-1][j-1];
                } else {
                    dp[i][j] = min3(dp[i-1][j] + 1, dp[i][j-1] + 1, dp[i-1][j-1]);
                }
            } else {
                dp[i][j] = 1 + std::min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
            }
        }
    }
    
    return dp[m][n];
} 