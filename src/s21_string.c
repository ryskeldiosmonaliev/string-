#include<stdio.h>
#include <ctype.h>
#include<stdlib.h>
#include "s21_string.h"
#define MAX_ERRLIST 107
#define MIN_ERRLIST -1
#define ERROR "Unknown error: "


void *s21_memchr(const void *str, int c, size_t n);//1+++ на вход (строка,символ,число) ищет заданный символ в строке в первых n ячейках памяти
int s21_memcmp(const void *str1, const void *str2, size_t n);//2+++ сравнивает строки и в зависимости от результата возвращает значение
void *s21_memcpy(void *dest, const void *src, size_t n);//3+++ вставлет часть строки src в dest 
void *s21_memset(void *str, int c, size_t n);//4+++ заполняет строку символами с
char *s21_strncat(char *dest, const char *src, size_t n);//5+++ соединяет строки без переполнения буффера
char *s21_strchr(const char *str, int c);//6+++ ищет первое совпадание символа и выводит позицию на которой он находится
char *s21_strcpy(char *dest, const char *src);
int s21_strncmp(const char *str1, const char *str2, size_t n);//7+++ сравнивает строки 
char *s21_strncpy(char *dest, const char *src, size_t n);//8+++ копирует определенное количество символов  из одной строки в другую
size_t s21_strcspn(const char *str1, const char *str2);//9+++ выводит строку которая не содержит символов из другой строки
char *s21_strerror(int errnum);//10+++ для вывода ошибок
size_t s21_strlen(const char *str);//11+++ вычисляет длинну строки
char *s21_strpbrk(const char *str1, const char *str2);//12+++ ищет первый символ который совпадает со второй стрококй
char *s21_strrchr(const char *str, int c);//13+++ поиск последнего вхождения подстроки в строку
char *s21_strstr(const char *haystack, const char *needle);//14+++ ищет подстроку в строке и выводит номер символа начала подстроки в строке
char *s21_strtok(char *str, const char *delim);//15+++ делит строку на токены по разделителям
void *s21_to_upper(const char *str);//16+++ чистить память в мейне при тестах повышает регистр всех букв
void *s21_to_lower(const char *str);//17+++ чистить память в мейне при тестах понижает регистр всех букв
void *s21_insert(const char *src, const char *str, size_t start_index);//18+++ чистить память в мейне при тестах \\ используетс для помещения символа на определенное место в строке
void *s21_trim(const char *src, const char *trim_chars);//19+++ чистить память в мейне при тестах убирает лишние пробелы в конце и в начале строк
int s21_end_with(const char *src, const char *totrim, int offset);//побочная функция
int s21_start_with(const char *src, const char *totrim, int offset);// побочные функицюя
size_t s21_strspn(const char *str, const char *keys); // ищет саму юдлинную последовательность символов в первой строке которая состоит из символов второй строки

size_t s21_strspn(const char *str, const char *keys) {
  // Получаем длину исходной строки
  size_t s21_strlength = s21_strlen(str);
  
  // Инициализируем результат нулем
  size_t result = 0;

  // Цикл по каждому символу в строке
  for (size_t x = 0; result == 0 && x < s21_strlength; x ++) {
    // Проверяем, является ли текущий символ частью строки ключевых символов
    if (s21_strchr(keys, str[x])!= NULL) {
      // Если да, увеличиваем счетчик результата на 1 и продолжаем цикл
      result = x + 1;
    }
  }

  // Возвращаем результат - длину наибольшей подстроки, состоящей только из символов из строки keys
  return result;
}


