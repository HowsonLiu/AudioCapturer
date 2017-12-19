#include "maindialog.h"
#include "singleapplication.h"

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv,"Single");
    if(app.get_isrunning()){
        return 0;
    }
    MainDialog d;
    d.show();
    return app.exec();
}
