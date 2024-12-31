#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Matris çarpımı fonksiyonu (seri)
void multiply_matrices_serial(int* A, int* B, int* C, int rowsA, int colsA, int colsB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            C[i * colsB + j] = 0;
            for (int k = 0; k < colsA; k++) {
                C[i * colsB + j] += A[i * colsA + k] * B[k * colsB + j];
            }
        }
    }
}

// Matris çarpımı fonksiyonu (paralel)
void multiply_matrices_parallel(int* A, int* B, int* C, int rowsA, int colsA, int colsB) {
    #pragma omp parallel for
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            C[i * colsB + j] = 0;
            for (int k = 0; k < colsA; k++) {
                C[i * colsB + j] += A[i * colsA + k] * B[k * colsB + j];
            }
        }
    }
}

// Matris okuma fonksiyonu
void read_matrix(const char* filename, int** matrix, int* rows, int* cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya açılmadı: %s\n", filename);
        exit(1);  // Programı sonlandır
    }
    fscanf(file, "%d %d", rows, cols);

    *matrix = (int*)malloc((*rows) * (*cols) * sizeof(int));

    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            fscanf(file, "%d", &(*matrix)[i * (*cols) + j]);
        }
    }
    fclose(file);
}

// Matrisi dosyaya yazma fonksiyonu
void write_matrix(const char* filename, int* matrix, int rows, int cols) {
    FILE* file = fopen(filename, "w");
    fprintf(file, "%d %d\n", rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrix[i * cols + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Kullanım: %s matrix_a.txt matrix_b.txt sonuclar.txt\n", argv[0]);
        return 1;
    }

    int *A, *B, *C_serial, *C_parallel;
    int rowsA, colsA, rowsB, colsB;

    // Dosyalardan matrisleri oku
    read_matrix(argv[1], &A, &rowsA, &colsA);
    read_matrix(argv[2], &B, &rowsB, &colsB);

    // Sonuç matrislerini oluştur
    if (colsA != rowsB) {
        printf("Matris boyutları uyumsuz ancak işlem yapılacak.\n");
    }

    C_serial = (int*)malloc(rowsA * colsB * sizeof(int));
    C_parallel = (int*)malloc(rowsA * colsB * sizeof(int));

    // Seri matris çarpımı
    clock_t start_serial = clock();
    multiply_matrices_serial(A, B, C_serial, rowsA, colsA, colsB);
    clock_t end_serial = clock();
    double serial_time = ((double)(end_serial - start_serial)) / CLOCKS_PER_SEC;

    // Paralel matris çarpımı
    clock_t start_parallel = clock();
    multiply_matrices_parallel(A, B, C_parallel, rowsA, colsA, colsB);
    clock_t end_parallel = clock();
    double parallel_time = ((double)(end_parallel - start_parallel)) / CLOCKS_PER_SEC;

    // Sonuçları dosyaya yaz
    write_matrix(argv[3], C_parallel, rowsA, colsB);

    // Zamanları yazdır
    printf("Seri matris çarpımı süresi: %f saniye\n", serial_time);
    printf("Paralel matris çarpımı süresi: %f saniye\n", parallel_time);

    // Bellek temizleme
    free(A);
    free(B);
    free(C_serial);
    free(C_parallel);

    return 0;
}