static const char *s21_errorList[] = {
    "Undefined error: 0",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "Device not configured",
    "Argument list too long",
    "Exec format error",
    "Bad file descriptor",
    "No child processes",
    "Resource deadlock avoided",
    "Cannot allocate memory",
    "Permission denied",
    "Bad address",
    "Block device required",
    "Resource busy",
    "File exists",
    "Cross-device link",
    "Operation not supported by device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many open files in system",
    "Too many open files",
    "Inappropriate ioctl for device",
    "Text file busy",
    "File too large",
    "No space left on device",
    "Illegal seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Numerical argument out of domain",
    "Result too large",
    "Resource temporarily unavailable",
    "Operation now in progress",
    "Operation already in progress",
    "Socket operation on non-socket",
    "Destination address required",
    "Message too long",
    "Protocol wrong type for socket",
    "Protocol not available",
    "Protocol not supported",
    "Socket type not supported",
    "Operation not supported",
    "Protocol family not supported",
    "Address family not supported by protocol family",
    "Address already in use",
    "Can't assign requested address",
    "Network is down",
    "Network is unreachable",
    "Network dropped connection on reset",
    "Software caused connection abort",
    "Connection reset by peer",
    "No buffer space available",
    "Socket is already connected",
    "Socket is not connected",
    "Can't send after socket shutdown",
    "Too many references: can't splice",
    "Operation timed out",
    "Connection refused",
    "Too many levels of symbolic links",
    "File name too long",
    "Host is down",
    "No route to host",
    "Directory not empty",
    "Too many processes",
    "Too many users",
    "Disc quota exceeded",
    "Stale NFS file handle",
    "Too many levels of remote in path",
    "RPC struct is bad",
    "RPC version wrong",
    "RPC prog. not avail",
    "Program version wrong",
    "Bad procedure for program",
    "No locks available",
    "Function not implemented",
    "Inappropriate file type or format",
    "Authentication error",
    "Need authenticator",
    "Device power is off",
    "Device error",
    "Value too large to be stored in data type",
    "Bad executable (or shared library)",
    "Bad CPU type in executable",
    "Shared library version mismatch",
    "Malformed Mach-o file",
    "Operation canceled",
    "Identifier removed",
    "No message of desired type",
    "Illegal byte sequence",
    "Attribute not found",
    "Bad message",
    "EMULTIHOP (Reserved)",
    "No message available on STREAM",
    "ENOLINK (Reserved)",
    "No STREAM resources",
    "Not a STREAM",
    "Protocol error",
    "STREAM ioctl timeout",
    "Operation not supported on socket",
    "Policy not found",
    "State not recoverable",
    "Previous owner died",
    "Interface output queue is full"};

char *s21_strcpy(char *dest, const char *src) {
  char *ret = dest;
  while (*src != '\0') {
    *dest = *src;
    ++dest;
    ++src;
  }
  *dest = *src;
  return ret;
}
// Функция для определения длины строки в C.
size_t s21_strlen(const char *str) {
    // Инициализация указателя str1 с началом строки str.
    const char *str1 = str;

    // Цикл продолжается до тех пор, пока не будет достигнут символ конца строки ('\0').
    while (*str1!= '\0') {
        // Увеличение указателя str1 на один шаг вперед к следующему символу строки.
        str1++;
    }

    // Возвращаем разницу между концом строки и началом строки, что дает нам длину строки.
    return str1 - str;
}


// Функция s21_memchr ищет первое вхождение указанного символа 'c' в строке 'str', начиная с позиции 0.
void *s21_memchr(const void *str, int c, size_t n) {
    // Приводим указатель на void* к указателю на unsigned char*, чтобы работать с байтами.
    const unsigned char* str1 = (const unsigned char*)str;

    // Проходим по каждому байту строки до тех пор, пока не достигнем конца или найдем символ 'c'.
    for (size_t i = 0; i < n; ++i) {
        // Сравниваем текущий байт со значением 'c'. Если они равны, возвращаем указатель на этот байт.
        if (str1[i] == (unsigned char)c) {
            return (char*)(str1 + i); // Возвращаем адрес найденного символа, приведенный к типу char*.
        }
    }

    // Если символ 'c' не найден в пределах заданной длины 'n', возвращаем NULL.
    return NULL;
}


// Функция сравнения двух блоков памяти на равенство.
int s21_memcmp(const void *str1, const void *str2, size_t n) {
    // Преобразование указателей на void в указатели на unsigned char для удобства работы с байтами.
    const unsigned char* str11 = (const unsigned char*)str1;
    const unsigned char* str12 = (const unsigned char*)str2;

    // Цикл по каждому байту в заданном диапазоне.
    for(size_t i = 0; i < n; ++i) {
        // Сравнение текущих байтов двух строк. Если они не равны, возвращаем разницу между ними.
        if(str11[i]!= str12[i]) {
            return str11[i] - str12[i]; 
        }
    }

    // Если все байты совпали, возвращаем 0, что означает равенство блоков памяти.
    return 0; 
}


void *s21_memcpy(void *dest, const void *src, size_t n) {
    // Преобразование указателей на void в указатели на unsigned char для удобства работы с байтами.
    const unsigned char* src_str = (const unsigned char*)src;
    unsigned char* dest_str = (unsigned char*)dest;

    // Копирование каждого байта из исходного массива в целевой массив.
    for(size_t i = 0; i < n; ++i) {
        dest_str[i] = src_str[i];
    }

    // Возвращение указателя на начало целевого массива после завершения операции копирования.
    return dest;
}

