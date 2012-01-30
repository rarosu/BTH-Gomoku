#ifndef GLOBALS_H
#define GLOBALS_H

#define SafeRelease(x) { if(x) { (x)->Release(); x = NULL; } }

#endif