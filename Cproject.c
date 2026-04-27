#include <stdio.h>
#include <locale.h>
#include <string.h>

#define MAX_SOFTWARE 1000

enum Version { ALPHA, BETA, RELEASE, DEVELOPMENT, NONINFO };
enum OperatingSystem { WINDOWS, LINUX, MACOS, OTHER };

struct Date {
    int day, month, year;
};

struct Software {
    char name[100];
    double price;
    char info[200];
    enum Version version;
    struct Date releaseDate;
    int users;
    enum OperatingSystem os;
    double rating;
};

/* ПРОТОТИПЫ */
const char* versionToString(enum Version v);
const char* osToString(enum OperatingSystem os);
enum Version stringToVersion(const char* str);
enum OperatingSystem stringToOS(const char* str);

int saveToFile(struct Software* database, int count);
int loadFromFile(struct Software* database);
int addRecord(struct Software* database, int count);
void viewAllRecords(struct Software* database, int count);
int searchByPrice(struct Software* database, int count);
int combinedSearch(struct Software* database, int count);
void sortData(struct Software* database, int count);
int showMenu(void);

/* MAIN */
int main(void) {
    setlocale(LC_ALL, "RUS");

    struct Software database[MAX_SOFTWARE];
    int count = 0;

    count = loadFromFile(database);

    int choice;

    do {
        choice = showMenu();

        switch (choice) {
        case 1:
            viewAllRecords(database, count);
            break;
        case 2:
            count = addRecord(database, count);
            break;
        case 3:
            saveToFile(database, count);
            break;
        case 4:
            combinedSearch(database, count);
            break;
        case 5:
            searchByPrice(database, count);
            break;
        case 6:
            sortData(database, count);
            break;
        case 7:
            count = loadFromFile(database);
            break;
        case 8:
            printf("\nВыход из программы...\n");
            break;
        default:
            printf("\nНеверный выбор! Попробуйте снова.\n");
        }

    } while (choice != 8);

    saveToFile(database, count);

    return 0;
}

/* МЕНЮ */
int showMenu(void) {
    int choice;

    printf("\n========== ГЛАВНОЕ МЕНЮ ==========\n");
    printf("1. Просмотреть все записи\n");
    printf("2. Добавить новую запись\n");
    printf("3. Сохранить данные в файл\n");
    printf("4. Комбинированный поиск (версия + рейтинг)\n");
    printf("5. Поиск по цене\n");
    printf("6. Сортировка данных\n");
    printf("7. Загрузить данные из файла\n");
    printf("8. Выход\n");

    printf("Выберите действие: ");
    scanf("%d", &choice);

    return choice;
}

/* ПРЕОБРАЗОВАНИЯ */
const char* versionToString(enum Version v) {
    const char* arr[] = {"Alpha","Beta","Release","Development","NonInfo"};
    if (v < 0 || v > 4) return "NonInfo";
    return *(arr + v);
}

const char* osToString(enum OperatingSystem os) {
    const char* arr[] = {"Windows","Linux","MacOS","Other"};
    if (os < 0 || os > 3) return "Other";
    return *(arr + os);
}

enum Version stringToVersion(const char* str) {
    const char* arr[] = {"Alpha","Beta","Release","Development","NonInfo"};
    for (int i = 0; i < 5; i++)
        if (strcmp(str, *(arr + i)) == 0) return i;
    return NONINFO;
}

enum OperatingSystem stringToOS(const char* str) {
    const char* arr[] = {"Windows","Linux","MacOS","Other"};
    for (int i = 0; i < 4; i++)
        if (strcmp(str, *(arr + i)) == 0) return i;
    return OTHER;
}

