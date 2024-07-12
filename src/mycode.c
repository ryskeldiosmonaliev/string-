#include "s21_string.h"

/**
 * Функция для форматированной записи строки в буфер.
 *
 * @param str Указатель на буфер, в который будет записан результат.
 * @param format Строка формата, определяющая, какие данные будут записаны в
 * буфер.
 * @param ... Переменное число аргументов, которые будут использованы для замены
 * в строке формата.
 * @return Количество символов, записанных в буфер (включая '\0').
 */

int s21_sprintf(char *str, const char *format, ...) {
  va_list va;  // переменная для работы с вариативными аргументами
  va_start(va, format);  // начало работы с вариативными аргументами
  int error = 1;      // флаг для отслеживания ошибок
  int processed = 0;  // счетчик обработанных символов
  struct PreparedFormatData prepared_format_data = {
      0};  // структура для подготовленных данных формата
  str[0] = '\0';  // инициализация вывода пустой строкой
  while (*format) {  // цикл по строке формата
    if (error) {     // если нет ошибок
      s21_memset(
          &prepared_format_data, 0,
          sizeof(prepared_format_data));  // сброс подготовленных данных формата
      if (*format == '%') {  // если текущий символ - спецификатор формата
        format++;  // переход к следующему символу
        format = sprintf_parser(format, &prepared_format_data, &error,
                                va);  // разбор спецификатора формата
        specifier_handler(&str, &prepared_format_data, va,
                          processed);  // обработка спецификатора
        processed += prepared_format_data
                         .counter;  // обновление счетчика обработанных символов
      } else {
        write_to_str(str, format);  // запись обычного символа в вывод
        processed++;  // увеличение счетчика обработанных символов
        format++;     // переход к следующему символу
      }
    } else {
      printf("error");  // вывод сообщения об ошибке
      *str = '\0';      // сброс вывода в пустую строку
      break;            // выход из цикла
    }
  }
  va_end(va);  // завершение работы с вариативными аргументами
  return processed;  // возврат количества обработанных символов
}

/**
 * @brief Разбирает форматную строку и заполняет структуру с данными формата.
 *
 * @param format Указатель на текущую позицию в форматной строке.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, куда
 * будут записаны данные формата.
 * @param error Указатель на флаг ошибки. Устанавливается в случае ошибки
 * разбора.
 * @param va Список вариативных аргументов для разбора спецификаторов формата.
 * @return const char* Указатель на текущую позицию в форматной строке после
 * разбора всех спецификаторов.
 */
const char *sprintf_parser(const char *format,
                           struct PreparedFormatData *prepared_format_data,
                           int *error, va_list va) {
  format = get_flags(
      format, prepared_format_data);  // Получение и обработка флагов формата
  format =
      get_width(format, prepared_format_data, va);  // Получение ширины поля
  format =
      get_precision(format, prepared_format_data, va);  // Получение точности
  format =
      get_length(format, prepared_format_data);  // Получение размера данных
  format = get_specifier(format, prepared_format_data,
                         error);  // Получение спецификатора формата

  return format;  // Возвращаем указатель на текущую позицию в форматной строке
}
/**
 * @brief Извлекает и обрабатывает флаги формата из начала форматной строки.
 *
 * @param format Указатель на текущую позицию в форматной строке.
 * @param prepared_format_data Указатель на структуру PreparedFormatData для
 * сохранения флагов.
 * @return const char* Указатель на позицию в форматной строке после обработки
 * флагов.
 */
const char *get_flags(const char *format,
                      struct PreparedFormatData *prepared_format_data) {
  while (*format == '-' || *format == '+' || *format == ' ' || *format == '#' ||
         *format == '0') {
    switch (*format) {
      case '-':
        prepared_format_data->flags.minus++;  // Увеличиваем счетчик флага минус
        format++;  // Переходим к следующему символу в форматной строке
        break;
      case '+':
        prepared_format_data->flags.plus++;  // Увеличиваем счетчик флага плюс
        format++;  // Переходим к следующему символу в форматной строке
        break;
      case ' ':
        prepared_format_data->flags
            .spase++;  // Увеличиваем счетчик флага пробел
        format++;  // Переходим к следующему символу в форматной строке
        break;
      case '#':
        prepared_format_data->flags
            .sharp++;  // Увеличиваем счетчик флага решетка
        format++;  // Переходим к следующему символу в форматной строке
        break;
      case '0':
        prepared_format_data->flags.zero++;  // Увеличиваем счетчик флага ноль
        format++;  // Переходим к следующему символу в форматной строке
        break;
    }
  }
  return format;  // Возвращаем указатель на текущую позицию в форматной строке
                  // после обработки флагов
}

const char *get_width(const char *format,
                      struct PreparedFormatData *prepared_format_data,
                      va_list va) {
  // Проверяем, является ли текущий символ '*'
  if (*format == '*') {
    // Если да, получаем ширину из списка аргументов
    prepared_format_data->width = va_arg(va, int);
    // Переходим к следующему символу в строке формата
    format++;
  } else {
    // Если текущий символ - цифра, считываем ширину из строки формата
    char widthBuff[1024] = "";  // Буфер для хранения числового значения ширины
    int k = 0;  // Индекс для записи в буфер ширины
    // Читаем символы, пока они являются цифрами
    while (*format >= '0' && *format <= '9') {
      widthBuff[k] = *format;  // Сохраняем текущий символ в буфер
      k++;  // Увеличиваем индекс буфера
      format++;  // Переходим к следующему символу в строке формата
    }
    // Преобразуем строку из буфера в целое число и сохраняем в prepared_format_data->width
    prepared_format_data->width = s21_atoi(widthBuff);
  }
  // Возвращаем текущую позицию в строке формата
  return format;
}


const char *get_precision(const char *format,
                          struct PreparedFormatData *prepared_format_data,
                          va_list va) {
  // Проверяем, начинается ли формат с символа '.'
  if (*format == '.') {
    // Устанавливаем флаг наличия точности
    prepared_format_data->has_precision = 1;
    // Переходим к следующему символу в строке формата
    format++;
    // Проверяем, является ли текущий символ '*'
    if (*format == '*') {
      // Если да, получаем значение точности из списка аргументов
      prepared_format_data->precision = va_arg(va, int);
      // Переходим к следующему символу в строке формата
      format++;
    } else {
      // Если текущий символ - цифра, считываем значение точности из строки формата
      char precisionBuff[1024] = "";  // Буфер для хранения числового значения точности
      int i = 0;  // Индекс для записи в буфер точности
      // Читаем символы, пока они являются цифрами
      while (*format >= '0' && *format <= '9') {
        precisionBuff[i] = *format;  // Сохраняем текущий символ в буфер
        i++;  // Увеличиваем индекс буфера
        format++;  // Переходим к следующему символу в строке формата
      }
      // Преобразуем строку из буфера в целое число и сохраняем в prepared_format_data->precision
      prepared_format_data->precision = s21_atoi(precisionBuff);
    }
  }
  // Возвращаем текущую позицию в строке формата
  return format;
}


const char *get_length(const char *format,
                       struct PreparedFormatData *prepared_format_data) {
  // Проверяем, является ли текущий символ спецификатором длины ('h', 'l', 'L')
  if (*format == 'h' || *format == 'l' || *format == 'L') {
    // Если да, сохраняем символ длины в структуру prepared_format_data
    prepared_format_data->length = *format++;
  }
  // Возвращаем текущую позицию в строке формата
  return format;
}

/**
 * @brief Считывает спецификатор формата из строки формата.
 *
 * @param format Строка формата, содержащая спецификаторы для форматирования.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, куда будет сохранен спецификатор формата.
 * @param error Указатель на переменную, которая будет установлена в 0, если обнаружена ошибка (недопустимый спецификатор).
 * @return Возвращает текущую позицию в строке формата после обработки спецификатора формата.
 */
const char *get_specifier(const char *format,
                          struct PreparedFormatData *prepared_format_data,
                          int *error) {
  char tmp = *format;  // Сохраняем текущий символ в переменную tmp
  if (tmp != '\0') {  // Проверяем, не достигли ли конца строки формата
    char *pattern = "cdieEfgGosuxXpn";  // Массив допустимых спецификаторов
    if (s21_strchr(pattern, tmp) != S21_NULL) {  // Проверяем, является ли символ допустимым спецификатором
      prepared_format_data->specifier = *format++;  // Сохраняем спецификатор и переходим к следующему символу
    } else if (tmp == '%') {  // Проверяем, является ли символ '%'
      prepared_format_data->specifier = *format++;  // Сохраняем символ '%' как спецификатор и переходим к следующему символу
    } else {
      *error = 0;  // Если символ не является допустимым спецификатором, устанавливаем флаг ошибки
    }
  }
  return format;  // Возвращаем текущую позицию в строке формата
}


/**
 * @brief Обрабатывает спецификатор формата и вызывает соответствующую функцию для обработки.
 *
 * В зависимости от спецификатора формата, вызывается соответствующая функция обработки,
 * которая выполняет форматирование аргументов и запись в строку вывода.
 *
 * @param str Двойной указатель на строку вывода, куда будет записан результат форматирования.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, содержащую информацию о текущем спецификаторе формата.
 * @param va Список аргументов переменной длины, переданных в функцию.
 * @param processed Количество символов, уже обработанных и записанных в строку вывода.
 * @return Возвращает количество обработанных символов после выполнения соответствующей функции обработки.
 */
