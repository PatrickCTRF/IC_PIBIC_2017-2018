/*
 * string.h
 *
 *  Created on: 4 de jul de 2018
 *      Author: patrick
 */

#ifndef MYSTRING_H_
#define MYSTRING_H_

//Swap function definition
void mySwap(int *a, int *b)
{
   int t;

   t  = *b;
   *b = *a;
   *a = t;
}

void mySwapChar(char *a, char *b)
{
   char t;

   t  = *b;
   *b = *a;
   *a = t;
}

/* A utility function to reverse a string  */
void myReverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        mySwapChar((str+start), (str+end));
        start++;
        end--;
    }
}

// Implementation of itoa()
char* myItoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    myReverse(str, i);

    return str;
}

// A simple atoi() function
int myAtoi(char *str)
{
    int res = 0; // Initialize result

    // Iterate through all characters of input string and
    // update result
    int i = 0;
    for (i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';

    // return result.
    return res;
}

char *myStrcat(char *dest, const char *src)
{
    char *ret = dest;
    while (*dest)
        dest++;
    while (*dest++ = *src++);

    return ret;
}

int myStrlen(const char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) ;
    return i;
}

/**
 * Esta função recebe a primeira string e verifica se esta contêm
 * a segunda. Se sim, retorna um ponteiro para a posição onde se encontra o início
 * da segunda string DENTRO da primeira. Ela não te dá uma nova string, apenas o
 * ponteiro para a posição onde se inicia a subString.
 */
char* myStrstr(char *str, char *substr){

	char *Begin;
	char *pattern;

	while (*str)
	{
		Begin = str;
		pattern = substr;

		// If first character of sub string match, check for whole string
		while (*str && *pattern && *str == *pattern)
		{
			  str++;
			  pattern++;
		}
		// If complete sub string match, return starting address
		if (!*pattern)
			  return Begin;

		str = Begin + 1;	// Increament main string
	}
	return 0;
}

char *myStrcpy(char *destination, char *source)
{
    char *start = destination;

    while(*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }

    *destination = '\0'; // add '\0' at the end
    return start;
}

/**

int myMemcmp(const void* s1, const void* s2,int n)
{
    const unsigned char *p1 = s1, *p2 = s2;
    while(n--)
        if( *p1 != *p2 )
            return *p1 - *p2;
        else
            p1++,p2++;
    return 0;
}

char *myStrstr(const char *s1, const char *s2)
{
    int n = myStrlen(s2);
    while(*s1)
        if(!myMemcmp(s1++,s2,n))
            return s1-1;
    return 0;
}
 */


#endif /* MYSTRING_H_ */

