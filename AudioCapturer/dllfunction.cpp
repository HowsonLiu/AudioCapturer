#include "dllfunction.h"

QString ChangeCharpToQString(const char* pchar){
    return  QString::fromStdString(std::string(pchar));
}

std::vector<std::string> ChangeCharppToVector(char** cpp){
    using namespace std;
    vector<string> vs;
    if(cpp != nullptr){
        auto start = cpp;
        auto end = start;
        while (*end) {
            auto sstart = *end;
            auto send = *end;
            while (*send) {
                send++;
            }
            vs.push_back(string(sstart, send));
            end++;
        }
    }
    return vs;
}
