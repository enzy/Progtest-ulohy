/*
    Title: BI-AAG - Domaci uloha 2 - Flex #13
    Author: Matej Simek - www.matejsimek.cz
    Date: 25.12.2010
*/

%{
  int max_lenght=0;
%}

WORD [^ \t\n]

%%

{WORD}+   {
           printf("%d %s ", yyleng, yytext);
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