int specifier_handler(char **str,
                      struct PreparedFormatData *prepared_format_data,
                      va_list va, int processed) {
  // Обрабатываем спецификатор формата, используя switch-case для вызова соответствующей функции
  switch (prepared_format_data->specifier) {
    case 'c':
    case '%':
      // Если спецификатор 'c' или '%', вызываем функцию char_handler
      char_handler(str, va, prepared_format_data);
      break;
    case 'd':
    case 'i':
      // Если спецификатор 'd' или 'i', вызываем функцию integer_handler
      integer_handler(*str, prepared_format_data, va);
      break;
    case 's':
      // Если спецификатор 's', вызываем функцию string_handler
      string_handler(*str, va, prepared_format_data);
      break;
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      // Если спецификатор 'f', 'e', 'E', 'g' или 'G', вызываем функцию double_handler
      double_handler(*str, prepared_format_data, va);
      break;
    case 'u':
    case 'x':
    case 'X':
    case 'o':
      // Если спецификатор 'u', 'x', 'X' или 'o', вызываем функцию unsigned_handler
      unsigned_handler(*str, prepared_format_data, va);
      break;
    case 'n':
      // Если спецификатор 'n', вызываем функцию n_handler
      n_handler(va, processed);
      break;
    case 'p':
      // Если спецификатор 'p', вызываем функцию pointer_handler
      pointer_handler(*str, prepared_format_data, va);
      break;
  }
  // Возвращаем количество обработанных символов
  return processed;
}


/**
 * @brief Обрабатывает спецификаторы формата для чисел с плавающей запятой и записывает их в строку вывода.
 *
 * В зависимости от длины и спецификатора формата, считывает значение числа с плавающей запятой из аргументов
 * и вызывает соответствующую функцию для записи значения в строку вывода.
 *
 * @param str Указатель на строку вывода, куда будет записан результат форматирования.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, содержащую информацию о текущем спецификаторе формата.
 * @param va Список аргументов переменной длины, переданных в функцию.
 * @return Возвращает 0 после завершения обработки.
 */
int double_handler(char *str, struct PreparedFormatData *prepared_format_data,
                   va_list va) {
  long double value = 0;

  // Обработка длины спецификатора формата для числа с плавающей запятой
  switch (prepared_format_data->length) {
    case 'l':
      // Если длина спецификатора 'l', считываем значение типа double
      value = (double)va_arg(va, double);
      break;
    case '\0':
      // Если длина спецификатора не указана, считываем значение типа double
      value = va_arg(va, double);
      break;
    case 'L':
      // Если длина спецификатора 'L', считываем значение типа long double
      value = (long double)va_arg(va, long double);
      break;
    default:
      break;
  }

  // Обработка спецификатора формата и запись значения в строку вывода
  switch (prepared_format_data->specifier) {
    case 'f':
      // Если спецификатор 'f', вызываем функцию write_float
      write_float(str, prepared_format_data, value);
      break;
    case 'e':
    case 'E':
      // Если спецификатор 'e' или 'E', вызываем функцию write_e
      write_e(str, prepared_format_data, value);
      break;
    case 'g':
    case 'G':
      // Если спецификатор 'g' или 'G', вызываем функцию write_g
      write_g(str, prepared_format_data, value);
      break;
  }

  // Возвращаем 0 после завершения обработки
  return 0;
}


/**
 * @brief Обрабатывает спецификатор формата для символов и записывает их в строку вывода.
 *
 * В зависимости от спецификатора формата, считывает символ из аргументов
 * и записывает его в строку вывода с учетом заданной ширины и флагов.
 *
 * @param str Двойной указатель на строку вывода, куда будет записан результат форматирования.
 * @param va Список аргументов переменной длины, переданных в функцию.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, содержащую информацию о текущем спецификаторе формата.
 * @return Возвращает 0 после завершения обработки.
 */
int char_handler(char **str, va_list va,
                 struct PreparedFormatData *prepared_format_data) {
  char c;
  
  // Проверяем, является ли спецификатор '%'
  if (prepared_format_data->specifier == '%') {
    c = '%';  // Если да, то устанавливаем символ '%'
  } else {
    c = va_arg(va, int);  // Иначе считываем следующий аргумент как символ
  }

  char *pointer = *str;

  // Переходим к концу текущей строки вывода
  while (*pointer) {
    pointer++;
  }

  // Уменьшаем ширину на единицу для текущего символа
  prepared_format_data->width--;

  // Проверяем флаг выравнивания по левому краю
  if (prepared_format_data->flags.minus != 0) {
    *pointer = c;  // Записываем символ в строку
    pointer++;
    prepared_format_data->counter++;  // Увеличиваем счетчик обработанных символов
    *pointer = '\0';  // Добавляем завершающий нуль
    *str = pointer;  // Обновляем указатель строки вывода
    write_space(*str, prepared_format_data);  // Дописываем пробелы, если необходимо
    write_zero(*str, prepared_format_data);  // Дописываем нули, если необходимо
  } else {
    write_space(*str, prepared_format_data);  // Дописываем пробелы, если необходимо
    write_zero(*str, prepared_format_data);  // Дописываем нули, если необходимо

    // Переходим к концу текущей строки вывода
    while (*pointer) {
      pointer++;
    }

    *pointer++ = c;  // Записываем символ в строку
    prepared_format_data->counter++;  // Увеличиваем счетчик обработанных символов
    *pointer = '\0';  // Добавляем завершающий нуль
    *str = pointer;  // Обновляем указатель строки вывода
  }
  return 0;  // Возвращаем 0 после завершения обработки
}


/**
 * @brief Обрабатывает спецификатор формата для строк и записывает их в строку вывода.
 *
 * В зависимости от спецификатора формата, считывает строку из аргументов
 * и записывает её в строку вывода с учетом заданной ширины, точности и флагов.
 *
 * @param str Указатель на строку вывода, куда будет записан результат форматирования.
 * @param va Список аргументов переменной длины, переданных в функцию.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, содержащую информацию о текущем спецификаторе формата.
 * @return Возвращает 0 после завершения обработки.
 */
int string_handler(char *str, va_list va,
                   struct PreparedFormatData *prepared_format_data) {
  // Считываем строку из аргументов
  char *s = va_arg(va, char *);

  // Если точность не указана, устанавливаем её равной длине строки
  if (prepared_format_data->has_precision == 0) {
    prepared_format_data->precision = s21_strlen(s);
  }

  // Обработка ширины и точности строки
  if (prepared_format_data->width > 0 &&
      (prepared_format_data->width - prepared_format_data->precision) <= 0 &&
      prepared_format_data->has_precision != 0) {
    // Если ширина больше нуля, и разница между шириной и точностью меньше или равна нулю,
    // и точность указана, корректируем ширину и счетчик обработанных символов
    if (prepared_format_data->width > (int)s21_strlen(s)) {
      prepared_format_data->width -= (int)s21_strlen(s);
    } else {
      prepared_format_data->width = 0;
    }
    prepared_format_data->counter = s21_strlen(s);
  } else {
    // Иначе корректируем ширину и увеличиваем счетчик обработанных символов на значение точности
    prepared_format_data->width -= prepared_format_data->precision;
    prepared_format_data->counter += prepared_format_data->precision;
  }

  // Обработка флага выравнивания по левому краю
  if (prepared_format_data->flags.minus != 0) {
    s21_strncat(str, s, prepared_format_data->precision);  // Конкатенируем строку с учетом точности
    write_space(str, prepared_format_data);  // Дописываем пробелы, если необходимо
    write_zero(str, prepared_format_data);  // Дописываем нули, если необходимо
  } else {
    write_space(str, prepared_format_data);  // Дописываем пробелы, если необходимо
    write_zero(str, prepared_format_data);  // Дописываем нули, если необходимо
    s21_strncat(str, s, prepared_format_data->precision);  // Конкатенируем строку с учетом точности
  }

  return 0;  // Возвращаем 0 после завершения обработки
}


/**
 * @brief Обрабатывает спецификатор формата для указателей и записывает их в строку вывода.
 *
 * Функция преобразует переданный указатель в строку в шестнадцатеричном формате,
 * добавляет префикс "0x" и записывает результат в указанную строку вывода.
 *
 * @param str Указатель на строку вывода, куда будет записан результат форматирования.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, содержащую информацию о текущем спецификаторе формата.
 * @param va Список аргументов переменной длины, переданных в функцию.
 * @return Возвращает 0 после завершения обработки.
 */
