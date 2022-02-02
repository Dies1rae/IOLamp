#include <QCoreApplication>
#include <string>
#include <iostream>
#include <string>

#include "devicebcklight.h"

using namespace std;
using namespace IODEV;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DeviceBckLight test(argv[1]);

    while(1) {
        std::string R;
        std::string G;
        std::string B;
        std::string lamp;
        cout << "Set R: ";
        cin >> R;
        cout << "Set G: ";
        cin >> G;
        cout << "Set B: ";
        cin >> B;
        cout << "Set lamp NO: ";
        cin >> lamp;
        cout << "setting your color\n";
        test.setColor(std::stoul(R),std::stoul(G), std::stoul(B), std::stoul(lamp));
    }
    return a.exec();
}
