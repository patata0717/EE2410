#include <iostream>
using namespace std;

int main() {
#ifdef USE_DEFAULT
    cout << "USE_DEFAULT is defined. Default settings are used." << endl;
#else
    cout << "USE_DEFAULT is not defined. Custom settings are used." << endl;
#endif

    return 0;
}
