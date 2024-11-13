#include "conclude.h"

int add_size(const std::string& str)
{
    int add_size = 1;
    if (str.find('A') != std::string::npos)
        add_size += 41;
    if (str.find('B') != std::string::npos)
        add_size += 41;
    if (str.find('C') != std::string::npos)
        add_size += 41;
    return add_size;
}

std::string list_pusher(std::string str, char sim, const std::string& var)
{
    std::string result = "";
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == sim)
            result += var;
        else
            result += str[i];
    }
    return result;
}

std::vector<struct statement> enumeration(std::vector<struct statement> list, const std::string& str)
{
    // DATA
    std::vector<std::string> variants_A = {"B", "C", "!A", "!B", "!C",
                                           "(A>A)", "(A>B)", "(A>C)", "(B>A)", "(B>B)", "(B>C)", "(C>A)", "(C>B)", "(C>C)",
                                           "(!A>A)", "(!A>B)", "(!A>C)", "(!B>A)", "(!B>B)", "(!B>C)", "(!C>A)", "(!C>B)", "(!C>C)",
                                           "(A>!A)", "(A>!B)", "(A>!C)", "(B>!A)", "(B>!B)", "(B>!C)", "(C>!A)", "(C>!B)", "(C>!C)",
                                           "(!A>!A)", "(!A>!B)", "(!A>!C)", "(!B>!A)", "(!B>!B)", "(!B>!C)", "(!C>!A)", "(!C>!B)", "(!C>!C)"};
    std::vector<std::string> variants_B = {"A", "C", "!A", "!B", "!C",
                                           "(A>A)", "(A>B)", "(A>C)", "(B>A)", "(B>B)", "(B>C)", "(C>A)", "(C>B)", "(C>C)",
                                           "(!A>A)", "(!A>B)", "(!A>C)", "(!B>A)", "(!B>B)", "(!B>C)", "(!C>A)", "(!C>B)", "(!C>C)",
                                           "(A>!A)", "(A>!B)", "(A>!C)", "(B>!A)", "(B>!B)", "(B>!C)", "(C>!A)", "(C>!B)", "(C>!C)",
                                           "(!A>!A)", "(!A>!B)", "(!A>!C)", "(!B>!A)", "(!B>!B)", "(!B>!C)", "(!C>!A)", "(!C>!B)", "(!C>!C)"};
    std::vector<std::string> variants_C = {"A", "B", "!A", "!B", "!C",
                                           "(A>A)", "(A>B)", "(A>C)", "(B>A)", "(B>B)", "(B>C)", "(C>A)", "(C>B)", "(C>C)",
                                           "(!A>A)", "(!A>B)", "(!A>C)", "(!B>A)", "(!B>B)", "(!B>C)", "(!C>A)", "(!C>B)", "(!C>C)",
                                           "(A>!A)", "(A>!B)", "(A>!C)", "(B>!A)", "(B>!B)", "(B>!C)", "(C>!A)", "(C>!B)", "(C>!C)",
                                           "(!A>!A)", "(!A>!B)", "(!A>!C)", "(!B>!A)", "(!B>!B)", "(!B>!C)", "(!C>!A)", "(!C>!B)", "(!C>!C)"};
    std::vector<std::vector<std::string>> variants = {variants_A, variants_B, variants_C};
    std::vector<char> variables = {'A', 'B', 'C'};

    // PROCESS
    if (variants.size() != variables.size())
    {
        std::cerr << "enumeration(): ERROR: variants.size() != variables.size()" << std::endl;
    }
    int orig_ind = list.size();
    struct statement original(orig_ind, str);
    list.push_back(original);
    for (int i = 0; i < variants.size(); i++)
    {
        if (str.find(variables[i]) != std::string::npos)
        {
            for (const std::string& var: variants[i])
            {
                std::string to_push = list_pusher(str, variables[i], var);
                not_double_denial(to_push);
                std::string sim(1, variables[i]);
                struct statement new_state(orig_ind, "с подстановкой вместо " + sim + ": ", var, to_push);
                list.push_back(new_state);
            }
        }
    }
    return list;
}

int compare_str(const std::vector<struct statement>& list, size_t start_ind, const std::string& str)
{
    int result = -1;
    for (size_t i = start_ind; i < list.size(); i++)
    {
        if (list[i].this_statement == str)
        {
            result = i;
            break;
        }
    }
    return result;
}

