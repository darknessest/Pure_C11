//7_2
/*
假设n个部门名称的基本数据存储在字符数组name[N][25]中，5≤n≤N≤20。
 试设计一个起泡排序算法，将n个部门名称按字典序重新排列顺序。
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>

void Names(wchar_t A[][25], const int n) {
    srand(time(0));
    int i, j, k;
    for (i = 0; i < n; i++) {
        k = 2*(rand()%10 + 3);    //部门字数
        for (j = 0; j < k; j++)
            A[i][j] = rand()%30 + L'不'; //汉字区
        A[i][j] = '\0';
    }
}
void swapString(wchar_t *A, wchar_t *B) {
    wchar_t *temp = (wchar_t *) malloc((wcslen(A) + 1)*sizeof(wchar_t));
    wcscpy(temp, A);
    wcscpy(A, B);
    wcscpy(B, temp);
    free(temp);
}
void bubbleSort(wchar_t A[][25], const int n) {
    for (int i = 0; i < n - 1; ++i)
        for (int j = i + 1; j < n; ++j)
            if (wcscmp(A[i], A[j]) > 0)
                swapString(A[i], A[j]);

}
int main() {
    setlocale(LC_ALL, "");
    wchar_t array[15][25];
    Names(array, 15);

    for (int i = 0; i < 15; ++i)
        wprintf(L"%ls\n", array[i]);
    bubbleSort(array, 15);
    printf("-------------------------------------------\n");
    for (int i = 0; i < 15; ++i)
        wprintf(L"%ls\n", array[i]);

}