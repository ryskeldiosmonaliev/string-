#ifndef SRC_HEADERS_S21_SPRINTF_H_
#define SRC_HEADERS_S21_SPRINTF_H_

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

struct Flags {
  int plus;          // Флаг '+'
  int minus;         // Флаг '-'
  int spase;         // Флаг пробела
  int sharp;         // Флаг '#'
  int zero;          // Флаг '0'
};

struct PreparedFormatData {
  struct Flags flags;     // Флаги форматирования
  int counter;            // Счетчик символов
  int width;              // Ширина поля
  int precision;          // Точность вывода
  int has_precision;      // Флаг наличия точности
  char length;            // Длина модификатора
  char specifier;         // Спецификатор формата
};

int s21_sprintf(char *str, const char *format, ...);
/* Форматирует строку в соответствии с указанным форматом и аргументами. */

const char *sprintf_parser(const char *format,
                           struct PreparedFormatData *prepared_format_data,
                           int *error, va_list va);
/* Парсит строку формата, заполняя структуру PreparedFormatData и проверяя на ошибки. */

const char *get_flags(const char *format,
                      struct PreparedFormatData *prepared_format_data);
/* Извлекает флаги из строки формата и сохраняет их в структуру PreparedFormatData. */

const char *get_width(const char *format,
                      struct PreparedFormatData *prepared_format_data,
                      va_list va);
/* Извлекает ширину поля из строки формата и сохраняет её в структуру PreparedFormatData. */


//.*  или %.5d
const char *get_precision(const char *format,
                          struct PreparedFormatData *prepared_format_data,
                          va_list va);
/* Извлекает точность вывода из строки формата и сохраняет её в структуру PreparedFormatData. */


// h l L
const char *get_length(const char *format,
                       struct PreparedFormatData *prepared_format_data);
/* Извлекает модификатор длины из строки формата и сохраняет его в структуру PreparedFormatData. */


// проверяет есть ли такие символы cdieEfgGosuxXpn и %
const char *get_specifier(const char *format,
                          struct PreparedFormatData *prepared_format_data,
                          int *error);
/* Извлекает спецификатор формата из строки формата и сохраняет его в структуру PreparedFormatData. */



int write_to_str(char *str, const char *format);
/* Записывает форматированную строку в буфер str. */

int write_space(char *str, struct PreparedFormatData *prepared_format_data);
/* Записывает пробелы в буфер str в соответствии с флагами форматирования. */

int write_sign(char *str, long double value,
               struct PreparedFormatData *prepared_format_data);
/* Записывает знак числа в буфер str в соответствии с флагами форматирования. */

int write_zero(char *str, struct PreparedFormatData *prepared_format_data);
/* Записывает нули в буфер str в соответствии с флагами форматирования. */

int write_int(char *str, struct PreparedFormatData *prepared_format_data,
              long long value);
/* Записывает целое число в буфер str в соответствии с флагами форматирования. */

int write_dot(char **value_str,
              struct PreparedFormatData *prepared_format_data);
/* Записывает точку в строку значений в соответствии с флагами форматирования. */

int write_float(char *str, struct PreparedFormatData *prepared_format_data,
                long double value);
/* Записывает число с плавающей точкой в буфер str в соответствии с флагами форматирования. */

int write_double(char *value_str, long double value,
                 struct PreparedFormatData *prepared_format_data);
/* Записывает двойное число в строку значений в соответствии с флагами форматирования. */

int write_zeroes(char *str, struct PreparedFormatData *prepared_format_data);
/* Записывает нули в буфер str в соответствии с флагами форматирования. */

int write_unsigned(char *str, struct PreparedFormatData *prepared_format_data,
                   unsigned long long value);
/* Записывает беззнаковое целое число в буфер str в соответствии с флагами форматирования. */

int write_prefix_spaces(char **str,
                        struct PreparedFormatData *prepared_format_data,
                        unsigned long long value);
/* Записывает префиксные пробелы в строку в соответствии с флагами форматирования. */

int write_e(char *str, struct PreparedFormatData *prepared_format_data,
            long double value);
/* Записывает число в экспоненциальном формате в буфер str в соответствии с флагами форматирования. */

int write_g(char *str, struct PreparedFormatData *prepared_format_data,
            long double value);
/* Записывает число в общем формате в буфер str в соответствии с флагами форматирования. */

int write_exponent(char *value_str,
                   struct PreparedFormatData *prepared_format_data, int power);
/* Записывает экспоненту числа в строку значений в соответствии с флагами форматирования. */

