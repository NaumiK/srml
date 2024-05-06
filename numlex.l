%option c++

%{

#include "tokens.hh"

%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
VAL     [x|y]

%%

{WS}              /* skip blanks and tabs */
{DIGIT1}{DIGIT}*  { return Flex::Tokens::CONST;}
{DIGIT}           { return Flex::Tokens::CONST;}
{VAL}             { return Flex::Tokens::VALUE;}
"&"               { return Flex::Tokens::BINOP;}
"j_1("            { return Flex::Tokens::UNROP;}
"("               { return Flex::Tokens::LBRAC;}
")"               { return Flex::Tokens::RBRAC;}
"$"               { return Flex::Tokens::ENDCH;}
.                 { return Flex::Tokens::UNKNOWN;}
%%