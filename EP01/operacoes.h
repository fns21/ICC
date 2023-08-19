#ifndef OPERACOES_H
#define OPERACOES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <fenv.h>
#include "analiseInterv.h"

Interval_t somaIntervalo(Interval_t x, Interval_t y);

Interval_t mulIntervalo(Interval_t x, Interval_t y);

Interval_t subIntervalo(Interval_t x, Interval_t y);

Interval_t divIntervalo(Interval_t x, Interval_t y);

Interval_t opcoesOperacoes(Interval_t x, Interval_t y, char op, Precisao_t* pres);

#endif