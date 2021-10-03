/* DECLARACIONES*/
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#define ERROR -1
#define QFIN 33

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

#define CANT_ESTADOS 26    //filas de la matriz de estados
#define CANT_TERMINALES 23 //columnas de la matriz de estados
#define CANTPR 13         //cantidad de palabras reservadas
#define LARGOMAX 15        //largo maximo de las palabras reservadas
#define LONG_MAX 30        //largo maximo de los string y nombre de id
#define MAX_INT 65535      //largo maximo de los enteros de 16 bit
#define MAX_REAL FLT_MAX   //largo maximo de los reales de 32 bit
#define TAMMAX 100

//Funciones de la matriz
void limpiar_token();
//comentario
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

void error();
void nada();

void init();

char proximo_caracter();
void get_elementos_esperados(char*);
int get_evento(char);
int esPalabraRes();

void a_minuscula (char*);
int insertarTS();
void guardarTS();

int nuevo_estado[CANT_ESTADOS][CANT_TERMINALES];
void (*proceso[CANT_ESTADOS][CANT_TERMINALES])();

/* TABLA DE SIMBOLOS */
struct tablaDeSimbolos
{
    char nombre[100];
    char tipo  [11];
    char valor [100];
    char ren   [31];
    int longitud;
};
struct tablaDeSimbolos TS[TAMMAX];
%}

/* TOKENS */
%right OP_ASIG
%token OP_IGUAL OP_MENOR OP_MAYOR OP_MAYOR_IGUAL OP_MENOR_IGUAL OP_DISTINTO
%left OP_SUMA OP_MENOS OP_MUL OP_DIV
%token OP_CONCATENAR 
/* palabras reservadas */
%token IF IIF ELSE WHILE INTEGER FLOAT STRING PUT GET
%token AND OR NOT DECLARE
%token PUNTO_Y_COMA COMA PARENT_ABRE PARENT_CIERRA LLAVE_ABRE LLAVE_CIERRA
/* operandos */
%token ID CTE_ENTERO CTE_STRING CTE_REAL
%%
programa: {printf("INICIO PROGRAMA 1\n");} seccion_de_declaracion sentencias {printf("FIN PROGRAMA\n");};

seccion_de_declaracion: DECLARE LLAVE_ABRE declaracion LLAVE_CIERRA;

declaracion: type lista_id PUNTO_Y_COMA | type lista_id PUNTO_Y_COMA declaracion;

lista_id: ID | lista_id COMA ID | lista_id COMA asignacion | asignacion;

sentencias: sentencia | sentencias sentencia;

sentencia: seleccion | iteracion | asignacion PUNTO_Y_COMA;

seleccion: IF PARENT_ABRE condicion PARENT_CIERRA LLAVE_ABRE sentencias LLAVE_CIERRA 
					| IF PARENT_ABRE condicion PARENT_CIERRA LLAVE_ABRE sentencias LLAVE_CIERRA ELSE LLAVE_ABRE sentencias LLAVE_CIERRA;

seleccion_iif: IIF PARENT_ABRE condicion COMA expresion COMA expresion PARENT_CIERRA;

iteracion: WHILE PARENT_ABRE condicion PARENT_CIERRA LLAVE_ABRE sentencias LLAVE_CIERRA;

asignacion: ID OP_ASIG expresion 
						| ID OP_ASIG expresion_string 
						| PUT CTE_STRING PUNTO_Y_COMA 
						| GET ID PUNTO_Y_COMA;

condicion: comparacion 
					| condicion AND comparacion 
					| condicion OR comparacion 
					| NOT PARENT_ABRE condicion PARENT_CIERRA;

comparacion: expresion comparador expresion 
						| expresion comparador expresion_string 
						| expresion_string comparador expresion 
						| expresion_string comparador expresion_string;

expresion: expresion OP_SUMA termino | expresion OP_MENOS termino | termino;

expresion_string: CTE_STRING | CTE_STRING OP_CONCATENAR CTE_STRING;

termino: termino OP_MUL factor | termino OP_DIV factor | factor;

factor: ID | constante | PARENT_ABRE expresion PARENT_CIERRA | seleccion_iif;

