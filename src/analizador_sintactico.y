%{
void yyerror (char *s);
int yylex();
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <ctype.h>
#include "y.tab.h"
int yylval;
int yystopparser = 0;
FILE *yyin;
char *yyltext;
char *yytext;
%}
%token OP_ASIG
%token OP_IGUAL OP_MENOR OP_MAYOR OP_MAYOR_IGUAL OP_MENOR_IGUAL OP_DISTINTO
%token OP_SUMA OP_MENOS OP_MUL OP_DIV
%token OP_CONCATENAR 
/* palabras reservadas */
%token WHILE IF DECLARE FLOAT INTEGER STRING  ELSE PUT GET
%token AND OR NOT
%token PUNTO_Y_COMA COMA PARENT_ABRE PARENT_CIERRA LLAVE_ABRE LLAVE_CIERRA
/* operandos */
%token ID CTE_ENTERO CTE_STRING CTE_REAL
%%
programa: {printf("INICIO PROGRAMA 1\n");} declaraciones lista_sentencias {printf("FIN PROGRAMA\n");}
          | {printf("INICIO PROGRAMA 2\n");} lista_sentencias {printf("FIN PROGRAMA\n");}
          ;

declaraciones: {printf("INICIO DECLARE\n");} DECLARE {printf("DECLARE\n");} LLAVE_ABRE {printf("LLAVE ABRE\n");} lista_declaraciones LLAVE_CIERRA {printf("LLAVE CIERRA\n");} {printf("FIN DECLARE\n");};

lista_declaraciones: declaracion
                    | lista_declaraciones COMA declaracion;

declaracion: {printf("INICIO DECLARACION\n");} tipo ID {printf("ID\n");} OP_ASIG {printf("OP_ASIG\n");} cte {printf("FIN DECLARACION\n");};

lista_sentencias: sentencia
                | lista_sentencias sentencia;

// Modificar y agregar IF ternario EJ: id = IF(condicion_logica, expresion, expresion)
sentencia: seleccion
          | seleccion ELSE LLAVE_ABRE lista_sentencias LLAVE_CIERRA
          | PUT ID
          | PUT CTE_STRING
          | GET ID
          | asignacion;

seleccion: IF condicion_logica LLAVE_ABRE lista_sentencias LLAVE_CIERRA;

tipo: INTEGER {printf("INTEGER\n");}
      | FLOAT {printf("FLOAT\n");}
      | STRING {printf("STRING\n");};

cte: CTE_STRING {printf("CTE STRING\n");}
    | CTE_ENTERO {printf("CTE ENTERO\n");}
    | CTE_REAL {printf("CTE REAL\n");};

condicion_logica: condicion AND condicion
                  | condicion OR condicion
                  | NOT condicion
                  | condicion;

condicion: expresion OP_MENOR expresion
          | expresion OP_MENOR_IGUAL expresion
          | expresion OP_IGUAL expresion
          | expresion OP_DISTINTO expresion
          | expresion OP_MAYOR expresion
          | expresion OP_MAYOR_IGUAL expresion;

asignacion: ID OP_ASIG expresion
            | ID OP_ASIG concatenacion;

expresion: expresion OP_SUMA termino
          | expresion OP_MENOS termino
          | termino;

termino: termino OP_MUL factor
        | termino OP_DIV factor
        | factor;

factor: ID
        | cte
        | PARENT_ABRE expresion PARENT_CIERRA;

concatenacion: ID OP_CONCATENAR ID
              | ID OP_CONCATENAR CTE_STRING
              | CTE_STRING OP_CONCATENAR ID
              | CTE_STRING OP_CONCATENAR CTE_STRING;
%%
int main(int argc,char *argv[])
{
  if ((yyin = fopen(argv[1], "rt")) == NULL)
  {
	printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
  }
  else
  {
	yyparse();
  }
  fclose(yyin);
  return 0;
}
void yyerror(char *s){
  printf("Syntax Error\n");
	system ("Pause");
	exit (1);
}