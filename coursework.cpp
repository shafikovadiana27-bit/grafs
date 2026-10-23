#include <iostream>
#include <cstring>
#include <set>
#include <list>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <bitset>
#include <chrono>

using namespace std;
using namespace std::chrono;

const string U = "0123456789ABCDEF";

/*
0123AB
45CD
6789EF
1ACF
*/

/*
0123GZ
45*#
6789?
1ACF
*/

// Проверка корректного символа
bool is_valid_char(char c) {
    return U.find(toupper(c)) != string::npos;
}

// Преобразование строки в множество
set<char> str_to_set(const string& input) {
    set<char> result;

    for (char c : input) {
        c = toupper(c);

        if (is_valid_char(c)) {
            result.insert(c);
        }
    }

    return result;
}

// Генерация случайного множества
set<char> random() {
    set<char> result;

    for (int i = 0; i < 16; ++i) {
        result.insert(U[rand() % 16]);
    }

    return result;
}

// 1 способ: операции с множествами (set)
set<char> res_e(
    const set<char>& a,
    const set<char>& b,
    const set<char>& c,
    const set<char>& d
) {
    set<char> ac_union, bd_union, result;

    set_union(
        a.begin(), a.end(),
        c.begin(), c.end(),
        inserter(ac_union, ac_union.begin())
    );

    set_union(
        b.begin(), b.end(),
        d.begin(), d.end(),
        inserter(bd_union, bd_union.begin())
    );

    set_difference(
        ac_union.begin(), ac_union.end(),
        bd_union.begin(), bd_union.end(),
        inserter(result, result.begin())
    );

    return result;
}

// Преобразование set в линейный список
list<char> set_to_list(const set<char>& s) {
    return list<char>(s.begin(), s.end());
}

// 2 способ: операции с линейными списками
list<char> res_e_list(
    const list<char>& a,
    const list<char>& b,
    const list<char>& c,
    const list<char>& d
) {
    list<char> ac_union = a;
    ac_union.insert(ac_union.end(), c.begin(), c.end());
    ac_union.sort();
    ac_union.unique();

    list<char> bd_union = b;
    bd_union.insert(bd_union.end(), d.begin(), d.end());
    bd_union.sort();
    bd_union.unique();

    list<char> result;

    auto it_ac = ac_union.begin();
    auto it_bd = bd_union.begin();

    while (it_ac != ac_union.end()) {
        if (it_bd == bd_union.end() || *it_ac < *it_bd) {
            result.push_back(*it_ac);
            ++it_ac;
        }
        else if (*it_ac == *it_bd) {
            ++it_ac;
            ++it_bd;
        }
        else {
            ++it_bd;
        }
    }

    return result;
}

int main() {
    srand(time(0));

    set<char> a, b, c, d, e;

    int cycles = 200000;
    int ch = 0;

    char A[80], B[80], C[80], D[80];

    cout << "Choose, how do you want to fill sets\n"
         << " 1)Keyboard\n"
         << " 2)Random\n"
         << "Your choice: ";

    cin >> ch;
    cout << endl;

    if (ch == 1) {
        cout << "Enter set A: ";
        cin >> A;

        cout << "Enter set B: ";
        cin >> B;

        cout << "Enter set C: ";
        cin >> C;

        cout << "Enter set D: ";
        cin >> D;

        a = str_to_set(A);
        b = str_to_set(B);
        c = str_to_set(C);
        d = str_to_set(D);
    }
    else {
        a = random();
        b = random();
        c = random();
        d = random();
    }

    cout << "\nInput sets\n";

    cout << "A=";
    for (char r : a) cout << r;
    cout << "\n";

    cout << "B=";
    for (char r : b) cout << r;
    cout << "\n";

    cout << "C=";
    for (char r : c) cout << r;
    cout << "\n";

    cout << "D=";
    for (char r : d) cout << r;
    cout << "\n";

    cout << "\n-Results-" << endl;

    // 1 способ: set
    auto start = high_resolution_clock::now();

    for (int i = 0; i < cycles; ++i) {
        e = res_e(a, b, c, d);
    }

    auto stop = high_resolution_clock::now();
    auto durat = duration_cast<duration<double, micro>>(stop - start);

    cout << "1 result (set): E=";

    for (char r : e) cout << r;

    cout << "\nWork time: " << durat.count() / cycles << " us\n";

    // 2 способ: линейные списки
    list<char> a_list = set_to_list(a);
    list<char> b_list = set_to_list(b);
    list<char> c_list = set_to_list(c);
    list<char> d_list = set_to_list(d);

    list<char> e_list;

    auto start2 = high_resolution_clock::now();

    for (int i = 0; i < cycles; ++i) {
        e_list = res_e_list(a_list, b_list, c_list, d_list);
    }

    auto stop2 = high_resolution_clock::now();
    auto dur2 = duration_cast<duration<double, micro>>(stop2 - start2);

    cout << "\n2 result (linear lists): E=";

    for (char r : e_list) cout << r;

    cout << "\nWork time: " << dur2.count() / cycles << " us\n";

    // 3 способ: bitset
    bitset<16> Abit, Bbit, Cbit, Dbit, Ebit;

    for (char r : a) Abit.set(U.find(r));
    for (char r : b) Bbit.set(U.find(r));
    for (char r : c) Cbit.set(U.find(r));
    for (char r : d) Dbit.set(U.find(r));

    auto start3 = high_resolution_clock::now();

    for (int i = 0; i < cycles; i++) {
        Ebit = (Abit | Cbit) & ~(Bbit | Dbit);
    }

    auto stop3 = high_resolution_clock::now();
    auto dur3 = duration_cast<duration<double, micro>>(stop3 - start3);

    cout << "\n3 result (bitset): E=";

    for (int i = 0; i < 16; i++) {
        if (Ebit.test(i)) cout << U[i];
    }

    cout << "\nWork time: " << dur3.count() / cycles << " us\n";

    // 4 способ: uint16_t (машинное слово)
    uint16_t Ab = 0, Bb = 0, Cb = 0, Db = 0, Eb = 0;

    for (char r : a) Ab |= (1u << U.find(r));
    for (char r : b) Bb |= (1u << U.find(r));
    for (char r : c) Cb |= (1u << U.find(r));
    for (char r : d) Db |= (1u << U.find(r));

    auto start4 = high_resolution_clock::now();

    for (int i = 0; i < cycles; i++) {
        Eb = (Ab | Cb) & ~(Bb | Db);
    }

    auto stop4 = high_resolution_clock::now();
    auto dur4 = duration_cast<duration<double, micro>>(stop4 - start4);

    cout << "\n4 result (machine word): E=";

    for (int i = 0; i < 16; i++) {
        if (Eb & (1u << i)) cout << U[i];
    }

    cout << "\nWork time: " << dur4.count() / cycles << " us\n";

    // Сравнение и вывод самого быстрого способа
    double times[4] = {
        durat.count() / cycles,
        dur2.count() / cycles,
        dur3.count() / cycles,
        dur4.count() / cycles
    };

    const char* names[4] = {
        "set",
        "linear list",
        "bitset",
        "machine word"
    };

    int min_index = 0;

    for (int i = 1; i < 4; i++) {
        if (times[i] < times[min_index]) {
            min_index = i;
        }
    }

    cout << "\nFastest method: "
         << names[min_index] << " "
         << times[min_index]
         << " us per cycle\n";

    return 0;
}
