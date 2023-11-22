#include<iostream>
#include<string>
#include <unordered_map>
#include<stack>
#include <algorithm>
#include <sstream>
#include <cctype>

using namespace std;
const bool OK = true;
const bool ERROR = false;
std::unordered_map<std::string, double> table = {
     {"a", 1.2},
     {"b", 3.3},
     {"c", 5.0},
     {"d", 2.5},
     {"e", 1.2}
};
bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

int getPrecedence(char op) {/*判断操作符的优先级*/
    if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    return 0; // 如果不是操作符，返回0
}



/*判断中缀表达式是否合法*/
//str为输入的中缀表达式字符串
int ExpressionCheck(string st)
{
    string str;

    // 迭代方式去除空格
    for (char c : st) {
        if (!isspace(c)) {
            str += c;
        }
    }


    //初始化一个字符栈
    stack<char> s;
    char e;
    int i;
    //不合法情况1，缺少计算符号
   for (i = 1; i < str.length() - 1; i++)
    {
        if (str[i] == '(' && isdigit(str[i-1]))
        {
            std::cout << "缺少计算符号" << endl;
            return ERROR;
        }
        if (str[i] == ')' && isdigit(str[i+1]))
        {
            std::cout << "缺少计算符号" << endl;
            return ERROR;
        }
    }

    

    //不合法情况2:缺少计算操作数
    for (i = 0; i < str.length(); i++)
    {
        //先判断 '-' 是不是负数符号
        if ('-' == str[i])
        {
            if (0 == i ||(i>0&& '(' == str[i - 1])) //如果 - 在第一位或者前面有(,一定是作为负数符号而非操作符
                i++;
        }
        if (isOperator(str[i]))
        {
            if (0 == i || str.length() - 1 == i)
            {
                std::cout << "缺少计算操作数" << endl;
                return 0;//以符号开始或结尾不合法
            }
            if (i > 0 && isOperator(str[i - 1]) || i < str.length() - 1 && isOperator(str[i + 1])) {
                std::cout << "缺少计算操作数" << endl;
                return 0;//若符号两侧有符号，则不合法
            }
            if (i > 0 && '(' == str[i - 1] || i < str.length() - 1 && ')' == str[i + 1]) {
                std::cout << "缺少计算操作数" << endl;
                return ERROR;//若运算符号两侧有括号，不合法
            }
        }
    }

    //不合法情况3:左括号和右括号相邻,例：a+()和(a+b)(a+b)
    for (i = 0; i < str.length()-1; i++)
    {
        if (str[i] == '(' && str[i + 1] == ')')
            return ERROR;
        if (str[i] == ')' && str[i + 1] == '(')
            return ERROR;
    }

    //不合法情况3:括号不匹配

    for (i = 0; i < str.length(); i++)  //遍历中缀表达式
    {
        if (str[i] == '(')
            s.push(str[i]);
        if (str[i] == ')')
        {
            if (s.empty()) {
                std::cout << "括号不匹配" << endl;
                return ERROR;
                
            }
            s.pop();
        }
    }
    if (!s.empty()) {
        std::cout << "括号不匹配" << endl;
        return ERROR;
    }
    
    return OK;
   
    
}

vector<string> infixToPostfix(const string& in) {
    string infix = in;


    
    vector<string> postfix;
    stack<char> operatorStack;

    istringstream iss(infix);
    string token;

    while (iss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            postfix.push_back(token);
        }
        else if (isalpha(token[0])) {
            auto it = table.find(token);
            if (it != table.end()) {
                postfix.push_back(to_string(it->second));
            }
        }
        else if (isOperator(token[0])) {
            while (!operatorStack.empty() && getPrecedence(operatorStack.top()) >= getPrecedence(token[0])) {
                postfix.push_back(string(1, operatorStack.top()));
                operatorStack.pop();
            }
            operatorStack.push(token[0]);
        }
        else if (token == "(") {
            operatorStack.push('(');
        }
        else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                postfix.push_back(string(1, operatorStack.top()));
                operatorStack.pop();
            }
            operatorStack.pop(); // Pop the '('
        }
    }

    while (!operatorStack.empty()) {
        postfix.push_back(string(1, operatorStack.top()));
        operatorStack.pop();
    }

    return postfix;
}

