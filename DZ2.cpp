#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <stack>
#include <iterator>
#include <cmath>

unsigned int prior(char i) {
    switch (i) {
        case '+':
        case '-': {
            return 1;
        }
        case '*':
        case '^':
        case '/': {
            return 2;
        }
        default: {
            return 0;
        }
    }
}

void expressiontopolskay(std::string expression, std::vector<std::string> &polskay) {
    std::stack<std::string> stackofsign;
    while (expression.find("exp") != -1) {
        expression.replace(expression.find("exp"), 3, "e^");
    }
    std::string temp;
    std::string::iterator it = expression.begin();
    while (it != expression.end()) {
        if (isdigit(*it)) {
            while (isdigit(*it) || *it == '.') {
                temp.push_back(*it);
                it++;
            }
            polskay.push_back(temp);
        } else if (isalpha(*it)) {
            if (expression.compare(std::distance(expression.begin(), it), 3, "cos") == 0) {
                it += 3;
                stackofsign.push("cos");
            } else if (expression.compare(std::distance(expression.begin(), it), 1, "e") == 0) {
                it++;
                polskay.push_back("e");
            } else if (expression.compare(std::distance(expression.begin(), it), 2, "pi") == 0) {
                it += 2;
                polskay.push_back("pi");
            } else if (expression.compare(std::distance(expression.begin(), it), 3, "sin") == 0) {
                it += 3;
                stackofsign.push("sin");
            } else if (expression.compare(std::distance(expression.begin(), it), 3, "tan") == 0) {

                it += 3;
                stackofsign.push("tan");
            } else if (expression.compare(std::distance(expression.begin(), it), 3, "ctg") == 0) {
                it += 3;
                stackofsign.push("ctg");
            } else {
                while (isalpha(*it)) {
                    temp.push_back(*it);
                    it++;
                }
                std::cout << "Enter " << temp << std::endl;
                std::string temp1;
                std::cin >> temp1;
                polskay.push_back(temp1);
                *(it - 1) = '0'; // посмотреть как ведет себя итератор костыыыыыыыыыль
            }
        } else if (*it == '(') {
            stackofsign.push("(");
            it++;
        } else if (*it == ')') {
            if (stackofsign.empty()) {
                std::cout << "Error: check the brackets or operation " << std::endl;
                polskay.clear();
                return;
            }
            while (stackofsign.top() != "(") {
                polskay.push_back(stackofsign.top());
                stackofsign.pop();
                if (stackofsign.empty()) {
                    std::cout << "Error: check the brackets or operation " << std::endl;
                    polskay.clear();
                    return;
                }
            }
            stackofsign.pop();
            it++;
        } else {
            while ((!stackofsign.empty() && (stackofsign.top().length() == 3)) ||
                   (!stackofsign.empty() && (prior(stackofsign.top()[0]) >= prior(*it)))) {
                polskay.push_back(stackofsign.top());
                stackofsign.pop();
            }
            if (*it == '+') {
                stackofsign.push("+");
            } else if (*it == '-') {
                if (!isdigit(*(it - 1)) && (*(it - 1) != ')') && (*(it - 1) != 'e') &&
                    (expression.compare(std::distance(expression.begin(), it - 2), 2, "pi") != 0)) {
                    stackofsign.push("___");
                } else {
                    stackofsign.push("-");
                }
            } else if (*it == '*') {
                stackofsign.push("*");
            } else if (*it == '/') {
                stackofsign.push("/");
            } else if (*it == '^') {
                stackofsign.push("^");
            } else {
                std::cout << "check the expression" << std::endl;
                return;
            }
            it++;
        }
        temp.clear();
    }
    if (it == expression.end()) {
        while (!stackofsign.empty()) {
            polskay.push_back(stackofsign.top());
            stackofsign.pop();
        }
    }
    for (std::string str : polskay) {
        if (str == "(" || str == ")") {
            std::cout << "Error cheack (" << std::endl;
            polskay.clear();
            return;
        }
    }
}

void polskaytodouble(std::vector<std::string> polskay, double &i) {
    std::stack<double> doubl;
    std::vector<std::string>::iterator str = polskay.begin();
    for (std::string str : polskay) {
        if (isdigit(str[0])) {

            doubl.push(std::stod(str));
        } else if (isalpha(str[0])) {
            if (str == "sin") {
                doubl.top() = sin(doubl.top());
            } else if (str == "tan") {
                doubl.top() = tan(doubl.top());
            } else if (str == "ctg") {
                doubl.top() = 1 / tan(doubl.top());
            } else if (str == "cos") {
                doubl.top() = cos(doubl.top());
            } else if (str == "pi") {
                doubl.push(M_PI);
            } else if (str == "e") {
                doubl.push(M_E);
            } else {
                std::cout << "check expression" << std::endl;
            }
        } else if (str[0] == '_') {
            doubl.top() = doubl.top() * (-1);
        } else if (ispunct(str[0])) {
            if (doubl.size() == 1) {
                std::cout << "Check the arguments or operator" << std::endl;
                return;
            }
            double i = 0;
            switch (str[0]) {
                case '+':
                    i = doubl.top();
                    doubl.pop();
                    doubl.top() = doubl.top() + i;

                    break;
                case '-':
                    i = doubl.top();
                    doubl.pop();
                    doubl.top() = doubl.top() - i;
                    break;
                case '*':
                    i = doubl.top();
                    doubl.pop();
                    doubl.top() = doubl.top() * i;
                    break;
                case '/':
                    i = doubl.top();
                    doubl.pop();
                    doubl.top() = doubl.top() / i;
                    break;
                case '^':
                    i = doubl.top();
                    doubl.pop();
                    doubl.top() = pow(doubl.top(), i);
                    break;

                default:
                    std::cout << "Check operation" << std::endl;
                    return;
            }
        }
    }
    i = doubl.top();
}

int main() {
    std::string expression;
    std::vector<std::string> polskay;
    getline(std::cin, expression);
    while (expression.find(' ') != -1) {
        expression.erase(expression.find(' '), 1);
    }
    expressiontopolskay(expression, polskay);
    for (std::string str : polskay) {
        std::cout << str << " ";
    }
    std::cout << '\n';
    double i = 0;
    polskaytodouble(polskay, i);
    std::cout << i << std::endl;
    return 0;
}