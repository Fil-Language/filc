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
    })* EOF;

expression returns[std::shared_ptr<filc::Expression> tree]
    : l=literal {
        $tree = $l.tree;
    }
    | v=variable_declaration {
        $tree = $v.tree;
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
    : i=INTEGER {
        $tree = std::make_shared<filc::IntegerLiteral>(stoi($i.text));
    }
    | f=FLOAT {
        $tree = std::make_shared<filc::FloatLiteral>(stod($f.text));
    };

variable_declaration returns[std::shared_ptr<filc::VariableDeclaration> tree]
@init {
    bool is_constant = true;
    std::string type;
    std::shared_ptr<filc::Expression> value = nullptr;
}
@after {
    $tree = std::make_shared<filc::VariableDeclaration>(is_constant, $name.text, type, value);
}
    : (VAL | VAR {
        is_constant = false;
    }) name=IDENTIFIER (COLON type=IDENTIFIER {
        type = $type.text;
    })? (EQ value=expression {
        value = $value.tree;
    })?;
