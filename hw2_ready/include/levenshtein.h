#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#include <string>
#include <vector>
#include <algorithm>

int min3(int a, int b, int c);

/**
 * Вычисляет расстояние Левенштейна между двумя строками.
 * 
 * @param s1 Первая строка
 * @param s2 Вторая строка
 * @param useOptimization Использовать ли оптимизацию для пустых строк
 * @param useDiagonalOptimization Использовать ли оптимизацию для диагональных переходов
 * @return Расстояние Левенштейна
 */
unsigned int levenshteinDistance(const std::string& s1, const std::string& s2,
                        bool useOptimization,
                        bool useDiagonalOptimization);

#endif