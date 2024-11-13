#include "to_str.h"

statement::statement(size_t orig, std::string state)
        : original_ind(orig), where_sub("без подстановки \t"), substitution(""), this_statement(state) {}
statement::statement(size_t orig, std::string where, std::string sub, std::string state)
        : original_ind(orig), where_sub(where), substitution(sub), this_statement(state) {}

std::string statement::alignment() const {
    std::string result; // = this_statement;
    for (int i = 0; i < this_statement.size(); i++)
    {
        result += this_statement[i];
    }
    while (result.length() < 33)
    {
        result += " ";
    }
    return result;
}

std::string parser(const std::string& str)
{
    std::string new_str;

    std::map<char, std::string> parser_dict;
    parser_dict['!'] = "-";
    parser_dict['|'] = "∨";
    parser_dict['*'] = "∧";
    parser_dict['>'] = "→";
    parser_dict['+'] = "⊕";
    parser_dict['='] = "≡";

    for (int i = 0; i < str.size(); i++)
    {
        if (parser_dict.find(str[i]) == parser_dict.end())
        { // если не нашёл символ в словаре
            new_str += str[i];
        }
        else
        { // если нашёл
            new_str += parser_dict[str[i]];
        }
    }

    return new_str;
}

std::string substitution_to_str(const std::vector<struct statement>& A, int ind)
{
    size_t orig_ind = A[ind].original_ind;
    std::string result = A[orig_ind].alignment() + A[ind].where_sub + A[ind].substitution + "\t будет \t" + A[ind].this_statement;
    return parser(result);
}

std::string conclusion_to_str(size_t deep, const std::vector<struct statement>& A, int j, int i,
                              const std::string& rule, const std::string& A_new)
{
    std::string A_j = substitution_to_str(A, j);
    std::string A_i = substitution_to_str(A, i);
    return std::to_string(deep + 1) + ".\n" + A_j + ",\n" + A_i + "\n" + rule + " ⊢ " + parser(A_new);
}
