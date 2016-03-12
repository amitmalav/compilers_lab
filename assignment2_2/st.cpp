#include "st.h"
#include <iostream>
using namespace std;









map<string, SymbolTableEntry*> paraMap;
int offset = 0;
GlobalTable *gtable = new GlobalTable()