int decrease_sign_width(long double value,
                        struct PreparedFormatData *prepared_format_data);
/* Уменьшает ширину знака числа в соответствии с флагами форматирования. */

int decrease_zeroes_width(struct PreparedFormatData *prepared_format_data);
/* Уменьшает количество нулей в соответствии с флагами форматирования. */

int specifier_handler(char **str,
                      struct PreparedFormatData *prepared_format_data,
                      va_list va, int processed);
/* Обрабатывает спецификатор формата и записывает соответствующие значения в буфер str. */

int char_handler(char **str, va_list va,
                 struct PreparedFormatData *prepared_format_data);
/* Обрабатывает спецификатор формата для символов и записывает значение в буфер str. */

int string_handler(char *str, va_list va,
                   struct PreparedFormatData *prepared_format_data);
/* Обрабатывает спецификатор формата для строк и записывает значение в буфер str. */

int unsigned_handler(char *str, struct PreparedFormatData *prepared_format_data,
                     va_list va);
/* Обрабатывает спецификатор формата для беззнаковых целых чисел и записывает значение в буфер str. */

int double_handler(char *str, struct PreparedFormatData *prepared_format_data,
                   va_list va);
/* Обрабатывает спецификатор формата для чисел с плавающей точкой и записывает значение в буфер str. */

int integer_handler(char *str, struct PreparedFormatData *prepared_format_data,
                    va_list va);
/* Обрабатывает спецификатор формата для целых чисел и записывает значение в буфер str. */

int n_handler(va_list va, int processed);
/* Обрабатывает спецификатор формата для записи количества обработанных символов в переменную. */

int x_handler(char *str, struct PreparedFormatData *prepared_format_data,
              unsigned long value);
/* Обрабатывает спецификатор формата для шестнадцатеричных чисел и записывает значение в буфер str. */

int o_handler(char *str, struct PreparedFormatData *prepared_format_data,
              unsigned long long value);
/* Обрабатывает спецификатор формата для восьмеричных чисел и записывает значение в буфер str. */

int pointer_handler(char *str, struct PreparedFormatData *prepared_format_data,
                    va_list va);
/* Обрабатывает спецификатор формата для указателей и записывает значение в буфер str. */

int minus_handler(char *str, char *value_str,
                  struct PreparedFormatData *prepared_format_data,
                  long double value);
/* Обрабатывает флаг '-' для числа и записывает значение в буфер str. */

int remove_zero(char *str, struct PreparedFormatData *prepared_format_data);
/* Удаляет нули из буфера str в соответствии с флагами форматирования. */

int remove_zeroes_in_g(char *value_str,
                       struct PreparedFormatData *prepared_format_data);
/* Удаляет нули из строки значений в общем формате в соответствии с флагами форматирования. */

int integer_length(long long int value);
/* Возвращает длину целого числа. */

int double_length(long double value);
/* Возвращает длину числа с плавающей точкой. */

int unsigned_length(unsigned long long value);
/* Возвращает длину беззнакового целого числа. */

int x_to_upper(char *value_str);
/* Переводит шестнадцатеричное число в верхний регистр. */

int is_NaN_or_INF(char *value_str,
                  struct PreparedFormatData *prepared_format_data,
                  long double value);
/* Проверяет, является ли значение NaN или INF, и записывает соответствующее представление в строку значений. */

int decimal_to_hexoct(char *value_str, unsigned long long value,
                      struct PreparedFormatData *prepared_format_data,
                      int base);
/* Конвертирует десятичное число в шестнадцатеричное или восьмеричное и записывает его в строку значений. */

int itoa(char *value_str, long long int value,
         struct PreparedFormatData *prepared_format_data);
/* Конвертирует целое число в строку и записывает его в строку значений. */

int ftoa(char *value_str, struct PreparedFormatData *prepared_format_data,
         long double value);
/* Конвертирует число с плавающей точкой в строку и записывает его в строку значений. */

int utoa(char *value_str, unsigned long long value,
         struct PreparedFormatData *prepared_format_data);
/* Конвертирует беззнаковое целое число в строку и записывает его в строку значений. */

int s21_atoi(char *str);
/* Конвертирует строку в целое число. */

long double mantissa(char **valueStr, long double exponent,
                     long long *precision,
                     struct PreparedFormatData *prepared_format_data);
/* Возвращает мантиссу числа с плавающей точкой. */


#endif  //  SRC_HEADERS_S21_SPRINTF_H_

