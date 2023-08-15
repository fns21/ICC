#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

typedef union {
    int32_t i;
    float f;    
    struct {   // Bitfields for exploration (32 bits = sign|exponent|mantissa)
        uint32_t mantissa : 23; // primeiros 23 bits (da direita para a esquerda)
        uint32_t exponent : 8;  // próximos 8 bits
        uint32_t sign : 1;      // proximo bit (ou seja, o bit mais a esquerda)
    } parts;
} Float_t;

typedef struct {
    float maior;
    float menor;
} Interval_t;

typedef struct {
    float erroAbs;
    float erroRel;
    float ULPs;
} Precisao_t;

/* imprime a union Float_t como ponto flutuante, hexadecimal e 
 * suas partes na forma de inteiros */
void printFloat_t (Float_t num){

    printf("f:%1.9e, ix:0x%08X, s:%d, e:%d, mx:0x%06X\n",
          num.f, num.i, num.parts.sign, num.parts.exponent, num.parts.mantissa); 
}

/* calcula o epsilon relativo a um numero NUM. Ou seja, o valor tal
 * que NUM + epsilon seja > NUM */
Float_t calculaEpsilonRelativo(Float_t num){

    Float_t epsilon;
    epsilon.f = num.f / 2.0f;
    while (num.f + epsilon.f / 2.0f > num.f)
        epsilon.f /= 2.0f;
    
    return epsilon;
}

//Calcula o maior numero de maquina menor ou igual a num (subtrai de num, o epsilon relativo)
float calculaMenorNumMaquina(Float_t num){

    float menorNum;

    menorNum = num.f - calculaEpsilonRelativo(num).f;
    return menorNum;
}

//Calcula o menor numero de maquina maior ou igual a num (soma o epsilon relativo ao num)
float calculaMaiorNumMaquina(Float_t num){

    float maiorNum;

    maiorNum = calculaEpsilonRelativo(num).f + num.f;
    return maiorNum;
}

//Operações Intervalares
//Soma
Interval_t somaIntervalo(Interval_t x, Interval_t y){

    Interval_t soma;
    Float_t somaMenor, somaMaior;
    
    somaMenor.f = x.menor + y.menor;
    somaMaior.f = x.maior + y.maior;

    soma.menor = calculaMenorNumMaquina(somaMenor);
    soma.maior = calculaMaiorNumMaquina(somaMaior);
    return soma;
}

//Multiplica
Interval_t mulIntervalo(Interval_t x, Interval_t y){

    Interval_t mul;
    Float_t mulMenor, mulMaior;
    float a, b, c, d;

    a = x.menor;
    b = x.maior;
    c = y.menor;
    d = y.maior;
    
    mulMenor.f = fminf(fminf((a * c), (a * d)), fminf((b * c), (b * d)));  
    mulMaior.f = fmaxf(fmaxf((a * c), (a * d)), fmaxf((b * c), (b * d)));

    mul.menor = calculaMenorNumMaquina(mulMenor);
    mul.maior = calculaMaiorNumMaquina(mulMaior);
    return mul;
}

//Subtrai
Interval_t subIntervalo(Interval_t x, Interval_t y){
    
    Interval_t sub;
    Float_t subMenor, subMaior;

    subMenor.f = x.menor - y.maior;
    subMaior.f = x.maior - y.menor;

    sub.menor = calculaMenorNumMaquina(subMenor);
    sub.maior = calculaMaiorNumMaquina(subMaior);
    return sub;
}

//Divide
Interval_t divIntervalo(Interval_t x, Interval_t y){
    
    Interval_t div;
    Float_t divMenor, divMaior;
    float erro = 1e-10;
    float a, b, c, d;

    a = x.menor;
    b = x.maior;
    c = y.menor;
    d = y.maior;

    if(abs(c) >= erro && abs(d) >= erro){
        divMenor.f = fminf(fminf((a * (1 / d)), (a * (1 / c))), fminf((b * (1 / d)), (b * (1 / c))));
        divMaior.f = fmaxf(fmaxf((a * (1 / d)), (a * (1 / c))), fmaxf((b * (1 / d)), (b * (1 / c))));
    }
    else{
        divMenor.f = -INFINITY;
        divMaior.f = INFINITY;
    }

    div.menor = divMenor.f;
    div.maior = divMaior.f;
    return div;
}


float erroAbsoluto(Interval_t interv){

    return interv.maior - interv.menor;
}

float erroRelativo(Interval_t interv){

    return (interv.maior - interv.menor) / interv.menor;
}

Interval_t opcoesOperacoes(Interval_t x, Interval_t y, char op, Precisao_t pres){

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

    pres.erroAbs = erroAbsoluto(result);
    pres.erroRel = erroRelativo(result);
    return result;
}

void atribuiIntervNumMaq(Interval_t nx[], Float_t x[]){
    for (int i = 0; i < 5; i++) {
        nx[i].menor = calculaMenorNumMaquina(x[i]);
        nx[i].maior = calculaMaiorNumMaquina(x[i]);
    }
}

int main (){

    Float_t x[5], erroAbs;
    char o[4];
    Interval_t result[4], nx[5];
    Precisao_t pres[4];
        
    fscanf(stdin, "%f %c %f %c %f %c %f %c %f", &(x[0].f), &o[0], &(x[1].f), &o[1], &(x[2].f), &o[2], &(x[3].f), &o[3], &(x[4].f));
    getc(stdin);
    while(!feof(stdin)){
        atribuiIntervNumMaq(nx, x);
        result[0] = opcoesOperacoes(nx[0], nx[1], o[0], pres[0]);
        result[1] = opcoesOperacoes(result[0], nx[2], o[1], pres[1]);
        result[2] = opcoesOperacoes(result[1], nx[3], o[2], pres[2]);
        result[3] = opcoesOperacoes(result[2], nx[4], o[3], pres[3]);
        fscanf(stdin, "%f %c %f %c %f %c %f %c %f", &x[0].f, &o[0], &x[1].f, &o[1], &x[2].f, &o[2], &x[3].f, &o[3], &x[4].f);
        getc(stdin);
    }
    
   // printf("Erro Absoluto1: %1.8e, Erro Relativo1: %1.8e\n Erro Absoluto2: %1.8e, Erro Relativo2: %1.8e\n Erro Absoluto3: %1.8e, Erro Relativo3: %1.8e\n Erro Absoluto4: %1.8e, Erro Relativo4: %1.8e\n", pres[0].erroAbs, pres[0].erroRel, pres[1].erroAbs, pres[1].erroRel, pres[2].erroAbs, pres[2].erroRel, pres[3].erroAbs, pres[3].erroRel);
    printf("result1min: %1.8e result1max: %1.8e result2min: %1.8e result2max: %1.8e result3min: %1.8e result3max: %1.8e result4min: %1.8e result4max: %1.8e\n", result[0].menor, result[0].maior, result[1].menor, result[1].maior, result[2].menor, result[2].maior, result[3].menor, result[3].maior); 
    return 0;
}
