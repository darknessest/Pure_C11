//6_2
/*
哈希表设计。为班级30个人的姓氏(单字姓)设计一个哈希表，假设姓氏用汉语拼音表示。
要求用除留取余法构造哈希函数，用线性探测再散列法处理冲突，平均查找长度的上限为2。
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <stdbool.h>

#define phi 0.6180339887
#define ULL unsigned long long

typedef struct hn {
  ULL hash;
  char name[25];
  struct hn *next;
} HN; //hashed names
void swap_HN(HN *i, HN *j) {
    HN t;
    t = *i;
    *i = *j;
    *j = t;
}
int compare(const void *s1, const void *s2) {
    HN *e1 = (HN *) s1;
    HN *e2 = (HN *) s2;
    //"casting" to int
    return e1->hash/1000000 - e2->hash/1000000;
}
ULL hashing(const char *name) {
    ULL x = 0;
    int sum = 0, iter = 2;
    for (int i = 0; name[i] != '\0'; ++i) {
        if (name[i] >= 'a' && name[i] <= 'z') {
            sum += (int) ((name[i]/phi)*100 + name[i]*i - 10000);
        } else if (name[i] == ' ') {
            x += sum*(ULL) pow(100000, iter--);
            sum = 0;
        } else if (name[i] == '\n') {
            x += sum*(ULL) pow(100000, iter);
            iter = 2;
            break;
        }
    }
    x += sum*(ULL) pow(100000, iter);
//    printf("#%s: %llu\n", name, x);
    return x;
}
void print_hn(const HN *array, const int size) {
    for (int k = 0; k < size; ++k) {
        printf("%s : %llu", array[k].name, array[k].hash);
        if (array[k].next) {
            printf(" -> %s : %llu", array[k].next->name, array[k].next->hash);
            if (array[k].next->next)
                printf(" -> %s : %llu", array[k].next->next->name, array[k].next->next->hash);

        }
        printf("\n");
    }
}
void cleanup(HN *array, int *size) {
    qsort(array, (size_t) *size, sizeof(HN), compare);
//    for (int k = 0; k < *size; ++k)
//        printf("%s : %llu\n", array[k].name, array[k].hash);
//
//    printf("\n");
    bool flag = 0;
    //looking for repeating elements
    for (int k = 0; k < *size; ++k) {
        if (array[k].hash == array[k + 1].hash) {
            flag = 1;
            if (array[k].next == NULL) {
                array[k].next = (struct hn *) malloc(sizeof(HN));
                //memcpy
                strcpy(array[k].next->name, array[k + 1].name);
                array[k].next->hash = array[k + 1].hash;
                array[k].next->next = NULL;
                array[k + 1].name[0] = '\t';
                //special symbol to detect repeating elements
            } else {
                array[k].next->next = (struct hn *) malloc(sizeof(HN));
                //memcpy
                strcpy(array[k].next->next->name, array[k + 1].name);
                array[k].next->next->hash = array[k + 1].hash;
                array[k + 1].name[0] = '\t';
                //special symbol to detect repeating elements
            }
        }
    }
    if (flag) {
        //handling repeating elements
        for (int k = 0; k < *size; ++k) {
            if (array[k].name[0] == '\t') {
                //swapping useless element with last defined
                swap_HN(&array[k], &array[*size - 1]);
                --(*size);
            }
        }
        qsort(array, (size_t) *size, sizeof(HN), compare);
    }
}
int main() {
    FILE *fp;
    fp = fopen("in.txt", "r");
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    HN class[50];
    int i = 0, size_of_array = 0;
    char ch, t_name[25];

    //start of input
    while ((ch = fgetc(fp)) != EOF) {
        if (ch != '\n')
            t_name[i++] = ch;
        else {
            t_name[i] = '\0';
            i = 0;
            strcpy(class[size_of_array].name, t_name);
            class[size_of_array].hash = hashing(t_name);
            class[size_of_array].next = NULL;
            memset(t_name, 0, strlen(t_name));  //reusing the same string
            ++size_of_array;
        }
    }
    t_name[i] = '\0';
    strcpy(class[size_of_array].name, t_name);
    class[size_of_array].hash = hashing(t_name);
    class[size_of_array++].next = NULL;
    fclose(fp);
    //end of input

    cleanup(class, &size_of_array); //size of array may change

    print_hn(class, size_of_array);
}