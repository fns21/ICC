#include "operacoes.h"

//Operações Intervalares
//Soma
Interval_t somaIntervalo(Interval_t x, Interval_t y){

    Interval_t soma;
    float somaMenor, somaMaior;
    
    somaMenor = x.menor.f + y.menor.f;
    somaMaior = x.maior.f + y.maior.f;

    soma.menor.f = calculaMenorNumMaquina(somaMenor);
    soma.maior.f = calculaMaiorNumMaquina(somaMaior);
    return soma;
}

//Multiplica
Interval_t mulIntervalo(Interval_t x, Interval_t y){

    Interval_t mul;
    float mulMenor, mulMaior;
    float a, b, c, d;

    a = x.menor.f;
    b = x.maior.f;
    c = y.menor.f;
    d = y.maior.f;

    mulMenor = fmin(fmin((a * c), (a * d)), fmin((b * c), (b * d)));
    mulMaior = fmax(fmax((a * c), (a * d)), fmax((b * c), (b * d)));
 
    mul.menor.f = calculaMenorNumMaquina(mulMenor);
    mul.maior.f = calculaMaiorNumMaquina(mulMaior);

    return mul;
}

//Subtrai
Interval_t subIntervalo(Interval_t x, Interval_t y){
    
    Interval_t sub;
    float subMenor, subMaior;

    subMenor = x.menor.f - y.maior.f;
    subMaior = x.maior.f - y.menor.f;

    sub.menor.f = calculaMenorNumMaquina(subMenor);
    sub.maior.f = calculaMaiorNumMaquina(subMaior);
    return sub;
}

//Divide
Interval_t divIntervalo(Interval_t x, Interval_t y){
    
    Interval_t div;
    float divMenor, divMaior;
    float a, b, c, d;

    a = x.menor.f;
    b = x.maior.f;
    c = y.menor.f;
    d = y.maior.f;

    if(fabs(c) >= QUASEZERO && fabs(d) >= QUASEZERO){
        divMenor = fminf(fminf((a * (1 / d)), (a * (1 / c))), fminf((b * (1 / d)), (b * (1 / c))));
        divMaior = fmaxf(fmaxf((a * (1 / d)), (a * (1 / c))), fmaxf((b * (1 / d)), (b * (1 / c))));
    }
    else{
        divMenor = -INFINITY;
        divMaior = INFINITY;
    }

    div.menor.f = calculaMenorNumMaquina(divMenor);
    div.maior.f = calculaMaiorNumMaquina(divMaior);
    return div;
}

Interval_t opcoesOperacoes(Interval_t x, Interval_t y, char op, Precisao_t* pres){

    Interval_t result;    
    
    switch(op){
        case '+':
            result = somaIntervalo(x, y);
            break;
        case '-':
            result = subIntervalo(x, y);
            break;
        case '*':
            result = mulIntervalo(x, y);
            break;
        case '/':
            result = divIntervalo(x, y);
            break;
    }

    pres->erroAbs = erroAbsoluto(result);
    pres->erroRel = erroRelativo(result);
    pres->ULPs = ULP(result);
    return result;
}