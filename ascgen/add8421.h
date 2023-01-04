#pragma once
#include "result.h"
#include "strmath.h"
#include "shared.h"

struct result add8421_mod(FILE* out_stream, struct mathdata data);
struct result add8421_odw(FILE* out_stream, struct mathdata data);
struct result add8421_dop(FILE* out_stream, struct mathdata data);