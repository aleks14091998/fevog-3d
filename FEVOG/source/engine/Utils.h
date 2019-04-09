#ifndef _UTILS_H
#define _UTILS_H

#include"FEV_Eng.h"
#include<string>
using namespace std;
using namespace valk;

//-----------------------------------------------------------------------
uchar EngKeyToASCIIKey(const uint8 key);
uint8 ASCIIKeyToEngKey(const uchar key);
//-----------------------------------------------------------------------

#endif // !_UTILS_H
