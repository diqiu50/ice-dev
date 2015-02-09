#ifndef IceUtil_Type
#define IceUtil_Type

typedef unsigned char U8;
typedef char I8;
typedef unsigned short U16;
typedef short I16;
typedef unsigned int U32;
typedef short I32;
typedef float F32;
typedef double F64;
typedef unsigned long ADR;

#ifndef BIT64
typedef unsigned long long U64;
typedef long long I64;
#else 
typedef unsigned long U64;
typedef long I64;
#endif

#endif
