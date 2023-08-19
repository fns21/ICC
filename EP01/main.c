#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <fenv.h>
#include "analiseInterv.h"
#include "operacoes.h"

int main (){

    float x[5];
    char o[4];
    Interval_t result[4], nx[5];
    Precisao_t pres[4];
    
    fscanf(stdin, "%f %c %f %c %f %c %f %c %f", &x[0], &o[0], &x[1], &o[1], &x[2], &o[2], &x[3], &o[3], &x[4]);
    getc(stdin);
    while(!feof(stdin)){
        atribuiIntervNumMaq(nx, x);
        result[0] = opcoesOperacoes(nx[0], nx[1], o[0], &pres[0]);
        result[1] = opcoesOperacoes(result[0], nx[2], o[1], &pres[1]);
        result[2] = opcoesOperacoes(result[1], nx[3], o[2], &pres[2]);
        result[3] = opcoesOperacoes(result[2], nx[4], o[3], &pres[3]);
        imprime(result, nx, pres, o);
        fscanf(stdin, "%f %c %f %c %f %c %f %c %f", &x[0], &o[0], &x[1], &o[1], &x[2], &o[2], &x[3], &o[3], &x[4]);
        getc(stdin);
    }
    
    return 0;
}