#include <stdio.h>

int main() {
    char buffer[200];
    
    // Спецификатор %c - одиночный символ
    char ch = 'A';
    sprintf(buffer, "Character: %c", ch);
    printf("%s\n", buffer); // Output: "Character: A"

    // Спецификатор %d - десятичное целое число со знаком
    int num1 = 123;
    sprintf(buffer, "Decimal: %d", num1);
    printf("%s\n", buffer); // Output: "Decimal: 123"

    // Спецификатор %f - число с плавающей точкой
    double num2 = 123.456;
    sprintf(buffer, "Float: %f", num2);
    printf("%s\n", buffer); // Output: "Float: 123.456000"

    // Спецификатор %s - строка
    char str[] = "Hello";
    sprintf(buffer, "String: %s", str);
    printf("%s\n", buffer); // Output: "String: Hello"

    // Спецификатор %u - десятичное целое число без знака
    unsigned int num3 = 123;
    sprintf(buffer, "Unsigned: %u", num3);
    printf("%s\n", buffer); // Output: "Unsigned: 123"

    // Спецификатор %% - вывод знака %
    sprintf(buffer, "Percent: %%");
    printf("%s\n", buffer); // Output: "Percent: %"

    // Флаг - (минус) - выравнивание по левому краю
    sprintf(buffer, "Left aligned: %-10d end", num1);
    printf("%s\n", buffer); // Output: "Left aligned: 123        end"

    // Флаг + - вывод знака всегда
    sprintf(buffer, "Always sign: %+d", num1);
    printf("%s\n", buffer); // Output: "Always sign: +123"

    // Флаг пробел - вставка пробела перед положительным числом
    sprintf(buffer, "Space: % d", num1);
    printf("%s\n", buffer); // Output: "Space:  123"

    // Ширина (число) - минимальная ширина поля
    sprintf(buffer, "Width: %10d", num1);
    printf("%s\n", buffer); // Output: "Width:        123"

    // Точность .(число) - количество цифр после запятой для чисел с плавающей точкой
    sprintf(buffer, "Precision: %.2f", num2);
    printf("%s\n", buffer); // Output: "Precision: 123.46"

    // Длина h - короткий модификатор для целых чисел
    short int num4 = 123;
    sprintf(buffer, "Short: %hd", num4);
    printf("%s\n", buffer); // Output: "Short: 123"

    // Длина l - длинный модификатор для целых чисел
    long int num5 = 123456789L;
    sprintf(buffer, "Long: %ld", num5);
    printf("%s\n", buffer); // Output: "Long: 123456789"

 
//      Спецификаторы:
//         %c: Символ.
//         %d: Десятичное целое число со знаком.
//         %f: Число с плавающей точкой.
//         %s: Строка.
//         %u: Десятичное целое число без знака.
//         %%: Процентный знак.

//     Флаги:
//         -: Выравнивание по левому краю.
//         +: Вывод знака всегда (для положительных чисел добавляется +).
//         (пробел): Вставка пробела перед положительным числом.

//     Ширина:
//         (число): Минимальная ширина поля.

//     Точность:
//         .(число): Количество цифр после запятой для чисел с плавающей точкой.

//     Длина:
//         h: Короткий модификатор для целых чисел.
//         l: Длинный модификатор для целых чисел.

// Этот код демонстрирует использование различных спецификаторов, флагов, ширины, точности и длины в функции sprintf для форматирования строк.



    // Спецификатор %g и %G - число с плавающей точкой (научное или обычное представление)
    double num1 = 12345.6789;
    sprintf(buffer, "%%g: %g", num1);
    printf("%s\n", buffer); // Output: "%g: 12345.7"
    sprintf(buffer, "%%G: %G", num1);
    printf("%s\n", buffer); // Output: "%G: 12345.7"

    // Спецификатор %e и %E - число с плавающей точкой в научной нотации
    double num2 = 12345.6789;
    sprintf(buffer, "%%e: %e", num2);
    printf("%s\n", buffer); // Output: "%e: 1.234568e+04"
    sprintf(buffer, "%%E: %E", num2);
    printf("%s\n", buffer); // Output: "%E: 1.234568E+04"

    // Спецификатор %x и %X - шестнадцатеричное целое число
    int num3 = 255;
    sprintf(buffer, "%%x: %x", num3);
    printf("%s\n", buffer); // Output: "%x: ff"
    sprintf(buffer, "%%X: %X", num3);
    printf("%s\n", buffer); // Output: "%X: FF"

    // Спецификатор %o - восьмеричное целое число
    int num4 = 255;
    sprintf(buffer, "%%o: %o", num4);
    printf("%s\n", buffer); // Output: "%o: 377"

    // Спецификатор %p - указатель
    int num5 = 255;
    int *ptr = &num5;
    sprintf(buffer, "Pointer: %p", ptr);
    printf("%s\n", buffer); // Output: "Pointer: 0x..."

    // Флаг # - спецификатор альтернативной формы
    sprintf(buffer, "%%#x: %#x", num3);
    printf("%s\n", buffer); // Output: "%#x: 0xff"
    double pi = 3.14159;
    sprintf(buffer, "%%#g: %#g", pi);
    printf("%s\n", buffer); // Output: "%#g: 3.14159"

    // Флаг 0 - заполнение нулями
    int num6 = 42;
    sprintf(buffer, "Number: %05d", num6);
    printf("%s\n", buffer); // Output: "Number: 00042"

    // Ширина * - ширина, указанная аргументом
    int width = 10;
    sprintf(buffer, "Number: %*d", width, num6);
    printf("%s\n", buffer); // Output: "Number:         42"

    // Точность .* - точность, указанная аргументом
    int precision = 2;
    sprintf(buffer, "Pi: %.*f", precision, pi);
    printf("%s\n", buffer); // Output: "Pi: 3.14"

    // Длина L - длинный модификатор для чисел с плавающей точкой
    long double num7 = 12345.6789L;
    sprintf(buffer, "Long double: %Le", num7);
    printf("%s\n", buffer); // Output: "Long double: 1.234568e+04"

    return 0;
//     Спецификаторы:

//     %g и %G: Число с плавающей точкой в научной или обычной нотации, в зависимости от величины числа.
//     %e и %E: Число с плавающей точкой в научной нотации.
//     %x и %X: Шестнадцатеричное представление целого числа.
//     %o: Восьмеричное представление целого числа.
//     %p: Указатель.

// Флаги:

//     #: Альтернативная форма представления (например, для шестнадцатеричных чисел добавляет 0x или 0X префикс).
//     0: Заполнение нулями вместо пробелов.

// Ширина:

//     *: Ширина поля задается дополнительным аргументом.

// Точность:

//     .*: Точность задается дополнительным аргументом.

// Длина:

//     L: Длинный модификатор для чисел с плавающей точкой, используется для long double.
// }


}


