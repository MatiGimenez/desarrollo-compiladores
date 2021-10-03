#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include "y.tab.h"
FILE *yyin;

//TOKENS
#define QFIN 33
#define OP_SUMA 257
#define OP_MENOS 258
#define OP_MUL 259
#define OP_DIV 260
#define OP_ASIG 261
#define OP_IGUAL 262
#define OP_MENOR 263
#define OP_MAYOR 264
#define OP_MAYOR_IGUAL 265
#define OP_MENOR_IGUAL 266
#define OP_DISTINTO 267
#define OP_CONCATENAR 268
#define PUNTO_Y_COMA 269
#define COMA 270
#define PARENT_ABRE 271
#define PARENT_CIERRA 272
#define LLAVE_ABRE 273
#define LLAVE_CIERRA 274
#define ID 275
#define CTE_STRING 276
#define CTE_ENTERO 277
#define CTE_REAL 278
#define NUMERAL 279
//#define PUNTO 280

//Palabras Reservadas
#define IIF 299
#define IF 300
#define ELSE 301
#define WHILE 302
#define INTEGER 303
#define FLOAT 304
#define STRING 305
#define PUT 306
#define GET 307
#define OR 308
#define AND 309
#define NOT 310
#define DECLARE 311
#define ERROR -1
/****************************/


/* Terminales */
#define T_mas 0
#define T_menos 1
#define T_asterisco 2
#define T_barra 3
#define T_letra 4
#define T_digito 5
#define T_igual 6
#define T_menor 7
#define T_mayor 8
#define T_comillas 9
#define T_punto 10
#define T_pyc 11
#define T_CAR 12
#define T_parentesis_abre 13
#define T_parentesis_cierra 14
#define T_coma 15
#define T_tab 16
#define T_espacio 17
#define T_newline 18
#define T_EOF 19
#define T_llave_abre 20
#define T_llave_cierra 21
#define T_numeral 22
/****************************/

#define CANT_ESTADOS 26    //filas de la matriz de estados
#define CANT_TERMINALES 23 //columnas de la matriz de estados
#define CANTPR 13          //cantidad de palabras reservadas
#define LARGOMAX 15        //largo maximo de las palabras reservadas
#define LONG_MAX 30        //largo maximo de los string y nombre de id
#define MAX_INT 65535      //largo maximo de los enteros de 16 bit
#define MAX_REAL FLT_MAX   //largo maximo de los reales de 32 bit
#define TAMMAX 100

//Funciones de la matriz
void limpiar_token();
void inic_com();
void cont_com();
void fin_com();

void op_suma();
void op_menos();
void op_mul();
void op_div();
void op_div2();
void op_asig();
void op_igualdad();

void op_menor();
void op_mayor();
void op_menor_ig();
void op_mayor_ig();
void op_distinto();

void puntoycoma();
void coma();
void par_abre();
void par_cierra();
void llave_abre();
void llave_cierra();

void inic_id();
void cont_id();
void fin_id();

void inic_string();
void cont_string();
void fin_string();

void inic_entero();
void cont_entero();
void fin_entero();

void inic_real();
void cont_real();
void fin_real();

void salto_linea();
void op_concaten();

void yyerror();
void nada();

char proximo_caracter();
void get_elementos_esperados(char *);
int get_evento(char);
int esPalabraRes();
void a_mayuscula(char *);
int insertarTS();
void guardarTS();
void guardarToken();

int nuevo_estado[CANT_ESTADOS][CANT_TERMINALES] = {
    6,8,9,10,1,2,4,15,13,22,3,24,0,18,19,23,0,0,QFIN,20,21,-1,
QFIN,QFIN,QFIN,QFIN,1,1,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,2,QFIN,QFIN,QFIN,QFIN,3,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
-1,-1,-1,-1,-1,25,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,5,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
7,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,11,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,-1,11,11,12,
11,11,11,0,11,11,11,11,11,11,11,11,11,11,11,11,11,11,-1,11,11,11,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,14,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,16,QFIN,17,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
22,22,22,22,22,22,22,22,22,QFIN,22,22,22,22,22,22,22,22,-1,22,22,22,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,
QFIN,QFIN,QFIN,QFIN,QFIN,25,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,QFIN,

    };