int pointer_handler(char *str, struct PreparedFormatData *prepared_format_data,
                    va_list va) {
  char value_str[1024] = {'\0'};  // Буфер для хранения строки с шестнадцатеричным представлением указателя
  long long unsigned int value = (long long unsigned int)va_arg(va, long long unsigned int);  // Получаем значение указателя

  // Преобразуем значение указателя в строку в шестнадцатеричном формате
  decimal_to_hexoct(value_str, value, prepared_format_data, 16);

  int value_len = s21_strlen(value_str) + 2;  // Вычисляем длину строки значения плюс два символа для префикса "0x"
  prepared_format_data->precision -= value_len - 2;  // Уменьшаем точность на длину строки минус два символа

  // Вычисляем оставшуюся ширину
  if (prepared_format_data->precision <= 0) {
    prepared_format_data->width -= value_len;
  } else {
    prepared_format_data->width -= value_len + prepared_format_data->precision;
  }

  prepared_format_data->counter += value_len;  // Увеличиваем счетчик обработанных символов на длину строки значения

  // Обработка флага выравнивания по левому краю
  if (prepared_format_data->flags.minus != 0) {
    s21_strcat(str, "0x");  // Добавляем префикс "0x" к строке вывода
    write_zeroes(str, prepared_format_data);  // Дописываем нули, если необходимо
    s21_strcat(str, value_str);  // Добавляем строку с шестнадцатеричным значением
    write_space(str, prepared_format_data);  // Дописываем пробелы, если необходимо
  } else {
    write_space(str, prepared_format_data);  // Дописываем пробелы, если необходимо
    s21_strcat(str, "0x");  // Добавляем префикс "0x" к строке вывода
    write_zeroes(str, prepared_format_data);  // Дописываем нули, если необходимо
    s21_strcat(str, value_str);  // Добавляем строку с шестнадцатеричным значением
  }

  return 0;  // Возвращаем 0 после завершения обработки
}

/**
 * @brief Обрабатывает спецификатор формата для целочисленных значений и записывает их в строку вывода.
 *
 * Функция считывает целочисленное значение из списка аргументов в зависимости от указанной длины,
 * вызывает функцию для записи целого числа в строку вывода и возвращает результат.
 *
 * @param str Указатель на строку вывода, куда будет записан результат форматирования.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, содержащую информацию о текущем спецификаторе формата.
 * @param va Список аргументов переменной длины, переданных в функцию.
 * @return Возвращает 0 после завершения обработки.
 */
int integer_handler(char *str, struct PreparedFormatData *prepared_format_data,
                    va_list va) {
  long long value = 0;  // Переменная для хранения целочисленного значения

  // В зависимости от указанной длины считываем значение из списка аргументов
  switch (prepared_format_data->length) {
    case 'h':
      value = (short int)va_arg(va, int);
      break;
    case '\0':
      value = (int)va_arg(va, int);
      break;
    case 'l':
      value = (long long)va_arg(va, long long int);
      break;
  }

  // Вызываем функцию для записи целого числа в строку вывода
  write_int(str, prepared_format_data, value);

  return 0;  // Возвращаем 0 после завершения обработки
}


/**
 * @brief Обрабатывает случаи отрицательных значений при форматировании чисел с плавающей точкой.
 *
 * Функция в зависимости от флагов форматирования выполняет необходимые действия для отрицательных значений:
 * добавляет знак, выравнивает ширину, дописывает нули и пробелы, добавляет значение и возвращает результат.
 *
 * @param str Указатель на строку вывода, куда будет записан результат форматирования.
 * @param value_str Строка с числом в текстовом формате.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, содержащую информацию о текущем спецификаторе формата.
 * @param value Значение числа с плавающей точкой, для которого выполняется форматирование.
 * @return Возвращает 0 после завершения обработки.
 */
int minus_handler(char *str, char *value_str,
                  struct PreparedFormatData *prepared_format_data,
                  long double value) {
  if (prepared_format_data->flags.minus != 0) {
    // Если установлен флаг выравнивания по левому краю
    write_sign(str, value, prepared_format_data);  // Добавляем знак числа
    decrease_sign_width(value, prepared_format_data);  // Уменьшаем ширину знака
    write_zeroes(str, prepared_format_data);  // Дописываем нули
    decrease_zeroes_width(prepared_format_data);  // Уменьшаем ширину нулей
    write_zero(str, prepared_format_data);  // Дописываем ноль
    s21_strcat(str, value_str);  // Добавляем строку с числом
    write_space(str, prepared_format_data);  // Дописываем пробелы, если необходимо
  } else {
    // Если флаг выравнивания по левому краю не установлен
    decrease_sign_width(value, prepared_format_data);  // Уменьшаем ширину знака
    decrease_zeroes_width(prepared_format_data);  // Уменьшаем ширину нулей
    write_space(str, prepared_format_data);  // Дописываем пробелы, если необходимо
    write_sign(str, value, prepared_format_data);  // Добавляем знак числа
    write_zeroes(str, prepared_format_data);  // Дописываем нули
    write_zero(str, prepared_format_data);  // Дописываем ноль
    s21_strcat(str, value_str);  // Добавляем строку с числом
  }
  return 0;  // Возвращаем 0 после завершения обработки
}


/**
 * @brief Обрабатывает спецификаторы формата для беззнаковых целочисленных значений и записывает их в строку вывода.
 *
 * Функция принимает указатель на строку вывода, структуру `PreparedFormatData`, содержащую информацию
 * о форматировании, и список аргументов переменной длины `va`, из которого извлекается беззнаковое целочисленное значение.
 * В зависимости от указанной длины и спецификатора формата вызывает соответствующую функцию для записи значения в строку вывода.
 *
 * @param str Указатель на строку вывода, куда будет записан результат форматирования.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, содержащую информацию о текущем спецификаторе формата.
 * @param va Список аргументов переменной длины, переданных в функцию.
 * @return Возвращает 0 после завершения обработки.
 */
int unsigned_handler(char *str, struct PreparedFormatData *prepared_format_data,
                     va_list va) {
  unsigned long long value = 0;  // Переменная для хранения беззнакового целочисленного значения

  // В зависимости от указанной длины считываем значение из списка аргументов
  switch (prepared_format_data->length) {
    case 'h':
      value = (short unsigned)va_arg(va, unsigned int);
      break;
    case '\0':
      value = (unsigned)va_arg(va, unsigned int);
      break;
    case 'l':
      value = (unsigned long)va_arg(va, unsigned long int);
      break;
    default:
      break;
  }

  // В зависимости от спецификатора формата вызываем соответствующую функцию для записи значения в строку вывода
  switch (prepared_format_data->specifier) {
    case 'u':
      write_unsigned(str, prepared_format_data, value);
      break;
    case 'x':
    case 'X':
      x_handler(str, prepared_format_data, value);
      break;
    case 'o':
      o_handler(str, prepared_format_data, value);
      break;
  }

  return 0;  // Возвращаем 0 после завершения обработки
}


/**
 * @brief Обрабатывает спецификатор формата %o (восьмеричное представление) и записывает результат в строку вывода.
 *
 * Функция принимает строку вывода `str`, структуру `PreparedFormatData`, содержащую информацию
 * о форматировании, и беззнаковое целочисленное значение `value`, которое необходимо отформатировать.
 * Выполняет форматирование значения в восьмеричное представление и записывает его в строку вывода `str`.
 *
 * @param str Указатель на строку вывода, куда будет записан результат форматирования.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, содержащую информацию о текущем спецификаторе формата.
 * @param value Беззнаковое целочисленное значение для форматирования в восьмеричное представление.
 * @return Возвращает 0 после завершения обработки.
 */
int o_handler(char *str, struct PreparedFormatData *prepared_format_data,
              unsigned long long value) {
  char value_str[1024] = {'\0'};  // Буфер для хранения строкового представления числа в восьмеричном формате
  decimal_to_hexoct(value_str, value, prepared_format_data, 8);  // Преобразование числа в восьмеричное представление
  int value_len = s21_strlen(value_str);  // Длина строки, представляющей число в восьмеричном формате

  prepared_format_data->precision -= value_len;  // Уменьшаем точность на длину числа

  // Вычисление ширины вывода
  if (prepared_format_data->precision <= 0) {
    prepared_format_data->width -= value_len;
  } else {
    prepared_format_data->width -= value_len + prepared_format_data->precision;
  }

  prepared_format_data->counter += value_len;  // Увеличиваем счетчик символов

  // Запись в строку вывода в зависимости от флага минус
  if (prepared_format_data->flags.minus != 0) {
    write_prefix_spaces(&str, prepared_format_data, value);  // Запись префикса и пробелов перед числом
    write_zeroes(str, prepared_format_data);  // Запись нулей перед числом (если требуется)
    write_zero(str, prepared_format_data);  // Запись флага нулевого символа (если требуется)
    s21_strcat(str, value_str);  // Добавление числа в восьмеричном формате в строку вывода
    write_space(str, prepared_format_data);  // Запись пробелов после числа (если требуется)
  } else {
    write_prefix_spaces(&str, prepared_format_data, value);  // Запись префикса и пробелов перед числом
    write_zeroes(str, prepared_format_data);  // Запись нулей перед числом (если требуется)
    write_zero(str, prepared_format_data);  // Запись флага нулевого символа (если требуется)
    s21_strcat(str, value_str);  // Добавление числа в восьмеричном формате в строку вывода
  }

  return 0;  // Возвращаем 0 после завершения обработки
}