// Функция s21_memset заполняет первые 'n' байтов области памяти, на которую указывает 'str', символом 'c'.
void *s21_memset(void *str, int c, size_t n) {
    // Преобразование указателя 'str' в указатель типа unsigned char*, чтобы обеспечить корректное присваивание байтам.
    unsigned char* str1 = (unsigned char*)str;

    // Цикл по 'n' байтам, начиная с первого байта области памяти, на которую указывает 'str'.
    for(size_t i = 0; i < n; ++i) {
        // Присваивание каждого байта значения 'c'. Это изменяет каждый из первых 'n' байтов на значение 'c'.
        str1[i] = (unsigned char)c;
    }

    // Возвращение указателя на исходный адрес 'str', так как функция должна возвращать указатель на измененную область памяти.
    return str;
}


int s21_strncmp(const char *str1, const char *str2, size_t n) {
    // Проверяем строки на равенство до n-го символа включительно.
    while(n-- && (*str1!= '\0' || *str2!= '\0')) {
        // Если текущие символы не равны и не являются нулевым символом,
        // возвращаем разницу между ними. Это позволяет определить порядок следования символов.
        if(*str1!= *str2) {
            return *str1 - *str2;
        }
        // Переходим к следующим символам в обеих строках.
        str1++;
        str2++;
    }
    // Если все символы до n-го включительно совпадают или достигнут конец одной из строк,
    // возвращаем 0, указывая на их равенство.
    return 0;
}


size_t s21_strcspn(const char *str1, const char *str2) {
    // Инициализация указателя p на начало строки str1
    const char *p = str1;

    // Цикл по всем символам строки str1
    while (*p!= '\0') {
        // Переменная found для отслеживания наличия совпадения с символами из str2
        int found = 0;

        // Инициализация указателя q на начало строки str2
        const char *q = str2;

        // Внутренний цикл для проверки каждого символа str2 на совпадение с текущим символом str1
        while (*q!= '\0' &&!found) {
            // Если текущие символы совпадают, устанавливаем found в 1 и прерываем внутренний цикл
            if (*p == *q) {
                found = 1;
            }
            q++; // Переход к следующему символу в str2
        }

        // Если после проверки всех символов str2 совпадений не найдено (found остался равным 0),
        // переходим к следующему символу в str1
        if (!found) {
            p++;
        } 
        // В противном случае (совпадение найдено), прерываем цикл
        else {
            break;
        }
    }

    // Возвращаем разницу между указателем p (который указывает на первый символ после последнего совпадения)
    // и исходным указателем str1, что дает нам длину неповторяющегося начала строки str1
    return p - str1;
}


// Функция s21_strchr ищет первый вхождение символа 'c' в строку 'str'.
// Возвращает указатель на первый найденный символ или NULL, если символ не найден.

char *s21_strchr(const char *str, int c) {
    // Проходим по каждому символу строки 'str', начиная с первого ('*str').
    while(*str!= '\0') { // Пока текущий символ не является нулевым символом конца строки.
        // Если текущий символ равен искомому символу 'c',
        // то возвращаем указатель на этот символ.
        if(*str == c) {
            return (char*)str;
        }
        // Переход к следующему символу в строке.
        str++;
    }
    // Если после прохода по всей строке символ 'c' не был найден,
    // возвращаем NULL.
    return NULL;
}


char *s21_strncat(char *dest, const char *src, size_t n) {
  char *ret = dest;
  while (*ret) {
    ret++;
  }
  size_t len_src = s21_strlen(src);
  size_t i = 0;
  while (i < n) {
    if (i == len_src) {
      break;
    }
    *ret++ = src[i];
    i++;
  }
  *ret = '\0';
  return dest;
}


char *s21_strncpy(char *dest, const char *src, size_t n) {
    // Инициализация счетчика для итерации по строке src
    size_t i;

    // Копирование символов из src в dest до тех пор, пока не достигнут конец строки src или не будет скопировано n символов
    for(i = 0; i < n && src[i]!= '\0'; i++) {
        dest[i] = src[i];
    }

    // Добавление нулевого символа ('\0') в конец dest, если было скопировано меньше n символов
    // Это гарантирует, что dest всегда будет корректной строкой (т.е., заканчивающейся нулевым символом)
    for(; i < n; i++) {
        dest[i] = '\0';
    }

    // Возвращение указателя на начало строки dest
    return dest;
}


// Функция для преобразования номера ошибки в строку с описанием ошибки.
char *s21_strerror(int errorNumber) {
  // Инициализация статического массива символов, который будет использоваться для хранения результата.
  static char result[512] = {'\0'};

  // Проверка, находится ли номер ошибки вне допустимого диапазона.
  if (errorNumber <= MIN_ERRLIST || errorNumber >= MAX_ERRLIST) {
    // Если номер ошибки не соответствует ни одному из известных кодов ошибок,
    // формируется строка с сообщением о неизвестной ошибке.
    s21_sprintf(result, "Unknown error: %d", errorNumber);
  } else {
    // В противном случае, если номер ошибки соответствует одному из известных,
    // копируется соответствующее описание ошибки из глобального списка ошибок.
    s21_strcpy(result, ((char *)s21_errorList[errorNumber]));
  }

  // Возвращение строки с описанием ошибки.
  return result;
}


