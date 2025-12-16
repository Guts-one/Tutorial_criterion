#include <criterion/criterion.h>
#include <criterion/new/assert.h>  //para a nova API
#include <criterion/parameterized.h> //para parametrizacao
#include <stdlib.h>   // getenv
#include <math.h>     // fabs
#include "calculator.h"


// sanityOnly controlado por variável de ambiente.
static int sanityOnly = 0;

static void load_flags(void) {
    const char *v = getenv("SANITY_ONLY");
    sanityOnly = (v && v[0] == '1');
}

TestSuite(Calculator, .init = load_flags);  //fixture parasanityOnly


Test(Calculator, test_addition2) {
    cr_skip("Duplicado!");
    cr_assert(ieee_ulp_eq(dbl, addition(2.00001, 3), 5, 4)); // cr_assert(Criterion, Format, ...) nesse caso Criterion=ieee_ulp_eq(dbl, addition(2.00001, 3), 5, 4)
}

Test(Calculator, test_subtraction_near_zero){
    cr_assert(epsilon_eq(dbl, subtraction(1, 1), 0.0, 16*DBL_EPSILON)); //IEEE 754 valor perto de zero e com Epsilon=16*DBL_EPSILON: epsilon_eq(Tag, Actual, Expected, Epsilon) 
}

Test(Calculator, test_multiplication){
    cr_assert(ieee_ulp_eq(dbl, multiplication(3, 5), 15, 4)); //IEEE 754 valor longe de zero e com o paramentro Ulp=4: ieee_ulp_eq(Tag, Actual, Expected, Ulp) 
}

Test(Calculator, test_power){
    cr_assert(ieee_ulp_eq(dbl, power(2, 3), 8, 4)); //IEEE 754
}

Test(Calculator, test_division_exact){
    cr_assert(ieee_ulp_eq(dbl, division(10, 2), 5, 4));
}

Test(Calculator, test_division_byzero){
    cr_assert(isinf(division(1, 0)));
}

Test(Calculator, test_division_indet){
    cr_assert(isnan(division(0, 0)));
}

/*Test para caso com parametrização1*/

// Defina a struct para os parâmetros
typedef struct {
    double a;
    double b;
    double expected;
} mul_params;

ParameterizedTestParameters(Calculator, test_multipli_paramet) {
    static mul_params params[] = {
        {2.0, 3.0, 6.0},       // Caso simples
        {0.5, 4.0, 2.0},       // Com decimal
        {-1.0, 5.0, -5.0},     // Com negativo
        {0.0, 10.0, 0.0},      // Multiplicação por zero
        {1.00001, 1.0, 1.00001} // Caso com precisão flutuante
    };
    size_t nb_params = sizeof(params) / sizeof(mul_params);
    return cr_make_param_array(mul_params, params, nb_params);
}

ParameterizedTest(mul_params *param, Calculator, test_multipli_paramet) {
       if (sanityOnly) {
        cr_skip("SANITY_ONLY=1: pulando teste parametrizado de divisão.");
    }
    cr_assert(ieee_ulp_eq(dbl, multiplication(param->a, param->b), param->expected, 4));
}
