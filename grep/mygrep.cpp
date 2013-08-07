#include "mygrep.h"

int find(string str, string key) {
    for(int i = 0; i < (int)str.size(); ++i) if(str.substr(i, (int)key.size()) == key) return i;
    return -1;
}
