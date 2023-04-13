#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <windows.h>

typedef struct {
    int ind;
    int imp_level;
    char *string;
} event;

typedef int comparator(const void *, const void *);

void help1() {
    printf("Выберите что будем сортировать:\n1 - Идентификатор\n2 - Уровень важности\n3 - Строки\n9 - Закончить действие программы\n");
}

void help2() {
    printf("Выберите направление  сортировки:\n1 - По возрастанию\n2 - По убыванию\n9 - Закончить действие программы\n");
}

void help3() {
    printf("Выберите метод сортировки:\n1 - Сортировка расчёской (Comb sort).\n2 - Сортировка вставками (Insertion sort).\n3 - Быстрая сортировка (Quick Sort)\n9 - Закончить действие программы\n");
}

int my_stoi(int *n, char *s) {
    const int size_s = strlen(s);
    int value = 0;
    for (int i = 0; i < size_s; ++i) {
        int digit = s[i] - '0';
        value = value * 10 + digit;
    }

    *n = value;
}

void comb_sort(event *structure, int cnt, int (*comp)(const void *, const void *)) {
    float factor = 1.24733;
    int gap = cnt;
    int swaps = 1;
    int i, j;
    while (gap > 1 || (swaps != 0)) {
        gap = (int) (gap / factor);
        if (gap < 1)
            gap = 1;
        swaps = 0;
        for (i = 0; i < cnt - gap; ++i) {
            j = i + gap;
            if (comp(&structure[i], &structure[j]) > 0) {
                event tmp = structure[i];
                structure[i] = structure[j];
                structure[j] = tmp;
                swaps = 1;
            }
        }
    }
}

void insertion_sort(event *structure, int cnt, int (*comp)(const void *, const void *)) {
    int poz, i;
    event elem;
    for (i = 1; i < cnt; i++) {
        elem = structure[i];
        poz = i - 1;
        while (poz >= 0 && (comp(&structure[poz], &elem) > 0)) {
            structure[poz + 1] = structure[poz];
            poz--;
        }
        structure[poz + 1] = elem;
    }
}

char *fin_str(FILE *stream) {
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n;
    do {
        n = fscanf(stream, "%80[^\n]", buf);
        if (n < 0) {
            return NULL;
        } else if (n > 0) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, sizeof(char) * (str_len + 1));
            memmove(res + len, buf, chunk_len);
            len = str_len;
        } else {
            fscanf(stream, "%*c");
        }
    } while (n > 0);
    if (len > 0) {
        res[len] = '\0';
    } else {
        res = (char *) calloc(1, sizeof(char));
    }

    return res;
}

void event_maker(event *sorting, int *cnt) {
    FILE *file;
    int i = 0;
    char *s;
    file = fopen("C:\\Users\\vadim\\CLionProjects\\untitled\\inp.txt", "r");
    while ((s = fin_str(file)) != NULL) {
        int len_str = 0;
        int space = 0;
        char *num = NULL;
        int len_num = 0;
        sorting[(*cnt)].string = (char*)malloc(sizeof(char));
        while (s[i]) {
            if (s[i] == ' ') {
                space++;
            }
            if (space < 1) {
                len_num++;
                num = realloc(num, sizeof(char)*len_num);
                num[len_num-1] = s[i];
            }
            if (space == 1 && s[i] != ' ') {
                my_stoi(&sorting[(*cnt)].imp_level, s[i]);
            }
            if (space > 2 && s[i] != ' ') {
                sorting[(*cnt)].string = realloc(sorting[(*cnt)].string, (len_str+1)*(sizeof(char)));
                sorting[(*cnt)].string[len_str] = s[i];
                len_str++;
            }
            i++;
        }
        my_stoi(&sorting[(*cnt)].ind, num);
        sorting[(*cnt)].string = realloc(sorting[(*cnt)].string, sizeof(char)*(len_str+1));
        sorting[(*cnt)].string[len_str] = '\0';
        (*cnt)++;
    }
    (*cnt)++;
    fclose(file);
}


int comp_for_ind(const void *a, const void *b) {
    const event *r1 = a;
    const event *r2 = b;
    return r1->ind - r2->ind;
}

int comp_for_imp(const void *a, const void *b) {
    const event *r1 = a;
    const event *r2 = b;
    return r1->imp_level - r2->imp_level;
}

int comp_for_str(const void *a, const void *b) {
    const event *r1 = a;
    const event *r2 = b;
    return strcmp(r1->string, r2->string);
}

int comp_for_ind1(const void *a, const void *b) {
    const event *r1 = a;
    const event *r2 = b;
    return -(r1->ind - r2->ind);
}

int comp_for_imp1(const void *a, const void *b) {
    const event *r1 = a;
    const event *r2 = b;
    return -(r1->imp_level - r2->imp_level);
}

int comp_for_str1(const void *a, const void *b) {
    const event *r1 = a;
    const event *r2 = b;
    return -strcmp(r1->string, r2->string);
}

comparator *cmp(char column, char direction) {
    if (column == 'N') {
        if (direction == 'U') {
            return comp_for_ind;
        } else {
            return comp_for_ind1;
        }
    } else if (column == 'L') {
        if (direction == 'U') {
            return comp_for_imp;
        } else {
            return comp_for_imp1;
        }
    } else if (column == 'S') {
        if (direction == 'U') {
            return comp_for_str;
        } else {
            return comp_for_str1;
        }
    }
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    while (1) {
        FILE *file;
        help1();
        int choose1;
        int a1 = scanf("%d", &choose1);
        while (a1 != 1) {
            printf("Введите корректное значение: ");
            scanf("%*[^\n]");
            a1 = scanf("%d", &choose1);
        }
        if (choose1 == 9) {
            printf("Программа завершена");
            break;
        }
        help2();
        int choose2;
        int a2 = scanf("%d", &choose2);
        while (a2 != 1) {
            printf("Введите корректное значение: ");
            scanf("%*[^\n]");
            a2 = scanf("%d", &choose2);
        }
        if (choose2 == 9) {
            printf("Программа завершена");
            break;
        }
        help3();
        int choose3;
        int a3 = scanf("%d", &choose3);
        while (a3 != 1) {
            printf("Введите корректное значение: ");
            scanf("%*[^\n]");
            a3 = scanf("%d", &choose3);
        }
        if (choose2 == 9) {
            printf("Программа завершена");
            break;
        }
        comparator *comp = cmp(choose1, choose2);
        int count = 0;
        event sorting1[100];
        event_maker(sorting1, &count);
        if (choose3 == 1) {
            comb_sort(sorting1, count, comp);
        } else if (choose3 == 2) {
            insertion_sort(sorting1, count, comp);
        } else if (choose3 == 3) {
            qsort(sorting1, count, sizeof(sorting1[0]), comp);
        }
        file = fopen("C:\\Users\\vadim\\CLionProjects\\untitled\\out.txt", "wb");
//        for (int i = 0; i < count; i++) {
//            printf("%d %d %s\n", sorting1[i].ind, sorting1[i].imp_level, sorting1[i].string);
//            fprintf(file, "%d %d %s\n", sorting1[i].ind, sorting1[i].imp_level, sorting1[i].string);
//            free(sorting1[i].string);
//        }
        fclose(file);
        break;
    }
}