// Функция s21_strpbrk ищет первый вхождение символов из str2 в str1.
char *s21_strpbrk(const char *str1, const char *str2) {
    // Инициализация указателей для прохода по строкам.
    const char *ptr1 = str1;
    const char *ptr2 = str2;

    // Цикл по всем символам первой строки.
    while (*ptr1) {
        // Внутренний цикл проверяет каждый символ второй строки.
        for (; *ptr2; ptr2++) {
            // Если текущие символы совпадают, возвращаем указатель на этот символ в str1.
            if (*ptr1 == *ptr2) {
                return (char *)ptr1;
            }
        }
        // Переход к следующему символу в первой строке.
        ptr1++;
    }

    // Если ни один совпадения не найдено, возвращаем NULL.
    return NULL;
}


// Функция для поиска последнего вхождения символа 'c' в строку 'str'.
char *s21_strrchr(const char *str, int c) {
    // Указатель на начало строки.
    const char* start = str;
    // Указатель на конец строки, вычисленный с помощью длины строки.
    const char* end = str + s21_strlen(str); 

    // Цикл, который итерирует от конца строки к началу.
    while (end >= start) {
        // Проверяем, равен ли текущий символ искомому символу 'c'.
        if (*end == c) {
            // Если да, то возвращаем указатель на этот символ.
            return (char*)end; 
        }
        // Сдвигаем указатель назад, т.е., двигаемся к предыдущему символу.
        end--;
    }

    // Если символ 'c' не найден, возвращаем NULL.
    return NULL; 
}


// Функция для поиска подстроки 'needle' в строке 'haystack'.
char *s21_strstr(const char *haystack, const char *needle) {
    // Инициализация указателей на начало строки и подстроки соответственно.
    const char* h = haystack;
    const char* n = needle;

    // Если подстрока пуста (т.е., состоит только из нулевого символа), 
    // то считаем, что она присутствует в любом месте исходной строки.
    if (*n == '\0') {
        return (char*)haystack; // Возвращаем указатель на начало исходной строки.
    }

    // Цикл поиска подстроки в строке.
    while (*h) {
        // Проверяем, совпадает ли текущий символ строки с первым символом подстроки.
        if (*h == *n && s21_memcmp(h, n, s21_strlen(n)) == 0) {
            // Если да, то возвращаем указатель на найденное место в строке.
            return (char*)h;
        }
        // Переход к следующему символу в строке.
        h++;
    }

    // Если подстрока не найдена в строке, возвращаем NULL.
    return NULL;
}


// Функция для разбиения строки на токены по заданным разделителям
char *s21_strtok(char *str, const char *delim) {
  // Статические переменные для хранения состояния между вызовами функции
  static int next, restok;
  static size_t idx;
  static char *ptr;
  static char *addr;

  // Проверяем, была ли передана строка для разбиения
  if (str) {
    // Сброс переменных состояния
    restok = 1;
    idx = 0, next = 0;
    
    // Определение начала следующего токена путем пропуска пробельных символов в начале строки
    addr = str + s21_strspn(str, delim);
    ptr = str + s21_strlen(str); // Установка указателя на конец строки
  }

  // Инициализация результата
  char *res = NULL;

  // Проверка условий для начала разбиения строки
  if (!(addr >= ptr || next)) {
    int exdelim = 1; // Флаг, указывающий на наличие разделителя
    
    // Перебор символов в адресе до конца токена
    for (int i = 0; addr[i]; i++) {
      size_t step = s21_strspn(addr + i, delim); // Вычисление длины непрерывной подстроки разделителей
      
      // Если подстрока разделителей найдена, заменяем её нулями
      if (step!= 0) {
        exdelim = 0;
        for (size_t j = i; j < i + step; j++) addr[j] = '\0';
        
        // Обновление индекса и выход из цикла
        idx = step + i;
        break;
      }
    }
    
    // Если был найден разделитель и это первый вызов функции, устанавливаем флаг next
    if (restok == 1 && exdelim) next = 1;
    
    // Указываем результат на начало токена
    res = addr;
    addr += idx; // Перемещаем указатель на начало следующего токена
  }
  
  // Увеличиваем счетчик разбиений
  restok ++;
  
  // Возвращаем указатель на начало текущего токена
  return res;
}


