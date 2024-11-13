#include <iostream>
#include <vector>
#include <map>

// Вспомогательные функции:
// нахождение левой и правой границ найденной дизъюнкциии, конъюнкциии, XOR-а и эквивалентности;
std::vector<std::string> find_left_and_right(std::string str, size_t ind);
// избавление от двойного отрицания в выражении;
void not_double_denial(std::string& str);
// Основная функция:
// преобразование выражений с дизъюнкциями, конъюнкциями, XOR-ами и эквивалентностями в выражения только с отрицаниями и импликациями;
std::string converter(const std::string& str);
