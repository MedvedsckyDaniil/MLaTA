#include "converter.h"

std::vector<std::string> find_left_and_right(std::string str, size_t ind)
{
    // DATA
    // ind – index of one off them: '|', '*', '+', '='
    size_t start;
    size_t finish;
    std::vector<std::string> result;

    // PROCESS

    // left
    if (std::isalpha(str[ind-1]))
    {
        if (str[ind-2] == '!')
            start = ind - 2;
        else
            start = ind - 1;
    }
    else if (str[ind-1] == ')')
    {
        size_t i = ind-1;
        int num = 1;
        while (num != 0)
        {
            i -= 1;
            if (str[i] == ')')
                num += 1;
            if (str[i] == '(')
                num -= 1;
        }
        start = i;
    }
    else
    {
        std::cerr << "find_left_and_right(): ERROR: wrong str" << std::endl;
        start = ind - 1;
    }
    std::string left = str.substr(0, start);
    result.push_back(left);
    left = str.substr(start, ind - start);
    result.push_back(left);

    // center
    std::string center(1, str[ind]);
    result.push_back(center);

    // right
    if (std::isalpha(str[ind+1]))
    {
        finish = ind + 1;
    }
    else if (str[ind+1] == '!')
    {
        if (std::isalpha(str[ind+2]))
        {
            finish = ind + 2;
        }
        if (str[ind+2] == '(')
        {
            size_t i = ind+2;
            int num = 1;
            while (num != 0)
            {
                i += 1;
                if (str[i] == '(')
                    num += 1;
                if (str[i] == ')')
                    num -= 1;
            }
            finish = i;
        }
    }
    else if (str[ind+1] == '(')
    {
        size_t i = ind+1;
        int num = 1;
        while (num != 0)
        {
            i += 1;
            if (str[i] == '(')
                num += 1;
            if (str[i] == ')')
                num -= 1;
        }
        finish = i;
    }
    else
    {
        std::cerr << "find_left_and_right(): ERROR: wrong str" << std::endl;
        finish = ind + 1;
    }
    std::string right = str.substr(ind+1, finish - ind);
    result.push_back(right);
    right = str.substr(finish+1, str.size() - finish);
    result.push_back(right);

    // str = result[0] + result[1] + result[2] + result[3] + result[4];
    return result;
}

void not_double_denial(std::string& str)
{
    std::string fixed_string;
    for (int i = 0; i < str.size() - 1; i++)
    {
        if (str[i] == '!' & str[i+1] == '!')
        {
            i += 1;
        }
        else
        {
            fixed_string += str[i];
        }
    }
    fixed_string += str[str.size() - 1];
    str = fixed_string;
}

// converter all '|', '*', '+', '=' to '>' and '!'
// converter all "∨", "∧", "⊕", "≡" to "→" and "-"
std::string converter(const std::string& str)
{
    // DATA
    std::string new_str = str;
    std::vector<std::string> vec;
    size_t ind;

    // PROCESS
    ind = new_str.find('|'); // "∨"
    while (ind != std::string::npos)            // A∨B = -A→B
    {
        vec = find_left_and_right(new_str, ind);
        new_str = vec[0]
                  + "!" + vec[1] + ">" + vec[3]
                  + vec[4];
        ind = new_str.find('|');
    }
    ind = new_str.find('*'); // "∧"
    while (ind != std::string::npos)            // (A∧B) = -(A→-B)
    {
        vec = find_left_and_right(new_str, ind);
        std::string start; // (start != vec[0])
        for (int i = 0; i < vec[0].size() - 1; i++)
        {
            start += vec[0][i];
        }
        new_str = start
                  + "!(" + vec[1] + ">" + "!" + vec[3]
                  + vec[4];
        ind = new_str.find('*');
    }
    ind = new_str.find('+'); // "⊕"
    while (ind != std::string::npos)            // A⊕B = (-A→-B)→-(A→B)
    {
        vec = find_left_and_right(new_str, ind);
        new_str = vec[0]
                  + "(!" + vec[1] + ">" + "!" + vec[3] + ")"
                  + ">"
                  + "!(" + vec[1] + ">" + vec[3] + ")"
                  + vec[4];
        ind = new_str.find('+');
    }
    ind = new_str.find('='); // "≡"
    while (ind != std::string::npos)            // A≡B = (A→-B)→-(-A→B)
    {
        vec = find_left_and_right(new_str, ind);
        new_str = vec[0]
                  + "(" + vec[1] + ">" + "!" + vec[3] + ")"
                  + ">"
                  + "!(!" + vec[1] + ">" + vec[3] + ")"
                  + vec[4];
        ind = new_str.find('=');
    }
    not_double_denial(new_str);

    return new_str;
}
