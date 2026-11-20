#include <cstddef>
#include <iostream>
#include <limits>

std::size_t lengthOf(const char* s) {
    std::size_t n = 0;
    while (s[n] != '\0') {
        ++n;
    }
    return n;
}

char lowerChar(char c) {
    if (c >= 'A' && c <= 'Z') {
        return static_cast<char>(c - 'A' + 'a');
    }
    return c;
}

struct Stats {
    std::size_t comparisons;
    std::size_t positions;
};

bool sameSymbol(char a, char b, bool ignoreCase) {
    if (ignoreCase) {
        a = lowerChar(a);
        b = lowerChar(b);
    }
    return a == b;
}

template <typename T>
bool sameSymbol(const T& a, const T& b, bool) {
    return a == b;
}

template <typename T>
long long firstOccurrence(const T* text, std::size_t n, const T* pat, std::size_t m, bool ignoreCase, Stats& st) {
    if (m == 0) {
        return 0;
    }
    if (m > n) {
        return -1;
    }
    for (std::size_t i = 0; i + m <= n; ++i) {
        ++st.positions;
        std::size_t j = 0;
        while (j < m) {
            ++st.comparisons;
            if (!sameSymbol(text[i + j], pat[j], ignoreCase)) {
                break;
            }
            ++j;
        }
        if (j == m) {
            return static_cast<long long>(i);
        }
    }
    return -1;
}

template <typename T>
std::size_t allOccurrences(const T* text, std::size_t n, const T* pat, std::size_t m, bool ignoreCase, Stats& st, std::size_t* out) {
    if (m == 0) {
        out[0] = 0;
        return 1;
    }
    if (m > n) {
        return 0;
    }
    std::size_t count = 0;
    for (std::size_t i = 0; i + m <= n; ++i) {
        ++st.positions;
        std::size_t j = 0;
        while (j < m) {
            ++st.comparisons;
            if (!sameSymbol(text[i + j], pat[j], ignoreCase)) {
                break;
            }
            ++j;
        }
        if (j == m) {
            out[count] = i;
            ++count;
        }
    }
    return count;
}

void printList(const std::size_t* a, std::size_t k) {
    std::cout << "[";
    for (std::size_t i = 0; i < k; ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << a[i];
    }
    std::cout << "]";
}

struct TextCase {
    const char* text;
    const char* pattern;
    long long first;
    std::size_t count;
    std::size_t occ[4];
};

const TextCase textCases[] = {
    {"algorithm", "algo", 0, 1, {0}},
    {"algorithm", "rith", 4, 1, {4}},
    {"algorithm", "thm", 6, 1, {6}},
    {"algorithm", "sort", -1, 0, {}},
    {"aaaa", "aa", 0, 3, {0, 1, 2}},
    {"abababa", "aba", 0, 3, {0, 2, 4}},
    {"abc", "abcd", -1, 0, {}},
    {"Data data DATA", "data", 5, 1, {5}},
    {"one two two", "two", 4, 2, {4, 8}},
    {"1, 2, 1, 2, 1", "1, 2, 1", 0, 2, {0, 6}},
    {"4, 5, 6", "5, 7", -1, 0, {}},
    {"", "abc", -1, 0, {}},
    {"mississippi river", "issi", 1, 2, {1, 4}},
    {"aaaaabaaaaabaaaa", "aaaab", 1, 2, {1, 7}},
    {"abc", "", 0, 1, {0}},
};