void *s21_to_upper(const char *str) {
  // Выделяем память под новую строку, которая будет содержать результат преобразования.
  // Размер вычисляется на основе длины исходной строки и добавляем один символ для нулевого конца строки.
  char *newstr = calloc(s21_strlen(str) + 1, 1);

  // Проходим по каждому символу в исходной строке.
  for (unsigned long long i = 0; i < s21_strlen(str); i++) {
    // Проверяем, является ли текущий символ строчной буквой.
    if (str[i] >= 'a' && str[i] <= 'z') {
      // Если да, то преобразуем его к верхнему регистру, уменьшив код символа на 32.
      newstr[i] = str[i] - 32;
    } else {
      // Если символ не является строчной буквой, копируем его без изменений.
      newstr[i] = str[i];
    }
  }

  // Возвращаем указатель на новую строку с преобразованными символами.
  return newstr;
}

// Функция преобразует все заглавные буквы в строке str в строчные и возвращает новую строку с результатом.
void *s21_to_lower(const char *str) {
  // Выделяем память под новую строку, размер которой равен длине исходной строки плюс один символ для нулевого конца строки.
  char *newstr = calloc(s21_strlen(str) + 1, 1);

  // Проходим по каждому символу исходной строки.
  for (unsigned long long i = 0; i < s21_strlen(str); i++) {
    // Если текущий символ не является заглавной буквой (его код ASCII меньше 'A' или больше 'Z'), копируем его в новую строку без изменений.
    if (str[i] < 'A' || str[i] > 'Z') {
      newstr[i] = str[i];
    } 
    // В противном случае, если символ является заглавной буквой, добавляем к его коду ASCII 32, чтобы получить соответствующую строчную букву.
    else {
      newstr[i] = str[i] + 32;
    }
  }

  // Возвращаем указатель на новую строку с преобразованными символами.
  return newstr;
}


void *s21_insert(const char *src, const char *str, size_t start_index) {
  // Инициализация переменных для хранения результатов и длин строк
  char *res = NULL;
  size_t src_length, str_length;

  // Вычисление длины исходной строки, если она не равна NULL
  if (src == NULL) src_length = 0;
  else src_length = s21_strlen(src);

  // Аналогично вычисляется длина вставляемой строки
  if (str == NULL) str_length = 0;
  else str_length = s21_strlen(str);

  // Вычисление общей длины результата
  size_t fin_length = src_length + str_length;

  // Если индекс вставки меньше или равен длине исходной строки,
  // выделение памяти под результирующую строку
  if (start_index <= src_length) {
    res = (char *)calloc(src_length + str_length + 1, sizeof(char)); // +1 для нулевого символа конца строки
  }

  // Проверка успешности выделения памяти
  if (res) {
    // Цикл по всем символам результирующей строки
    for (size_t i = 0; i < fin_length; i++) {
      // Копирование символов из исходной строки до места вставки
      if (i < start_index) {
        res[i] = src[i];
      } 
      // Копирование символов из вставляемой строки после места вставки
      else if (i < str_length + start_index) {
        res[i] = str[i - start_index];
      } 
      // Копирование оставшихся символов из исходной строки после вставки
      else {
        res[i] = src[i - str_length];
      }
    }
  }

  // Возвращение указателя на результирующую строку
  return res;
}

void *s21_trim(const char *src, const char *trim_chars) {
  // Инициализация указателя на новый строковый буфер
  char *newstr = NULL;

  // Проверка наличия исходной строки
  if (src) {
    // Проверка наличия символов для удаления и их первого элемента
    if (trim_chars && trim_chars[0]) {
      // Выделение памяти под новый буфер, размер которого равен длине исходной строки плюс один символ для нулевого конца строки
      newstr = calloc(s21_strlen(src) + 1, sizeof(char));

      // Определение начальной позиции в исходной строке
      size_t start = 0;
      // Определение конечной позиции в исходной строке
      size_t end = s21_strlen(src);

      // Поиск начала строки, которая начинается с символов для удаления
      while (s21_start_with(src, trim_chars, start)) {
        start++;
      }

      // Если начальная и конечная позиции различаются, значит есть символы для удаления с обоих концов
      if (start!= end) {
        // Поиск конца строки, который заканчивается на символы для удаления
        while (s21_end_with(src, trim_chars, end)) end--;
      } else {
        // Если нет символов для удаления, просто инициализируем новую строку пустой строкой
        newstr[0] = '\0';
      }

      // Копирование символов из исходной строки, пропуская символы до начала и после конца строки
      for (int i = 0; start < end; i++) {
        newstr[i] = src[start];
        start++;
      }
    } else {
      // Если символы для удаления не заданы, используем стандартные пробельные символы для удаления
      newstr = s21_trim(src, "\t\n ");
    }
  }
  // Возвращаем результат работы функции
  return newstr;
}


// Функция проверяет, заканчивается ли строка src на подстроку totrim,
// начиная с позиции offset в src. Возвращает 1, если условие выполняется,
// и 0 в противном случае.

