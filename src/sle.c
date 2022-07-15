#include <stdio.h>
#include <stdlib.h>

int input(double **buffer, int rows, int columns);
void output(double **buffer, int rows, int columns);
void output_str(double **buffer, int len);
double ** matrix_init(int *rows, int *columns);
void free_m(double **a, int rows);
int inversions(int *permutation, int n);
double determinant(double **src, int columns_src, int index, int *used, int *generate);
double det(double **src, int columns_src);
double alg_addition(double **matrix, int rows, int columns, int i, int j);
double **invert(double **matrix, int rows, int columns);
double **solve(double **matrix, int rows, int columns);

int main() {
    int rows_a, columns_a;
    double **a, **result;

    a = matrix_init(&rows_a, &columns_a);

    if (a != NULL) {
        result = solve(a, rows_a, columns_a);
        output_str(result, columns_a - 1);
        free_m(result, columns_a);
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

void output(double **buffer, int rows, int columns) {
    for (int i = 0; i < rows - 1; i++) {
        printf("%.6lf", buffer[i][0]);

        for (int j = 1; j < columns; j++)
            printf(" %.6lf", buffer[i][j]);

        printf("\n");
    }

    printf("%.6lf", buffer[rows - 1][0]);

    for (int j = 1; j < columns; j++)
        printf(" %.6lf", buffer[rows - 1][j]);
}

void output_str(double **buffer, int len) {
     printf("%.6lf", buffer[0][0]);

    for (int j = 1; j < len; j++)
        printf(" %.6lf", buffer[j][0]);
}

double ** matrix_init(int *rows, int *columns) {
    double **pointers_matrix = NULL;

    if (scanf("%d", rows) == 0 || *rows <= 0) {
        printf("n/a");
    } else if (scanf("%d", columns) == 0 || *columns <= 0) {
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

double alg_addition(double **matrix, int rows, int columns, int i, int j) {
    double **minor = malloc((rows - 1) * sizeof(double *));
    double ans;

    for (int k = 0; k < rows - 1; k++)
        minor[k] = malloc((columns - 1) * sizeof(double));

    for (int k = 0; k < rows; k++)
        for (int m = 0; m < columns; m++)
            if ((k != i) && (m != j))
                minor[k > i ? k - 1 : k][m > j ? m - 1 : m] = matrix[k][m];

    ans = det(minor, columns - 1);
    ans *= (i + j) % 2 == 0 ? 1.0 : -1.0;

    free_m(minor, rows - 1);
    return ans;
}

double **invert(double **matrix, int rows, int columns) {
    double **ans = malloc((columns) * sizeof(double *));
    double det_matrix = det(matrix, rows);

    for (int i = 0; i < columns; i++)
        ans[i] = malloc((rows) * sizeof(double));

    for (int i = 0; i < columns; i++)
        for (int j = 0; j < rows; j++)
            ans[i][j] = alg_addition(matrix, rows, columns, j, i) / det_matrix;


    return ans;
}

double **multiply(double **a, int rows_a, int columns_a, double **b, int columns_b) {
    double **ans = calloc(rows_a, sizeof(double *));

        for (int i = 0; i < rows_a; i++) {
            ans[i] = calloc(columns_b, sizeof(double));

            for (int j = 0; j < columns_b; j++) {
                ans[i][j] = 0;

                for (int k = 0; k < columns_a; k++)
                    ans[i][j] += a[i][k] * b[k][j];
            }
        }

    return ans;
}

double **solve(double **matrix, int rows, int columns) {
    double **a = calloc(rows, sizeof(double *));
    double **b = calloc(rows, sizeof(double *));
    double **res;



    for (int i = 0; i < rows; i++)
        a[i] = calloc(columns - 1, sizeof(double));

    for (int i = 0; i < rows; i++)
        b[i] = calloc(1, sizeof(double));

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns - 1; j++)
                a[i][j] = matrix[i][j];

    for (int i = 0; i < rows; i++)
        b[i][0] = matrix[i][columns - 1];

    res = multiply(invert(a, rows, columns - 1), columns - 1, rows, b, 1);
    free_m(a, rows);
    free_m(b, rows);
    return res;
}
