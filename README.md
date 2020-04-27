# 8L_Lang
8L is a programming language by TheSeems and me. The aim is to practice in formal language theory and program our interpreter

## Example: n-th fibonacci number

```
func fib(int n) : int {
  int prev1 = 1, prev2 = 1, res = 1;
  for (int i = 0; i < n - 2; i++) {
    res = prev1 + prev2;
    prev1 = prev2;
    prev2 = res;
  }
  
  return res;
} 

func main() : int {
  return fib(n);
}
```

## Formal description
```
<program> ::= <operator> | <function>
<function> ::= func <name> (<params> {, <params> }) <return type><body>
<params> ::= <type>... <name> {, <name> }; | <type> <name> | ε
<return type> ::= ε | : <type>
<name> ::= <char> | <name><char> | <name><digit>
<body> ::= { <operator> {;<operator>} }
<operator> ::= <assignment operator> | <print operator> | <read operator> | <body> | <operator> {;<operator>}   | <assignment operator> | break <break type>; | continue;
<break type> ::= ε | <normal number>
<assignment operator> ::= <name> = <expr> | <name>[<expr>]=<expr>
<expr> ::= <simple expr> | <expr><boolean operator><expr> | <expr> if <expr> else <expr>
<boolean operator> ::= > | < | != | <= | >= | ==
<simple expr> ::= <term> { <additive operator><term> } | <term> { <additive operator><term> }
<term> ::= <atom1> { <multiplication operator><atom1> }
<atom1> ::= <atom> | <atom><power operator><atom> | <increment><atom>
<atom> ::= <name> | (<expr>) | <spec atom> | <name> {[<expr>]}
<spec atom> ::= <const> | <logical expr> | !<atom> | ~<atom>
<additive operator> ::= + | - | ||
<multiplication operator> ::= * | / | && | % | ^
<power operator> ::= **
<increment> ::= ++ | --
<sign> ::= ε | + | –
<if operator> ::= if (<expr>) <body> | if (<expr>) <body> else <body> | if (<expr>) <body> else <if operator>
<normal number> ::= <digit> | <normal number><digit> 
<const> ::= <sign><digit> {<digit>} | <digit> {<digit>}.<digit> {<digit>} | “{<letter>}”
<logical value> ::= true | false
<print operator> ::= print(<list of elements>)
<read operator> ::= read(<list of elements>)
<list of elements> ::= <list of elements> {,<element>}
<element> ::= <expr>
<description section> ::= <type section>
<type section> ::= <type><section> {; <type section>}
<section> ::= <name> = <assignment>
<assignment> ::= <name> | <const>
<type> ::= char | int32 | int64 | bool | float | double | <type> [<expr>]
<spec operator> ::= <while operator> | <for operator> | <if operator>
<while operator> ::= while (<expr>) { <оператор> }
<for operator> ::= for (<type section>; <expr>; <expr>) <body>
```

## Other

### Done
* Lexical Analyzer
* Syntax Analyzer
* Semantic Analyzer