/**
 * @brief Обрабатывает спецификатор формата %n и записывает количество обработанных символов в переданную переменную.
 *
 * Функция принимает список аргументов переменной длины `va` и указатель на переменную `value`, в которую
 * необходимо записать количество обработанных символов (переменная `processed`).
 *
 * @param va Список аргументов переменной длины, переданных в функцию.
 * @param processed Количество обработанных символов, которое будет записано в переменную `value`.
 * @return Возвращает 0 после завершения обработки.
 */
int n_handler(va_list va, int processed) {
  int *value = va_arg(va, int *);  // Получаем указатель на переменную, куда нужно записать значение
  *value = processed;  // Записываем количество обработанных символов в переменную, на которую указывает `value`

  return 0;  // Возвращаем 0 после завершения обработки
}


/**
 * @brief Обрабатывает спецификаторы формата %x и %X (шестнадцатеричное представление) и записывает результат в строку вывода.
 *
 * Функция принимает строку вывода `str`, структуру `PreparedFormatData`, содержащую информацию
 * о форматировании, и беззнаковое целочисленное значение `value`, которое необходимо отформатировать.
 * Выполняет форматирование значения в шестнадцатеричное представление и записывает его в строку вывода `str`.
 *
 * @param str Указатель на строку вывода, куда будет записан результат форматирования.
 * @param prepared_format_data Указатель на структуру PreparedFormatData, содержащую информацию о текущем спецификаторе формата.
 * @param value Беззнаковое целочисленное значение для форматирования в шестнадцатеричное представление.
 * @return Возвращает 0 после завершения обработки.
 */
int x_handler(char *str, struct PreparedFormatData *prepared_format_data,
              unsigned long value) {
  char value_str[1024] = {'\0'};  // Буфер для хранения строкового представления числа в шестнадцатеричном формате
  decimal_to_hexoct(value_str, value, prepared_format_data, 16);  // Преобразование числа в шестнадцатеричное представление
  if (prepared_format_data->specifier == 'X') {
    x_to_upper(value_str);  // Если спецификатор формата 'X', переводим символы в верхний регистр
  }
  
  int value_len = s21_strlen(value_str);  // Длина строки, представляющей число в шестнадцатеричном формате
  prepared_format_data->precision -= value_len;  // Уменьшаем точность на длину числа
  
  // Вычисление ширины вывода
  if (prepared_format_data->precision <= 0) {
    prepared_format_data->width -= value_len;
  } else {
    prepared_format_data->width -= value_len + prepared_format_data->precision;
  }
  
  prepared_format_data->counter += value_len;  // Увеличиваем счетчик символов
  
  // Запись в строку вывода в зависимости от флага минус
  if (prepared_format_data->flags.minus != 0) {
    write_prefix_spaces(&str, prepared_format_data, value);  // Запись префикса и пробелов перед числом
    write_zeroes(str, prepared_format_data);  // Запись нулей перед числом (если требуется)
    write_zero(str, prepared_format_data);  // Запись флага нулевого символа (если требуется)
    s21_strcat(str, value_str);  // Добавление числа в шестнадцатеричном формате в строку вывода
    write_space(str, prepared_format_data);  // Запись пробелов после числа (если требуется)
  } else {
    write_prefix_spaces(&str, prepared_format_data, value);  // Запись префикса и пробелов перед числом
    write_zeroes(str, prepared_format_data);  // Запись нулей перед числом (если требуется)
    write_zero(str, prepared_format_data);  // Запись флага нулевого символа (если требуется)
    s21_strcat(str, value_str);  // Добавление числа в шестнадцатеричном формате в строку вывода
  }
  
  return 0;  // Возвращаем 0 после завершения обработки
}


/**
 * @brief Записывает один символ из строки `format` в конец строки `str`.
 *
 * Функция принимает указатель `str` на строку, в которую будет производиться запись,
 * и указатель `format` на строку, из которой будет взят символ для записи.
 * Символ из строки `format` добавляется в конец строки `str`, после чего строка `str`
 * завершается нулевым символом.
 *
 * @param str Указатель на строку, в которую будет производиться запись символа.
 * @param format Указатель на строку, из которой будет взят символ для записи.
 * @return Возвращает 0 после успешной записи символа в строку `str`.
 */
int write_to_str(char *str, const char *format) {
  char *end_index = str;  // Указатель на конец строки `str`

  // Поиск конца строки `str`
  while (*end_index) {
    end_index++;
  }

  *end_index++ = *format;  // Запись символа из строки `format` в конец строки `str`
  *end_index = '\0';  // Завершение строки `str` нулевым символом

  return 0;  // Возвращаем 0 после успешной записи
}


/**
 * @brief Форматирует число типа long double в строку согласно спецификации 'g' или 'G'.
 *
 * Функция принимает указатель `str` на строку, в которую будет записан результат,
 * указатель `prepared_format_data` на структуру с данными для форматирования,
 * и значение `value` типа long double, которое нужно отформатировать.
 * Функция определяет, какой из форматов 'g' или 'G' использовать для записи числа в строку.
 *
 * @param str Указатель на строку, в которую будет записан результат.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @param value Значение типа long double, которое нужно отформатировать.
 * @return Возвращает 0 после успешного форматирования числа в строку `str`.
 */
int write_g(char *str, struct PreparedFormatData *prepared_format_data,
            long double value) {
  if (prepared_format_data->has_precision == 0) {
    prepared_format_data->precision = 6;
  }
  prepared_format_data->has_precision = 1;

  // Проверка условий для использования экспоненциальной формы или обычной десятичной
  if ((fabsl(value) / powl(10, prepared_format_data->precision) > 1) ||
      (((fabsl(value) * (powl(10, 4))) < 1) && value != 0)) {
    // Уменьшение точности перед вызовом функции write_e в случае использования экспоненциальной формы
    if (prepared_format_data->precision != 0) {
      prepared_format_data->precision--;
    }
    write_e(str, prepared_format_data, value);  // Форматирование числа как 'e' или 'E'
  } else {
    // Обработка числа как обычной десятичной формы
    if (fabsl(value) < 0.1 && value != 0) {
      int i = 1;
      if (prepared_format_data->precision == 0) {
        prepared_format_data->precision++;
      }
      // Увеличение точности для маленьких чисел
      while ((fabsl(value) * (powl(10, i))) < 1) {
        prepared_format_data->precision++;
        i++;
      }
    } else if (value != 0) {
      // Уменьшение точности для чисел большого размера
      if (prepared_format_data->precision == 0) {
        prepared_format_data->precision++;
      } else {
        prepared_format_data->precision -= double_length(value);
      }
    } else {
      prepared_format_data->precision--;
    }
    write_float(str, prepared_format_data, value);  // Форматирование числа как 'f'
  }

  return 0;
}


/**
 * @brief Форматирует число типа long double в строку в экспоненциальной форме 'e' или 'E'.
 *
 * Функция принимает указатель `str` на строку, в которую будет записан результат,
 * указатель `prepared_format_data` на структуру с данными для форматирования,
 * и значение `value` типа long double, которое нужно отформатировать в экспоненциальную форму.
 * Функция проверяет число на NaN или бесконечность, вычисляет степень и форматирует число
 * согласно указанной точности и спецификации 'e' или 'E'.
 *
 * @param str Указатель на строку, в которую будет записан результат форматирования.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @param value Значение типа long double, которое нужно отформатировать.
 * @return Возвращает 0 после успешного форматирования числа в строку `str`.
 */
int write_e(char *str, struct PreparedFormatData *prepared_format_data,
            long double value) {
  char valueStr[1024] = {'\0'};  // Буфер для хранения строкового представления числа
  int power = 0;
  
  // Проверка на NaN или бесконечность, и обработка
  int infNan = is_NaN_or_INF(valueStr, prepared_format_data, value);
  
  // Вычисление степени для чисел >= 1
  if (fabsl(value) >= 1.0 && infNan == 0) {
    while (floorl(fabsl(
               roundl(value * powl(10, prepared_format_data->precision + 1)) /
               powl(10, prepared_format_data->precision + 1))) >= 10) {
      value = value / 10;
      power++;
    }
  } 
  // Вычисление степени для чисел < 1
  else if (value != 0 && infNan == 0) {
    while (fabsl(value) < 1) {
      value = value * 10;
      power--;
    }
  }
  
  // Форматирование числа в строку в соответствии с указанной точностью
  if (infNan == 0) {
    ftoa(valueStr, prepared_format_data, value);  // Преобразование числа в строку
    remove_zeroes_in_g(valueStr, prepared_format_data);  // Удаление лишних нулей
    if (s21_strchr("gG", prepared_format_data->specifier) != S21_NULL) {
      if (prepared_format_data->specifier == 'g') {
        prepared_format_data->specifier = 'e';
      } else {
        prepared_format_data->specifier = 'E';
      }
    }
    write_exponent(valueStr, prepared_format_data, power);  // Запись экспоненты
  }
  
  // Обработка знака и вывод в строку
  minus_handler(str, valueStr, prepared_format_data, value);
  
  return 0;
}


