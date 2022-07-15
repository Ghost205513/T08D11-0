#include <stdio.h>
#include <stdlib.h>

int input(double **buffer, int rows, int columns);
void output(double determinant);
double ** matrix_init(int *rows, int *columns);
void free_m(double **a, int rows);
double det(double **src, int columns_src);


int main() {
    int rows_a, columns_a;
    double **a;

    a = matrix_init(&rows_a, &columns_a);

    if (a != NULL) {
        printf("%.6lf", det(a, columns_a));
        free_m(a, rows_a);
    }
}

int input(double **buffer, int rows, int columns) {
    int flag, trash;

    flag = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            double temp;

            if (scanf("%lf", &temp) == 0) {
                flag = 1;
                break;
            } else {
                buffer[i][j] = temp;
            }
        }
    }

    trash = getchar();

    while (trash == ' ' || trash == '\t')
        trash = getchar();

    if (trash != '\n')
        flag = 1;

    return flag;
}

void output(double determinant) {
     printf("%.6lf", determinant);
}

double ** matrix_init(int *rows, int *columns) {
    double **pointers_matrix = NULL;

    if (scanf("%d", rows) == 0 || *rows <= 0) {
        printf("n/a");
    } else if (scanf("%d", columns) == 0 || *columns <= 0) {
        printf("n/a");
    } else if (*rows != *columns) {
        printf("n/a");
    } else {
        pointers_matrix = malloc(*rows * sizeof(double *));

        for (int i = 0; i < *rows; i++)
            pointers_matrix[i] = malloc(*columns * sizeof(double));

        if (input(pointers_matrix, *rows, *columns) == 1) {
            printf("n/a");

            free_m(pointers_matrix, *rows);
            pointers_matrix = NULL;
        }
    }

    return pointers_matrix;
}

void free_m(double **a, int rows) {
    for (int i = 0; i < rows; i++)
        free(a[i]);

    free(a);
}

int inversions(int *permutation, int n) {
    int *sorted;
    int ans = 1;

    sorted = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        sorted[i] = i;

    for (int i = 0; i < n; i++) {
        if (sorted[permutation[i]] % 2 == 1)
            ans *= -1;

        for (int j = permutation[i] + 1; j < n; j++)
            if (sorted[j])
                sorted[j]--;
    }

    free(sorted);

    return ans;
}

double determinant(double **src, int columns_src, int index, int *used, int *generate) {
    double ans = 0;

    if (index == columns_src) {
        ans = inversions(generate, index);

        for (int i = 0; i < index; i++)
            ans *= src[i][generate[i]];
    } else {
        for (int i = 0; i < columns_src; i++) {
            if (used[i] == 1)
                continue;

            generate[index] = i;
            used[i] = 1;
            ans += determinant(src, columns_src, index + 1, used, generate);
            used[i] = 0;
        }
    }

    return ans;
}

double det(double **src, int columns_src) {
    int *used = calloc(columns_src, sizeof(int));
    int *generate = calloc(columns_src, sizeof(int));
    double ans = determinant(src, columns_src, 0, used, generate);

    free(used);
    free(generate);

    return ans;
}
