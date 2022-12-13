#include <stdio.h>
#include<stdbool.h>
#include<string.h>

#define tamMax 200

enum {FIM = -1, ID = 0, INT = 1, VOID, IF, ELSE, WHILE, RETURN, NUM, SOMA, MULT, IGUAL, RELACIONAL, LBRACKET, RBRACKET, LPAREN, RPAREN, LBRACE, RBRACE, COMMA, SEMICOLON, ERRO};

int linhas = 1;
int indexBufferEntrada = 0;
int indexBufferSaida = 0;
int fimBufferEntrada = 0;
char bufferEntrada[tamMax];
char bufferSaida[tamMax];
FILE * arquivo;
FILE * escrita;
int tabelaTransicao[9][12];

typedef struct 
{
    int type;
    char * lexem;
} token;

int mapeamento(int caracter){
    if (((caracter >= 65) && (caracter <= 90)) || ((caracter >= 97) && (caracter <= 122)) )
    {
        return 0;
    } 
    else if ((caracter >= 48) && (caracter <= 57))
    {
        return 1;
    }
    else if (caracter == 33) // !
    {
        return 2;
    }
    else if (caracter == 42) // *
    {
        return 3;
    }
    else if ((caracter == 60) || (caracter == 62)) // <, >
    {
        return 4;
    }
    else if ((caracter == 43) || (caracter == 45)) // +, -
    {
        return 5;
    }
    else if (caracter == 47) // /
    {
        return 6;
    }
    else if ((caracter == 40) || (caracter == 41) || (caracter == 91) || (caracter == 93) || (caracter == 123) || (caracter == 125) || (caracter == 59) || (caracter == 44)) // () [] {}
    {
        return 7;
    }
    else if (caracter == 61) // =
    {
        return 8;
    }
    else if ((caracter == 9) ||(caracter == 10) || (caracter == 0) || (caracter == 32)) // espaco
    {
        return 9;
    }    
    else if(caracter == -1){

        return 11;

    }
    else{      
        return 10; // erros
    }
}


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
    if (fimBufferEntrada == tamMax - 1)
    {
        if (bufferEntrada[fimBufferEntrada] == '\n')
        {
            linhas++;
        }
    }
    else{
        linhas++;
    }
    return 0;
}