/**
 * @brief Форматирует число типа long double в строку с плавающей точкой.
 *
 * Функция принимает указатель `str` на строку, в которую будет записан результат,
 * указатель `prepared_format_data` на структуру с данными для форматирования,
 * и значение `value` типа long double, которое нужно отформатировать.
 * Функция проверяет число на NaN или бесконечность, преобразует число в строку
 * с плавающей точкой согласно указанной точности и спецификации. Затем обрабатывает
 * знак числа и записывает результат в строку `str`.
 *
 * @param str Указатель на строку, в которую будет записан результат форматирования.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @param value Значение типа long double, которое нужно отформатировать.
 * @return Возвращает 0 после успешного форматирования числа в строку `str`.
 */
int write_float(char *str, struct PreparedFormatData *prepared_format_data,
                long double value) {
  char value_str[1024] = {'\0'};  // Буфер для хранения строкового представления числа
  int infNan = is_NaN_or_INF(value_str, prepared_format_data, value);  // Проверка на NaN или бесконечность

  if (infNan == 0) {
    ftoa(value_str, prepared_format_data, value);  // Преобразование числа в строку с плавающей точкой
    remove_zeroes_in_g(value_str, prepared_format_data);  // Удаление лишних нулей (для 'g' и 'G')
  }

  minus_handler(str, value_str, prepared_format_data, value);  // Обработка знака числа и запись в строку

  return 0;
}


/**
 * @brief Функция write_double форматирует значение типа long double в строку.
 * 
 * @param value_str Указатель на массив символов, куда будет записан результат.
 * @param value Значение типа long double, которое нужно преобразовать в строку.
 * @param prepared_format_data Указатель на структуру PreparedFormatData,
 *        которая содержит параметры форматирования (ширина поля, точность и т.д.).
 * 
 * @return Всегда возвращает 0, так как функция не предполагает обработки ошибок.
 */
int write_double(char *value_str, long double value,
                 struct PreparedFormatData *prepared_format_data) {
  
  if (value == 0.0 && (prepared_format_data->has_precision == 0 ||
                       (prepared_format_data->has_precision &&
                        prepared_format_data->precision != 0))) {
    // Если значение равно 0 и не требуется точность или точность не равна 0,
    // записываем '0' в строку.
    value_str[0] = '0';
    prepared_format_data->precision--;  // Уменьшаем точность на единицу.
    prepared_format_data->counter++;    // Увеличиваем счетчик символов.
  } else {
    int i = 0;
    int sign = 1;
    if (value < 0.0) {
      sign *= -1;  // Если значение отрицательное, устанавливаем знак -1.
    }
    int length = double_length(value);  // Вычисляем длину целой части числа.
    i += length - 1;
    prepared_format_data->precision -= length;  // Уменьшаем точность на длину числа.

    while (fabsl(value) >= 1) {
      // Пока число больше или равно 1, вычисляем остаток от деления на 10 и
      // записываем его как символ в строку value_str.
      char residue = ((int)fmodl(value, 10)) * sign + 48;
      value_str[i] = residue;
      value /= 10;  // Делим число на 10 для обработки следующей цифры.
      i--;
      prepared_format_data->counter++;  // Увеличиваем счетчик символов.
    }
  }
  
  prepared_format_data->width -= s21_strlen(value_str);  // Уменьшаем ширину поля.

  return 0;  // Возвращаем 0, функция всегда успешно завершается.
}


/**
 * @brief Функция write_dot добавляет десятичную точку в строку **value_str.
 * 
 * @param value_str Указатель на указатель на строку, куда будет добавлена точка.
 * @param prepared_format_data Указатель на структуру PreparedFormatData,
 *        содержащую параметры форматирования.
 * 
 * @return Всегда возвращает 0, так как функция не предполагает обработки ошибок.
 */
int write_dot(char **value_str,
              struct PreparedFormatData *prepared_format_data) {
  
  // Перемещаем указатель **value_str в конец строки, чтобы добавить точку.
  while (**value_str) {
    (*value_str)++;
  }
  
  // Добавляем десятичную точку '.' в конец строки.
  **value_str = '.';
  (*value_str)++;
  **value_str = '\0'; // Устанавливаем нулевой символ в конце строки.
  
  // Увеличиваем счетчик символов и уменьшаем ширину поля и точность.
  prepared_format_data->counter++;
  prepared_format_data->width--;
  prepared_format_data->precision--;

  return 0; // Всегда возвращаем 0, функция успешно завершается.
}


/**
 * @brief Функция write_prefix_spaces добавляет префиксные символы перед числовым значением в строке **str.
 * 
 * @param str Указатель на указатель на строку, куда будут добавлены префиксные символы.
 * @param prepared_format_data Указатель на структуру PreparedFormatData,
 *        содержащую параметры форматирования.
 * @param value Значение числа, перед которым добавляются префиксные символы.
 * 
 * @return Всегда возвращает 0, так как функция не предполагает обработки ошибок.
 */
int write_prefix_spaces(char **str,
                        struct PreparedFormatData *prepared_format_data,
                        unsigned long long value) {
  if (prepared_format_data->specifier != 'o') {
    if (prepared_format_data->flags.minus != 0) {
      // Добавляем префикс "0x" или "0X" для шестнадцатеричных чисел при выравнивании по левому краю.
      if (prepared_format_data->flags.sharp == 1 && value != 0) {
        prepared_format_data->width -= 2; // Уменьшаем ширину на 2 символа (для "0x" или "0X").
        prepared_format_data->counter += 2; // Увеличиваем счетчик символов на 2.
        if (prepared_format_data->specifier == 'x') {
          s21_strcat(*str, "0x"); // Добавляем "0x".
        } else {
          s21_strcat(*str, "0X"); // Добавляем "0X".
        }
      }
    } else {
      // Добавляем пробелы перед числовым значением, если не выравниваем по левому краю.
      if (prepared_format_data->flags.sharp == 1 && value != 0) {
        prepared_format_data->width -= 2; // Уменьшаем ширину на 2 символа (для "0x" или "0X").
        prepared_format_data->counter += 2; // Увеличиваем счетчик символов на 2.
        write_space(*str, prepared_format_data); // Добавляем пробелы.
        if (prepared_format_data->specifier == 'x') {
          s21_strcat(*str, "0x"); // Добавляем "0x".
        } else {
          s21_strcat(*str, "0X"); // Добавляем "0X".
        }
      } else {
        write_space(*str, prepared_format_data); // Добавляем пробелы перед числом.
      }
    }
  } else {
    // Добавляем префикс "0" перед восьмеричным числом или пробелы перед числом.
    if (prepared_format_data->flags.minus != 0) {
      if (prepared_format_data->flags.sharp == 1 &&
          prepared_format_data->precision < 0 && value != 0) {
        prepared_format_data->width--; // Уменьшаем ширину на 1 символ.
        prepared_format_data->counter++; // Увеличиваем счетчик символов на 1.
        s21_strcat(*str, "0"); // Добавляем "0".
      }
    } else {
      if (prepared_format_data->flags.sharp == 1 && value != 0) {
        write_space(*str, prepared_format_data); // Добавляем пробелы перед числом.
        prepared_format_data->width--; // Уменьшаем ширину на 1 символ.
        prepared_format_data->counter++; // Увеличиваем счетчик символов на 1.
        prepared_format_data->precision--; // Уменьшаем точность на 1.
        s21_strcat(*str, "0"); // Добавляем "0".
      } else {
        write_space(*str, prepared_format_data); // Добавляем пробелы перед числом.
      }
    }
  }
  return 0; // Всегда возвращаем 0, функция успешно завершается.
}


/**
 * @brief Функция write_zero добавляет нулевые символы в строку str в соответствии с параметрами форматирования.
 * 
 * @param str Указатель на строку, куда добавляются нулевые символы.
 * @param prepared_format_data Указатель на структуру PreparedFormatData,
 *        содержащую параметры форматирования.
 * 
 * @return Всегда возвращает 0, так как функция не предполагает обработки ошибок.
 */
int write_zero(char *str, struct PreparedFormatData *prepared_format_data) {
  if (prepared_format_data->flags.zero != 0 &&
      prepared_format_data->flags.minus == 0) {
    // Если установлен флаг 'zero' и не установлен флаг 'minus', добавляем нулевые символы.
    while (prepared_format_data->width > 0) {
      s21_strcat(str, "0"); // Добавляем символ '0' в конец строки str.
      prepared_format_data->counter++; // Увеличиваем счетчик символов.
      prepared_format_data->width--; // Уменьшаем ширину.
    }
  }
  return 0; // Всегда возвращаем 0, функция успешно завершается.
}


/**
 * @brief Форматирует экспоненту числа и добавляет её к строковому представлению.
 *
 * Функция принимает строку `value_str`, в которую будет добавлена экспонента,
 * указатель `prepared_format_data` на структуру с данными для форматирования,
 * и значение `power`, представляющее порядок числа.
 * Функция добавляет спецификатор экспоненты ('e' или 'E'), знак экспоненты ('+' или '-'),
 * и значение экспоненты, форматируя его с учётом длины и знака.
 *
 * @param value_str Указатель на строку, в которую будет добавлена экспонента.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @param power Значение порядка числа.
 * @return Возвращает 0 после успешного форматирования экспоненты.
 */
