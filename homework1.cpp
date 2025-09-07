#include <stdexcept>
#include <cmath>

long double powIterative(double number, int power) {
    if (number == 0 && power <= 0) {
        throw std::invalid_argument("Indeterminate form: 0^0 or division by zero.");
    }

    bool isNegativePower = (power < 0);
    int absPower = std::abs(power);
    long double base = static_cast<long double>(number);
    long double result = 1.0;

    while (absPower > 0) {
        if (absPower % 2 == 1) {
            result *= base;
        }
        base *= base;
        absPower /= 2;
    }

    return isNegativePower ? 1.0 / result : result;
}

long double powRecursiveHelper(long double baseVal, int exponent) {
    if (exponent == 0) {
        return 1.0;
    }
    if (exponent == 1) {
        return baseVal;
    }

    long double half = powRecursiveHelper(baseVal * baseVal, exponent / 2);
    return (exponent % 2 == 0) ? half : baseVal * half;
}

long double powRecursive(double number, int power) {
    if (number == 0 && power <= 0) {
        throw std::invalid_argument("Indeterminate form: 0^0 or division by zero.");
    }

    bool isNegativePower = (power < 0);
    int absPower = std::abs(power);
    long double baseVal = static_cast<long double>(number);

    long double result = powRecursiveHelper(baseVal, absPower);

    return isNegativePower ? 1.0 / result : result;
}