comparador: OP_IGUAL | OP_MENOR | OP_MAYOR | OP_MENOR_IGUAL | OP_MAYOR_IGUAL | OP_DISTINTO;

type: INTEGER | FLOAT | STRING;

constante: CTE_ENTERO | CTE_REAL;
%%

/* FUNCIONES AUXILIARES */    
/*----------VARIABLES GLOBALES-----------------*/
int yylval;
FILE *entrada, *tos;

int tipo_token; //numero identificador del token

int longitud; //longitud del string, id o cte
char token[200]; //Nombre del token identificado
char caracter; //caracter que se lee del archivo


const char *terminal[CANT_TERMINALES];


int main(int argc, char **argv)
{
    // inicializo matriz de estados de automata finito
    init();

    //Apertura del archivo con el lote de pruebas
    entrada = argc == 2 ? fopen(argv[1], "r") : fopen("prueba.txt", "r"); 
    if(entrada == NULL){
        printf("No se puede abrir el archivo %s\n", argc == 2 ? argv[1] : 
                                                    "prueba.txt");
        exit(ERROR);
    }

    //XXX: Para el analizador sintactico ya no la necesitamos mostrar
    if((tos = fopen("tabla_de_simbolos.txt", "w"))==NULL){
        printf("No se puede crear el archivo tabla_de_simbolos.txt\n");
        exit(ERROR);
    }

   	yyparse();

    guardarTS();
    fclose(entrada);
    fclose(tos);
    return 0;
}