int write_exponent(char *value_str,
                   struct PreparedFormatData *prepared_format_data, int power) {
  // Перемещаем указатель к концу строки value_str
  while (*value_str) {
    value_str++;
  }

  // Указываем начальную позицию для добавления экспоненты
  char *start_position = value_str;

  // Добавляем спецификатор экспоненты ('e' или 'E')
  *value_str = prepared_format_data->specifier;
  value_str++;

  // Добавляем знак экспоненты ('+' или '-')
  if (power < 0) {
    *value_str = '-';
  } else {
    *value_str = '+';
  }
  value_str++;

  // Если значение экспоненты по абсолютному значению меньше 10, добавляем ведущий ноль
  if (abs(power) < 10) {
    *value_str = '0';
    value_str++;
    if (power == 0) {
      *value_str = '0';
      value_str++;
    }
  }

  // Обновляем счетчики длины и ширины форматирования
  prepared_format_data->counter += value_str - start_position;
  prepared_format_data->width -= value_str - start_position;

  // Переменные для форматирования значения экспоненты
  int i = 0;
  int sign = 1;
  if (power < 0) {
    sign *= -1;
  }
  int num = integer_length(power);
  i += num - 1;

  // Форматируем значение экспоненты
  while (power != 0) {
    char ost = (power % 10) * sign + 48;
    value_str[i] = ost;
    power /= 10;
    i--;
    prepared_format_data->counter++;
    prepared_format_data->width--;
  }
  
  return 0;
}


// long double mantissa(char **valueStr, long double exponent,
//                      long long *precision,
//                      struct PreparedFormatData *prepared_format_data) {
//   long double mantissa = fabsl(exponent) * powl(10, *precision);

//   if (mantissa != 0) {
//     long double tmp = fabsl(exponent) * 10;

//     while (tmp < 1 && (*precision) > 0) {
//       tmp = tmp * 10;
//       **valueStr = '0';
//       (*valueStr)++;
//       prepared_format_data->counter++;
//       prepared_format_data->width--;
//       (*precision)--;
//     }
//   }
//   if (double_length(roundl(mantissa)) > double_length(mantissa)) {
//     mantissa = 0;
//   } else {
//     mantissa = roundl(mantissa);
//   }
//   return mantissa;
// }
long double mantissa(char **valueStr, long double exponent,
                     long long *precision,
                     struct PreparedFormatData *prepared_format_data) {
    // Вычисляем мантиссу как абсолютное значение экспоненты, умноженное на 10 в степени precision
    long double mantissa = fabsl(exponent) * powl(10, *precision);

    // Если мантисса не равна нулю
    if (mantissa != 0) {
        // Временная переменная tmp, которая равна абсолютному значению экспоненты, умноженному на 10
        long double tmp = fabsl(exponent) * 10;

        // Пока tmp меньше 1 и precision больше 0
        while (tmp < 1 && (*precision) > 0) {
            // Умножаем tmp на 10
            tmp = tmp * 10;
            // Записываем '0' в текущую позицию строки и смещаем указатель valueStr
            **valueStr = '0';
            (*valueStr)++;
            // Увеличиваем счетчик в prepared_format_data и уменьшаем ширину
            prepared_format_data->counter++;
            prepared_format_data->width--;
            // Уменьшаем precision
            (*precision)--;
        }
    }

    // Если длина округленной мантиссы больше, чем длина мантиссы
    if (double_length(roundl(mantissa)) > double_length(mantissa)) {
        // Устанавливаем мантиссу в 0
        mantissa = 0;
    } else {
        // Иначе округляем мантиссу
        mantissa = roundl(mantissa);
    }
    // Возвращаем мантиссу
    return mantissa;
}

/**
 * @brief Добавляет пробелы в строку в соответствии с параметрами форматирования.
 *
 * Функция принимает строку `str`, в которую будут добавлены пробелы,
 * указатель `prepared_format_data` на структуру с данными для форматирования.
 * Если флаг `zero` равен 0 или флаг `minus` не равен 0, функция добавляет пробелы в строку,
 * уменьшая значение ширины и увеличивая счетчик символов до тех пор, пока ширина не достигнет 0.
 *
 * @param str Указатель на строку, в которую будут добавлены пробелы.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @return Возвращает 0 после успешного добавления пробелов.
 */
int write_space(char *str, struct PreparedFormatData *prepared_format_data) {
  // Проверяем условия: флаг zero должен быть равен 0 или флаг minus не должен быть равен 0
  if (prepared_format_data->flags.zero == 0 ||
      prepared_format_data->flags.minus != 0) {
    // Пока ширина больше 0, добавляем пробелы в строку
    while (prepared_format_data->width > 0) {
      s21_strcat(str, " ");  // Добавляем пробел к строке str
      prepared_format_data->counter++;  // Увеличиваем счетчик символов
      prepared_format_data->width--;  // Уменьшаем ширину
    }
  }
  return 0;  // Возвращаем 0 после успешного добавления пробелов
}

/**
 * @brief Записывает целое число в строку с учетом параметров форматирования.
 *
 * Функция принимает строку `str`, в которую будет записано целое число,
 * указатель `prepared_format_data` на структуру с данными для форматирования и
 * значение `value`, которое необходимо записать.
 * Преобразует значение в строку, обрабатывает отрицательные значения и добавляет результат в строку.
 *
 * @param str Указатель на строку, в которую будет записано значение.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @param value Значение типа long long, которое нужно записать в строку.
 * @return Возвращает 0 после успешной записи значения.
 */
int write_int(char *str, struct PreparedFormatData *prepared_format_data,
              long long value) {
  char valueStr[1024] = {'\0'};  // Буфер для строкового представления значения

  // Преобразуем целое значение в строку с учетом параметров форматирования
  itoa(valueStr, value, prepared_format_data);

  // Обрабатываем отрицательные значения и добавляем результат в строку
  minus_handler(str, valueStr, prepared_format_data, (long double)value);

  return 0;  // Возвращаем 0 после успешной записи значения
}


/**
 * @brief Записывает беззнаковое целое число в строку с учетом параметров форматирования.
 *
 * Функция принимает строку `str`, в которую будет записано беззнаковое целое число,
 * указатель `prepared_format_data` на структуру с данными для форматирования и
 * значение `value`, которое необходимо записать.
 * Преобразует значение в строку и добавляет результат в строку назначения.
 *
 * @param str Указатель на строку, в которую будет записано значение.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @param value Значение типа unsigned long long, которое нужно записать в строку.
 * @return Возвращает 0 после успешной записи значения.
 */
int write_unsigned(char *str, struct PreparedFormatData *prepared_format_data,
                   unsigned long long value) {
  char valueStr[1024] = {'\0'};  // Буфер для строкового представления значения

  // Преобразуем беззнаковое целое значение в строку с учетом параметров форматирования
  utoa(valueStr, value, prepared_format_data);

  // Обрабатываем строковое представление значения и добавляем его в строку
  minus_handler(str, valueStr, prepared_format_data, (long double)value);

  return 0;  // Возвращаем 0 после успешной записи значения
}


/**
 * @brief Добавляет нули в строку в соответствии с параметрами форматирования.
 *
 * Функция принимает строку `str`, в которую будут добавлены нули, и указатель
 * `prepared_format_data` на структуру с данными для форматирования. Она учитывает
 * точность (precision) и спецификатор формата, чтобы решить, нужно ли добавлять нули.
 *
 * @param str Указатель на строку, в которую будут добавлены нули.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @return Возвращает 0 после успешного добавления нулей.
 */
int write_zeroes(char *str, struct PreparedFormatData *prepared_format_data) {
  int tmp = prepared_format_data->precision;  // Временная переменная для хранения значения точности

  const char ignore[] = "feEgG";  // Массив спецификаторов, для которых добавление нулей игнорируется
  // Добавляем нули, пока значение точности больше нуля и спецификатор не принадлежит массиву ignore
  while (tmp > 0 &&
         s21_strchr(ignore, prepared_format_data->specifier) == S21_NULL) {
    s21_strcat(str, "0");  // Добавляем нуль в строку
    prepared_format_data->counter++;  // Увеличиваем счетчик символов
    tmp--;  // Уменьшаем значение временной переменной точности
  }
  return 0;  // Возвращаем 0 после успешного добавления нулей
}


/**
 * @brief Добавляет знак числа в строку в соответствии с параметрами форматирования.
 *
 * Функция принимает строку `str`, значение `value` и указатель
 * `prepared_format_data` на структуру с данными для форматирования. Она учитывает,
 * нужно ли добавлять знак минуса для отрицательных чисел, знак плюса для положительных чисел или пробел.
 *
 * @param str Указатель на строку, в которую будет добавлен знак.
 * @param value Значение числа, для которого нужно добавить знак.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @return Возвращает 0 после успешного добавления знака.
 */
