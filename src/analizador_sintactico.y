%{
void yyerror (char *s);

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