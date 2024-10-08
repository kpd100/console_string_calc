#define CATCH_CONFIG_RUNNER

#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <cctype>
#include "catch.hpp"

using namespace std;

// Функция, которая возвращает приоритет операторов
int getPrecedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

// Функция для выполнения математической операции
double applyOperation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b != 0) return a / b;
            cerr << "Error: Division by zero!" << endl;
            return 0;
        default: return 0;
    }
}

// Функция для вычисления выражения в постфиксной записи
double evaluatePostfix(const string& postfix) {
    stack<double> values;
    stringstream ss(postfix);
    string token;

    while (ss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            values.push(stod(token)); // Преобразуем строку в число
        } else {
            // Это оператор
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();
            values.push(applyOperation(a, b, token[0]));
        }
    }

    return values.top();
}

// Функция для преобразования инфиксного выражения в постфиксное
string infixToPostfix(const string& infix) {
    stack<char> ops;
    stringstream output;
    for (size_t i = 0; i < infix.length(); ++i) {
        char token = infix[i];

        // Пропускаем пробелы
        if (isspace(token))
            continue;

        // Если число, считываем полное число (включая возможную десятичную точку)
        if (isdigit(token) || token == '.') {
            string number;
            while (i < infix.length() && (isdigit(infix[i]) || infix[i] == '.')) {
                number += infix[i++];
            }
            --i; // возвращаемся на один символ назад
            output << number << " ";
        } else if (token == '(') {
            ops.push('(');
        } else if (token == ')') {
            // Выгружаем все до открывающей скобки
            while (!ops.empty() && ops.top() != '(') {
                output << ops.top() << " ";
                ops.pop();
            }
            ops.pop(); // Убираем '('
        } else {
            // Это оператор, нужно проверить приоритет
            while (!ops.empty() && getPrecedence(ops.top()) >= getPrecedence(token)) {
                output << ops.top() << " ";
                ops.pop();
            }
            ops.push(token);
        }
    }

    // Выгружаем оставшиеся операторы
    while (!ops.empty()) {
        output << ops.top() << " ";
        ops.pop();
    }

    return output.str();
}

// Тесты для функции инфикс->постфикс
    TEST_CASE("Infix to Postfix Conversion", "[infixToPostfix]") {

        REQUIRE(infixToPostfix("3 + 4") == "3 4 + ");
        REQUIRE(infixToPostfix("(1 + 2) * 3") == "1 2 + 3 * ");
        REQUIRE(infixToPostfix("3 + 4 * 2 / ( 1 - 5 )") == "3 4 2 * 1 5 - / + ");
        REQUIRE(infixToPostfix("10 + ( 6 - 4 ) * 2") == "10 6 4 - 2 * + ");
    }

// Тесты для функции вычисления постфиксного выражения
    TEST_CASE("Postfix Evaluation", "[evaluatePostfix]") {
        REQUIRE(evaluatePostfix("3 4 + ") == 7.0);
        REQUIRE(evaluatePostfix("1 2 + 3 * ") == 9.0);
        REQUIRE(evaluatePostfix("3 4 2 * 1 5 - / + ") == Catch::Approx(1).epsilon(0.001));
        REQUIRE(evaluatePostfix("10 6 4 - 2 * + ") == 14.0);
    }

// Тесты для отдельных операций
    TEST_CASE("Basic Operations", "[applyOperation]") {
        REQUIRE(applyOperation(5, 3, '+') == 8.0);
        REQUIRE(applyOperation(5, 3, '-') == 2.0);
        REQUIRE(applyOperation(5, 3, '*') == 15.0);
        REQUIRE(applyOperation(6, 3, '/') == 2.0);
    }

// Тест деления на 0
    TEST_CASE("Division by Zero", "[division]") {
        REQUIRE(applyOperation(5, 0, '/') == 0); // Должно вернуть 0 и вывести ошибку
    }

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "simple_text_calc") {
        argv[1]=""; //отключаем фильтры catch 2
        // Если передан аргумент "simple_text_calc", запускаем тесты
        Catch::Session session;
        return session.run(argc, argv);
    }
    string input;
    cout << "Advanced Text Calculator" << endl;
    cout << "Enter expressions with operators (+, -, *, /) and parentheses (). Type 'exit' to quit." << endl;

    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input == "exit") {
            break;
        }

        try {
            string postfix = infixToPostfix(input);
            double result = evaluatePostfix(postfix);
            cout << "Result: " << result << endl;
        } catch (const exception& e) {
            cerr << "Error: Invalid expression!" << endl;
        }
    }

    return 0;
}
