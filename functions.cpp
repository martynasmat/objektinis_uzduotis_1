#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <chrono>
#include "functions.h"

using namespace std;

bool compare_name(const Student& first, const Student& second) {
    return first.name > second.name;
}

bool compare_last_name(const Student& first, const Student& second) {
    return first.last_name > second.last_name;
}

bool compare_avg(const Student& first, const Student& second) {
    return first.final_res_avg > second.final_res_avg;
}

bool compare_med(const Student& first, const Student& second) {
    return first.final_res_med > second.final_res_med;
}

string generate_surname() {
    return SURNAMES[rand() % (NAME_COUNT - 1)];
}

string generate_name() {
    return NAMES[rand() % (NAME_COUNT - 1)];
}

int generate_mark() {
    return rand() % 9 + 2;
}

bool valid_mark(int input) {
    try {
        if (input < MIN_MARK) {
            throw(0);
        }else if(input > MAX_MARK) {
            throw(1);
        }
        return true;
    }
    catch (int err_num) {
        if (err_num == 0) {
            cout << "Bloga ivestis, pazymys turi buti lygus arba didesnis nei 2." << endl << endl;
            return false;
        } else if (err_num == 1) {
            cout << "Bloga ivestis, pazymys turi buti lygus arba mazenis nei 10." << endl << endl;
            return false;
        }
    }
}

bool valid_alphabet(string input) {
    try {
        if(input.length() > NAME_MAX_SYMBOLS || input.length() < NAME_MIN_SYMBOLS) {
            throw(0);
        }
        for (int i = 0; i < input.length(); i++) {
            if (!isalpha(input[i])) {
                throw(1);
            }
        }
        return true;
    }catch(int err_num) {
        if (err_num == 0) {
            cout << "Bloga ivestis, min. " << NAME_MIN_SYMBOLS << ", maks. " << NAME_MAX_SYMBOLS << " simboliu." << endl;
        } else if (err_num == 1) {
            cout << "Bloga ivestis, galima ivesti tik raides." << endl << endl;
        }
        return false;
    }
}

void read_data_from_file(const string& file_name, vector <Student> &stud) {
    stringstream buffer;
    int mark;
    bool first_line = true;
    // Read file
    auto start = std::chrono::high_resolution_clock::now(); auto st = start;
    try {
        ifstream file(file_name);
        if (!file) {
            throw(0);
        }
        while (!file.eof()) {
            if (first_line) {
                string line;
                getline(file, line);
                first_line = false;
            } else {
                Student student;
                file >> student.name >> student.last_name;
                do {
                    if (file.peek() == 32) {
                        file >> student.exam_res;
                        file.clear();
                        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    } else {
                        file >> mark;
                        student.hw_res.push_back(mark);
                    }
                } while (file.peek() == 10);
                student.final_res_avg = final(student.final_hw_avg, student.exam_res);
                student.final_res_med = final(student.final_hw_med, student.exam_res);
                stud.push_back(student);
            }
        }
        file.close();

        std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
        cout << "Failo nuskaitymas uztruko " << diff.count() << "s" << endl;
    }
    catch (int err_num) {
        if (err_num == 0) {
            cout << "Failo negalima atidaryti..." << endl;
            exit(1);
        }
    }
}

