#include "to_str.h"
#include "converter.h"

// Вспомогательные функции:
// нахождение индекса центральной операции;
size_t center_ind(std::string str);
// проверка содержания str2 в str1 на позиции ind;
bool check_position(std::string str1, size_t ind, std::string str2);
// Основная функция:
// вывод нового выражения по одному из заданных правил: "modus_ponens", "modus_tollens", "дизъюнктивный_силлогизм", "гипотетический_силлогизм", "разделительный_силлогизм";
std::pair<std::string, std::string> conclude(const std::vector<struct statement>& A, int i, int j, const std::vector<std::string>& rulse);
// функция для проверки работы правил вывода;
void check_conclude();
