#ifndef SRC_S21_STRING_H_
#define SRC_S21_STRING_H_

#define S21_NULL ((void *)0)
#define s21_isNan(x) __builtin_isnan(x)

#include <ctype.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "s21_sprintf.h"


char *s21_strcpy(char *dest, const char *src);
char *s21_strcat(char *dest, const char *src);
void *s21_memchr(const void *str, int c, size_t n);//1+++
int s21_memcmp(const void *str1, const void *str2, size_t n);//2+++
void *s21_memcpy(void *dest, const void *src, size_t n);//3+++
void *s21_memset(void *str, int c, size_t n);//4+++
char *s21_strncat(char *dest, const char *src, size_t n);//5+++
char *s21_strchr(const char *str, int c);//6+++
int s21_strncmp(const char *str1, const char *str2,  size_t n);//7+++
char *s21_strncpy(char *dest, const char *src, size_t n);//8+++
size_t s21_strcspn(const char *str1, const char *str2);//9+++
char *s21_strerror(int errnum);//10+++s21_
size_t s21_strlen(const char *str);//11+++
char *s21_strpbrk(const char *str1, const char *str2);//12+++
char *s21_strrchr(const char *str, int c);//13+++
char *s21_strstr(const char *haystack, const char *needle);//14+++
char *s21_strtok(char *str, const char *delim);//15+++
void *s21_to_upper(const char *str);//16+++ чистить память в мейне при тестах
void *ts21_o_lower(const char *str);//17+++ чистить память в мейне при тестах
void *s21_insert(const char *src, const char *str, size_t start_index);//18-
void *s21_trim(const char *src, const char *trim_chars);//19-

#endif  
