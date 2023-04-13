#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct Item {
    unsigned int info;
} Item;

typedef struct KeySpace {
    int busy;
    char *key;
    int release;
    Item info;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    int msize;
    int csize;
} Table;


int myln(const char *st) {
    int i = 0;
    while (st[i]) {
        i++;
    }
    return i;
}

void summator(char *res, char *sm, size_t n)
{
    int i = 0;

    if (n == 0) {
        return;
    }
    while (i < n) {
        res[i] = sm[i];
        i++;
    }
}



char *in_str() {
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            return NULL;
        } else if (n > 0) {
            int chunck_len = myln(buf);
            int len_all = len + chunck_len;
            res = realloc(res, len_all + 1);
            summator(&res[len], buf, chunck_len);
            len = len_all;
        } else {
            scanf("%*c");
        }
    } while (n > 0);
    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
        res[0] = '\0';
    }
    return res;
}

char *hash_func(char *str) {
    int len = strlen(str);
    char *ans = strdup(str);
    char alph[27] = "abcdefghijklmnopqrstuvwxyz";
    int e;
    printf("Enter hash-key-->");
    scanf("%d", &e);
    for (int i = 0; i < len; i++) {
        int a = (ans[i] - 96) * e * e;
        ans[i] = alph[a % 27];
    }
    char *res = strdup(ans);
    free(ans);
    return res;
}

void help() {
    printf("0. Quit\n1. Add\n2. Find\n3. Delete\n4. Show\n5. Find versions\n");
}

int check_command() {
    int n = 0;
    help();
    printf("Choose a command-->");
    int f = scanf("%d", &n);
    if (f != EOF) {
        while (f != 1 || (n > 5) || (n < 0)) {
            help();
            printf("Wrong input. Try again-->");
            scanf("%*[^\n]");
            if (f != EOF) {
                f = scanf("%d", &n);
            } else return 0;
        }
    } else return 0;
    return n;
}

int D_Find_key_w_b(Table *ptab, char *key) {
    int ret = -2;
    for (int i = 0; i < ptab->csize; i++) {
        if (strcmp(ptab->ks[i].key, key) == 0 && ptab->ks[i].busy == 1) {
            ret = i;
        }
    }
    return ret+1;
}

int D_Find_key(Table *ptab, char *key) {
    int ret = -2;
    for (int i = 0; i < ptab->csize; i++) {
        if (strcmp(ptab->ks[i].key, key) == 0) {
            ret = i;
        }
    }
    return ret+1;
}

int insert_w_v(Table *ptab, char *k, int rel, unsigned int info) {
    ptab->ks[ptab->csize].info.info = info;
    ptab->ks[ptab->csize].key = strdup(k);
    ptab->ks[ptab->csize].busy = 1;
    ptab->ks[ptab->csize].release = rel;
    ptab->csize += 1;
}

int insert(Table *ptab, char *k, unsigned int info) {
    int ind = D_Find_key(ptab, k);
    int rel = 0;
    if (ind != -1) {
        rel = ptab->ks[ind-1].release;
        ptab->csize += 1;
        if (ind != (ptab->csize - 1)) {
            for (int i = ptab->csize; i > ind - 1; i--) {
                ptab->ks[i].info.info = ptab->ks[i - 1].info.info;
                ptab->ks[i].key = strdup(ptab->ks[i - 1].key);
                ptab->ks[i].busy = 1;
                ptab->ks[i].release = ptab->ks[i - 1].release;
            }
            ptab->ks[ind].info.info = info;
            ptab->ks[ind].key = strdup(k);
            ptab->ks[ind].busy = 1;
            ptab->ks[ind].release = rel + 1;
        } else {
            ptab->ks[ind].info.info = info;
            ptab->ks[ind].key = strdup(k);
            ptab->ks[ind].busy = 1;
            ptab->ks[ind].release = rel + 1;
        }

    } else {
        ptab->ks[ptab->csize].info.info = info;
        ptab->ks[ptab->csize].key = strdup(k);
        ptab->ks[ptab->csize].busy = 1;
        ptab->ks[ptab->csize].release = rel;
        ptab->csize += 1;
    }
    return 1;
}


