#include <iostream>
#include <cstring>
#include <string>
#include <windows.h>
#include "huffmanHelper.h"

using namespace std;

int main (int argc, char *argv[]) {
    huffmanHelper* helper = huffmanHelper::getInstance();
    if (argc != 4) {
        int i;
        string output, input;
        cin>>i>>input>>output;
        if (i == 1) helper->encode(input, output);
        system("pause");
        return 0;
    }
#ifdef COUT_IN_CONSOLE
    DWORD t1 = GetTickCount();
#endif
    
    if (strcmp(argv[1], "-encode") == 0) helper->encode(argv[2], argv[3]);
    else if (strcmp(argv[1], "-decode") == 0) helper->decode(argv[2], argv[3]);

#ifdef COUT_IN_CONSOLE
    DWORD t2 = GetTickCount();
    cout<<(t2 - t1)<<"ms"<<endl;
#endif
    return 0;
}