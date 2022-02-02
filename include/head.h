#ifndef __HEAD_H__
#define __HEAD_H__



#include "color.h"
#include <stdio.h>

#ifdef DEBUG   
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#endif // !__HEAD_H__