#include <bits/stdc++.h>
using namespace std;
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

int eval_term(const string &exp, int &idx);

int eval_exp(const string &exp, int &idx)
{
    int res = eval_term(exp, idx);

    while (idx < exp.length() && exp[idx] != ')')
    {
        if (isspace(exp[idx]))
        {
            idx++;
            continue;
        }
        char curr_opr = exp[idx];
        if (curr_opr == '+')
        {
            idx++;
            res = add(res, eval_term(exp, idx));
        }
        else
        {
            break;
        }
    }
    return res;
}

int eval_term(const string &exp, int &idx)
{
    int res = 0;
    if (exp[idx] == '(')
    {
        idx++;
        res = eval_exp(exp, idx);
        if (idx < exp.length() && exp[idx] == ')')
        {
            idx++;
        }
    }
    else if (isdigit(exp[idx]))
    {
        int num = 0;
        while (idx < exp.length() && isdigit(exp[idx]))
        {
            num = num * 10 + (exp[idx] - '0');
            idx++;
        }
        res = num;
    }

    while (idx < exp.length() && exp[idx] == '*')
    {
        idx++;
        int next_val = 0;
        if (exp[idx] == '(')
        {
            idx++;
            next_val = eval_exp(exp, idx);
            if (idx < exp.length() && exp[idx] == ')')
            {
                idx++;
            }
        }
        else if (isdigit(exp[idx]))
        {
            int num = 0;
            while (idx < exp.length() && isdigit(exp[idx]))
            {
                num = num * 10 + (exp[idx] - '0');
                idx++;
            }
            next_val = num;
        }
        res = multiply(res, next_val);
    }
    return res;
}
int cal(const string &exp)
{
    int index = 0;
    return eval_exp(exp, index);
}

bool areBracketsBalanced(const string &expr)
{
    stack<char> temp;
    for (int i = 0; i < expr.length(); i++)
    {
        if (temp.empty())
        {
            temp.push(expr[i]);
        }
        else if ((temp.top() == '(' && expr[i] == ')'))
        {

            temp.pop();
        }
        else
        {
            temp.push(expr[i]);
        }
    }
    if (temp.empty())
    {

        return true;
    }
    return false;
}

// bool validity_check(const string &exp)
// {
//     if (!areBracketsBalanced(exp))
//         return false;
//     if (exp[0] == '+' || exp[0] == '*')
//         return false;
//     if (exp[exp.length() - 1] == '+' || exp[exp.length() - 1] == '*')
//         return false;
//     for (auto i : exp)
//     {
//         if (i != '+' || i != '*' || i != ')' || i != '(' || i != '1' || i != '2' || i != '3' || i != '4' || i != '5' || i != '6' || i != '7' || i != '8' || i != '9' || i != '0' || i != ' ')
//             return false;
//     }
//     return true;
// }

int main(int argc, char const *argv[])
{
    string exp;
    cout << "Enter : ";
    getline(cin, exp);
    string exp2 = "";
    for (auto i : exp)
    {
        if (!isspace(i))
            exp2 += i;
    }
    // if (!validity_check(exp2))
    //     cout << "INVALID" << endl;
    // else
    //     cout << "Result : " << cal(exp2) << endl;
    cout << "Result : " << cal(exp2) << endl;
    return 0;
}