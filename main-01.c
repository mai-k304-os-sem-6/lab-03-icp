#include <unistd.h> // Работа с POSIX
#include <stdlib.h> // Стандарт для СИ
#include <stdio.h> // Работа с вводом и выводом
#include <string.h> // Работа со строками

int main(int argc, char *argv[]) { // Главная точка вхождения
    // Открытие файла
    char *fileName; // Имя файла
    fileName = argv[1]; // Взятие имени файла из аргумента
    FILE *file = fopen(fileName, "r"); // Открытие файла
    if (file == NULL) { // Проверка на открытие файла
        printf("!Ошибка: Файл не открыт\n"); // Сообщение об ошибки
        return -1; // Выключение программы при ошибки открытия
    }

    // Считываение строки из файла
    char buff[256]; // Буфер для считвания строки
    fgets(buff, sizeof(buff), file); // Считывание строки
    char* massive = ""; // Массив строки
    massive = buff; // Переписование строки в массив
    fclose(file); // Закрытие файла

    // Печать массива
    int dataProcessed; // Массив в канале данных
    int filePipes; // Номер канала данных
    filePipes = atoi(argv[2]); // Считывание номера канала данных из аргумента
    dataProcessed = write(filePipes, massive, strlen(massive)); // Запись массива в канал данных

    // Вывод информации
    printf("File: ", fileName, "\n"); // Вывод информации о файле
    printf("Massive: ", massive, "\n"); // Вывод содержимого файла
    printf("Write: ", dataProcessed, " bytes", "\n"); // Вывод размер данных

    return 0; // Завершение программы
}