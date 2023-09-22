#include <vector>

#ifndef TUI_HPP_
#include "tui.hpp"
#endif

using namespace std;

int main(const int argc, const char* argv[]) {
    if (argc == 1) {
        main_title();
    }

    return quick_operate(argc, argv);
}