token myGetToken(){
    int estadoAtual = 0;
    int temp;
    bufferSaida[0] = '\0';
    token aux;
    while ((estadoAtual < 9))
    {
        if (indexBufferEntrada <= fimBufferEntrada)
        {
            temp = mapeamento(bufferEntrada[indexBufferEntrada]);
            estadoAtual = tabelaTransicao[estadoAtual][temp];
            if ((estadoAtual!=5) && (estadoAtual != 4) && (estadoAtual != 0))
            {
                bufferSaida[indexBufferSaida] = bufferEntrada[indexBufferEntrada];
                indexBufferSaida++;
            }
            else{
                bufferSaida[0] = '\0';
                indexBufferSaida = 0;
            }
            indexBufferEntrada++;
        }
        else{
            getInput();
            indexBufferEntrada = 0;
        }
        
    }
    if (estadoAtual == 9)
    {
        if(mapeamento(bufferSaida[indexBufferSaida-1]) == 5){
            bufferSaida[indexBufferSaida] = '\0';
            fprintf(escrita, "<SOMA, %s>", bufferSaida);
            aux.type = SOMA;
            indexBufferSaida = 0;
        }
        else if(mapeamento(bufferSaida[indexBufferSaida-2]) == 8){
            bufferSaida[indexBufferSaida - 1] = '\0';
            fprintf(escrita, "<IGUAL, %s>", bufferSaida);
            aux.type = IGUAL;
            indexBufferSaida = 0;
            indexBufferEntrada--;
        }
        else if(mapeamento(bufferSaida[indexBufferSaida-2]) == 6){
            bufferSaida[indexBufferSaida - 1] = '\0';
            fprintf(escrita, "<MULT, %s>", bufferSaida);
            aux.type = MULT;
            indexBufferSaida = 0;
            indexBufferEntrada--;
        }
        else{
            bufferSaida[indexBufferSaida] = '\0';
            fprintf(escrita, "<MULT, %s>", bufferSaida);
            aux.type = MULT;
            indexBufferSaida = 0;
        }
        return aux;
    }
    else if (estadoAtual == 10)
    {
        bufferSaida[indexBufferSaida - 1] = '\0';
        indexBufferSaida = 0;
        indexBufferEntrada--;
        if (strcmp(bufferSaida, "int") == 0)
        {
            fprintf(escrita, "<int, %s>", bufferSaida);
            aux.type = INT;
        }
        else if (strcmp(bufferSaida, "void") == 0)
        {
            fprintf(escrita, "<void, %s>", bufferSaida);
            aux.type = VOID;
        }
        else if (strcmp(bufferSaida, "if") == 0)
        {
            fprintf(escrita, "<if, %s>", bufferSaida);
            aux.type = IF;
        }
        else if (strcmp(bufferSaida, "else") == 0)
        {
            fprintf(escrita, "<else, %s>", bufferSaida);
            aux.type = ELSE;
        }
        else if (strcmp(bufferSaida, "return") == 0)
        {
            fprintf(escrita, "<return, %s>", bufferSaida);
            aux.type = RETURN;
        }
        else if (strcmp(bufferSaida, "while") == 0)
        {
            fprintf(escrita, "<while, %s>", bufferSaida);
            aux.type = WHILE;
        }
        else 
        {
            fprintf(escrita, "<ID, %s>", bufferSaida);
            aux.type = ID;
        }
        return aux;
    }
    else if (estadoAtual == 11)
    {
        bufferSaida[indexBufferSaida - 1] = '\0';
        fprintf(escrita, "<Num, %s>", bufferSaida);
        aux.type = NUM;
        indexBufferSaida = 0;
        indexBufferEntrada--;
        return aux;
    }
    else if (estadoAtual == 12)
    {
        if(bufferSaida[indexBufferSaida-1] == 33){
            bufferSaida[indexBufferSaida - 1] = '\0';
            fprintf(escrita, "<Erro, %s>", bufferSaida);
            aux.type = ERRO;
            indexBufferSaida = 0;
            indexBufferEntrada--;
        }
        else{
            bufferSaida[indexBufferSaida] = '\0';
            fprintf(escrita, "<Erro, %s>", bufferSaida);
            aux.type = ERRO;
            indexBufferSaida = 0;
        }
        return aux;
    }
    else if (estadoAtual == 13)
    {
        bufferSaida[indexBufferSaida] = '\0';
        if (bufferSaida[indexBufferSaida -1] == 40)
        {
            fprintf(escrita, "<LPAREN, %s>", bufferSaida);
            aux.type = LPAREN;
        }
        else if (bufferSaida[indexBufferSaida -1] == 41)
        {
            fprintf(escrita, "<RPAREN, %s>", bufferSaida);
            aux.type = RPAREN;
        }
        else if (bufferSaida[indexBufferSaida -1] == 91)
        {
            fprintf(escrita, "<LBRACKET, %s>", bufferSaida);
            aux.type = LBRACKET;
        }
        else if (bufferSaida[indexBufferSaida -1] == 93)
        {
            fprintf(escrita, "<RBRACKET, %s>", bufferSaida);
            aux.type = RBRACKET;
        }
        else if (bufferSaida[indexBufferSaida -1] == 123)
        {
            fprintf(escrita, "<LBRACE, %s>", bufferSaida);
            aux.type = LBRACE;
        }
        else if (bufferSaida[indexBufferSaida -1] == 125)
        {
            fprintf(escrita, "<RBRACE, %s>", bufferSaida);
            aux.type = RBRACE;
        }
        else if (bufferSaida[indexBufferSaida -1] == 44)
        {
            fprintf(escrita, "<COMMA, %s>", bufferSaida);
            aux.type = COMMA;
        }
        else if (bufferSaida[indexBufferSaida -1] == 59)
        {
            fprintf(escrita, "<SEMICOLON, %s>", bufferSaida);
            aux.type = SEMICOLON;
        }
        indexBufferSaida = 0;
        return aux;
    }
    else if (estadoAtual == 14)
    {
        bufferSaida[indexBufferSaida] = '\0';
        fprintf(escrita, "<Relacional, %s>", bufferSaida);
        aux.type = RELACIONAL;
        indexBufferSaida = 0;
        return aux;
    }
    else if (estadoAtual == 15)
    {
        bufferSaida[indexBufferSaida] = '\0';
        fprintf(escrita, "<Fim>");
        aux.type = FIM;
        //aux.lexem = bufferSaida;
        indexBufferSaida = 0;
        return aux;
    }
    else if(estadoAtual == 16){
        bufferSaida[indexBufferSaida] = '\0';
        fprintf(escrita, "%s", bufferSaida);
        aux.type = SOMA;
        indexBufferSaida = 0;
    }
    
    
}



