#include <iostream>

using namespace std;

void plotFunc1();

void plotFunc2();

void plotFunc3();

void plotFunc4();

void plotFunc5();

void plotFunc6();

void demo();

int main(int argnum, char **arg) {
    demo();
    if (argnum > 1) {
        switch (int(*arg[1] - '0')) {
            case 1:
                plotFunc1();
                break;
            case 2:
                plotFunc2();
                break;
            case 3:
                plotFunc3();
                break;
            case 4:
                plotFunc4();
                break;
            case 5:
                plotFunc5();
                break;
            case 6:
                plotFunc6();
                break;

            default:
                cout << "Please choose a number from 1 to 6." << endl;
                demo();
                break;
        }
    } else {
        cout << "Please choose the function you want to plot." << endl;
        plotFunc1();
        plotFunc2();
        plotFunc3();
        plotFunc4();
        plotFunc5();
        plotFunc6();
    }
    return 0;
}