/* ФАЙЛ */
int saveToFile(struct Software* database, int count) {
    FILE* f = fopen("database.txt", "w");

    if (!f) {
        printf("Ошибка: не удалось открыть файл!\n");
        return -1;
    }

    fprintf(f, "# База данных ПО\n\n");

    for (int i = 0; i < count; i++) {
        fprintf(f,"%s|%.2f|%s|%s|%d.%d.%d|%d|%s|%.1f\n",
            database[i].name,
            database[i].price,
            database[i].info,
            versionToString(database[i].version),
            database[i].releaseDate.day,
            database[i].releaseDate.month,
            database[i].releaseDate.year,
            database[i].users,
            osToString(database[i].os),
            database[i].rating);
    }

    fclose(f);

    printf("Данные успешно сохранены (%d записей)\n", count);
    return count;
}

int loadFromFile(struct Software* database) {
    FILE* f = fopen("database.txt", "r");

    if (!f) {
        printf("Файл не найден. Он будет создан при сохранении.\n");
        return 0;
    }

    int count = 0;
    char line[500];

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        struct Software t;
        char ver[20], date[20], os[20];

        if (sscanf(line,"%[^|]|%lf|%[^|]|%[^|]|%[^|]|%d|%[^|]|%lf",
            t.name,&t.price,t.info,ver,date,&t.users,os,&t.rating) == 8) {

            t.version = stringToVersion(ver);
            t.os = stringToOS(os);

            sscanf(date,"%d.%d.%d",
                &t.releaseDate.day,
                &t.releaseDate.month,
                &t.releaseDate.year);

            database[count++] = t;
        }
    }

    fclose(f);

    printf("Загружено записей: %d\n", count);
    return count;
}

/* ДОБАВЛЕНИЕ */
int addRecord(struct Software* database, int count) {
    if (count >= MAX_SOFTWARE) {
        printf("Ошибка: достигнут лимит записей!\n");
        return count;
    }

    printf("\n=== ДОБАВЛЕНИЕ НОВОЙ ЗАПИСИ ===\n");

    printf("Введите название программы: ");
    scanf(" %[^\n]", database[count].name);

    printf("Введите цену: ");
    scanf("%lf", &database[count].price);

    printf("Введите информацию: ");
    scanf(" %[^\n]", database[count].info);

    printf("Выберите версию:\n");
    printf("0 - Alpha\n1 - Beta\n2 - Release\n3 - Development\n4 - NonInfo\n");

    int v;
    scanf("%d", &v);
    database[count].version = v;

    printf("Введите дату (день месяц год): ");
    scanf("%d %d %d",
        &database[count].releaseDate.day,
        &database[count].releaseDate.month,
        &database[count].releaseDate.year);

    printf("Введите количество пользователей: ");
    scanf("%d", &database[count].users);

    printf("Выберите ОС:\n");
    printf("0 - Windows\n1 - Linux\n2 - MacOS\n3 - Other\n");

    int o;
    scanf("%d", &o);
    database[count].os = o;

    printf("Введите рейтинг (0.0 - 5.0): ");
    scanf("%lf", &database[count].rating);

    printf("Запись успешно добавлена!\n");

    return count + 1;
}

/* ПРОСМОТР */
void viewAllRecords(struct Software* database, int count) {
    if (count == 0) {
        printf("База данных пуста!\n");
        return;
    }

    printf("\n=== ВСЕ ЗАПИСИ (%d) ===\n", count);

    for (int i = 0; i < count; i++) {
        printf("\n[Запись %d]\n", i + 1);
        printf("  Название: %s\n", database[i].name);
        printf("  Цена: %.2f руб.\n", database[i].price);
        printf("  Информация: %s\n", database[i].info);
        printf("  Версия: %s\n", versionToString(database[i].version));
        printf("  Дата: %d.%d.%d\n",
            database[i].releaseDate.day,
            database[i].releaseDate.month,
            database[i].releaseDate.year);
        printf("  Пользователи: %d\n", database[i].users);
        printf("  ОС: %s\n", osToString(database[i].os));
        printf("  Рейтинг: %.1f/5.0\n", database[i].rating);
        printf("----------------------------------------\n");
    }
}

