#include <vector>
#include "functions.cpp"

using namespace std;

int main() {
    while(true) {
        vector<Student> students;
        bool output_console = false;
        bool generate_marks = false;
        bool generate_names = false;
        bool read_from_file = false;
        bool use_median = false;
        int menu_choice = 5;

        main_menu(menu_choice, read_from_file, generate_marks, generate_names);
        output_menu(output_console);
        clear_stream();

        if (read_from_file) {
            read_data_from_file("studentai100000.txt", students);
        }else {
            read_data_from_console(students, use_median, generate_marks, generate_names);
        }

        sort_menu(students);

        if (output_console) {
            avg_median_menu(use_median);
            print_data_choice(students, students.size(), use_median);
        }else {
            print_data_file(students, students.size(), use_median);
        }
    }
}

