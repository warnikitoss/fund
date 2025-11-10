#include <assert.h>
#include <stdio.h>
#include "functions.h"

void test_check_brackets() {
    assert(check_brackets("") == 1);
    assert(check_brackets("()") == 1);
    assert(check_brackets("[]") == 1);
    assert(check_brackets("{}") == 1);
    assert(check_brackets("<>") == 1);
    assert(check_brackets("()[]{}<>") == 1);
    assert(check_brackets("({[<>]})") == 1);
    assert(check_brackets("((((()))))") == 1);
    assert(check_brackets("a(b[c{d<e>f}g]h)i") == 1);
    
    assert(check_brackets("(") == 0);
    assert(check_brackets(")") == 0);
    assert(check_brackets("())") == 0);
    assert(check_brackets("(()") == 0);
    assert(check_brackets("([)]") == 0);
    assert(check_brackets("({)}") == 0);
    assert(check_brackets("<{>}") == 0);
    assert(check_brackets("){") == 0);
    assert(check_brackets("(]") == 0);
    assert(check_brackets("([)])") == 0);
    
    assert(check_brackets("abc(def)ghi") == 1);
    assert(check_brackets("a(b)c[d]e{f}g<h>i") == 1);
    assert(check_brackets("no brackets here") == 1);
    
    printf("All check_brackets tests passed!\n");
}

int main() {
    test_check_brackets();
    printf("All unit tests passed!\n");
    return 0;
}