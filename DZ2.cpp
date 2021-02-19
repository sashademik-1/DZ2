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
        case '/': {
            return 2;
        }
        case '^': {
            return 3;
        }
        default: {
            return 0;
        }
    }
}
// перваначальная проверка

void expressiontovector(std::string expression, std::vector<std::pair<int, std::string>> &expr) {
    while (expression.find("exp") != std::string::npos) {
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
            if (std::count_if(temp.begin(), temp.end(), [](char i) { return i == '.'; }) > 1) {
                std::cout << "Check the ." << std::endl;
                expr.clear();
                return;
            }
            expr.emplace_back(std::make_pair(1, temp));

        } else if (isalpha(*it)) {
            if (expression.compare(static_cast<unsigned int>(std::distance(expression.begin(), it)), 3, "cos") == 0) {
                it += 3;
                expr.emplace_back(std::make_pair(4, "cos"));

            } else if (expression.compare(std::abs(std::distance(expression.begin(), it)), 1, "e") == 0) {
                it++;
                expr.emplace_back(std::make_pair(1, "e"));
            } else if (expression.compare(std::abs(std::distance(expression.begin(), it)), 2, "pi") == 0) {
                it += 2;
                expr.emplace_back(std::make_pair(1, "pi"));
            } else if (expression.compare(std::abs(std::distance(expression.begin(), it)), 3, "sin") == 0) {
                it += 3;
                expr.emplace_back(std::make_pair(4, "sin"));
            } else if (expression.compare(std::abs(std::distance(expression.begin(), it)), 3, "tan") == 0) {

                it += 3;
                expr.emplace_back(std::make_pair(4, "tan"));
            } else if (expression.compare(std::abs(std::distance(expression.begin(), it)), 3, "ctg") == 0) {
                it += 3;
                expr.emplace_back(std::make_pair(4, "ctg"));
            } else if (expression.compare(std::abs(std::distance(expression.begin(), it)), 4, "sqrt") == 0) {
                it += 4;
                expr.emplace_back(std::make_pair(4, "sqrt"));
            } else {
                while (isalpha(*it)) {
                    temp.push_back(*it);
                    it++;
                }
                std::cout << "Enter " << temp << std::endl;
                std::string temp1;
                std::cin >> temp1;
                expr.emplace_back(std::make_pair(1, temp1));
                while (expression.find(temp) != std::string::npos) {
                    expression.replace(expression.find(temp), temp.size(), temp1);
                }

                it += temp1.size() - temp.size();

            }

        } else if (*it == '(') {
            expr.emplace_back(std::make_pair(3, "("));
            it++;
        } else if (*it == ')') {
            expr.emplace_back(std::make_pair(5, ")"));
            it++;
        } else if (*it == '+') {
            expr.emplace_back(std::make_pair(2, "+"));
            it++;
        } else if (*it == '-') {
            expr.emplace_back(std::make_pair(2, "-"));
            it++;
        } else if (*it == '*') {
            expr.emplace_back(std::make_pair(2, "*"));
            it++;
        } else if (*it == '/') {
            expr.emplace_back(std::make_pair(2, "/"));
            it++;
        } else if (*it == '^') {
            expr.emplace_back(std::make_pair(2, "^"));
            it++;
        }
        temp.clear();
    }

}

void vectortopolskay(std::vector<std::pair<int, std::string>> &expr, std::vector<std::string> &polskay) {
    auto it = expr.begin();
    std::stack<std::pair<int, std::string>> temp;
    while (true) {
        if (it == expr.end()) {
            while (!temp.empty()) {
                polskay.push_back(temp.top().second);
                temp.pop();
            }
            return;
        }
        if (it->first == 1) {
            polskay.push_back(it->second);
        } else if (it->first == 4) {
            temp.push(*it);
        } else if (it->first == 3) {
            temp.push(*it);
        } else if (it->first == 5) {
            while (temp.top().second != "(") {
                if (temp.empty()) {
                    return;
                }
                polskay.push_back(temp.top().second);
                temp.pop();
            }
            temp.pop();
        } else if ((it->second == "-") && ((it) == expr.end() - 1)) {
        } else if ((it->second == "+") && ((it) == expr.end() - 1)) {
        } else if (((it->second == "-") && ((it) == expr.begin())) || (it->second == "-" && (it - 1)->first == 3) ||
                   ((it->second == "-") && (it - 1)->first == 2)) {
            it->first = 4;
            it->second = "_";
            continue;
        } else if ((it->second == "+" && ((it) == expr.begin())) || ((it->second == "+") && ((it) == expr.end() - 1)) ||
                   (it->second == "+" && (it - 1)->first == 3) ||
                   (it->second == "+" && (it - 1)->first == 2)) {
            it->first = 4;
            it->second = "?";
            continue;
        } else if (it->first == 2) {
            if (!temp.empty()) {
                while ((temp.top().first == 4) || prior(temp.top().second[0]) >= prior(it->second[0])) {
                    polskay.push_back(temp.top().second);
                    temp.pop();
                    if (temp.empty()) { break; }
                }
            }
            temp.push(*it);
        }
        it++;
    }
}

