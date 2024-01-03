#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx;  // Мьютекс для синхронизации доступа к общим данным

// Функция для вычисления произведения матриц C = A * B
void matrixMultiplication(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C, int i, int j) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Задержка в выполнении вычислений

    int m = A.size();  // Размерность матрицы A
    int n = A[0].size();  // Размерность матрицы B
    int k = B[0].size();  // Размерность матрицы C

    if (j < k) {
        for (int l = j; l < k; ++l) {
            int sum = 0;
            for (int t = 0; t < n; ++t) {
                sum += A[i][t] * B[t][l];
            }
            mtx.lock();  // Блокировка мьютекса перед обновлением общих данных
            C[i][l] = sum;
            mtx.unlock();  // Разблокировка мьютекса
        }
    }
    else if (j == k) {
        int sum = 0;
        for (int t = 0; t < n; ++t) {
            sum += A[i][t] * B[t][j];
        }
        mtx.lock();  // Блокировка мьютекса перед обновлением общих данных
        C[i][j] = sum;
        mtx.unlock();  // Разблокировка мьютекса
    }
}

int main() {
    int m = 2, n = 3, k = 2;  // Размерности матриц A, B и C
    std::vector<std::vector<int>> A = { {2, 3, 4}, {5, 6, 7} };
    std::vector<std::vector<int>> B = { {8, 9}, {10, 11}, {12, 13} };
    std::vector<std::vector<int>> C(m, std::vector<int>(k, 0));  // Инициализация матрицы C нулями

    std::vector<std::thread> threads;  // Вектор для хранения потоков

    // Создание и запуск потоков для вычисления элементов матрицы C
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) {
            threads.emplace_back(matrixMultiplication, std::ref(A), std::ref(B), std::ref(C), i, j);
        }
    }

    // Ожидание завершения работы всех потоков
    for (auto& t : threads) {
        t.join();
    }

    // Вывод результата на экран
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) {
            std::cout << C[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // TODO: Запись результата в файл-протокол

    return 0;
}