void (*proceso[CANT_ESTADOS][CANT_TERMINALES])() = {
    op_suma,op_menos,op_mul,op_div,inic_id,inic_entero,op_asig,op_menor,op_mayor,inic_string,inic_real,puntoycoma,nada,par_abre,par_cierra,coma,nada,nada,nada,llave_abre,llave_cierra,yyerror,
fin_id,fin_id,fin_id,fin_id,cont_id,cont_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,fin_id,
fin_entero,fin_entero,fin_entero,fin_entero,fin_entero,cont_entero,fin_entero,fin_entero,fin_entero,fin_entero,cont_real,fin_entero,fin_entero,fin_entero,fin_entero,fin_entero,fin_entero,fin_entero,fin_entero,fin_entero,fin_entero,fin_entero,
yyerror,yyerror,yyerror,yyerror,yyerror,cont_real,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,yyerror,
nada,nada,nada,nada,nada,nada,op_igualdad,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
op_concaten,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,inic_com,
cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,yyerror,cont_com,cont_com,cont_com,
cont_com,cont_com,cont_com,fin_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,cont_com,yyerror,cont_com,cont_com,cont_com,
nada,nada,nada,nada,nada,nada,op_mayor_ig,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,op_menor_ig,nada,op_distinto,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,fin_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,yyerror,cont_string,cont_string,cont_string,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,nada,
fin_real,fin_real,fin_real,fin_real,fin_real,cont_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,fin_real,

   };

/* TABLA DE SIMBOLOS */
struct tablaDeSimbolos
{
    char nombre[100];
    char tipo[11];
    char valor[100];
    int longitud;
};

struct tablaDeSimbolos TS[TAMMAX];

int yylval;
FILE *salida, *entrada, *tos;
int TStop = 0;   // �ndice de la TS
int tipo_token;  //numero identificador del token
int linea = 1;   //linea por la que esta leyendo
int estado = 0;  // estado actual
int longitud;    //longitud del string, id o cte
char token[200]; //Nombre del token identificado
char caracter;   //caracter que se lee del archivo
const char palabrasRes[CANTPR][LARGOMAX] = {
    {"IF"},
    {"IIF"},
    {"ELSE"},
    {"WHILE"},
    {"INTEGER"},
    {"FLOAT"},
    {"STRING"},
    {"PUT"},
    {"GET"},
    {"OR"},
    {"AND"},
    {"NOT"},
    {"DECLARE"}
    };

const char *terminal[CANT_TERMINALES] = {"+", "-", "*", "/", "a-z A-Z", "0-9", "=", "<", ">", "\"", ".", ";", "caracter", "(", ")", ",", "tab", "espacio", "nueva linea", "EOF", "{", "}", "#"};

int NroPalabrasRes[CANTPR] = {
    IF,
    IIF,
    ELSE,
    WHILE,
    INTEGER,
    FLOAT,
    STRING,
    PUT,
    GET,
    OR,
    AND,
    NOT,
    DECLARE
    };

int yylex()
{
    tipo_token = 0;
    estado = 0;
    while (estado != QFIN && tipo_token != -1)
    {
        if ((caracter = proximo_caracter()) != EOF)
        {
            printf("%c\n", caracter);
            if (caracter != '\037')
            {
                int columna = get_evento(caracter);
                //printf("[%d][%d]\n", estado, columna);

                (proceso[estado][columna])();
                estado = nuevo_estado[estado][columna];
                //printf("%d\n", estado);
            }
            else
            {
                estado = 0;
            }
        }
        else
        {
            printf("%c\n", caracter);
            (proceso[estado][17])();
            estado = QFIN;
            //tipo_token = 0;
        }
    }
    if (!feof(yyin))
    {
        fseek(yyin, -sizeof(char), SEEK_CUR);
    }
    //printf("%d\n", tipo_token);

    return tipo_token;
}

char proximo_caracter()
{
    char _caracter;
    // obtengo caracter desde el archivo de entrada
    _caracter = fgetc(yyin);

    return _caracter;
}

void limpiar_token()
{
    *token = '\0';
}

void inic_com()
{
    limpiar_token();
}

void cont_com()
{
}

void fin_com()
{
    tipo_token = 0;
}

void op_suma()
{
    tipo_token = OP_SUMA;
}

void op_menos()
{
    tipo_token = OP_MENOS;
}

void op_mul()
{
    tipo_token = OP_MUL;
}

void op_div()
{
    tipo_token = OP_DIV;
}

void op_div2()
{
    fseek(yyin, -sizeof(char), SEEK_CUR);
    tipo_token = OP_DIV;
}

void op_asig()
{
    tipo_token = OP_ASIG;
}

void op_menor()
{
    tipo_token = OP_MENOR;
}

void op_menor_ig()
{
    tipo_token = OP_MENOR_IGUAL;
}

void op_mayor()
{
    tipo_token = OP_MAYOR;
}

void op_mayor_ig()
{
    tipo_token = OP_MAYOR_IGUAL;
}

void puntoycoma()
{
    tipo_token = PUNTO_Y_COMA;
}

void par_abre()
{
    tipo_token = PARENT_ABRE;
}

void par_cierra()
{
    tipo_token = PARENT_CIERRA;
}

void llave_abre()
{
    tipo_token = LLAVE_ABRE;
}

void llave_cierra()
{
    tipo_token = LLAVE_CIERRA;
}

