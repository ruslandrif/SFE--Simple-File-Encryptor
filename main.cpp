
#include "Main_interface.h"
#include "encryptor.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Main_interface program;
    program.show();
   

    return a.exec();
}
