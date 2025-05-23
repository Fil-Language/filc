/**
 * MIT License
 *
 * Copyright (c) 2024-Present Kevin Traini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
lexer grammar FilLexer;

// Reserved words
VAR: 'var';
VAL: 'val';
TRUE: 'true';
FALSE: 'false';
NEW: 'new';

// Operators
EQ: '=';
COLON: ':';
SEMI: ';';
PLUS: '+';
MINUS: '-';
STAR: '*';
DIV: '/';
MOD: '%';
AND: '&&';
OR: '||';
EQEQ: '==';
NEQ: '!=';
LT: '<';
LTE: '<=';
GT: '>';
GTE: '>=';
LPAREN: '(';
RPAREN: ')';
LBRACK: '[';
RBRACK: ']';
COMMA: ',';
AMP: '&';

// Assignation operators
PLUS_EQ: '+=';
MINUS_EQ: '-=';
STAR_EQ: '*=';
DIV_EQ: '/=';
MOD_EQ: '%=';
AND_EQ: '&&=';
OR_EQ: '||=';

// String and chars
fragment UPPERCASE: [A-Z];
fragment LOWERCASE: [a-z];
fragment LETTER: UPPERCASE | LOWERCASE;
fragment ESCAPE_CHAR: '\\' ['"?abfnrtv\\];
fragment STRING_CHAR: ~('"' | '\\' | '\n') | ESCAPE_CHAR;
STRING: '"' (STRING_CHAR | '\\"' | '\\\\')* '"';
CHARACTER: '\'' (~('\'' | '\\' | '\n') | ESCAPE_CHAR) '\'';

// Numbers
fragment DIGIT: [0-9];
INTEGER: DIGIT+;
FLOAT: DIGIT* '.' DIGIT+;

IDENTIFIER: (LETTER | '_') (LETTER | DIGIT | '_')*;

// All ignored
SEPARATOR: (' ' | '\t' | '\r' | '\n' | EOF) -> skip;
COMMENT: '//' (~('\n' | '\r'))* -> skip;
COMMENT_BLOCK: '/*' .*? '*/' -> skip;