void read_data_from_console(vector <Student> &stud, bool use_median, bool gen_marks, bool gen_names) {
    bool do_continue = false;
    bool do_continue_inner = false;
    bool entered, entered_inner;
    string response;
    Student stud_var;
    int hw, mark;
    do{
        entered = false;
        if (gen_names) {
            string name = generate_name();
            string surname = generate_surname();
            cout << "Sugeneruotas vardas: " << name << endl;
            cout << "Sugeneruota pavarde: " << surname << endl;
            stud_var.name = name;
            stud_var.last_name = surname;
        }else {
            while (!entered) {
                cout << "Vardas (maks. 20 simboliu):";
                getline(cin, stud_var.name);
                if (valid_alphabet(stud_var.name)) {
                    entered = true;
                }
            }

            entered = false;
            while (!entered) {
                cout << "Pavarde (maks. 20 simboliu):";
                getline(cin, stud_var.last_name);
                if (valid_alphabet(stud_var.last_name)) {
                    entered = true;
                }
            }
        }

        do {
            entered = false;
            cout << "Ivesti namu darbu rezultata? (y - taip, n - ne):";
            cin >> response;
            while (!entered) {
                if (response == "n") {
                    do_continue_inner = false;
                    entered = true;
                } else if (response == "y") {
                    if (gen_marks) {
                        mark = generate_mark();
                        stud_var.hw_res.push_back(mark);
                        cout << "Sugeneruotas pazymys: " << mark << endl;
                        entered = true;
                        do_continue_inner = true;
                    } else {
                        entered = true;
                        do_continue_inner = true;
                        entered_inner = false;
                        while (!entered_inner) {
                            cout << "Namu darbo nr. " << stud_var.hw_res.size() + 1 << " rezultatas:";
                            if (cin >> hw) {
                                if (valid_mark(hw)) {
                                    stud_var.hw_res.push_back(hw);
                                    entered_inner = true;
                                }
                            } else {
                                cout <<  "Bloga ivestis, galima ivesti tik sveikuosius skaicius." << endl << endl;
                            }
                            cin.clear();
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }
                } else {
                    cout << "Bloga ivestis, bandykite dar karta." << endl << endl;
                    entered = true;
                    do_continue_inner = true;
                }
            }
        }while(do_continue_inner);

        entered = false;
        if (gen_marks) {
            mark = generate_mark();
            stud_var.exam_res = mark;
            cout << "Sugeneruotas egzamino rezultatas: " << mark << endl << endl;
        } else {
            while (!entered) {
                cout << "Egzamino rezultatas:";
                if (cin >> stud_var.exam_res) {
                    if (valid_mark(stud_var.exam_res)) {
                        entered = true;
                    }
                } else {
                    cout << "Bloga ivestis, galima ivesti tik sveikuosius skaicius." << endl << endl;
                }
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        stud_var.final_hw_med = median(stud_var.hw_res);
        stud_var.final_hw_avg = average(stud_var.hw_res);

        stud_var.final_res_avg = final(stud_var.final_hw_avg, stud_var.exam_res);
        stud_var.final_res_med = final(stud_var.final_hw_med, stud_var.exam_res);
        stud.push_back(stud_var);
        cout << endl;

        entered = false;
        cout << endl;
        while(!entered) {
            cout << "Ivesti dar vieno studento duomenis? (y - taip, n - ne):";
            cin >> response;
            if (response == "n") {
                do_continue = false;
                entered = true;
            }else if (response == "y") {
                do_continue = true;
                entered = true;
            }else {
                cout << "Bloga ivestis, bandykite dar karta." << endl << endl;
                entered = false;
            }
        }
        cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    }while(do_continue);
}

float average(vector <int> &res) {
    int vec_size = res.size();
    int sum = 0;
    for(int i = 0; i < vec_size; i++) {
        sum += res[i];
    }

    if(vec_size > 0) {
        return (float)sum / (float)vec_size;
    }else {
        return 0.0;
    }
}

float median(vector <int> &res) {
    sort(res.begin(), res.end());
    int vec_size = res.size();
    int size_divided = ceil(vec_size / 2.0);

    // If there are no entries, median = 0.0
    if(vec_size > 0) {
        if (vec_size % 2 == 0) {
            // If number of entries is even, median is calculated by taking the average of the 2 middle numbers.
            return (float)(res[size_divided - 1] + res[size_divided]) / 2.0;
        } else {
            // If number of entries is odd, median is the middle entry.
            return (float)res[size_divided - 1];
        }
    }else {
        return 0.0;
    }
}

float final(float hw, int exam) {
    // Final grade is calculated with the following formula:
    // 0.4 * (average / median homework mark) + 0.6 * (exam mark)
    return 0.4 * hw + 0.6 * exam;
}

void print_data_choice(vector <Student> &stud, int num, bool use_median) {
    // Different output for average and median
    string galutinis = use_median ? "Galutinis (med.)" : "Galutinis (vid.)";
    int width = 20;
    cout << left << setw(width) << "Pavarde";
    cout << left << setw(width) << "Vardas";

    cout << left << setw(width) << galutinis << endl;
    cout << "------------------------------------------------------------" << endl;
    for(int i = 0; i < num; i++) {
        cout << left << setw(width) << stud[i].last_name;
        cout << left << setw(width) << stud[i].name;
        if(use_median) {
            cout << left << setw(width) << fixed << setprecision(2) << stud[i].final_res_med << endl;
        }else {
            cout << left << setw(width) << fixed << setprecision(2) << stud[i].final_res_avg << endl;
        }
    }
}

void print_data_file(vector <Student> &stud, int num, bool use_median) {
    // Different output for average and median
    auto start = std::chrono::high_resolution_clock::now(); auto st = start;
    ofstream file("results.txt");
    string galutinis = use_median ? "Galutinis (med.)" : "Galutinis (vid.)";
    int width = 20;
    file << left << setw(width) << "Pavarde";
    file << left << setw(width) << "Vardas";
    file << left << setw(width) << "Galutinis (med.)";
    file << left << setw(width) << "Galutinis (vid.)" << endl;
    file << "----------------------------------------------------------------------------------" << endl;
    for(int i = 0; i < num; i++) {
        file << left << setw(width) << stud[i].last_name;
        file << left << setw(width) << stud[i].name;
        file << left << setw(width) << fixed << setprecision(2) << stud[i].final_res_med;
        file << left << setw(width) << fixed << setprecision(2) << stud[i].final_res_avg << endl;
    }
    file.close();
    std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
    cout << "Rasymas i faila uztruko " << diff.count() << " s" << endl;
}