int s21_end_with(const char *src, const char *totrim, int offset) {
  // Инициализация переменной для хранения результата.
  int res = 0;

  // Смещение индекса offset на одну позицию назад, так как индексы в C начинаются с 0.
  offset--;

  // Получение длины строки totrim.
  int size = s21_strlen(totrim);

  // Проверка каждой символа в totrim на соответствие символу в src, начиная с позиции offset.
  for (int i = 0; i < size; i++) {
    // Если текущий символ в src совпадает с текущим символом в totrim,
    // устанавливаем флаг res в 1, указывая на то, что строка src заканчивается на totrim.
    if (src[offset] == totrim[i]) res = 1;
  }

  // Возвращаем результат проверки.
  return res;
}


// Функция проверяет, начинается ли строка src с подстроки totrim, начиная с указанного смещения offset.
int s21_start_with(const char *src, const char *totrim, int offset) {
  // Инициализация переменной для хранения результата сравнения.
  int res = 0;

  // Получение длины строки totrim.
  int size = s21_strlen(totrim);

  // Цикл по каждому символу в строке totrim.
  for (int i = 0; i < size; i++) {
    // Сравнение текущего символа строки src с соответствующим символом строки totrim.
    // Если они совпадают и это первый символ после указанного смещения, устанавливаем результат в 1.
    if (src[offset + i] == totrim[i]) res = 1;
  }

  // Возвращаем результат сравнения.
  return res;
}

// #include<stdio.h>
// #include <ctype.h>
// #include<stdlib.h>
// #include "s21_string.h"
// char *s21_strcat(char *dest, const char *src);
// void *s21_memchr(const void *str, int c, s21_size_t n);//1+++
// int s21_memcmp(const void *str1, const void *str2, s21_size_t n);//2+++
// void *s21_memcpy(void *dest, const void *src, s21_size_t n);//3+++
// void *s21_memset(void *str, int c, s21_size_t n);//4+++
// char *s21_strncat(char *dest, const char *src, s21_size_t n);//5+++
// char *s21_strchr(const char *str, int c);//6+++
// int s21_strncmp(const char *str1, const char *str2, s21_size_t n);//7+++
// char *s21_strncpy(char *dest, const char *src, s21_size_t n);//8+++
// size_t s21_strcspn(const char *str1, const char *str2);//9+++
// char *s21_strerror(int errnum);//10+++
// size_t s21_strlen(const char *str);//11+++
// char *s21_strpbrk(const char *str1, const char *str2);//12+++
// char *s21_strrchr(const char *str, int c);//13+++
// char *s21_strstr(const char *haystack, const char *needle);//14+++
// char *s21_strtok(char *str, const char *delim);//15+++
// void *s21_to_upper(const char *str);//16+++ чистить память в мейне при тестах
// void *s21_to_lower(const char *str);//17+++ чистить память в мейне при тестах
// void *s21_insert(const char *src, const char *str, s21_size_t start_index);//18-
// void *s21_trim(const char *src, const char *trim_chars);//19-


// s21_size_t s21_strspn(const char *str, const char *keys) {
//   s21_size_t strLength = s21_strlen(str);
//   s21_size_t result = 0;

//   for (s21_size_t x = 0; result == 0 && x < strLength; x += 1) {
//     if (s21_strchr(keys, str[x]) != NULL) result = x + 1;
//   }

//   return result;
// }