int D_Add(Table *ptab) {
    if (ptab->csize == ptab->msize) {
        printf("Table is full.\n");
        ptab->msize = 2*ptab->msize;
        ptab->ks = realloc(ptab->ks, sizeof(KeySpace) * ptab->msize);
    }
    unsigned int info;
    printf("Enter key-->");
    scanf("%*c");
    char *k = in_str();
    printf("Enter info-->");
    scanf("%u", &info);
    scanf("%*c");
    char *k1 = hash_func(k);
    insert(ptab, k1, info);
    free(k);
    free(k1);
    return 1;
}

int D_Show(Table *ptab) {
    if (ptab->ks) {
        printf("------------------------------------------------------------------\n");
        for (int i = 0; i < ptab->csize; i++) {
            printf("|\t%d\t|\t%s\t|\t%d\t|\t%u\t|\n", ptab->ks[i].busy, ptab->ks[i].key, ptab->ks[i].release, ptab->ks[i].info.info);
        }
        printf("------------------------------------------------------------------\n");
    } else {
        printf("Table is empty\n");
    }
}


void D_Del_table(Table *ptab) {
    for (int i = 0; i < ptab->csize; i++) {
        free(ptab->ks[i].key);
    }
    free(ptab->ks);
    ptab->csize = 0;
}





int D_Find(Table *ptab) {
    Table find = {NULL, 100, 0};
    int ret = -1;
    char *key = NULL;
    printf("Enter key, which you want find-->");
    key = in_str();
    for (int i = 0; i < ptab->csize; i++) {
        if (strcmp(ptab->ks[i].key, key) == 0) {
            ret = i;
            insert(&find, ptab->ks[i].key, ptab->ks[i].info.info);
        }
    }
    if (ret != -1) {
        printf("The following table elements were found:\n");
        D_Show(&find);
    } else {
        printf("Nothing found\n");
    }
    D_Del_table(&find);
}



int D_Del(Table *ptab) {

    printf("Enter key which you want delete-->");
    scanf("%*c");
    char *k = in_str();
    int ind = D_Find_key_w_b(ptab, k) - 1;
    if (ind != -1) {
        ptab->ks[ind].busy = 0;
    } else {
        printf("That's key is parent of another. Try again\n");
    }
    free(k);
}

int D_Find_versions(Table *ptab) {
    Table vers = {NULL, 100, 0};
    vers.ks = realloc(vers.ks, sizeof(KeySpace)*vers.msize);
    int choose;
    printf("Choose pick: 2 - all versions; 1 - one version");
    scanf("%d", &choose);
    if (choose == 2) {
        printf("Enter key, which you would like to find-->");
        scanf("%*c");
        char *key = in_str();
        for (int i = 0; i < ptab->csize; i++) {
            if (strcmp(key, ptab->ks[i].key) == 0) {
                insert(&vers, key, ptab->ks[i].info.info);
            }
        }
        D_Show(&vers);
        D_Del_table(&vers);
        free(key);
    } else {
        printf("Enter key, which you would like to find-->");
        scanf("%*c");
        char *key = in_str();
        printf("Enter the version-->");
        int version;
        scanf("%d", &version);
        for (int i = 0; i < ptab->csize; i++) {
            if (strcmp(key, ptab->ks[i].key) == 0 && (ptab->ks[i].release == version)) {
                insert_w_v(&vers, key, ptab->ks[i].release, ptab->ks[i].info.info);
            }
        }
        D_Show(&vers);
        D_Del_table(&vers);
        free(key);
    }
}

int main() {
    Table table = {NULL, 100, 0};
    table.ks = realloc(table.ks, sizeof(KeySpace)*table.msize);
    int rc;
    int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Del, D_Show, D_Find_versions};
    while (rc = check_command()) {
        if (!fptr[rc](&table)) break;
    }
    D_Del_table(&table);

}