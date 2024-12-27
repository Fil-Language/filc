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
parser grammar FilParser;

options {
    tokenVocab = FilLexer;
}

@parser::header {
#include "filc/grammar/expression/Expression.h"
#include "filc/grammar/literal/Literal.h"
#include "filc/grammar/program/Program.h"
#include "filc/grammar/variable/Variable.h"
#include "filc/grammar/calcul/Calcul.h"
#include "filc/grammar/identifier/Identifier.h"
#include "filc/grammar/assignation/Assignation.h"
#include "filc/grammar/pointer/Pointer.h"
#include <memory>
#include <vector>
}

program returns[std::shared_ptr<filc::Program> tree]
@init {
    std::vector<std::shared_ptr<filc::Expression>> expressions;
}
@after {
    $tree = std::make_shared<filc::Program>(expressions);
}
    : (e=expression {
        expressions.push_back($e.tree);
    } SEMI?)* EOF;

expression returns[std::shared_ptr<filc::Expression> tree]
@after {
    $tree->setPosition(filc::Position($ctx->start, $ctx->stop));
}
    : l=literal {
        $tree = $l.tree;
    }
    | v=variable_declaration {
        $tree = $v.tree;
    }
    | i=IDENTIFIER {
        $tree = std::make_shared<filc::Identifier>($i.text);
    }
    | p=pointer {
        $tree = $p.tree;
    }
    | po=pointer_operation {
        $tree = $po.tree;
    }

    // === Binary calcul ===
    | el3=expression op3=MOD er3=expression {
        $tree = std::make_shared<filc::BinaryCalcul>($el3.tree, $op3.text, $er3.tree);
    }
    | el4=expression op4=(DIV | STAR) er4=expression {
        $tree = std::make_shared<filc::BinaryCalcul>($el4.tree, $op4.text, $er4.tree);
    }
    | el5=expression op5=(PLUS | MINUS) er5=expression {
        $tree = std::make_shared<filc::BinaryCalcul>($el5.tree, $op5.text, $er5.tree);
    }
    | el2=expression op2=(LT | GT | LTE | GTE | EQEQ | NEQ) er2=expression {
        $tree = std::make_shared<filc::BinaryCalcul>($el2.tree, $op2.text, $er2.tree);
    }
    | el1=expression op1=(AND | OR) er1=expression {
        $tree = std::make_shared<filc::BinaryCalcul>($el1.tree, $op1.text, $er1.tree);
    }
    // === Binary calcul ===

    | LPAREN e=expression RPAREN {
        $tree = $e.tree;
    }
    | a=assignation {
       $tree = $a.tree;
    };

literal returns[std::shared_ptr<filc::Expression> tree]
    : b=boolean {
        $tree = $b.tree;
    }
    | n=number {
        $tree = $n.tree;
    }
    | c=CHARACTER {
        $tree = std::make_shared<filc::CharacterLiteral>(filc::CharacterLiteral::stringToChar($c.text));
    }
    | s=STRING {
        $tree = std::make_shared<filc::StringLiteral>($s.text);
    };

boolean returns[std::shared_ptr<filc::BooleanLiteral> tree]
    : TRUE {
        $tree = std::make_shared<filc::BooleanLiteral>(true);
    }
    | FALSE {
        $tree = std::make_shared<filc::BooleanLiteral>(false);
    };

number returns[std::shared_ptr<filc::Expression> tree]
@init {
    bool is_negative = false;
}
    : (PLUS | MINUS {
        is_negative = true;
    })? (i=INTEGER {
        auto ivalue = stoi($i.text);
        if (is_negative) {
            ivalue = -ivalue;
        }
        $tree = std::make_shared<filc::IntegerLiteral>(ivalue);
    }
    | f=FLOAT {
        auto fvalue = stod($f.text);
        if (is_negative) {
            fvalue = -fvalue;
        }
        $tree = std::make_shared<filc::FloatLiteral>(fvalue);
    });

variable_declaration returns[std::shared_ptr<filc::VariableDeclaration> tree]
@init {
    bool is_constant = true;
    std::string type_name;
    std::shared_ptr<filc::Expression> value = nullptr;
}
@after {
    $tree = std::make_shared<filc::VariableDeclaration>(is_constant, $name.text, type_name, value);
}
    : (VAL | VAR {
        is_constant = false;
    }) name=IDENTIFIER (COLON type {
        type_name = $type.text;
    })? (EQ value=expression {
        value = $value.tree;
    })?;

type : IDENTIFIER STAR?;

assignation returns[std::shared_ptr<filc::Assignation> tree]
    : i1=IDENTIFIER EQ e1=expression {
        $tree = std::make_shared<filc::Assignation>($i1.text, $e1.tree);
    }
    | i2=IDENTIFIER op=(PLUS_EQ | MINUS_EQ | STAR_EQ | DIV_EQ | MOD_EQ | AND_EQ | OR_EQ) e2=expression {
        const auto calcul = std::make_shared<filc::BinaryCalcul>(std::make_shared<filc::Identifier>($i2.text), $op.text.substr(0, $op.text.size() - 1), $e2.tree);
        calcul->setPosition(filc::Position($op, $e2.stop));
        $tree = std::make_shared<filc::Assignation>($i2.text, calcul);
    };

pointer returns[std::shared_ptr<filc::Pointer> tree]
    : NEW t=IDENTIFIER LPAREN e=expression RPAREN {
        $tree = std::make_shared<filc::Pointer>($t.text, $e.tree);
    };

pointer_operation returns[std::shared_ptr<filc::Expression> tree]
    : STAR i=IDENTIFIER {
        $tree = std::make_shared<filc::PointerDereferencing>($i.text);
    }
    | AMP i=IDENTIFIER {
        $tree = std::make_shared<filc::VariableAddress>($i.text);
    };
