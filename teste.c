#include <stdio.h>
#include<stdbool.h>
#include<string.h>

#define tamMax 200

enum {FIM = -1, ID = 0, NUM = 1, OP, COMP, ERRO, DELIMITADOR};

int linhas;
int indexBufferEntrada = 0;
int indexBufferSaida = 0;
int fimBufferEntrada = 0;
char bufferEntrada[tamMax];
char bufferSaida[tamMax];
FILE * arquivo;
FILE * escrita;
int tabelaTransicao[9][12];


int getInput(){
    char temp;
    fimBufferEntrada = 0;
    while (fimBufferEntrada < tamMax && temp != '\n')
    {    
        temp = fgetc(arquivo);
        fimBufferEntrada++;
        if (temp != EOF)
        {
            bufferEntrada[fimBufferEntrada] = temp;
        }
        else{
            bufferEntrada[fimBufferEntrada] = temp;
            return -1;
        }
    }
    if (temp == EOF)  
        bufferEntrada[fimBufferEntrada + 1] = temp;
    return 0;
}

void main(void) { 

    arquivo = fopen("programa.c", "r");
    int temp = 0;
    while (temp != -1)
    {
        temp = getInput();
        for (int i = 0; i <= fimBufferEntrada; i++)
        {
            if (bufferEntrada[i] == EOF)
            {
                printf("FIM\n");
            }
            else{
                printf("%c", bufferEntrada[i]);
            }
            
        }
        
    }
}