int main(int argc, char ** argv){
    
    token next;
    int aux = 0;
    arquivo = fopen("programa.c", "r");
    escrita = fopen("tokens.txt", "w");

    next.type = 0;

    tabelaTransicao[0][0] = 1;
    tabelaTransicao[0][1] = 2;
    tabelaTransicao[0][2] = 6;
    tabelaTransicao[0][3] = 9;
    tabelaTransicao[0][4] = 7;
    tabelaTransicao[0][5] = 9;
    tabelaTransicao[0][6] = 3;
    tabelaTransicao[0][7] = 13;
    tabelaTransicao[0][8] = 8;
    tabelaTransicao[0][9] = 16;
    tabelaTransicao[0][10] = 12;
    tabelaTransicao[0][11] = 15;

    tabelaTransicao[1][0] = 1;
    tabelaTransicao[1][1] = 10;
    tabelaTransicao[1][2] = 10;
    tabelaTransicao[1][3] = 10;
    tabelaTransicao[1][4] = 10;
    tabelaTransicao[1][5] = 10;
    tabelaTransicao[1][6] = 10;
    tabelaTransicao[1][7] = 10;
    tabelaTransicao[1][8] = 10;
    tabelaTransicao[1][9] = 10;
    tabelaTransicao[1][10] = 10;
    tabelaTransicao[1][11] = 10;
    
    tabelaTransicao[2][0] = 11;
    tabelaTransicao[2][1] = 2;
    tabelaTransicao[2][2] = 11;
    tabelaTransicao[2][3] = 11;
    tabelaTransicao[2][4] = 11;
    tabelaTransicao[2][5] = 11;
    tabelaTransicao[2][6] = 11;
    tabelaTransicao[2][7] = 11;
    tabelaTransicao[2][8] = 11;
    tabelaTransicao[2][9] = 11;
    tabelaTransicao[2][10] = 11;
    tabelaTransicao[2][11] = 11;
    
    tabelaTransicao[3][0] = 9;
    tabelaTransicao[3][1] = 9;
    tabelaTransicao[3][2] = 9;
    tabelaTransicao[3][3] = 4;
    tabelaTransicao[3][4] = 9;
    tabelaTransicao[3][5] = 9;
    tabelaTransicao[3][6] = 9;
    tabelaTransicao[3][7] = 9;
    tabelaTransicao[3][8] = 9;
    tabelaTransicao[3][9] = 9;
    tabelaTransicao[3][10] = 9;
    tabelaTransicao[3][11] = 9;
    
    tabelaTransicao[4][0] = 4;
    tabelaTransicao[4][1] = 4;
    tabelaTransicao[4][2] = 4;
    tabelaTransicao[4][3] = 5;
    tabelaTransicao[4][4] = 4;
    tabelaTransicao[4][5] = 4;
    tabelaTransicao[4][6] = 4;
    tabelaTransicao[4][7] = 4;
    tabelaTransicao[4][8] = 4;
    tabelaTransicao[4][9] = 4;
    tabelaTransicao[4][10] = 4;
    tabelaTransicao[4][11] = 12;
    
    tabelaTransicao[5][0] = 4;
    tabelaTransicao[5][1] = 4;
    tabelaTransicao[5][2] = 4;
    tabelaTransicao[5][3] = 5;
    tabelaTransicao[5][4] = 4;
    tabelaTransicao[5][5] = 4;
    tabelaTransicao[5][6] = 0;
    tabelaTransicao[5][7] = 4;
    tabelaTransicao[5][8] = 4;
    tabelaTransicao[5][9] = 4;
    tabelaTransicao[5][10] = 4;
    tabelaTransicao[5][11] = 12;

    tabelaTransicao[6][0] = 12;
    tabelaTransicao[6][1] = 12;
    tabelaTransicao[6][2] = 12;
    tabelaTransicao[6][3] = 12;
    tabelaTransicao[6][4] = 12;
    tabelaTransicao[6][5] = 12;
    tabelaTransicao[6][6] = 12;
    tabelaTransicao[6][7] = 12;
    tabelaTransicao[6][8] = 14;
    tabelaTransicao[6][9] = 12;
    tabelaTransicao[6][10] = 12;
    tabelaTransicao[6][11] = 12;

    tabelaTransicao[7][0] = 14;
    tabelaTransicao[7][1] = 14;
    tabelaTransicao[7][2] = 14;
    tabelaTransicao[7][3] = 14;
    tabelaTransicao[7][4] = 14;
    tabelaTransicao[7][5] = 14;
    tabelaTransicao[7][6] = 14;
    tabelaTransicao[7][7] = 14;
    tabelaTransicao[7][8] = 14;
    tabelaTransicao[7][9] = 14;
    tabelaTransicao[7][10] = 14;
    tabelaTransicao[7][11] = 14;
    
    tabelaTransicao[8][0] = 9;
    tabelaTransicao[8][1] = 9;
    tabelaTransicao[8][2] = 9;
    tabelaTransicao[8][3] = 9;
    tabelaTransicao[8][4] = 9;
    tabelaTransicao[8][5] = 9;
    tabelaTransicao[8][6] = 9;
    tabelaTransicao[8][7] = 9;
    tabelaTransicao[8][8] = 14;
    tabelaTransicao[8][9] = 9;
    tabelaTransicao[8][10] = 9;
    tabelaTransicao[8][11] = 9;

    if ((arquivo == NULL) || (escrita == NULL))
        printf("deu ruim\n");

    while (next.type != FIM)
    {
        next = myGetToken();
    }
    printf("%d\n", linhas);
    printf("%d\n", EOF);
    return 0;
}