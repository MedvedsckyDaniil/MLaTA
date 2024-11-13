#include <iostream>
#include <vector>
#include <map>

typedef struct statement {
    size_t original_ind;
    std::string where_sub;
    std::string substitution;
    std::string this_statement;
    // constructor and destructor
    explicit statement(size_t orig, std::string state = "▢");
    statement(size_t orig, std::string where, std::string sub, std::string state);
    // alignment
    [[nodiscard]] std::string alignment() const;
} statement;

// Вспомогательные функции:
// замена используемых обозначений операций на принятые;
std::string parser(const std::string& str);
// получение строки простой подстановки;
std::string substitution_to_str(const std::vector<struct statement>& A, int ind);
// Основная функция:
// получение строки логического выражения;
std::string conclusion_to_str(size_t deep, const std::vector<struct statement>& A, int j, int i,
                                const std::string& rule, const std::string& A_new);