int write_sign(char *str, long double value,
               struct PreparedFormatData *prepared_format_data) {
  // Если спецификатор не 'u' (unsigned), добавляем знак
  if (prepared_format_data->specifier != 'u') {
    // Если значение отрицательное, добавляем знак минуса
    if (value < 0) {
      s21_strcat(str, "-");
      prepared_format_data->counter++;  // Увеличиваем счетчик символов
    } 
    // Если флаг плюс установлен, добавляем знак плюса
    else if (prepared_format_data->flags.plus != 0) {
      s21_strcat(str, "+");
      prepared_format_data->counter++;  // Увеличиваем счетчик символов
    } 
    // Если флаг пробела установлен, добавляем пробел
    else if (prepared_format_data->flags.spase != 0) {
      s21_strcat(str, " ");
      prepared_format_data->counter++;  // Увеличиваем счетчик символов
    }
  }
  return 0;  // Возвращаем 0 после успешного добавления знака
}


/**
 * @brief Удаляет конечные нули в строке, представляющей число, с учетом параметров форматирования.
 *
 * Функция принимает строку `str` и указатель `prepared_format_data` на структуру с данными для форматирования.
 * Она удаляет конечные нули в строке, если флаг `sharp` не установлен и указана точность.
 * Если последним символом остается точка, она также удаляется.
 *
 * @param str Указатель на строку, в которой нужно удалить конечные нули.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @return Возвращает 0 после успешного удаления нулей.
 */
int remove_zero(char *str, struct PreparedFormatData *prepared_format_data) {
  // Находим конечную позицию в строке
  int end_position = (int)s21_strlen(str) - 1;

  // Проверяем, что строка не пустая
  if (end_position > 0) {
    // Удаляем конечные нули, если флаг sharp не установлен и указана точность
    while (str[end_position] == '0' && !prepared_format_data->flags.sharp &&
           prepared_format_data->has_precision) {
      str[end_position] = '\0';  // Заменяем нуль-символ на конец строки
      end_position--;  // Сдвигаем конечную позицию
      prepared_format_data->counter--;  // Уменьшаем счетчик символов
      prepared_format_data->width++;  // Увеличиваем ширину
    }
    // Удаляем точку, если она осталась последним символом
    if (!prepared_format_data->flags.sharp && str[end_position] == '.') {
      str[end_position] = '\0';  // Заменяем точку на конец строки
      prepared_format_data->counter--;  // Уменьшаем счетчик символов
      prepared_format_data->width++;  // Увеличиваем ширину
    }
  }
  return 0;  // Возвращаем 0 после успешного удаления нулей
}

/**
 * @brief Удаляет лишние нули из строки, представляющей число, если спецификатор форматирования 'g' или 'G'.
 *
 * Функция проверяет, является ли спецификатор форматирования 'g' или 'G'.
 * Если да, то вызывает функцию remove_zero для удаления конечных нулей и, при необходимости, точки.
 *
 * @param value_str Указатель на строку, в которой нужно удалить конечные нули.
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @return Возвращает 0 после успешного выполнения.
 */
int remove_zeroes_in_g(char *value_str,
                       struct PreparedFormatData *prepared_format_data) {
  // Проверяем, является ли спецификатор 'g' или 'G'
  if (s21_strchr("gG", prepared_format_data->specifier) != S21_NULL) {
    // Вызываем функцию remove_zero для удаления конечных нулей и точки
    remove_zero(value_str, prepared_format_data);
  }
  return 0;  // Возвращаем 0 после успешного выполнения
}


/**
 * @brief Уменьшает ширину на величину точности, если точность больше нуля.
 *
 * Функция проверяет, больше ли точность (`precision`) нуля.
 * Если да, то уменьшает значение ширины (`width`) на величину точности (`precision`).
 *
 * @param prepared_format_data Указатель на структуру PreparedFormatData с данными для форматирования.
 * @return Возвращает 0 после успешного выполнения.
 */
int decrease_zeroes_width(struct PreparedFormatData *prepared_format_data) {
  // Проверяем, больше ли точность нуля
  if (prepared_format_data->precision > 0) {
    // Уменьшаем ширину на величину точности
    prepared_format_data->width -= prepared_format_data->precision;
  }
  return 0;  // Возвращаем 0 после успешного выполнения
}
  
/**
 * @brief Уменьшает ширину форматирования в зависимости от наличия знаков и флагов в формате числа.
 *
 * Функция уменьшает ширину `width` в структуре `prepared_format_data`, если текущее значение
 * числа `value` отрицательное (`value < 0`) или если присутствуют флаги `+` или ` ` (пробел) и `value >= 0`.
 * Уменьшение ширины происходит только в том случае, если спецификатор формата не равен 'u' (без знака).
 *
 * @param value Текущее значение числа.
 * @param prepared_format_data Указатель на структуру `PreparedFormatData` с данными для форматирования.
 * @return Всегда возвращает 0.
 */
int decrease_sign_width(long double value,
                        struct PreparedFormatData *prepared_format_data) {
  if (prepared_format_data->specifier != 'u') {
    // Проверка на отрицательное значение или присутствие флагов `+` или ` ` (пробел) и положительное значение
    if (value < 0 || (prepared_format_data->flags.plus != 0 && value >= 0) ||
        prepared_format_data->flags.spase != 0) {
      prepared_format_data->width--;  // Уменьшаем ширину
    }
  }
  return 0;  // Всегда возвращаем 0
}


/**
 * @brief Определяет количество цифр в целой части числа типа long double.
 *
 * Функция определяет количество разрядов в целой части числа типа long double `value`.
 * Целая часть числа определяется путем деления значения `value` на 10 до тех пор, пока
 * его абсолютное значение не станет меньше 1.
 *
 * @param value Значение типа long double, для которого нужно определить количество цифр в целой части.
 * @return Количество цифр в целой части числа.
 */
int double_length(long double value) {
  int length = 0;
  while (fabsl(value) >= 1) {
    length++;
    value /= 10;
  }
  return length;
}


/**
 * @brief Определяет количество цифр в целочисленном значении типа long long int.
 *
 * Функция определяет количество цифр в целочисленной части числа типа long long int `value`.
 * Целая часть числа определяется путем деления значения `value` на 10 до тех пор, пока
 * оно не станет равным 0.
 *
 * @param value Значение типа long long int, для которого нужно определить количество цифр.
 * @return Количество цифр в целочисленной части числа.
 */
int integer_length(long long int value) {
  int length = 0;
  while (value != 0) {
    length++;
    value /= 10;
  }
  return length;
}


/**
 * @brief Определяет количество цифр в беззнаковом целочисленном значении типа unsigned long long.
 *
 * Функция определяет количество цифр в беззнаковой части числа типа unsigned long long `value`.
 * Беззнаковая часть числа определяется путем деления значения `value` на 10 до тех пор,
 * пока оно не станет равным 0.
 *
 * @param value Значение типа unsigned long long, для которого нужно определить количество цифр.
 * @return Количество цифр в беззнаковой части числа.
 */
int unsigned_length(unsigned long long value) {
  int length = 0;
  while (value != 0) {
    length++;
    value /= 10;
  }
  return length;
}


/**
 * @brief Проверяет, является ли значение бесконечностью или NaN, и обновляет строку и параметры форматирования.
 *
 * Функция проверяет, является ли переданное значение `value` бесконечностью (INFINITY) или NaN (не числом).
 * Если значение является бесконечностью, в строку `value_str` записывается "inf".
 * Если значение является NaN, в строку `value_str` записывается "nan".
 * В обоих случаях обновляются параметры форматирования в структуре `PreparedFormatData`.
 *
 * @param value_str Указатель на строку, куда будет записано "inf" или "nan" при необходимости.
 * @param PreparedFormatData Указатель на структуру PreparedFormatData с данными для форматирования.
 * @param value Проверяемое значение.
 * @return Возвращает 1, если значение является бесконечностью или NaN, иначе 0.
 */
int is_NaN_or_INF(char *value_str,
                  struct PreparedFormatData *PreparedFormatData,
                  long double value) {
  int is_nanInf = 0;  // Флаг, указывающий, является ли значение бесконечностью или NaN

  // Проверка на бесконечность
  if (fabsl(value) == INFINITY) {
    // Записываем "inf" в строку
    value_str[0] = 'i';
    value_str[1] = 'n';
    value_str[2] = 'f';
    
    // Обновляем параметры форматирования
    PreparedFormatData->counter += 3;
    PreparedFormatData->width -= 3;
    PreparedFormatData->precision = 0;
    
    is_nanInf = 1;  // Устанавливаем флаг
  } 
  // Проверка на NaN
  else if (s21_isNan(value)) {
    // Записываем "nan" в строку
    value_str[0] = 'n';
    value_str[1] = 'a';
    value_str[2] = 'n';
    
    // Обновляем параметры форматирования
    PreparedFormatData->counter += 3;
    PreparedFormatData->width -= 3;
    PreparedFormatData->precision = 0;
    
    is_nanInf = 1;  // Устанавливаем флаг
  }
  
  return is_nanInf;  // Возвращаем флаг
}


/**
 * @brief Преобразует символы в строке value_str из нижнего регистра в верхний регистр для шестнадцатеричных чисел.
 *
 * Функция проходит по каждому символу в строке value_str и, если символ находится в диапазоне 'a'-'z',
 * то он преобразуется в верхний регистр, путем вычитания 32 (значение ASCII для преобразования).
 *
 * @param value_str Строка, в которой производится преобразование символов.
 * @return Всегда возвращает 0.
 */