std::string step(std::vector<struct statement> A, size_t start_i, const std::vector<std::string>& rulse,
                const std::string& str, size_t deep, const size_t max_deep)
{
    if (deep >= max_deep)
    {
        return "to_deep_branch";
    }
    // else go on
    std::string step_result;
    ////////////////////////////// [start_i to A.size()] [0 to A.size()] //////////////////////////////
    for (int i = start_i; i < A.size(); i++)      // for all A_i
    {
        for (int j = 0; j < A.size(); j++)              // for all A_j
        {
            std::pair<std::string, std::string> conclusion = conclude(A, i, j, rulse);
            std::string A_new = conclusion.second;
            if ((A_new != "no_conclusion") & (compare_str(A, 0, A_new) == -1))
            {
                size_t i_new = A.size();
                std::vector<struct statement> S = enumeration(A, A_new);
                int check_i = compare_str(S, i_new, str);
                if (A_new == str)
                {
                    step_result = conclusion_to_str(deep, A, j, i, conclusion.first, A_new) + ", ч.т.д.\n";
                    return step_result;
                }
                else if  (check_i != -1)
                {
                    step_result = conclusion_to_str(deep, A, j, i, conclusion.first, A_new) + "\n"
                                + substitution_to_str(S, check_i) + ", ч.т.д.\n";
                    return step_result;
                }
                else
                {
                    step_result = conclusion_to_str(deep, A, j, i, conclusion.first, A_new) + "\n";
                    // go deeper
                    std::string maybe_answer = step(S, i_new, rulse, str, deep + 1, max_deep);
                    if ((maybe_answer != "to_deep_branch") & (maybe_answer.find("CAN'T FIND A CONCLUSION") == std::string::npos))
                    {
                        return step_result + maybe_answer;
                    }
                } // else just go on
            }
        }
    }
    ////////////////////////////// [0 to A.size()] [start_i to A.size()] //////////////////////////////
    for (int i = 0; i < A.size(); i++)      // for all A_i
    {
        for (int j = start_i; j < A.size(); j++)        // for all A_j
        {
            std::pair<std::string, std::string> conclusion = conclude(A, i, j, rulse);
            std::string A_new = conclusion.second;
            if ((A_new != "no_conclusion") & (compare_str(A, 0, A_new) == -1))
            {
                size_t i_new = A.size();
                std::vector<struct statement> S = enumeration(A, A_new);
                int check_i = compare_str(S, i_new, str);
                if (A_new == str)
                {
                    step_result = conclusion_to_str(deep, A, j, i, conclusion.first, A_new) + ", ч.т.д.\n";
                    return step_result;
                }
                else if  (check_i != -1)
                {
                    step_result = conclusion_to_str(deep, A, j, i, conclusion.first, A_new) + "\n"
                                  + substitution_to_str(S, check_i) + ", ч.т.д.\n";
                    return step_result;
                }
                else
                {
                    step_result = conclusion_to_str(deep, A, j, i, conclusion.first, A_new) + "\n";
                    // go deeper
                    std::string maybe_answer = step(S, i_new, rulse, str, deep + 1, max_deep);
                    if ((maybe_answer != "to_deep_branch") & (maybe_answer.find("CAN'T FIND A CONCLUSION") == std::string::npos))
                    {
                        return step_result + maybe_answer;
                    }
                } // else just go on
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////
    return step_result + "CAN'T FIND A CONCLUSION";
}

int main()
{
    // DATA
    // all_A
    std::string A1 = "(A>(B>A))";                   // A1.   (A→(B→A))
    std::string A2 = "((A>(B>C))>((A>B)>(A>C)))";   // A2.   ((A→(B→C))→((A→B)→(A→C)))
    std::string A3 = "((!B>!A)>((!B>A)>B))";        // A3.   ((-B→-A)→((-B→A)→B))
    std::vector axioms = {A1, A2, A3};
    std::vector<struct statement> all_A;
    // push all_A
    for (const std::string& Ai: axioms)
    {
        all_A = enumeration(all_A, Ai);
    }
    // input & output
    std::string input;
    std::string str;
    std::string output;

    // INPUT
    std::cin >> input;
    str = converter(input);
    std::cout << "Преобразование дизъюнкций, конъюнкций, XOR-ов и эквивалентностей в выражения только с отрицаниями и импликациями:\n"
              << parser(input) << " = " << parser(str) << std::endl;

    // PROCESS
    int check_i = compare_str(all_A, 0, str);
    if (str == A1 | str == A2 | str == A3)
    {
        output = parser(str) + " ⊢ " + parser(str);
    }
    else if  (check_i != -1)
    {
        output = substitution_to_str(all_A, check_i) + ", ч.т.д.\n";
    }
    else
    {
        std::vector<std::string> rules = {"modus_ponens", "modus_tollens", "дизъюнктивный_силлогизм", "гипотетический_силлогизм", "разделительный_силлогизм"};
        //std::vector<std::string> rules = {"modus_ponens"};
        for (int d = 2; d < 5; d++)
        {
            output = step(all_A, 0, rules, str, 0, d);
            if (output.find("CAN'T FIND A CONCLUSION") == std::string::npos)
                break;
        }
    }

    // OUTPUT
    std::cout << output << std::endl;
    return 0;
}