/* inicializa matriz de automata finito del analizador lexico */
void init () {

    int i,j;
    /* lleno la matriz de proximo estado */
    for (i=0; i<CANT_ESTADOS; i++)
        for (j=0; j<CANT_TERMINALES; j++)
            nuevo_estado[i][j] = QFIN;

    /* lleno la matriz de proceso */
    for (i=0; i<CANT_ESTADOS; i++)
        for (j=0; j<CANT_TERMINALES; j++)
            proceso[i][j] = nada;

    nuevo_estado[0][T_mas] = 6;
    nuevo_estado[0][T_menos] = 8;
    nuevo_estado[0][T_asterisco] = 9;
    nuevo_estado[0][T_barra] = 10;
    nuevo_estado[0][T_letra] = 1;
    nuevo_estado[0][T_digito] = 2;
    nuevo_estado[0][T_punto] = 3;
    nuevo_estado[0][T_igual] = 4;
    nuevo_estado[0][T_menor] = 15;
    nuevo_estado[0][T_mayor] = 13;

    nuevo_estado[0][T_comillas] = 22;
    nuevo_estado[0][T_pyc] = 24;
    nuevo_estado[0][T_CAR] = 0;
    nuevo_estado[0][T_parentesis_abre] = 18;
    nuevo_estado[0][T_parentesis_cierra] = 19;
    nuevo_estado[0][T_coma] = 23;
    nuevo_estado[0][T_tab] = 0;
    nuevo_estado[0][T_espacio] = 0;
    nuevo_estado[0][T_newline] = 0;
    nuevo_estado[0][T_llave_abre] = 20;
    nuevo_estado[0][T_llave_cierra] = 21;

    

    nuevo_estado[1][T_letra] = 1;
    nuevo_estado[1][T_digito] = 1;

    nuevo_estado[2][T_digito] = 2;
    nuevo_estado[2][T_punto] = 3;

    nuevo_estado[3][T_digito] = 25;
    // nuevo_estado[3][T_EOF] = QFIN;

    nuevo_estado[25][T_digito] = 25;

    nuevo_estado[4][T_igual] = 5;

    nuevo_estado[6][T_mas] = 7;

    for (i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[22][i] = 22;
    nuevo_estado[22][T_comillas] = QFIN;
    nuevo_estado[22][T_EOF] = QFIN;

    for (i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[11][i] = 11;
    nuevo_estado[10][T_numeral] = 11;
    nuevo_estado[11][T_EOF] = QFIN;
    nuevo_estado[11][T_numeral] = 12;

    for (i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[12][i] = 11;
    //nuevo_estado[12][T_numeral] = 12;
    nuevo_estado[12][T_barra] = 0;
    nuevo_estado[12][T_EOF] = QFIN;

    nuevo_estado[13][T_igual] = 14;
    nuevo_estado[15][T_igual] = 16;
    nuevo_estado[15][T_mayor] = 17;

    proceso[0][T_mas] = op_suma;
    proceso[0][T_menos] = op_menos;
    proceso[0][T_asterisco] = op_mul;
    proceso[0][T_barra] = op_div;
    proceso[0][T_letra] = inic_id;
    proceso[0][T_digito] = inic_entero;
    proceso[0][T_igual] = op_asig;
    proceso[0][T_menor] = op_menor;
    proceso[0][T_mayor] = op_mayor;
    proceso[0][T_comillas] = inic_string;
    proceso[0][T_punto] = inic_real;
    proceso[0][T_pyc] = puntoycoma;
    proceso[0][T_parentesis_abre] = par_abre;
    proceso[0][T_parentesis_cierra] = par_cierra;
    proceso[0][T_llave_abre] = llave_abre;
    proceso[0][T_llave_cierra] = llave_cierra;
    proceso[0][T_coma] = coma;
    proceso[0][T_newline] = salto_linea;
    proceso[0][T_CAR] = error;

    for (i = 0; i < CANT_TERMINALES; i++)
        proceso[1][i] = fin_id;
    proceso[1][T_letra] = cont_id;
    proceso[1][T_digito] = cont_id;

    proceso[6][T_mas] = op_concaten;
    proceso[4][T_igual] = op_igualdad;
    proceso[15][T_igual] = op_menor_ig;
    proceso[13][T_igual] = op_mayor_ig;
    proceso[15][T_mayor] = op_distinto;

    for (i = 0; i < CANT_TERMINALES; i++)
        proceso[22][i] = cont_string;
    proceso[22][T_comillas] = fin_string;
    proceso[22][T_EOF] = error;

    for (i = 0; i < CANT_TERMINALES; i++)
        proceso[2][i] = fin_entero;
    proceso[2][T_digito] = cont_entero;
    proceso[2][T_punto] = cont_real;

    for (i = 0; i < CANT_TERMINALES; i++)
        proceso[3][i] = error;
    proceso[3][T_digito] = cont_real;

    for (i = 0; i < CANT_TERMINALES; i++)
        proceso[25][i] = fin_real;
    proceso[25][T_digito] = cont_real;
    //proceso[3][T_EOF] = error;

    /*for (i = 0; i < CANT_TERMINALES; i++)
        proceso[10][i] = op_div2;
    proceso[10][T_numeral] = inic_com;
    proceso[9][T_EOF] = nada;
    proceso[9][T_newline] = nada;*/

    for (i = 0; i < CANT_TERMINALES; i++)
        proceso[11][i] = nada;
    proceso[11][T_numeral] = cont_com;

  
    for (i = 0; i < CANT_TERMINALES; i++)
        proceso[12][i] = cont_com;
    proceso[12][T_barra] = fin_com;
    proceso[12][T_newline] = salto_linea;
    proceso[12][T_EOF] = error;

    terminal[T_mas] = "+";
    terminal[T_menos] = "-";
    terminal[T_asterisco] = "*";
    terminal[T_barra] = "/";
    terminal[T_letra] = "a-z A-Z";
    terminal[T_digito] = "0-9";
    terminal[T_igual] = "=";
    terminal[T_menor] = "<";
    terminal[T_mayor] = ">";
    terminal[T_comillas] = "\"";
    terminal[T_punto] = ".";
    terminal[T_pyc] = ";";
    terminal[T_CAR] = "caracter";
    terminal[T_parentesis_abre] = "(";
    terminal[T_parentesis_cierra] = ")";
    terminal[T_coma] = ",";
    terminal[T_tab] = "tab";
    terminal[T_espacio] = "espacio";
    terminal[T_newline] = "nueva linea";
    terminal[T_EOF] = "EOF";
    terminal[T_numeral] = "#";
    terminal[T_llave_abre] = "{";
    terminal[T_llave_cierra] = "}";

}

