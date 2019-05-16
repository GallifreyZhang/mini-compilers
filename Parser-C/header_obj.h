#ifndef _OBJECT_HEADER_H
#define _OBJECT_HEADER_H
#include "utils.h"

typedef enum {
    OT_CLASS,
    OT_STRING,
    OT_FUNCTION,
    OT_INSTANCE,
    OT_THREAD
} ObjType;

typedef struct _ObjHeader ObjHeader;

struct _ObjHeader {
    ObjType type;
    bool is_dark;

    ObjHeader* next;
};

typedef enum {
    VT_UNDEFINED,
    VT_NULL,
    VT_FALSE,
    VT_TRUE,
    VT_NUM,
    VT_OBJ
} ValueType;

typedef struct {
    ValueType type;
    union {
        double num;
        ObjHeader* obj_header;
    };
} Value;

//DECLARE_BUFFER_TYPE(Value);

//void init_obj_header(ObjHeader* obj_header, ObjType type, Class* class);

#endif