/* ПОИСК ПО ЦЕНЕ */
int searchByPrice(struct Software* database, int count) {
    if (count == 0) {
        printf("База данных пуста!\n");
        return 0;
    }

    printf("\n=== ПОИСК ПО ЦЕНЕ ===\n");
    printf("1 - По максимальной цене (дешевле чем...)\n");
    printf("2 - По минимальной цене (дороже чем...)\n");
    printf("3 - В диапазоне цен\n");
    printf("Ваш выбор: ");

    int choice;
    scanf("%d", &choice);

    int found = 0;

    switch (choice) {
    case 1: {
        double max;
        printf("Введите максимальную цену: ");
        scanf("%lf", &max);

        printf("\nПрограммы дешевле %.2f:\n", max);

        for (int i = 0; i < count; i++) {
            if (database[i].price <= max) {
                printf("[%d] %s - %.2f руб.\n", i + 1, database[i].name, database[i].price);
                found++;
            }
        }
        break;
    }
    case 2: {
        double min;
        printf("Введите минимальную цену: ");
        scanf("%lf", &min);

        printf("\nПрограммы дороже %.2f:\n", min);

        for (int i = 0; i < count; i++) {
            if (database[i].price >= min) {
                printf("[%d] %s - %.2f руб.\n", i + 1, database[i].name, database[i].price);
                found++;
            }
        }
        break;
    }
    case 3: {
        double min, max;
        printf("Введите минимальную цену: ");
        scanf("%lf", &min);
        printf("Введите максимальную цену: ");
        scanf("%lf", &max);

        printf("\nПрограммы в диапазоне %.2f - %.2f:\n", min, max);

        for (int i = 0; i < count; i++) {
            if (database[i].price >= min && database[i].price <= max) {
                printf("[%d] %s - %.2f руб.\n", i + 1, database[i].name, database[i].price);
                found++;
            }
        }
        break;
    }
    default:
        printf("Неверный выбор!\n");
        return 0;
    }

    if (found == 0)
        printf("Ничего не найдено.\n");
    else
        printf("Найдено записей: %d\n", found);

    return found;
}

/* КОМБИНИРОВАННЫЙ ПОИСК */
int combinedSearch(struct Software* database, int count) {
    if (count == 0) {
        printf("База данных пуста!\n");
        return 0;
    }

    printf("\n=== КОМБИНИРОВАННЫЙ ПОИСК ===\n");

    int v;
    printf("Выберите версию (0-4): ");
    scanf("%d", &v);

    double r;
    printf("Минимальный рейтинг: ");
    scanf("%lf", &r);

    int found = 0;

    for (int i = 0; i < count; i++) {
        if (database[i].version == v && database[i].rating >= r) {
            printf("[%d] %s\n", i + 1, database[i].name);
            printf("    Версия: %s\n", versionToString(database[i].version));
            printf("    Рейтинг: %.1f\n", database[i].rating);
            found++;
        }
    }

    if (found == 0)
        printf("Ничего не найдено.\n");
    else
        printf("Найдено записей: %d\n", found);

    return found;
}

/* СОРТИРОВКА */
void sortData(struct Software* database, int count) {
    if (count == 0) {
        printf("База данных пуста!\n");
        return;
    }

    printf("\n=== СОРТИРОВКА ===\n");
    printf("1 - По рейтингу (убывание)\n");
    printf("2 - По цене (возрастание)\n");
    printf("3 - По рейтингу (убывание) и цене (возрастание)\n");
    printf("Ваш выбор: ");

    int choice;
    scanf("%d", &choice);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {

            int swap = 0;

            if (choice == 1 && database[j].rating < database[j + 1].rating) swap = 1;
            if (choice == 2 && database[j].price > database[j + 1].price) swap = 1;
            if (choice == 3 && (database[j].rating < database[j + 1].rating ||
                (database[j].rating == database[j + 1].rating &&
                 database[j].price > database[j + 1].price))) swap = 1;

            if (swap) {
                struct Software t = database[j];
                database[j] = database[j + 1];
                database[j + 1] = t;
            }
        }
    }

    printf("Сортировка завершена!\n");
}
