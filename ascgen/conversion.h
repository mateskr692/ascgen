#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "result.h"
#include "shared.h"

#define CONV_PREC_DEF 5

struct result convert_il(FILE* out_stream, struct convdata data);
struct result convert_roz(FILE* out_stream, struct convdata data);