void coma()
{
    tipo_token = COMA;
}

void inic_id()
{
    limpiar_token();
    strcat(token, &caracter);
    tipo_token = ID;
}

void cont_id()
{
    strcat(token, &caracter);
    tipo_token = ID;
}

void fin_id()
{
    int i;
    if (strlen(token) > LONG_MAX)
    {
        fprintf(stderr, "identificador demasiado largo en linea: %d\n", linea);
        *token = '\0';
        tipo_token = 0;
    }
    else
    {
        if ((i = esPalabraRes()) != -1)
        {
            tipo_token = NroPalabrasRes[i];
        }
        else
        {
            tipo_token = ID;
            yylval = insertarTS();
        }
    }
}

void inic_entero()
{
    limpiar_token();
    strcat(token, &caracter);
    tipo_token = CTE_ENTERO;
}

void cont_entero()
{
    strcat(token, &caracter);
    tipo_token = CTE_ENTERO;
}

void fin_entero()
{
    int cte = atoi(token);
    if (cte > MAX_INT)
    {
        fprintf(stderr, "Entero sobrepasa limite maximo en linea: %d\n", linea);
        *token = '\0';
        tipo_token = 0;
    }
    else
    {
        tipo_token = CTE_ENTERO;
        yylval = insertarTS();
    }
}

void inic_real()
{
    limpiar_token();
    strcat(token, &caracter);
    tipo_token = CTE_REAL;
}

void cont_real()
{
    strcat(token, &caracter);
    tipo_token = CTE_REAL;
}

void fin_real()
{
    float cte = atof(token);
    if (cte > MAX_REAL)
    {
        fprintf(stderr, "Real sobrepasa limite maximo en linea: %d\n", linea);
        *token = '\0';
        tipo_token = 0;
    }
    else
    {
        tipo_token = CTE_REAL;
        yylval = insertarTS();
    }
}

void inic_string()
{
    limpiar_token();
    tipo_token = CTE_STRING;
}

void cont_string()
{
    strcat(token, &caracter);
    tipo_token = CTE_STRING;
}

void fin_string()
{
    if (strlen(token) > LONG_MAX)
    {
        fprintf(stderr, "String demasiado largo en linea: %d\n", linea);
        *token = '\0';
        tipo_token = 0;
    }
    else
    {
        tipo_token = CTE_STRING;
        yylval = insertarTS();
    }
}

void salto_linea()
{
    tipo_token = 0;
    linea++;
}

void op_concaten()
{
    limpiar_token();
    tipo_token = OP_CONCATENAR;
}

void op_igualdad()
{
    limpiar_token();
    tipo_token = OP_IGUAL;
}

void op_distinto()
{
    limpiar_token();
    tipo_token = OP_DISTINTO;
}

void nada()
{
}

void get_elementos_esperados(char *esperados)
{
    int i;
    for (i = 0; i < CANT_TERMINALES; i++)
        /* busco los terminales que no me deriven en un error, obviando EOF
         * y espacios en blanco */
        if (i != T_EOF && i != T_tab && i != T_espacio && i != T_newline &&
            proceso[estado][i] != yyerror)
        {
            /* agrego elemento a la lista de terminales esperados */
            strcat(esperados, terminal[i]);
            strcat(esperados, " ");
        }
}

void yyerror()
{
    char _elementos_esperados[300] = {'\0'};
    /* como es un error, descarto el contenido de token */
    *token = '\0';
    // en caso de un fin de archivo inesperado muestro mensaje y salgo
    if (caracter == EOF)
    {
        fprintf(stderr, "Error linea %d: Fin de archivo inesperado \n", linea);
        tipo_token = ERROR;
        return;
    }
    // obtengo elementos esperados
    get_elementos_esperados(_elementos_esperados);
    // muestro mensaje de error
    fprintf(stderr, "Error linea %d, cerca del elemento inesperado: '%c'\n", linea, caracter);
    // muestro elementos esperados
    fprintf(stderr, ">>>> Elementos esperados: %s\n", _elementos_esperados);
    tipo_token = ERROR;
}

int get_evento(char c)
{
    if (c >= 'a' && c <= 'z')
        return 4;
    if (c >= 'A' && c <= 'Z')
        return 4;
    if (c >= '0' && c <= '9')
        return 5;

    switch (c)
    {

    case '+':
        return 0;
    case '-':
        return 1;

    case '*':
        return 2;

    case '/':
        return 3;

    case '=':
        return 6;

    case '<':
        return 7;

    case '>':
        return 8;

    case '"':
        return 9;

    case '.':
        return 10;

    case ';':
        return 11;

    case '(':
        return 13;

    case ')':
        return 14;

    case ',':
        return 15;

    case '\t':
    case '\r':
    case ' ':
        return 17;

    case '\n':
        return 18;

    case EOF:
        return 19;

    case '{':
        return 20;

    case '}':
        return 21;

    case '#':
        return 22;

    default:
        return 12;
    }
}