double evaluatePostfix(const vector<string>& postfix) {
    stack<double> operandStack;

    for (const string& token : postfix) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            operandStack.push(stod(token));
        }
        else if (isOperator(token[0])) {
            double operand2 = operandStack.top();
            operandStack.pop();
            double operand1 = operandStack.top();
            operandStack.pop();

            switch (token[0]) {
            case '+':
                operandStack.push(operand1 + operand2);
                break;
            case '-':
                operandStack.push(operand1 - operand2);
                break;
            case '*':
                operandStack.push(operand1 * operand2);
                break;
            case '/':
                operandStack.push(operand1 / operand2);
                break;
            }
        }
    }

    return operandStack.top();
}
/*
* 以下的注释部分是并行处理，即不需要算出后缀表达式即可求值
*/

//double evaluate(const string& str) {
//    stack<double> operandStack;  // 数字栈
//    stack<char> operatorStack;   // 操作符栈
//
//    // 使用字符串流处理多位数
//    istringstream iss(str);
//
//    string token;
//    while (iss >> token) {
//        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
//            // 如果是数字，入栈
//            double operand = stod(token);
//            operandStack.push(operand);
//        }
//        else if (isOperator(token[0])) {
//            // 如果是操作符，将栈中优先级大于等于当前操作符的元素弹出，然后将当前操作符入栈
//            while (!operatorStack.empty() && getPrecedence(operatorStack.top()) >= getPrecedence(token[0])) {
//                char op = operatorStack.top();
//                operatorStack.pop();
//
//                // 出栈两个操作数，进行计算，再入栈结果
//                double operand2 = operandStack.top();
//                operandStack.pop();
//                double operand1 = operandStack.top();
//                operandStack.pop();
//
//                switch (op) {
//                case '+':
//                    operandStack.push(operand1 + operand2);
//                    break;
//                case '-':
//                    operandStack.push(operand1 - operand2);
//                    break;
//                case '*':
//                    operandStack.push(operand1 * operand2);
//                    break;
//                case '/':
//                    operandStack.push(operand1 / operand2);
//                    break;
//                }
//            }
//            operatorStack.push(token[0]);
//        }
//        else if (token == "(") {
//            // 左括号直接入栈
//            operatorStack.push('(');
//        }
//        else if (token == ")") {
//            // 右括号，将栈中左括号之上的操作符都弹出，进行计算
//            while (!operatorStack.empty() && operatorStack.top() != '(') {
//                char op = operatorStack.top();
//                operatorStack.pop();
//
//                // 出栈两个操作数，进行计算，再入栈结果
//                double operand2 = operandStack.top();
//                operandStack.pop();
//                double operand1 = operandStack.top();
//                operandStack.pop();
//
//                switch (op) {
//                case '+':
//                    operandStack.push(operand1 + operand2);
//                    break;
//                case '-':
//                    operandStack.push(operand1 - operand2);
//                    break;
//                case '*':
//                    operandStack.push(operand1 * operand2);
//                    break;
//                case '/':
//                    operandStack.push(operand1 / operand2);
//                    break;
//                }
//            }
//
//            // 弹出左括号
//            operatorStack.pop();
//        }
//        else {
//            // 如果是变量名，查找对应的值并入栈
//            auto it = table.find(token);
//            if (it != table.end()) {
//                operandStack.push(it->second);
//            }
//        }
//    }
//
//    // 处理剩余的操作符
//    while (!operatorStack.empty()) {
//        char op = operatorStack.top();
//        operatorStack.pop();
//
//        // 出栈两个操作数，进行计算，再入栈结果
//        double operand2 = operandStack.top();
//        operandStack.pop();
//        double operand1 = operandStack.top();
//        operandStack.pop();
//
//        switch (op) {
//        case '+':
//            operandStack.push(operand1 + operand2);
//            break;
//        case '-':
//            operandStack.push(operand1 - operand2);
//            break;
//        case '*':
//            operandStack.push(operand1 * operand2);
//            break;
//        case '/':
//            operandStack.push(operand1 / operand2);
//            break;
//        }
//    }
//
//    // 最终栈内剩下的就是计算结果
//    return operandStack.top();
//}


int main()
{
    string s;
    cout << "输入exit退出" << endl;
  
    while (1) {
        getline(cin, s);
        if (s == "exit") break;
        if (s == "") continue;
        if (!ExpressionCheck(s))
        {
            cout << "错误表达式！" << endl;
            continue;
        }

        /*非并行栈代码*/
        vector<string> postfix = infixToPostfix(s);
     /*   for (int i = 0; i < postfix.size(); i++)
        {
            cout << postfix[i] << " ";
        }
        cout<<endl;*/
        double result = evaluatePostfix(postfix);
        
        /*并行栈代码*/
        //double result = evaluate(s);
        
        cout << result << endl;
        continue;
    }


}