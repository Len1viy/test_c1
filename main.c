#include <stdio.h>
#include <stdlib.h>

//int main() {
//    int s = 8, c = 0, l = 6;
//    l += -s++ + ++c;
//    printf("%d %d %d", s, c, l);
//}
// 9 1 -1



//typedef struct QueueVector {
//    int size;
//    int count;
//    int head;
//    int tail;
//    int *data;
//} QueueVector;
//
//
//int pop(QueueVector *queue, int *rez) {
//    if (queue->data == NULL || queue->tail == 0) {
//        return 1;
//    }
//    *rez = queue->data[queue->head];
//    queue->head++;
//    if (queue->head == queue->tail) {
//        queue->tail = 0;
//    }
//    return 0;
//
//}

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct Line{
    int size;
    double *data;
}Line;
typedef struct Matrix{
    int size;
    Line* lines;
}Matrix;

//Matrix *func(Matrix *matrix) {
//    Matrix *ans = (Matrix *)calloc(1, sizeof(Matrix));
//    ans->size = matrix->size;
//    ans->lines = realloc(ans->lines, sizeof(Line) * ans->size);
//    for (int i = 0; i < matrix->size; i++) {
//        ans->lines[i].data = NULL;
//        ans->lines[i].size = matrix->lines[i].size;
//        ans->lines[i].data = realloc(ans->lines[i].data, sizeof(double) * matrix->lines[i].size);
//        double maxim = -10000;
//        double minim = 100000;
//        int ind_max = 0;
//        int ind_min = 0;
//        double smena;
//        for (int j = 0; j < matrix->lines[i].size; j++) {
//            if (matrix->lines[i].data[j] > maxim) {
//                maxim = matrix->lines[i].data[j];
//                ind_max = j;
//            }
//            if (matrix->lines[i].data[j] < minim) {
//                minim = matrix->lines[i].data[j];
//                ind_min = j;
//            }
//            ans->lines[i].data[j] = matrix->lines[i].data[j];
//        }
//        smena = ans->lines[i].data[ind_max];
//        ans->lines[i].data[ind_max] = ans->lines[i].data[ind_min];
//        ans->lines[i].data[ind_min] = smena;
//    }
//    return ans;
//}


Matrix *func(Matrix *matrix) {
    Matrix *ans = (Matrix *)calloc(1, sizeof(Matrix));
    ans->size = matrix->size;
    ans->lines = NULL;
    ans->lines = realloc(ans->lines, sizeof(Line) * ans->size);
    double maxim_elem = 0;
    double *zam = NULL;
    int zam_size = 0;
    int index_line = -1;
    for (int i = 0; i < ans->size; i++) {
        ans->lines[i].data = NULL;
        ans->lines[i].size = matrix->lines[i].size;
        ans->lines[i].data = realloc(ans->lines[i].data, sizeof(double) * ans->lines[i].size);
        for (int j = 0; j < ans->lines[i].size; j++) {
            ans->lines[i].data[j] = matrix->lines[i].data[j];
            if (matrix->lines[i].data[j] > maxim_elem) {
                maxim_elem = matrix->lines[i].data[j];
                index_line = i;
            }
        }
    }
    zam_size = ans->lines[index_line].size;

    zam = realloc(zam, sizeof(double) * zam_size);
    for (int i = 0; i < zam_size; i++) {
        zam[i] = ans->lines[index_line].data[i];
    }
    ans->lines[index_line].size = ans->lines[0].size;
    ans->lines[index_line].data = realloc(ans->lines[index_line].data, sizeof(double) * ans->lines[index_line].size);
    for (int i = 0; i < ans->lines[index_line].size; i++) {
        ans->lines[index_line].data[i] = ans->lines[0].data[i];
    }
    ans->lines[0].size = zam_size;
    ans->lines[0].data = realloc(ans->lines[0].data, sizeof(double) * zam_size);
    for (int i = 0; i < zam_size; i++) {
        ans->lines[0].data[i] = zam[i];
    }
    free(zam);
    return ans;


}


int getInt(int *a) {
    int n;
    do {
        n = scanf("%d", a);
        //n=EOF;
        if (n==EOF) {
            return -1;
        }
        if (n == 0) {
            printf("Erroneous input,try again: ");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while (n == 0);
    return 1;
}
int getintpol(int *a){
    if(getInt(a)){
        if(*a<=0){
            printf("Your number is not greater than zero, try again: ");
            int n=getInt(a);
            if(n==-1){
                return 0;
            }
        }
    }
    return 1;
}
int fill_matrix(Matrix *rm) {
    int m;
    printf("Enter the number of lines: ");
    //scanf("%d", &m);
    int n=getintpol(&m);
    if(n==0){
        return 0;
    }
    rm->size = m;
    rm->lines = (Line *) calloc(m, sizeof(Line));
    for (int i = 0; i < rm->size; ++i) {
        printf("Enter the number of elements for the %d-th row: ", i + 1);
        getintpol(&m);
        //scanf("%d", &m);
        rm->lines[i].size = m;
        rm->lines[i].data = (int *) calloc(m, sizeof(int));
    }
    printf("Now fill in the matrix itself)\n");
    for (int i = 0; i < rm->size; ++i) {
        for (int j = 0; j < rm->lines[i].size; ++j) {
            printf("Enter a value arr[%d][%d]: ", i, j);
            getInt(&m);
//            scanf("%d", &m);
            rm->lines[i].data[j] = m;
        }
    }
}
void print_matrix(Matrix *rm) {
    if(rm->lines==0){
        printf("Oh,sorry,you have an empty matrix(\n");
    }else {
        for (int i = 0; i < rm->size; ++i) {
            for (int j = 0; j < rm->lines[i].size; ++j) {
                printf("[%g] ", rm->lines[i].data[j]);
            }
            printf("\n");
        }
    }
}

int main() {
    printf("Hello, World!\n");
    Matrix Matr={0,NULL};
    fill_matrix(&Matr);
    Matrix *ask= func(&Matr);
    print_matrix(&Matr);
    print_matrix(ask);

    return 0;
}