int esPalabraRes()
{
    char aux[LONG_MAX];
    int i;
    strcpy(aux, token);
    // pasamos todo el token a minuscula
    a_mayuscula(token);
    for (i = 0; i < CANTPR; i++)
    {
        if (strcmp(palabrasRes[i], token) == 0)
        {
            return i;
        }
    }
    return -1;
}

void a_mayuscula(char *palabra)
{
    char *tmp = palabra;
    while (*tmp)
    {
        *tmp = toupper(*tmp);
        tmp++;
    }
}

void guardarTS()
{
    int i;
    fprintf(tos, "Nro\t | Nombre\t\t\t | Tipo\t| Valor\t | Longitud \n");
    for (i = 0; i < TStop; i++)
    {
        fprintf(tos, "%d     \t | %s     \t\t\t | %s     \t | %s \t | %d \n", i, TS[i].nombre, TS[i].tipo, TS[i].valor, TS[i].longitud);
    }
}

/* Inserta en la TS o si ya existe devuelve la posicion */
int insertarTS()
{
    int i = 0;
    // Reviso si ya existe en la TS
    for (i = 0; i < TStop; i++)
    {
        if (tipo_token == ID)
        {
            if (strcmp(TS[i].nombre, token) == 0)
                return i;
        }
        else
        {
            if (strcmp(TS[i].valor, token) == 0)
                return i;
        }
    }
    // Inserto en la TS
    switch (tipo_token)
    {
    case ID:
        strcpy(TS[TStop].nombre, token);
        strcpy(TS[TStop].tipo, "ID");
        TStop++;
        break;
    case CTE_ENTERO:
        strcpy(TS[TStop].nombre, "_");
        strcpy(TS[TStop].tipo, "CTE");
        strcpy(TS[TStop].valor, token);
        TStop++;
        break;
    case CTE_REAL:
        strcpy(TS[TStop].nombre, "_");
        strcpy(TS[TStop].tipo, "REAL");
        strcpy(TS[TStop].valor, token);
        TStop++;
        break;
    case CTE_STRING:
        strcpy(TS[TStop].nombre, "_");
        strcpy(TS[TStop].tipo, "STRING");
        strcpy(TS[TStop].valor, token);
        TS[TStop].longitud = (strlen(token));
        TStop++;
        break;
    }
    return TStop - 1;
}

void guardarToken()
{

    switch (tipo_token)
    {
    case ID:
        fputs("ID\n", salida);
        // fprintf(salida, "< \n ID >\n");
        break;
    case OP_SUMA:
        fprintf(salida, "OP_SUMA\n");
        break;
    case OP_MENOS:
        fprintf(salida, "OP_MENOS\n");
        break;
    case OP_MUL:
        fprintf(salida, "OP_MUL\n");
        break;
    case OP_DIV:
        fprintf(salida, "OP_DIV\n");
        break;
    case OP_ASIG:
        fprintf(salida, "OP_ASIG\n");
        break;
    case OP_IGUAL:
        fprintf(salida, "OP_IGUAL\n");
        break;
    case OP_MENOR:
        fprintf(salida, "OP_MENOR\n");
        break;
    case OP_MAYOR:
        fprintf(salida, "OP_MAYOR\n");
        break;
    case OP_MAYOR_IGUAL:
        fprintf(salida, "OP_MAYOR_IGUAL\n");
        break;
    case OP_MENOR_IGUAL:
        fprintf(salida, "OP_MENOR_IGUAL\n");
        break;
    case OP_DISTINTO:
        fprintf(salida, "OP_DISTINTO\n");
        break;
    case OP_CONCATENAR:
        fprintf(salida, "OP_CONCATENAR\n");
        break;
    case PUNTO_Y_COMA:
        fprintf(salida, "PUNTO_Y_COMA\n");
        break;
    case PARENT_ABRE:
        fprintf(salida, "PARENT_ABRE\n");
        break;
    case PARENT_CIERRA:
        fprintf(salida, "PARENT_CIERRA\n");
        break;
    case COMA:
        fprintf(salida, "COMA\n");
        break;
    case CTE_ENTERO:
        fprintf(salida, "CTE_ENTERO\n");
        break;
    case CTE_REAL:
        fprintf(salida, "CTE_REAL\n");
        break;
    case CTE_STRING:
        fprintf(salida, "CTE_STRING\n");
        break;
    case LLAVE_ABRE:
        fprintf(salida, "LLAVE_ABRE\n");
        break;
    case LLAVE_CIERRA:
        fprintf(salida, "LLAVE_CIERRA\n");
        break;
        //PALABRAS RESERVADAS
    default:
        fprintf(salida, "%s\n", token);
        break;
    }
}