// #define MAX_ERRLIST 107
// #define MIN_ERRLIST -1
// #define ERROR "Unknown error: "
// static const char *errorList[] __attribute__((unused))  = {
//      "Success",
//     "Operation not permitted",
//     "No such file or directory",
//     "No such process",
//     "Interrupted system call",
//     "Input/output error",
//     "No such device or address",
//     "Argument list too long",
//     "Exec format error",
//     "Bad file descriptor",
//     "No child processes",
//     "Resource temporarily unavailable",
//     "Cannot allocate memory",
//     "Permission denied",
//     "Bad address",
//     "Block device required",
//     "Device or resource busy",
//     "File exists",
//     "Invalid cross-device link",
//     "No such device",
//     "Not a directory",
//     "Is a directory",
//     "Invalid argument",
//     "Too many open files in system",
//     "Too many open files",
//     "Inappropriate ioctl for device",
//     "Text file busy",
//     "File too large",
//     "No space left on device",
//     "Illegal seek",
//     "Read-only file system",
//     "Too many links",
//     "Broken pipe",
//     "Numerical argument out of domain",
//     "Numerical result out of range",
//     "Resource deadlock avoided",
//     "File name too long",
//     "No locks available",
//     "Function not implemented",
//     "Directory not empty",
//     "Too many levels of symbolic links",
//     "Unknown error 41",
//     "No message of desired type",
//     "Identifier removed",
//     "Channel number out of range",
//     "Level 2 not synchronized",
//     "Level 3 halted",
//     "Level 3 reset",
//     "Link number out of range",
//     "Protocol driver not attached",
//     "No CSI structure available",
//     "Level 2 halted",
//     "Invalid exchange",
//     "Invalid request descriptor",
//     "Exchange full",
//     "No anode",
//     "Invalid request code",
//     "Invalid slot",
//     "Unknown error 58",
//     "Bad font file format",
//     "Device not a stream",
//     "No data available",
//     "Timer expired",
//     "Out of streams resources",
//     "Machine is not on the network",
//     "Package not installed",
//     "Object is remote",
//     "Link has been severed",
//     "Advertise error",
//     "Srmount error",
//     "Communication error on send",
//     "Protocol error",
//     "Multihop attempted",
//     "RFS specific error",
//     "Bad message",
//     "Value too large for defined data type",
//     "Name not unique on network",
//     "File descriptor in bad state",
//     "Remote address changed",
//     "Can not access a needed shared library",
//     "Accessing a corrupted shared library",
//     ".lib section in a.out corrupted",
//     "Attempting to link in too many shared libraries",
//     "Cannot exec a shared library directly",
//     "Invalid or incomplete multibyte or wide character",
//     "Interrupted system call should be restarted",
//     "Streams pipe error",
//     "Too many users",
//     "Socket operation on non-socket",
//     "Destination address required",
//     "Message too long",
//     "Protocol wrong type for socket",
//     "Protocol not available",
//     "Protocol not supported",
//     "Socket type not supported",
//     "Operation not supported",
//     "Protocol family not supported",
//     "Address family not supported by protocol",
//     "Address already in use",
//     "Cannot assign requested address",
//     "Network is down",
//     "Network is unreachable",
//     "Network dropped connection on reset",
//     "Software caused connection abort",
//     "Connection reset by peer",
//     "No buffer space available",
//     "Transport endpoint is already connected",
//     "Transport endpoint is not connected",
//     "Cannot send after transport endpoint shutdown",
//     "Too many references: cannot splice",
//     "Connection timed out",
//     "Connection refused",
//     "Host is down",
//     "No route to host",
//     "Operation already in progress",
//     "Operation now in progress",
//     "Stale file handle",
//     "Structure needs cleaning",
//     "Not a XENIX named type file",
//     "No XENIX semaphores available",
//     "Is a named type file",
//     "Remote I/O error",
//     "Disk quota exceeded",
//     "No medium found",
//     "Wrong medium type",
//     "Operation canceled",
//     "Required key not available",
//     "Key has expired",
//     "Key has been revoked",
//     "Key was rejected by service",
//     "Owner died",
//     "State not recoverable",
//     "Operation not possible due to RF-kill",
//     "Memory page has hardware error"};



// s21_size_t s21_strlen(const char *str){
//     const char *str1 =str;
//     while ( *str1!= '\0'){
//         str1++;
//     }
//     return str1-str;
// }

// void *s21_memchr(const void *str, int c, s21_size_t n) {
//     const unsigned char* str1 = (const unsigned char*)str;
//     for (s21_size_t i = 0; i < n; ++i) {
//         if (str1[i] == (unsigned char)c) {
//             return (char*)(str1 + i);
//         }
//     }
//     return NULL;
// }

// int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
//     const unsigned char* str11 = (const unsigned char*)str1;
//     const unsigned char* str12 = (const unsigned char*)str2;
//     for(s21_size_t i = 0; i < n; ++i) {
//         if(str11[i]!= str12[i]) {
//             return str11[i] - str12[i]; 
//         }
//     }
//     return 0; 
// }

// void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
//     const unsigned char* src_str = (const unsigned char*)src;
//     unsigned char* dest_str = (unsigned char*)dest;
//     for(s21_size_t i = 0; i < n; ++i) {
//         dest_str[i] = src_str[i];
//     }
//     return dest;
// }

// void *s21_memset(void *s, int c, size_t n) {
//     unsigned char *p = s;
//     while (n--) {
//         *p++ = (unsigned char)c;
//     }
//     return s; // Возвращаем значение
// }

// int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
//     while(n-- && (*str1!= '\0' || *str2!= '\0')) {
//         if(*str1!= *str2) {
//             return *str1 - *str2;
//         }
//         str1++;
//         str2++;
//     }
//     return 0;
// }

