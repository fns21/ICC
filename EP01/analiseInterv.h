#ifndef ANALISEINTERV_H
#define ANALISEINTERV_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <fenv.h>

#define QUASEZERO 1e-45

typedef union {
    float f;
    int32_t i;
} Float_t;

typedef struct {
    Float_t maior;
    Float_t menor;
} Interval_t;

typedef struct {
    float erroAbs;
    float erroRel;
    int ULPs;
} Precisao_t;

float calculaMenorNumMaquina(float num);

float calculaMaiorNumMaquina(float num);

float erroAbsoluto(Interval_t interv);

float erroRelativo(Interval_t interv);

int ULP(Interval_t interv);

void atribuiIntervNumMaq(Interval_t nx[], float x[]);

void imprime(Interval_t result[], Interval_t nx[], Precisao_t pres[], char op[]);

#endif