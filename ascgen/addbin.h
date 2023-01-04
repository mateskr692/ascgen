#pragma once
#include "result.h"
#include "strmath.h"
#include "shared.h"

struct result addbin_mod(FILE* out_stream, struct mathdata data);
struct result addbin_odw(FILE* out_stream, struct mathdata data);
struct result addbin_dop(FILE* out_stream, struct mathdata data);