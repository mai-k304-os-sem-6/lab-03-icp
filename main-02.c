#include <unistd.h> // Работа с POSIX
#include <stdlib.h> // Стандарт для СИ
#include <stdio.h> // Работа с вводом и выводом
#include <string.h> // Работа со строками
#include <sys/types.h> // Системные типы данных
#include <sys/wait.h> // Возможность кидать в ожидание

int main(int argc, char *argv[]) { // Главная точка вхождения
    // Объявление переменных
    int dataProcessed_1; // Массив в канале данных первого файла
    int dataProcessed_2; // Массив в канале данных второго файла
    int filePipes_1[2]; // Номер канала данных первого файла
    int filePipes_2[2]; // Номер канала данных второго файла
    pid_t forkResult_1; // Процесс для первого файла
    pid_t forkResult_2; // Процесс для второго файла
    char temp_1[BUFSIZ + 1]; // Врменный массив для информации из первого процесса
    char temp_2[BUFSIZ + 1]; // Врменный массив для информации из второго процесса
    char buffer[BUFSIZ + 1]; // Буфер для информации
    memset(buffer, '\0', sizeof(buffer)); // Указание размера буфера

    // Получение данных из канала данных
    if (pipe(filePipes_1) == 0 && pipe(filePipes_2) == 0) { // Проверка на наличие каналов данных
        forkResult_1 = fork(); // Создание первого процесса
        if (forkResult_1 == (pid_t) - 1) exit(EXIT_FAILURE); // Проверка на корректность первого процесса
        if (forkResult_1 == 0) { // Если мы в первом процессе потомке
            (void)execl(argv[2], argv[2], argv[3], buffer, (char*)0); // Запуска первого процесса
            exit(EXIT_FAILURE); // Завершение процесса
        } else { // Если мы в процессе основной программе
            forkResult_2 = fork(); // Создать второй процесс
            if (forkResult_2 == 0) { // Если мы в процессе второго потомка
                (void)execl(argv[2], argv[2], argv[4], buffer, (char*)0); // Запуск второго процесса
                exit(EXIT_FAILURE); // Завершение процесса
            } else { // Если мы в основной программе
                wait(0); // В ожидание первый процесс
                wait(0); // В ожидание второй процесс
                dataProcessed_1 = read(filePipes_1[0], temp_1, BUFSIZ); // Запись первого процесса
                dataProcessed_2 = read(filePipes_2[0], temp_2, BUFSIZ); // Запись второго процесса
                printf("Read ", dataProcessed_1, " bytes: ", temp_1, "\n"); // Вывод информации о первом процессе
                printf("Read ", dataProcessed_2, " bytes: ", temp_2, "\n"); // Вывод информации о втором процессе
            }
        }
    }

    // Шифровка и запись в файл
    char temp_3[BUFSIZ + 1]; // Временный массив для хранения исходных данных
    FILE *output = fopen("output", "w+"); // Открытие файла с результатов
    // Создание побитной матрицы XOR
    int i = 0;
    for (i = 0; i < strlen(temp_1); i++) { // Цикл по массиву из файла
        if (strlen(temp_1) < i) temp_1[i] = 0; // Если символы в первом массиве закончились
        if (strlen(temp_2) < i) temp_2[i] = 0; // Если символы во втором массиве закончились
        temp_3[i] = temp_1[i] ^ temp_2[i]; // XOR двух массивов
        printf("output[",i,"]: ",(int)temp_1[i]," ^ ",(int)temp_2[i]," = ",temp_3[i],"\n"); // Вывод промежуточного этапа
        fprintf(output, "%c", temp_3[i]); // Запись в файл символа
    }

    // Вывод конечного результата
    printf("output:");
    for(int j = 0; j < i; j++) printf(temp_3[j], "|", (int)temp_3[j], "  "); // Печать конечного результата
    printf("\n");
    exit(EXIT_SUCCESS); // Завершение программы
}