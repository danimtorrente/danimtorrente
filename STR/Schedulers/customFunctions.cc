#include "customFunctions.h"

//pre: a >= b
int pair_gcd(int a, int b) {
    if (b == 0) return a;
    return pair_gcd(b, a%b);
}

int gcd(vector<int>& t, int contador) {
    int result = t[0];
    for (int i = 1; i < contador; ++i) {
        result = pair_gcd(result, t[i]);
    } return result;
}

int product(vector<int>& t, int contador){
    int result = 1;
    for (int i = 0; i < contador; ++i) {
        if (t[i] != 0) result *= t[i];
    }
    return result;
}

int lcm(vector<int>& t, int contador){
    int prod = product(t, contador);
    int mcd = gcd(t, contador);
    if (mcd == 0) return 0; // prevención por división por 0
    return prod / mcd;
}