int x_to_upper(char *value_str) {
  for (s21_size_t i = 0; i < s21_strlen(value_str); i++) {
    if (value_str[i] >= 'a' && value_str[i] <= 'z') {
      value_str[i] -= 32;
    }
  }
  return 0;
}


/**
 * @brief Преобразует строку str в целое число типа int.
 *
 * Функция игнорирует начальные пробельные символы (пробелы, табуляции и т. д.).
 * Затем она определяет знак числа (положительный или отрицательный).
 * После этого функция считывает цифровые символы из строки str и вычисляет результат,
 * умножая каждую цифру на 10 и добавляя её к result.
 * Если строка начинается с символа '-', число считается отрицательным.
 *
 * @param str Строка, которую нужно преобразовать в целое число.
 * @return Преобразованное целое число типа int.
 */
int s21_atoi(char *str) {
  int result = 0;
  int sign = 1;

  // Пропускаем начальные пробельные символы
  str += s21_strspn(str, " \n\t\v\f\r");

  // Определяем знак числа
  if (*str == '+') {
    str++;
  } else if (*str == '-') {
    sign = -1;
    str++;
  }

  // Считываем цифры и вычисляем результат
  while (*str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    str++;
  }

  return result * sign;
}

/**
 * @brief Преобразует беззнаковое целое число в строку.
 *
 * Функция конвертирует значение value в строку value_str с учетом настроек форматирования
 * из prepared_format_data.
 * Если значение value равно 0 и precision равно 0 (или не установлено), функция добавляет '0' в строку.
 * В противном случае функция преобразует каждую цифру числа value в символ и добавляет в строку.
 *
 * @param value_str Указатель на строку, в которую записывается результат.
 * @param value Беззнаковое целое число для преобразования в строку.
 * @param prepared_format_data Структура с данными о форматировании.
 * @return Всегда возвращает 0.
 */
int utoa(char *value_str, unsigned long long value,
         struct PreparedFormatData *prepared_format_data) {
  if (value == 0 && (prepared_format_data->has_precision == 0 ||
                     (prepared_format_data->has_precision &&
                      prepared_format_data->precision != 0))) {
    // Если значение равно 0 и нет необходимости в точности (или точность не равна 0),
    // добавляем '0' в строку value_str.
    value_str[0] = '0';
    prepared_format_data->precision--;
    prepared_format_data->counter++;
  } else {
    // Иначе преобразуем каждую цифру числа value в символ и добавляем в строку.
    int i = 0;
    int length = unsigned_length(value); // Определяем количество цифр в числе value
    i += length - 1;
    prepared_format_data->precision -= length;

    while (value != 0) {
      char residue = (value % 10) + 48; // Получаем ASCII-код цифры
      value_str[i] = residue;           // Добавляем цифру в строку справа налево
      value /= 10;                      // Уменьшаем значение числа на порядок
      i--;
      prepared_format_data->counter++;  // Увеличиваем счетчик символов
    }
  }
  prepared_format_data->width -= s21_strlen(value_str); // Вычитаем длину строки из ширины

  return 0;
}


/**
 * @brief Преобразует десятичное число в шестнадцатеричное или восьмеричное представление.
 *
 * Функция конвертирует беззнаковое целое число value в строку value_str с указанным основанием base.
 * Использует pattern для получения символов шестнадцатеричного представления.
 * Обрабатывает точность из prepared_format_data для обработки нулевых значений.
 *
 * @param value_str Указатель на строку, в которую записывается результат.
 * @param value Беззнаковое целое число для преобразования.
 * @param prepared_format_data Структура с данными о форматировании.
 * @param base Основание системы счисления (8 для восьмеричной, 16 для шестнадцатеричной).
 * @return Всегда возвращает 0.
 */
int decimal_to_hexoct(char *value_str, unsigned long long value,
                      struct PreparedFormatData *prepared_format_data,
                      int base) {
  const char pattern[17] = {"0123456789abcdef"}; // Шаблон для символов шестнадцатеричной системы
  char str[1024] = {'\0'}; // Временный массив для хранения преобразованной строки
  int i = 0;
  int residue = 0;

  // Обработка случая, когда value равно 0 и есть точность или точность не указана
  if (value == 0 && ((prepared_format_data->has_precision == 1 &&
                      prepared_format_data->precision > 0) ||
                     (prepared_format_data->has_precision == 0))) {
    residue = value % base;
    value = value / base;
    str[i++] = pattern[residue];
  } else if (value == 0 && prepared_format_data->specifier == 'o' &&
             ((prepared_format_data->has_precision == 1 &&
               prepared_format_data->precision > 0) ||
              (prepared_format_data->has_precision == 0))) {
    str[i++] = '0';
  }

  // Преобразование числа value в строку в указанном основании системы счисления
  while (value != 0) {
    residue = value % base;
    value = value / base;
    str[i++] = pattern[residue];
  }
  i--;

  // Обратное копирование строки str в value_str для корректного порядка символов
  int j = 0;
  while (i >= 0) {
    value_str[j++] = str[i--];
  }

  return 0;
}


/**
 * @brief Преобразует число типа long double в строку с плавающей запятой.
 *
 * Функция конвертирует число типа long double в строку value_str с использованием
 * заданных параметров форматирования в prepared_format_data.
 * Использует другие вспомогательные функции, такие как write_double, write_dot и mantissa,
 * для обработки разных частей числа (целая и дробная части, точность и т.д.).
 *
 * @param value_str Указатель на строку, в которую записывается результат.
 * @param prepared_format_data Структура с данными о форматировании.
 * @param value Значение типа long double для преобразования.
 * @return Всегда возвращает 0.
 */
int ftoa(char *value_str, struct PreparedFormatData *prepared_format_data,
         long double value) {
  long double tmp = floorl(fabsl(value)); // Округляем абсолютное значение value вниз и сохраняем в tmp
  if (value < 0) {
    tmp = -tmp; // Если value отрицательное, приводим tmp к отрицательному значению
  }
  long double exponent = value - tmp; // Определяем дробную часть числа (экспоненту)
  long long precision = 6; // Устанавливаем точность по умолчанию
  int has_precision = 1; // Флаг наличия точности

  // Проверяем задана ли точность в prepared_format_data
  if (prepared_format_data->has_precision != 0) {
    precision = prepared_format_data->precision;

    // Если точность равна 0, обрабатываем особый случай
    if (prepared_format_data->precision == 0) {
      prepared_format_data->has_precision = 0;
      tmp = roundl(value); // Округляем value
      has_precision = 0; // Устанавливаем флаг отсутствия точности
    } else {
      // Иначе округляем value с учетом точности
      tmp = floorl(fabsl(roundl(value * powl(10, precision + 1)) /
                         powl(10, precision + 1)));
      if (value < 0) {
        tmp = -tmp;
      }
    }
  }

  // Записываем целую часть числа в строку value_str
  write_double(value_str, tmp, prepared_format_data);

  // Если есть точность, записываем точку и обрабатываем мантиссу
  if (has_precision == 1) {
    write_dot(&value_str, prepared_format_data); // Записываем точку

    // Обрабатываем мантиссу и дописываем нули при необходимости
    tmp = mantissa(&value_str, exponent, &precision, prepared_format_data);

    if (tmp == 0) {
      // Если мантисса равна нулю, дописываем нули до заданной точности
      for (int i = 0; i < precision; i++) {
        *value_str = '0';
        prepared_format_data->counter++;
        prepared_format_data->width--;
        prepared_format_data->precision--;
        value_str++;
      }
    } else {
      // Иначе записываем дробную часть числа
      write_double(value_str, tmp, prepared_format_data);
    }

  } else if (prepared_format_data->flags.sharp == 1) {
    write_dot(&value_str, prepared_format_data); // Дописываем точку, если установлен флаг sharp
  }

  return 0; // Возвращаем 0, указывая на успешное завершение функции
}


/**
 * @brief Преобразует целое число в строку.
 *
 * Функция конвертирует целое число value в строку value_str с использованием
 * заданных параметров форматирования в prepared_format_data.
 *
 * @param value_str Указатель на строку, в которую записывается результат.
 * @param value Значение типа long long int для преобразования.
 * @param prepared_format_data Структура с данными о форматировании.
 * @return Всегда возвращает 0.
 */
int itoa(char *value_str, long long int value,
         struct PreparedFormatData *prepared_format_data) {
  // Обработка нулевого значения
  if (value == 0 && (prepared_format_data->has_precision == 0 ||
                     (prepared_format_data->has_precision &&
                      prepared_format_data->precision != 0))) {
    value_str[0] = '0';
    prepared_format_data->precision--;
    prepared_format_data->counter++;
  } else {
    int i = 0;
    int sign = 1;

    // Определяем знак числа
    if (value < 0) {
      sign = -1;
    }

    // Определяем длину целого числа
    int length = integer_length(value);
    i += length - 1;
    prepared_format_data->precision -= length;

    // Преобразование числа в строку
    while (value != 0) {
      char residue = (value % 10) * sign + 48;
      value_str[i] = residue;
      value /= 10;
      i--;
      prepared_format_data->counter++;
    }
  }

  // Уменьшаем ширину на длину строки
  prepared_format_data->width -= s21_strlen(value_str);
  
  return 0;
}