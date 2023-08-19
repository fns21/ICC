#include "analiseInterv.h"

float calculaMenorNumMaquina(float num){

    float menorNum;
    
    if(fabs(num) <= QUASEZERO)
        menorNum = 0;
    else 
        menorNum = nextafterf(num, -INFINITY);

    return menorNum;
}

float calculaMaiorNumMaquina(float num){

    float maiorNum;

    if(fabs(num) <= QUASEZERO)
        maiorNum = 0;
    else
        maiorNum = nextafterf(num, INFINITY);

    return maiorNum;
}

float erroAbsoluto(Interval_t interv){

    return fabs(interv.maior.f - interv.menor.f);
}

float erroRelativo(Interval_t interv){
    
    if(fabs(interv.menor.f) >= QUASEZERO)
        return erroAbsoluto(interv) / fabs(interv.menor.f);
    
    return INFINITY; 
}

int ULP(Interval_t interv){
    
    int ulp;

    ulp = abs(interv.menor.i - interv.maior.i) - 1;

    if(ulp < 0)
        return 0;

    return ulp;
}

void atribuiIntervNumMaq(Interval_t nx[], float x[]){

    for (int i = 0; i < 5; i++) {
        nx[i].menor.f = calculaMenorNumMaquina(x[i]);
        nx[i].maior.f = calculaMaiorNumMaquina(x[i]);
    }
}

void imprime(Interval_t result[], Interval_t nx[], Precisao_t pres[], char op[]){

    printf("%d:\n", 1);
    printf("[%1.8e, %1.8e] %c [%1.8e, %1.8e] = \n", nx[0].menor.f, nx[0].maior.f, op[0], nx[1].menor.f, nx[1].maior.f);
    printf("[%1.8e, %1.8e]\n", result[0].menor.f, result[0].maior.f);
    printf("EA: %1.8e, ER: %1.8e, ULPs: %d\n\n", pres[0].erroAbs, pres[0].erroRel, pres[0].ULPs);
    for(int i = 1; i < 4; i++){
        printf("%d:\n", i+1);
        printf("[%1.8e, %1.8e] %c [%1.8e, %1.8e] = \n", result[i-1].menor.f, result[i-1].maior.f, op[i], nx[i+1].menor.f, nx[i+1].maior.f);
        printf("[%1.8e, %1.8e]\n", result[i].menor.f, result[i].maior.f);
        printf("EA: %1.8e, ER: %1.8e, ULPs: %d\n\n", pres[i].erroAbs, pres[i].erroRel, pres[i].ULPs);
    }
    printf("========================================================\n");
}