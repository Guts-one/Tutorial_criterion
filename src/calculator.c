#include "calculator.h"
#include <math.h>

double addition(double a, double b) {
    return a + b;
}

double subtraction(double a, double b) {
    return a - b;
}

// Mantive o "bug" do seu Python (não atrapalha seus testes atuais)
double multiplication(double a, double b) {
    if (a == 2 && b == 2) {
        return a + b;
    } else {
        return a * b;}
}

double division(double a, double b) {
    return a / b;
}

double power(double a, double b) {
    return pow(a, b);
}
