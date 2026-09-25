# 1096 Brace Expansion II

https://leetcode.com/problems/brace-expansion-ii/

Upon seeing this problem I immediately brought back the good memories of writing the [Zulu Compiler](https://github.com/crqch/zulu), which was written in Zig. I wrote the basic Lexer, Pratt Parser and bottom-up evaluator with Type Checking. It served as a sandbox to learn a good portion of the Zig language.

> Sidenote: I will definitely come back to writing own compilers, I have some ideas I would like to touch upon.

## The idea

Referencing the interpreter I've written I thought about writing an AST representation for the language, a lexer, parser, and evaluator. This is far fetched from an optimal solution, but it will do its job perfectly.

### Token types

We have 4 token types in our grammar:

- letter `a-z`,
- left bracket `{`,
- right bracket `}`,
- comma `,`.

Token struct will naturally hold the token type and its lexeme (we will use it only with letter token type).

### AST Nodes

We have 3 AST Nodes in our evaluator:

- Term – a single letter eg. `a`, holding lexeme,
- Union – eg. `a,b,s`, holding list of AST Nodes,
- Product – eg. `ab` or `s{ba}e`, holding left and right AST Node child.

### Lexer

Based on the problems criteria, we assume that the input is always correct and fits the productions of the grammar. Thus, we can make some helpful adjustments, like, skipping any other character that is not recognized instead of throwing an error (this may be helpful during debugging to visually separate the tokens with spaces).

The lexer is as simple as it can get. A basic switch conditional statement to manage the tokens, then we put the lexeme in a new struct.

### Parser

The parser is where the half of our problem hides. We need to correctly prioritize bindings of operations in our language. The curly braces serve as a group. The terms (letters) have the biggest precedence, then comes the conjuction (the product), finally ending with disjunction (the union).

We naturally steer our list of tokens through the recursive functions, starting with nodes with least precedence, so:

Union –> Product —> Term

### Evaluator

We finally get our AST version of the expression, and all we need to do is to evaluate the nodes to lists of strings from the bottom going up.

- Term – evaluates to singleton of lexeme
- Union – evaluates to a flatted list of evaluations of children
- Product – evaluates to a list of all concatenated string permutations of elements in left and right nodes evaluated string lists.

### Cleanup

After we evaluate our root node, we sort the list and return it (as the unique filter is done within the union's node logic [`join` function], which is the only logical path for duplicates to appear).
