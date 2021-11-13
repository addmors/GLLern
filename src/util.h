#pragma once
#include <string>


#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define ZERO_MEM(a) memset(a, 0, sizeof(a))