const int intArrayA[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
const int intPatA[] = {1, 5};
const int intArrayB[] = {7, 7, 7, 7};
const int intPatB[] = {7, 7};
const int intArrayC[] = {1, 2, 3};
const int intPatC[] = {0};

template <typename T>
int executeCase(const T* text, std::size_t n, const T* pat, std::size_t m, long long expFirst, std::size_t expCount, const std::size_t* expOcc, int id) {
    Stats s1{0, 0};
    long long f = firstOccurrence(text, n, pat, m, false, s1);
    std::size_t* buf = new std::size_t[n + 1];
    Stats s2{0, 0};
    std::size_t cnt = allOccurrences(text, n, pat, m, false, s2, buf);
    bool okF = f == expFirst;
    bool okA = cnt == expCount;
    for (std::size_t k = 0; okA && k < expCount; ++k) {
        okA = buf[k] == expOcc[k];
    }
    std::cout << "Тест " << (id < 10 ? "0" : "") << id << "  перше=" << f << "  усі=";
    printList(buf, cnt);
    std::cout << "  порівнянь=" << s1.comparisons << "/" << s2.comparisons;
    std::cout << "  позицій=" << s1.positions << "/" << s2.positions;
    std::cout << "  " << (okF && okA ? "УСПІШНО" : "ПОМИЛКА") << "\n";
    delete[] buf;
    return (okF ? 1 : 0) + (okA ? 2 : 0);
}

const std::size_t occA[] = {3};
const std::size_t occB[] = {0, 1, 2};
const std::size_t occC[] = {0};

void runTests() {
    std::cout << "Автоматичні тести (перше входження / усі входження)\n";
    std::size_t total = 0;
    std::size_t okFirst = 0;
    std::size_t okAll = 0;
    int id = 0;
    const std::size_t tc = sizeof(textCases) / sizeof(textCases[0]);
    for (std::size_t k = 0; k < tc; ++k) {
        const TextCase& c = textCases[k];
        std::size_t n = lengthOf(c.text);
        std::size_t m = lengthOf(c.pattern);
        ++id;
        int r = executeCase(c.text, n, c.pattern, m, c.first, c.count, c.occ, id);
        ++total;
        if (r & 1) {
            ++okFirst;
        }
        if (r & 2) {
            ++okAll;
        }
    }
    ++id;
    int r1 = executeCase(intArrayA, 11, intPatA, 2, 3, 1, occA, id);
    ++total;
    okFirst += r1 & 1;
    okAll += (r1 >> 1) & 1;
    ++id;
    int r2 = executeCase(intArrayB, 4, intPatB, 2, 0, 3, occB, id);
    ++total;
    okFirst += r2 & 1;
    okAll += (r2 >> 1) & 1;
    ++id;
    int r3 = executeCase(intArrayC, 3, intPatC, 0, 0, 1, occC, id);
    ++total;
    okFirst += r3 & 1;
    okAll += (r3 >> 1) & 1;
    std::cout << "Всього тестів: " << total << "\n";
    std::cout << "Перше входження пройдено: " << okFirst << " з " << total << "\n";
    std::cout << "Усі входження пройдено: " << okAll << " з " << total << "\n";
}

void runVariant() {
    const char* text = "merge sort merges sorted parts";
    const char* pat = "merge";
    std::size_t n = lengthOf(text);
    std::size_t m = lengthOf(pat);
    std::cout << "Варіант 22\n";
    std::cout << "Текст:    " << text << "\n";
    std::cout << "Зразок: " << pat << "\n\n";
    Stats s1{0, 0};
    long long f = firstOccurrence(text, n, pat, m, false, s1);
    std::cout << "Перше входження: " << f << "\n";
    std::cout << "Порівнянь: " << s1.comparisons << ", перевірено позицій: " << s1.positions << "\n\n";
    std::size_t* buf = new std::size_t[n + 1];
    Stats s2{0, 0};
    std::size_t cnt = allOccurrences(text, n, pat, m, false, s2, buf);
    std::cout << "Усі входження: ";
    printList(buf, cnt);
    std::cout << "\nКількість: " << cnt << "\n";
    std::cout << "Порівнянь: " << s2.comparisons << ", перевірено позицій: " << s2.positions << "\n\n";
    std::cout << "Повтор без урахування регістру (текст не змінено):\n";
    Stats s3{0, 0};
    long long fi = firstOccurrence(text, n, pat, m, true, s3);
    Stats s6{0, 0};
    std::size_t cntI = allOccurrences(text, n, pat, m, true, s6, buf);
    std::cout << "Зразок: " << pat << "  перше=" << fi << "  усі=";
    printList(buf, cntI);
    std::cout << "  порівнянь (перше / усі)=" << s3.comparisons << " / " << s6.comparisons << "\n";
    const char* upper = "MERGE";
    Stats s4{0, 0};
    long long fu = firstOccurrence(text, n, upper, 5, false, s4);
    Stats s5{0, 0};
    long long fv = firstOccurrence(text, n, upper, 5, true, s5);
    std::cout << "Зразок MERGE, з урахуванням регістру: перше=" << fu << ", без урахування регістру: перше=" << fv << "\n\n";
    std::cout << "Трасування пошуку першого входження:\n";
    for (std::size_t i = 0; i + m <= n; ++i) {
        std::size_t j = 0;
        std::size_t c = 0;
        while (j < m) {
            ++c;
            if (text[i + j] != pat[j]) {
                break;
            }
            ++j;
        }
        std::cout << "позиція " << (i < 10 ? " " : "") << i << ": збіг " << j << " з " << m << ", порівнянь " << c << (j == m ? "  ЗБІГ" : "") << "\n";
    }
    delete[] buf;
}

void runExperiment() {
    const std::size_t sizes[3] = {100, 1000, 10000};
    const char* pattern = "abcabd";
    const char* longPattern = "aaaaaaaaab";
    std::size_t m = lengthOf(pattern);
    std::size_t ml = lengthOf(longPattern);
    std::cout << "Експеримент: перше входження\n";
    for (std::size_t s = 0; s < 3; ++s) {
        std::size_t n = sizes[s];
        char* text = new char[n + 1];
        for (int sc = 0; sc < 4; ++sc) {
            char letter = (sc == 3) ? 'a' : 'x';
            for (std::size_t k = 0; k < n; ++k) {
                text[k] = letter;
            }
            text[n] = '\0';
            std::size_t place = 0;
            if (sc == 1) {
                place = n / 2;
            } else if (sc == 2) {
                place = n - m;
            }
            const char* pat = pattern;
            std::size_t pm = m;
            long long expected = static_cast<long long>(place);
            if (sc == 3) {
                pat = longPattern;
                pm = ml;
                expected = -1;
            } else {
                for (std::size_t k = 0; k < m; ++k) {
                    text[place + k] = pattern[k];
                }
            }
            Stats st{0, 0};
            long long f = firstOccurrence(text, n, pat, pm, false, st);
            char name = "ABCD"[sc];
            std::cout << "n=" << n << "  сценарій " << name << "  індекс=" << f << "  порівнянь=" << st.comparisons << "  позицій=" << st.positions << "  " << (f == expected ? "OK" : "ПОМИЛКА") << "\n";
        }
        delete[] text;
    }
}

void customSearch() {
    char text[257];
    char pat[129];
    char mode;
    std::cout << "Текст (до 256 символів): ";
    std::cin.getline(text, 257);
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Помилка: текст занадто довгий\n";
        return;
    }
    std::cout << "Зразок (до 128 символів): ";
    std::cin.getline(pat, 129);
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Помилка: зразок занадто довгий\n";
        return;
    }
    std::cout << "Враховувати регістр (y/n): ";
    std::cin >> mode;
    bool ic = (mode == 'n' || mode == 'N');
    std::size_t n = lengthOf(text);
    std::size_t m = lengthOf(pat);
    Stats s1{0, 0};
    long long f = firstOccurrence(text, n, pat, m, ic, s1);
    std::size_t* buf = new std::size_t[n + 1];
    Stats s2{0, 0};
    std::size_t cnt = allOccurrences(text, n, pat, m, ic, s2, buf);
    std::cout << "Перше входження: " << f << "\n";
    std::cout << "Усі входження: ";
    printList(buf, cnt);
    std::cout << "\nПорівнянь (перше / усі): " << s1.comparisons << " / " << s2.comparisons << "\n";
    std::cout << "Перевірено позицій (перше / усі): " << s1.positions << " / " << s2.positions << "\n";
    delete[] buf;
}

int main() {
    int choice = -1;
    while (true) {
        std::cout << "\nМеню\n";
        std::cout << "1. Автоматичні тести\n";
        std::cout << "2. Варіант 22\n";
        std::cout << "3. Експеримент\n";
        std::cout << "4. Власний пошук\n";
        std::cout << "0. Вихід\n";
        std::cout << "Вибір: ";
        if (!(std::cin >> choice)) {
            break;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 0) {
            break;
        } else if (choice == 1) {
            runTests();
        } else if (choice == 2) {
            runVariant();
        } else if (choice == 3) {
            runExperiment();
        } else if (choice == 4) {
            customSearch();
        } else {
            std::cout << "Невірний вибір\n";
        }
    }
    return 0;
}
