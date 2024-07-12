#include "s21_sscanf.h"

static int includes_exponent(const char *str);
static long double string_to_float(const char *str);
static long double apply_exponent(long double num, const char *str);
static int case_insensitive_search(const char *str, const char *pattern);
static int includes_inf_nan(const char *str);
static long double return_nan_inf(const char *str);

long double s21_strtold(const char *str) {
    long double result = 0.0;
    int has_inf_nan = includes_inf_nan(str);

    if (!has_inf_nan) {
        result = string_to_float(str);

        if (includes_exponent(str)) {
            result = apply_exponent(result, str);
        }
    }

    return (has_inf_nan) ? return_nan_inf(str) : result;
}

static int case_insensitive_search(const char *str, const char *pattern) {
    int found = 0;
    int len = (int)s21_strlen(pattern);

    for (int i = 0; str[i] && !found; i++) {
        int match_count = 0;
        for (int j = 0; j < len; j++) {
            if ((str[i] == (pattern[j] - 'A') + 'a') ||
                (str[i] == (pattern[j] - 'a') + 'A') ||
                pattern[j] == str[i]) {
                match_count++;
                i++;
            }

            if (len == match_count) {
                found = 1;
                break;
            }
        }
    }

    return found;
}

static int includes_inf_nan(const char *str) {
    int result = 0;

    int test_inf = case_insensitive_search(str, "inf");
    int test_nan = case_insensitive_search(str, "nan");

    if (test_inf || test_nan) {
        result = 1;
    }

    return result;
}

static long double return_nan_inf(const char *str) {
    int result = 0;

    for (int i = 0; str[i]; i++) {
        if (str[i] == 'i' || str[i] == 'I') {
            result = 1;
            break;
        }

        if (str[i] == 'n' || str[i] == 'N') {
            result = 2;
            break;
        }
    }

    return (result == 1) ? INFINITY : NAN;
}

static long double apply_exponent(long double num, const char *str) {
    char sign = '+';
    int exponent = 0;

    for (char *p = (char *)str; *p; p++) {
        if (*p == 'e' || *p == 'E') {
            sign = *(p + 1);
            exponent = s21_atoi(p + 2);
        }
    }

    while (exponent) {
        if (sign == '-') {
            num /= 10.0;
        } else {
            num *= 10.0;
        }
        exponent--;
    }

    return num;
}

static int includes_exponent(const char *str) {
    int result = 0;

    for (char *p = (char *)str; *p; p++) {
        if (s21_strspn(p, "eE")) {
            result = 1;
            break;
        }
    }

    return result;
}

static long double string_to_float(const char *str) {
    long double fraction = 0.0;
    char *p = (char *)str;
    int is_negative = (*p == '-');
    if (*p == '-' || *p == '+')
        p++;

    long double result = s21_atoi(p);

    while (isdigit(*p))
        p++;

    if (*p == '.') {
        p++;
        int leading_zeros = s21_strspn(p, "0");
        fraction = s21_atoi(p);
        int temp = (int)fraction;
        while (temp) {
            fraction /= 10.0;
            temp /= 10;
        }
        while (leading_zeros) {
            fraction /= 10.0;
            leading_zeros--;
        }
    }

    result += fraction;

    return is_negative ? -result : result;
}
