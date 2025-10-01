#ifndef SPEAR_CORELIBS_PARSE_PRIMITIVES_H_
#define SPEAR_CORELIBS_PARSE_PRIMITIVES_H_

#include "base.h"


int spear_parse_while(char const *data, uint32 size, int (*predicate)(char));
int spear_parse_until(char const *data, uint32 size, int (*predicate)(char));

int spear_parse_cstring(char const *data, uint32 size, char const *cstr);

int spear_parse_int32(char const *data, uint32 size, int32 *out);
int spear_parse_float32(char const *data, uint32 size, float32 *out);


#endif // SPEAR_CORELIBS_PARSE_PRIMITIVES_H_
