/*
    Title: BI-AAG - Domaci uloha 2 - Flex #12
    Author: Matej Simek - www.matejsimek.cz
    Date: 9.12.2011
*/

%{
  int max_lenght=0;
%}

WORD [^ \t\n]

%%

{WORD}+   {
           printf("%s %d ", yytext, yyleng);
           if(yyleng > max_lenght) max_lenght = yyleng;
          }
.    ;
\n   ;

%%

int main() {
  yylex();

  printf("\n%d\n", max_lenght);

  return 0;
}
