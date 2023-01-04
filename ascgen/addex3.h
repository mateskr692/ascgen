#pragma once
#include "result.h"
#include "strmath.h"
#include "shared.h"

struct result addex3_mod(FILE* out_stream, struct mathdata data);
struct result addex3_odw(FILE* out_stream, struct mathdata data);
struct result addex3_dop(FILE* out_stream, struct mathdata data);