bool testforexpression(std::string str) {
    int b = 0;
    for (char i: str) {
        if (i == '(') { b += 1; }
        else if (i == ')') { b += -1; }
        if (b < 0) {
            std::cout << "Check the brackets" << std::endl;
            return false;
        }
    }
    if (b != 0) {
        std::cout << "Check the brackets" << std::endl;
        return false;
    }
    return true;
}

void polskaytodouble(std::vector<std::string> polskay, double &doubl) {
    std::stack<double> stack;
    for (std::string str : polskay) {
        if (isdigit(str[0])) {

            stack.push(std::stod(str));

        } else if (str == "pi") {
            stack.push(M_PI);
        } else if (str == "e") {
            stack.push(M_E);
        } else if (isalpha(str[0])) {
            if (stack.empty()) {
                std::cout << "check chisla" << std::endl;
                doubl = 0;
                return;
            }
            if (str == "sin") {
                stack.top() = sin(stack.top());
            } else if (str == "tan") {
                stack.top() = tan(stack.top());
            } else if (str == "ctg") {
                stack.top() = 1 / tan(stack.top());
            } else if (str == "cos") {
                stack.top() = cos(stack.top());
            } else if (str == "sqrt") {
                stack.top() = sqrt(stack.top());
            } else {
                std::cout << "check expression" << std::endl;
            }
        } else if (str == "_") {
            if (stack.empty()) {
                std::cout << "check chisla" << std::endl;
                doubl = 0;
                return;
            }
            stack.top() = stack.top() * (-1);
        } else if (str == "?") {
            if (stack.empty()) {
                std::cout << "check chisla" << std::endl;
                doubl = 0;
                return;
            }
            stack.top() = stack.top() * (1);
        } else if (ispunct(str[0])) {
            if (stack.size() <= 1) {
                std::cout << "Check the arguments" << std::endl;
                return;
            }
            double i = 0;
            switch (str[0]) {
                case '+':
                    i = stack.top();
                    stack.pop();
                    stack.top() = stack.top() + i;

                    break;
                case '-':
                    i = stack.top();
                    stack.pop();
                    stack.top() = stack.top() - i;
                    break;
                case '*':
                    i = stack.top();
                    stack.pop();
                    stack.top() = stack.top() * i;
                    break;
                case '/':
                    i = stack.top();
                    stack.pop();
                    stack.top() = stack.top() / i;
                    break;
                case '^':
                    i = stack.top();
                    stack.pop();
                    stack.top() = pow(stack.top(), i);
                    break;
                default:
                    std::cout << "Check operation" << std::endl;
                    return;
            }
        }
    }
    if (!stack.empty()) {
        doubl = stack.top();
    }
}


int main() {

    std::string expression;
    std::getline(std::cin, expression);
    if (testforexpression(expression)) {
        std::vector<std::pair<int, std::string>> exp;
        std::vector<std::string> str;
        while (expression.find("--") != std::string::npos || expression.find("-+") != std::string::npos ||
               expression.find("+-") != std::string::npos || expression.find("++") != std::string::npos) {
            while (expression.find("--") != std::string::npos) {
                expression.replace(expression.find("--"), 2, "+");
            }
            while (expression.find("-+") != std::string::npos) {
                expression.replace(expression.find("-+"), 2, "-");
            }
            while (expression.find("+-") != std::string::npos) {
                expression.replace(expression.find("+-"), 2, "-");
            }
            while (expression.find("++") != std::string::npos) {
                expression.replace(expression.find("++"), 2, "+");
            }
        }
        while (expression.find(' ') != std::string::npos) {
            expression.erase(expression.find(' '), 1);
        }

        expressiontovector(expression, exp);
        if (!exp.empty()) {
            for (unsigned int i = 0; i < exp.size() - 1; i++) {
                if ((exp[i].first == 1 && exp[i + 1].first == 1) || (exp[i].first == 1 && exp[i + 1].first == 3) ||
                    (exp[i].first == 5 && exp[i + 1].first == 1)) {
                    exp.insert(exp.begin() + i + 1, std::pair(2, "*"));
                }
            }
            std::cout << '\n';
            double i = 0;
            vectortopolskay(exp, str);
            for (std::string str : str) {
                std::cout << str << " ";
            }
            polskaytodouble(str, i);
            std::cout << '\n';
            std::cout << i << std::endl;
        }

    }
    return 0;
}