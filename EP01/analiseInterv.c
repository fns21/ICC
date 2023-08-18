#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <fenv.h>

#define QUASEZERO 1e-45

typedef struct {
    double maior;
    double menor;
} Interval_t;

typedef struct {
    double erroAbs;
    double erroRel;
    int ULPs;
} Precisao_t;

//Calcula o maior numero de maquina menor ou igual a num (subtrai de num, o epsilon relativo)
double calculaMenorNumMaquina(double num){

    double menorNum;
    
    if(fabs(num) <= QUASEZERO)
        menorNum = 0;
    else 
        menorNum = nextafterf(num, -INFINITY);

    return menorNum;
}

//Calcula o menor numero de maquina maior ou igual a num (soma o epsilon relativo ao num)
double calculaMaiorNumMaquina(double num){

    double maiorNum;

    if(fabs(num) <= QUASEZERO)
        maiorNum = 0;
    else
        maiorNum = nextafterf(num, INFINITY);

    return maiorNum;
}

//Operações Intervalares
//Soma
Interval_t somaIntervalo(Interval_t x, Interval_t y){

    Interval_t soma;
    double somaMenor, somaMaior;
    
    somaMenor = x.menor + y.menor;
    somaMaior = x.maior + y.maior;

    soma.menor = calculaMenorNumMaquina(somaMenor);
    soma.maior = calculaMaiorNumMaquina(somaMaior);
    return soma;
}

//Multiplica
Interval_t mulIntervalo(Interval_t x, Interval_t y){

    Interval_t mul;
    double mulMenor, mulMaior;
    double a, b, c, d;

    a = x.menor;
    b = x.maior;
    c = y.menor;
    d = y.maior;

    mulMenor = fmin(fmin((a * c), (a * d)), fmin((b * c), (b * d)));
    mulMaior = fmax(fmax((a * c), (a * d)), fmax((b * c), (b * d)));
 
    mul.menor = calculaMenorNumMaquina(mulMenor);
    mul.maior = calculaMaiorNumMaquina(mulMaior);

    return mul;
}

//Subtrai
Interval_t subIntervalo(Interval_t x, Interval_t y){
    
    Interval_t sub;
    double subMenor, subMaior;

    subMenor = x.menor - y.maior;
    subMaior = x.maior - y.menor;

    sub.menor = calculaMenorNumMaquina(subMenor);
    sub.maior = calculaMaiorNumMaquina(subMaior);
    return sub;
}

//Divide
Interval_t divIntervalo(Interval_t x, Interval_t y){
    
    Interval_t div;
    double divMenor, divMaior;
    double a, b, c, d;

    a = x.menor;
    b = x.maior;
    c = y.menor;
    d = y.maior;

    if(fabs(c) >= QUASEZERO && fabs(d) >= QUASEZERO){
        divMenor = fminf(fminf((a * (1 / d)), (a * (1 / c))), fminf((b * (1 / d)), (b * (1 / c))));
        divMaior = fmaxf(fmaxf((a * (1 / d)), (a * (1 / c))), fmaxf((b * (1 / d)), (b * (1 / c))));
    }
    else{
        divMenor = -INFINITY;
        divMaior = INFINITY;
    }

    div.menor = calculaMenorNumMaquina(divMenor);
    div.maior = calculaMaiorNumMaquina(divMaior);
    return div;
}


double erroAbsoluto(Interval_t interv){

    return fabs(interv.maior - interv.menor);
}

double erroRelativo(Interval_t interv){
    
    if(fabs(interv.menor) >= QUASEZERO)
        return fabs(interv.maior - interv.menor) / interv.menor;
    
    return INFINITY; 
}

int ULP(Interval_t interv){
    
    double diff = fabs(interv.maior - interv.menor);
    int ulp = diff / fminf(fabs(interv.maior), fabs(interv.menor));  
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
    printf("%1.8e\n", pres.erroAbs);
    pres.erroRel = erroRelativo(result);
    printf("%1.8e\n", pres.erroRel);
    pres.ULPs = ULP(result);
    return result;
}

void atribuiIntervNumMaq(Interval_t nx[], double x[]){
    for (int i = 0; i < 5; i++) {
        nx[i].menor = calculaMenorNumMaquina(x[i]);
        nx[i].maior = calculaMaiorNumMaquina(x[i]);
    }
}

int main (){

    double x[5], erroAbs;
    char o[4];
    Interval_t result[4], nx[5];
    Precisao_t pres[4];
    
    fscanf(stdin, "%lf %c %lf %c %lf %c %lf %c %lf", &x[0], &o[0], &x[1], &o[1], &x[2], &o[2], &x[3], &o[3], &x[4]);
    getc(stdin);
    while(!feof(stdin)){
        atribuiIntervNumMaq(nx, x);
        result[0] = opcoesOperacoes(nx[0], nx[1], o[0], pres[0]);
        result[1] = opcoesOperacoes(result[0], nx[2], o[1], pres[1]);
        result[2] = opcoesOperacoes(result[1], nx[3], o[2], pres[2]);
        result[3] = opcoesOperacoes(result[2], nx[4], o[3], pres[3]);
        fscanf(stdin, "%lf %c %lf %c %lf %c %lf %c %lf", &x[0], &o[0], &x[1], &o[1], &x[2], &o[2], &x[3], &o[3], &x[4]);
        getc(stdin);
    }
    
    //printf("Erro Absoluto1: %1.8e, Erro Relativo1: %1.8e\n Erro Absoluto2: %1.8e, Erro Relativo2: %1.8e\n Erro Absoluto3: %1.8e, Erro Relativo3: %1.8e\n Erro Absoluto4: %1.8e, Erro Relativo4: %1.8e\n", pres[0].erroAbs, pres[0].erroRel, pres[1].erroAbs, pres[1].erroRel, pres[2].erroAbs, pres[2].erroRel, pres[3].erroAbs, pres[3].erroRel);
    printf("%1.8e, %1.8e, %1.8e, %1.8e, Menor: %1.8e, Maior: %1.8e\n", nx[0].menor, nx[0].maior, nx[1].menor, nx[1].maior, result[0].menor, result[0].maior); 
    return 0;
}
