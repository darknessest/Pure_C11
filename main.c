#include <stdio.h>
#include <string.h>

char unique[](char *strg)
{
    char temp[1000];
    for (int i = 0; i < strlen(strg); i++) {
        if (strg[i] != ' ') {
            temp[i] = strg[i];
        }
    }
    return temp;
}

int main()
{
    char string[10000];
    while (gets(string) != EOF) {
        printf("%s", unique(string));
    }
}
