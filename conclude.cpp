#include "conclude.h"

size_t center_ind(std::string str)
{
    size_t ind;
    if (str[0] == '(')
    {
        ind = 0;
        int num = 1;
        while ((num != 1) | (str[ind] != '>'))
        {
            ind += 1;
            if (str[ind] == '(')
                num += 1;
            if (str[ind] == ')')
                num -= 1;
        }
    }
    else if ((str[0] == '!') & (str[1] == '('))
    {
        ind = 1;
        int num = 1;
        while ((num != 1) | (str[ind] != '>'))
        {
            ind += 1;
            if (str[ind] == '(')
                num += 1;
            if (str[ind] == ')')
                num -= 1;
        }
    }
    else
    {
        std::cerr << "center_ind(): ERROR: wrong str:\n" <<
                  "str[0] != '('\n" <<
                  "(str[0] != '!') & (str[1] != '(')" << std::endl;
        ind = -1;
    }
    return ind;
}

bool check_position(std::string str1, size_t ind, std::string str2)
{
    bool result = true;
    for (size_t j = 0; j < str2.size(); j++)
    {
        if (ind + j > str1.size())
        {
            result = false;
            break;
        }
        if (str1[ind + j] != str2[j])
        {
            result = false;
            break;
        }
    }
    return result;
}

std::pair<std::string, std::string> conclude(const std::vector<struct statement>& A, int i, int j, const std::vector<std::string>& rulse) // ⊢
{
    std::string used = "no_conclusion";
    std::string A_new = "no_conclusion";
    bool find = false;
    for (const std::string& rule: rulse)
    {
        if (rule == "modus_ponens")                     // P, P→Q ⊢ Q
        {
            // if A[i] == "(" + A[j] + ">" + ... + ")"
            if ((A[i].this_statement.find(A[j].this_statement) == 1) & (A[i].this_statement.size() - A[j].this_statement.size() > 3))
            {
                used = "modus_ponens";
                size_t ind = A[j].this_statement.size() + 2;                                              // ind – индекс начала выведенного step высказывания
                A_new = A[i].this_statement.substr(ind, (A[i].this_statement.size() - 1) - ind);  // n – колличество символов, которые нужно скопировать в строку нового высказывания
                find = true;
            }
        }
        else if (rule == "modus_tollens")                // -Q, P→Q ⊢ -P
        {
            std::string Q = "!" + A[j].this_statement;
            not_double_denial(Q);
            // if A[i] == "(" + ... + ">" + !A[j] + ")"
            size_t ind = A[i].this_statement.find(Q);
            if ((ind != std::string::npos) & (A[i].this_statement.size() == ind + Q.size() + 1))
            {
                used = "modus_tollens";
                A_new = "!" + A[i].this_statement.substr(1, ind - 1 - 1);
                find = true;
            }
        }
        else if (rule == "дизъюнктивный_силлогизм")      // -P, P∨Q ⊢ Q
        {                                                // -P, -P→Q ⊢ Q
                                                         // P, P→Q ⊢ Q
            // if A[i] == "(" + A[j] + ">" + ... + ")"
            if ((A[i].this_statement.find(A[j].this_statement) == 1) & (A[i].this_statement.size() - A[j].this_statement.size() > 3))
            {
                used = "дизъюнктивный_силлогизм";
                size_t ind = A[j].this_statement.size() + 2;                                              // ind – индекс начала выведенного step высказывания
                A_new = A[i].this_statement.substr(ind, (A[i].this_statement.size() - 1) - ind);  // n – колличество символов, которые нужно скопировать в строку нового высказывания
                find = true;
            }
        }
        else if (rule == "гипотетический_силлогизм")     // P→Q, Q→R ⊢ P→R
        {
            size_t center = center_ind(A[j].this_statement);
            std::string Q = A[j].this_statement.substr(center + 1, (A[j].this_statement.size() - 1) - (center + 1));
            // if A[j] == "(" + P + ">" + Q + ")"
            //    A[i] == "(" + Q + ">" + ... + ")"
            if ((A[i].this_statement.find(Q) == 1) & (A[i].this_statement.size() - Q.size() > 3))
            {
                used = "гипотетический_силлогизм";
                size_t ind = Q.size() + 2;
                A_new = A[j].this_statement.substr(0, A[j].this_statement.find(Q));     // (P→
                A_new += A[i].this_statement.substr(ind, A[i].this_statement.size() - ind); // Q)
                find = true;
            }
        }
        else if (rule == "разделительный_силлогизм")     // P, P ⊕ Q ⊢ -Q
        {                                                // P, (-P→-Q)→-(P→Q) ⊢ -Q
            // if A[i] == "((" + !A[j] + ">" + !... + ")>!(" + A[j] + ">" + ... + "))"
            std::string Q = "!" + A[j].this_statement;
            not_double_denial(Q);
            size_t center = center_ind(A[i].this_statement);
            if ((A[i].this_statement.find(Q) == 2) & (check_position(A[i].this_statement, center + 3, A[j].this_statement)))
            {
                used = "разделительный_силлогизм";
                size_t ind = center + 3 + A[j].this_statement.size() + 1;
                A_new = A[i].this_statement.substr(ind, (A[i].this_statement.size() - 2) - ind);
                find = true;
            }
        }
        if (find)
        {
            break;
        }
    }
    not_double_denial(A_new);
    std::pair result(used, A_new);
    return result;
}

void check_conclude()
{
    struct statement A0(0, "A");
    struct statement A1(1, "B");
    struct statement A2(2, "C");
    struct statement A3(3, "!A");
    struct statement A4(4, "!B");
    struct statement A5(5, "!C");
    struct statement A6(6, "(A>B)");
    struct statement A7(7, "(!A>B)");
    struct statement A8(8, "(B>C)");
    struct statement A9(9, "((!A>!B)>!(A>B))");
    std::vector<struct statement> list = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
    std::pair<std::string, std::string> conclusion;
    std::string A_new;
    std::vector<std::string> rules = {"modus_ponens"};
    conclusion = conclude(list, 6, 0, rules);
    A_new = conclusion.second;
    std::cout << "\n__________modus_ponens__________\n"
                << conclusion_to_str(-1, list, 0, 6, conclusion.first, A_new) << std::endl;
    rules = {"modus_tollens"};
    conclusion = conclude(list, 6, 4, rules);
    A_new = conclusion.second;
    std::cout << "\n__________modus_tollens__________\n"
                << conclusion_to_str(-1, list, 4, 6, conclusion.first, A_new) << std::endl;
    rules = {"дизъюнктивный_силлогизм"};
    conclusion = conclude(list, 7, 3, rules);
    A_new = conclusion.second;
    std::cout << "\n__________дизъюнктивный_силлогизм__________\n"
                << conclusion_to_str(-1, list, 3, 7, conclusion.first, A_new) << std::endl;
    rules = {"гипотетический_силлогизм"};
    size_t center = center_ind(list[8].this_statement);
    conclusion = conclude(list, 8, 6, rules);
    A_new = conclusion.second;
    std::cout << "\n__________гипотетический_силлогизм__________\n" << "center_ind(A[j]) = " << center << "\n"
                << conclusion_to_str(-1, list, 6, 8, conclusion.first, A_new) << std::endl;
    rules = {"разделительный_силлогизм"};
    center = center_ind(list[9].this_statement);
    conclusion = conclude(list, 9, 0, rules);
    A_new = conclusion.second;
    std::cout << "\n__________разделительный_силлогизм__________\n" << "center_ind(A[i]) = " << center << "\n"
                << conclusion_to_str(-1, list, 0, 9, conclusion.first, A_new) << std::endl;
}
