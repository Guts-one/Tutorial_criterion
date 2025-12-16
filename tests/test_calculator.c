#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdlib.h>   // getenv
#include <math.h>     // fabs
#include "calculator.h"

// equivalente ao "sanityOnly = False" do pytest,
// mas controlado por variável de ambiente.
static int sanityOnly = 0;

static void load_flags(void) {
    const char *v = getenv("SANITY_ONLY");
    sanityOnly = (v && v[0] == '1');
}

TestSuite(Calculator, .init = load_flags);

static void assert_double_eq(double got, double expected) {
    cr_assert(fabs(got - expected) < 1e-12,
              "Expected %.12f but got %.12f", expected, got);
}

Test(Calculator, test_addition1) {
    assert_double_eq(addition(2, 2), 4);
}

Test(Calculator, test_addition2) {
    assert_double_eq(addition(4, 4), 8);
}

Test(Calculator, test_addition3) {
    cr_skip_test("Duplicado!");
    assert_double_eq(addition(2, 2), 4);
}

Test(Calculator, test_subtraction) {
    if (sanityOnly) cr_skip_test("Running only sanity!");
    assert_double_eq(subtraction(10, 4), 6);
}

Test(Calculator, test_multiplication1) {
    cr_skip_test("Duplicado!");
    assert_double_eq(multiplication(2, 3), 6);
}

Test(Calculator, test_multiplication2) {
    assert_double_eq(multiplication(2, 2), 4);
}

Test(Calculator, test_power) {
    assert_double_eq(power(2, 3), 8);
}

/*Test para caso com parametrização1*/

// ParameterizedTestParameters(Calculator, test_multipli_paramet) {
//     void *params;
//     size_t nb_params;

//     // generate parameter set
//     return cr_make_param_array(double, params, nb_params);
// }

// ParameterizedTest(double *param, Calculator, test_multipli_paramet) {
//     cr_assert(ieee_ulp_eq(dbl, multiplication(, ), , 4));
// }


// Test(Calculator, test_division) {
//     if (sanityOnly) cr_skip_test("Running only sanity!");
//     double out = 0;
//     cr_assert(eq(int, division(10, 5, &out), 0));
//     assert_double_eq(out, 2);
// }

// Test(Calculator, test_division_by_zero) {
//     double out = 123; // qualquer coisa
//     cr_assert(ne(int, division(10, 0, &out), 0),
//               "Expected division by zero to fail");
// }
