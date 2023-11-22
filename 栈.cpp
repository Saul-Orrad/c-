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

int getPrecedence(char op) {/*�жϲ����������ȼ�*/
    if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    return 0; // ������ǲ�����������0
}



/*�ж���׺���ʽ�Ƿ�Ϸ�*/
//strΪ�������׺���ʽ�ַ���
int ExpressionCheck(string st)
{
    string str;

    // ������ʽȥ���ո�
    for (char c : st) {
        if (!isspace(c)) {
            str += c;
        }
    }


    //��ʼ��һ���ַ�ջ
    stack<char> s;
    char e;
    int i;
    //���Ϸ����1��ȱ�ټ������
   for (i = 1; i < str.length() - 1; i++)
    {
        if (str[i] == '(' && isdigit(str[i-1]))
        {
            std::cout << "ȱ�ټ������" << endl;
            return ERROR;
        }
        if (str[i] == ')' && isdigit(str[i+1]))
        {
            std::cout << "ȱ�ټ������" << endl;
            return ERROR;
        }
    }

    

    //���Ϸ����2:ȱ�ټ��������
    for (i = 0; i < str.length(); i++)
    {
        //���ж� '-' �ǲ��Ǹ�������
        if ('-' == str[i])
        {
            if (0 == i ||(i>0&& '(' == str[i - 1])) //��� - �ڵ�һλ����ǰ����(,һ������Ϊ�������Ŷ��ǲ�����
                i++;
        }
        if (isOperator(str[i]))
        {
            if (0 == i || str.length() - 1 == i)
            {
                std::cout << "ȱ�ټ��������" << endl;
                return 0;//�Է��ſ�ʼ���β���Ϸ�
            }
            if (i > 0 && isOperator(str[i - 1]) || i < str.length() - 1 && isOperator(str[i + 1])) {
                std::cout << "ȱ�ټ��������" << endl;
                return 0;//�����������з��ţ��򲻺Ϸ�
            }
            if (i > 0 && '(' == str[i - 1] || i < str.length() - 1 && ')' == str[i + 1]) {
                std::cout << "ȱ�ټ��������" << endl;
                return ERROR;//������������������ţ����Ϸ�
            }
        }
    }

    //���Ϸ����3:�����ź�����������,����a+()��(a+b)(a+b)
    for (i = 0; i < str.length()-1; i++)
    {
        if (str[i] == '(' && str[i + 1] == ')')
            return ERROR;
        if (str[i] == ')' && str[i + 1] == '(')
            return ERROR;
    }

    //���Ϸ����3:���Ų�ƥ��

    for (i = 0; i < str.length(); i++)  //������׺���ʽ
    {
        if (str[i] == '(')
            s.push(str[i]);
        if (str[i] == ')')
        {
            if (s.empty()) {
                std::cout << "���Ų�ƥ��" << endl;
                return ERROR;
                
            }
            s.pop();
        }
    }
    if (!s.empty()) {
        std::cout << "���Ų�ƥ��" << endl;
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
* ���µ�ע�Ͳ����ǲ��д���������Ҫ�����׺���ʽ������ֵ
*/

//double evaluate(const string& str) {
//    stack<double> operandStack;  // ����ջ
//    stack<char> operatorStack;   // ������ջ
//
//    // ʹ���ַ����������λ��
//    istringstream iss(str);
//
//    string token;
//    while (iss >> token) {
//        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
//            // ��������֣���ջ
//            double operand = stod(token);
//            operandStack.push(operand);
//        }
//        else if (isOperator(token[0])) {
//            // ����ǲ���������ջ�����ȼ����ڵ��ڵ�ǰ��������Ԫ�ص�����Ȼ�󽫵�ǰ��������ջ
//            while (!operatorStack.empty() && getPrecedence(operatorStack.top()) >= getPrecedence(token[0])) {
//                char op = operatorStack.top();
//                operatorStack.pop();
//
//                // ��ջ���������������м��㣬����ջ���
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
//            // ������ֱ����ջ
//            operatorStack.push('(');
//        }
//        else if (token == ")") {
//            // �����ţ���ջ��������֮�ϵĲ����������������м���
//            while (!operatorStack.empty() && operatorStack.top() != '(') {
//                char op = operatorStack.top();
//                operatorStack.pop();
//
//                // ��ջ���������������м��㣬����ջ���
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
//            // ����������
//            operatorStack.pop();
//        }
//        else {
//            // ����Ǳ����������Ҷ�Ӧ��ֵ����ջ
//            auto it = table.find(token);
//            if (it != table.end()) {
//                operandStack.push(it->second);
//            }
//        }
//    }
//
//    // ����ʣ��Ĳ�����
//    while (!operatorStack.empty()) {
//        char op = operatorStack.top();
//        operatorStack.pop();
//
//        // ��ջ���������������м��㣬����ջ���
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
//    // ����ջ��ʣ�µľ��Ǽ�����
//    return operandStack.top();
//}


int main()
{
    string s;
    cout << "����exit�˳�" << endl;
  
    while (1) {
        getline(cin, s);
        if (s == "exit") break;
        if (s == "") continue;
        if (!ExpressionCheck(s))
        {
            cout << "������ʽ��" << endl;
            continue;
        }

        /*�ǲ���ջ����*/
        vector<string> postfix = infixToPostfix(s);
     /*   for (int i = 0; i < postfix.size(); i++)
        {
            cout << postfix[i] << " ";
        }
        cout<<endl;*/
        double result = evaluatePostfix(postfix);
        
        /*����ջ����*/
        //double result = evaluate(s);
        
        cout << result << endl;
        continue;
    }


}