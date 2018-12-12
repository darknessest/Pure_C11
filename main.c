//7_1
/*
荷兰国旗问题：设有一个仅由红、白、蓝三种颜色的条块组成的序列。
 试设计一个时间复杂度为O(n)的算法，使得这些条块按红、白、蓝的顺序排好，即排成荷兰国旗图案。
 */
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

void countSort(int *array, const int size) {
    int counter[3];
    counter[0] = 0;
    counter[1] = 0;
    counter[2] = 0;
    //counting numbers
    for (int i = 0; i < size; ++i) {
        if (array[i] == 0)
            ++counter[0];
        if (array[i] == 1)
            ++counter[1];
        if (array[i] == 2)
            ++counter[2];
    }
//    printf("#%d\t%d\t%d\n", counter[0], counter[1], counter[2]);
    int j = 0, s = 0;
    //inserting numbers
    for (j = 0; j < counter[0]; ++j)
        array[j] = 0;
    for (s = j; j < s + counter[1]; ++j)
        array[j] = 1;
    for (s = j; j < s + counter[2]; ++j)
        array[j] = 2;

    for (int i = 0; i < size; ++i)
        printf("%d\n", array[i]);
}

int main() {
    int array[SIZE];
    for (int i = 0; i < SIZE; ++i)
        array[i] = rand()%3;

    countSort(array, SIZE);
}