// s21_size_t s21_strcspn(const char *str1, const char *str2) {
//     const char *p = str1;
//     while (*p!= '\0') {
//         int found = 0;
//         const char *q = str2;
//         while (*q!= '\0' &&!found) {
//             if (*p == *q) {
//                 found = 1;
//             }
//             q++;
//         }
//         if (!found) {
//             p++;
//         } else {
//             break;
//         }
//     }
//     return p - str1;
// }

// char *s21_strchr(const char *str, int c) {
//     while(*str!= '\0') {
//         if(*str == c) {
//             return (char*)str;
//         }
//         str++;
//     }
//     return NULL;
// }
// char *s21_strcat(char *dest, const char *src) {
//   char *ret = dest;

//   while (*ret) {
//     ret++;
//   }
//   while (*src != '\0') {
//     *ret++ = *src++;
//   }
//   return dest;
// }
// char *s21_strncat(char *dest, const char *src, s21_size_t n) {
//     s21_size_t dest_len = s21_strlen(dest); 
//     s21_size_t i;
//     dest[dest_len] = '\0'; 
//     for(i = 0; i < n && src[i]!= '\0'; i++) {
//         dest[dest_len + i] = src[i];
//     }
//     dest[dest_len + i] = '\0';
//     return dest;
// }

// char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
//     s21_size_t i;
//     for(i = 0; i < n && src[i]!= '\0'; i++) {
//         dest[i] = src[i];
//     }
//     for(; i < n; i++) {
//         dest[i] = '\0';
//     }
//     return dest;
// }

// char *s21_strerror(int errorNumber) {
//   static char result[512] = {'\0'};

//   if (errorNumber <= MIN_ERRLIST || errorNumber >= MAX_ERRLIST) {
//     s21_sprintf(result, "Unknown error: %d", errorNumber);
//   } else {
//     // s21_strcpy(result, ((char *)errorList[errorNumber]));
//   }

//   return result;
// }

// char *s21_strpbrk(const char *str1, const char *str2) {
//     const char *ptr1 = str1;
//     const char *ptr2 = str2;
//     while (*ptr1) {
//         for (; *ptr2; ptr2++) {
//             if (*ptr1 == *ptr2) {
//                 return (char *)ptr1;
//             }
//         }
//         ptr1++;
//     }
//     return NULL;
// }

// // char *s21_strchr(const char *str, int c) {
// //     const char* start = str;
// //     const char* end = str + s21_strlen(str); 
// //     while (end >= start) {
// //         if (*end == c) {
// //             return (char*)end; 
// //         }
// //         end--;
// //     }
// //     return NULL; 
// // }

// char *s21_strstr(const char *haystack, const char *needle) {
//     const char* h = haystack;
//     const char* n = needle;
//     if (*n == '\0') {
//         return (char*)haystack;
//     }
//     while (*h) {
//         if (*h == *n && s21_memcmp(h, n, s21_strlen(n)) == 0) {
//             return (char*)h;
//         }
//         h++;
//     }
//     return NULL;
// }

// char *s21_strtok(char *str, const char *delim) {
//   static int next, restok;
//   static s21_size_t idx;
//   static char *ptr;
//   static char *addr;

//   if (str) {
//     restok = 1;
//     idx = 0, next = 0;
//     addr = str + s21_strspn(str, delim);
//     ptr = str + s21_strlen(str);
//   }
//   char *res = NULL;
//   if (!(addr >= ptr || next)) {
//     int exdelim = 1;
//     for (int i = 0; addr[i]; i++) {
//       s21_size_t step = s21_strspn(addr + i, delim);
//       if (step != 0) {
//         exdelim = 0;
//         for (s21_size_t j = i; j < i + step; j++) addr[j] = '\0';

//         idx = step + i;
//         break;
//       }
//     }
//     if (restok == 1 && exdelim) next = 1;
//     res = addr;
//     addr += idx;
//   }
//   restok += 1;
//   return res;
// }

// void *s21_to_upper(const char *str) {
//   char *newstr = calloc(s21_strlen(str) + 1, 1);
//   for (unsigned long long i = 0; i < s21_strlen(str); i++) {
//     if (str[i] < 'a' || str[i] > 'z') {
//       newstr[i] = str[i];
//     } else {
//       newstr[i] = str[i] - 32;
//     }
//   }
//   return newstr;
// }


// void *s21_to_lower(const char *str) {
//   char *newstr = calloc(s21_strlen(str) + 1, 1);
//   for (unsigned long long i = 0; i < s21_strlen(str); i++) {
//     if (str[i] < 'A' || str[i] > 'Z') {
//       newstr[i] = str[i];
//     } else {
//       newstr[i] = str[i] + 32;
//     }
//   }
//   return newstr;
// }


// // void *s21_insert(const char *src, const char *str, s21_size_t start_index){}//18-


// // void *s21_trim(const char *src, const char *trim_chars){}//19-
