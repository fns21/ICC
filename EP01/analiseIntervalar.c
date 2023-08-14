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
float somaIntervalo(float x, float y){

    float soma;

    soma = x + y;
    return soma;
}

//Multiplica
Interval_t mulIntervalo(Interval_t mx, Interval_t my){

    Interval_t mul;
    Float_t mulMenor, mulMaior;
    float a, b, c, d;

    a = mx.menor;
    b = mx.maior;
    c = my.menor;
    d = my.maior;
    
    mulMenor.f = fminf(fminf((a * c), (a * d)), fminf((b * c), (b * d)));  
    mulMaior.f = fmaxf(fmaxf((a * c), (a * d)), fmaxf((b * c), (b * d)));

    mul.menor = calculaMenorNumMaquina(mulMenor);
    mul.maior = calculaMaiorNumMaquina(mulMaior);
    return mul;
}

int main (){

    Float_t x, y, soma;
    Interval_t mul, mx, my;
    x.f = 2.347e-40;
    y.f = 0.001;

    mx.menor = calculaMenorNumMaquina(x);
    mx.maior = calculaMaiorNumMaquina(x);

    my.menor = calculaMenorNumMaquina(y);
    my.maior = calculaMaiorNumMaquina(y);

    mul = mulIntervalo(mx, my);

    printf("MenorX: %1.8e, MaiorX: %1.8e, MenorY: %1.8e, MaiorY: %1.8e, MulMin: %1.8e, MulMax: %1.8e\n", mx.menor, mx.maior, my.menor, my.maior, mul.menor, mul.maior);
    return 0;
}