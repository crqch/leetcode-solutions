#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Lexer {

public:
  string expression;
  int currentIndex = 0;

  enum TokenType {
    unknown,
    letter,
    lBra,
    rBra,
    comma,
  };

  struct Token {
    TokenType type;
    string lexeme;
  };

  vector<Token> tokens;

  Lexer(string expression) : expression(expression) {};

  vector<Token> scan() {
    while (!isAtEnd()) {
      scanToken();
    }

    return tokens;
  }

private:
  void scanToken() {
    char currentChar = expression[currentIndex];

    TokenType type = TokenType::unknown;
    switch (currentChar) {
    case '{':
      type = TokenType::lBra;
      break;
    case '}':
      type = TokenType::rBra;
      break;
    case ',':
      type = TokenType::comma;
      break;
    }

    if (currentChar >= 'a' && currentChar <= 'z') {
      type = TokenType::letter;
    }

    currentIndex++;

    if (type == TokenType::unknown) {
      return;
    }

    tokens.push_back(Token{
        .type = type,
        .lexeme = string(1, currentChar),
    });

    return;
  }

  bool isAtEnd() { return currentIndex >= expression.size(); }
};

enum AstType {
  term,
  uunion,
  product,
};

class ASTNode {
public:
  virtual ~ASTNode();

  virtual vector<string> evaluate() const { return {}; };
  virtual void print(int level) const {

  };
};

ASTNode::~ASTNode() = default;

class TermNode : public ASTNode {
  string letter;

public:
  explicit TermNode(string letter) : letter(letter) {};

  vector<string> evaluate() const override {
    vector<string> temp;
    temp.push_back(letter);
    return temp;
  }

  void print(int level) const override {
    for (int i = 0; i < level; i++) {
      cout << " ";
    }
    cout << "Term " << letter << "\n";
  }
};

vector<string> join(vector<string> a, vector<string> b) {
  for (int i = 0; i < b.size(); i++) {
    bool unique = true;
    for (int j = 0; j < a.size(); j++) {
      if (a[j] == b[i]) {
        unique = false;
        break;
      }
    }
    if (unique)
      a.push_back(b[i]);
  }

  return a;
}

class UnionNode : public ASTNode {
  vector<unique_ptr<ASTNode>> children;

public:
  explicit UnionNode(vector<unique_ptr<ASTNode>> children)
      : children(std::move(children)) {};

  vector<string> evaluate() const override {
    vector<string> temp;
    for (int i = 0; i < children.size(); i++) {
      temp = join(temp, children[i]->evaluate());
    }

    return temp;
  }

  void print(int level) const override {
    for (int i = 0; i < level; i++) {
      cout << " ";
    }
    cout << "Union " << "\n";
    for (int i = 0; i < children.size(); i++) {
      children[i]->print(level + 1);
    }
  }
};

class ProductNode : public ASTNode {
  unique_ptr<ASTNode> left;
  unique_ptr<ASTNode> right;

public:
  explicit ProductNode(unique_ptr<ASTNode> left, unique_ptr<ASTNode> right)
      : left(std::move(left)), right(std::move(right)) {};

  vector<string> evaluate() const override {
    vector<string> temp;

    vector<string> leftStrings = left->evaluate();
    vector<string> rightStrings = right->evaluate();

    for (int i = 0; i < leftStrings.size(); i++) {
      for (int j = 0; j < rightStrings.size(); j++) {
        temp.push_back(leftStrings[i] + rightStrings[j]);
      }
    }

    return temp;
  }

  void print(int level) const override {
    for (int i = 0; i < level; i++) {
      cout << " ";
    }
    cout << "Product " << "\n";
    left->print(level + 1);
    right->print(level + 1);
  }
};

class Parser {

  vector<Lexer::Token> tokens;
  int currentIndex = 0;

public:
  Parser(vector<Lexer::Token> tokens) : tokens(tokens) {};

  unique_ptr<ASTNode> parse() {
    unique_ptr<ASTNode> astNode = parseUnion();
    if (tokens.size() != currentIndex) {
      throw "expression not closed";
    }

    return astNode;
  }

private:
  unique_ptr<ASTNode> parseUnion() {
    vector<unique_ptr<ASTNode>> nodes;
    nodes.push_back(parseProduct());

    while (matchToken(Lexer::TokenType::comma)) {
      nodes.push_back(parseProduct());
    }

    if (nodes.size() == 1) {
      return std::move(nodes[0]);
    } else {
      return std::make_unique<UnionNode>(std::move(nodes));
    }
  }

  unique_ptr<ASTNode> parseProduct() {
    unique_ptr<ASTNode> left = parseTerm();
    while (!isAtEnd()) {
      Lexer::TokenType nextTokenType = tokens[currentIndex].type;
      if (nextTokenType == Lexer::TokenType::lBra ||
          nextTokenType == Lexer::TokenType::letter) {
        unique_ptr<ASTNode> right = parseTerm();
        left = std::make_unique<ProductNode>(std::move(left), std::move(right));
      } else {
        break;
      }
    }
    return std::move(left);
  }

  Lexer::Token previousToken() {
    if (currentIndex > 0)
      return tokens[currentIndex - 1];
    throw std::invalid_argument("attempted to access negative index on tokens");
  }

  unique_ptr<ASTNode> parseTerm() {
    if (matchToken(Lexer::TokenType::lBra)) {
      unique_ptr<ASTNode> node = parseUnion();
      if (!matchToken(Lexer::TokenType::rBra))
        throw std::invalid_argument("expected closing brace");
      return std::move(node);
    }
    if (matchToken(Lexer::TokenType::letter)) {
      return std::make_unique<TermNode>(previousToken().lexeme);
    }

    throw std::invalid_argument("term expected");
  }

  bool matchToken(Lexer::TokenType type) {
    if (!isAtEnd() && tokens[currentIndex].type == type) {
      currentIndex++;
      return true;
    };
    return false;
  };

  bool isAtEnd() { return currentIndex >= tokens.size(); }
};

vector<string> sortStrings(vector<string> strings) {
  for (int i = 0; i < strings.size(); i++) {
    for (int j = i; j < strings.size(); j++) {
      if (strings[i].compare(strings[j]) > 0) {
        string temp = strings[i];
        strings[i] = strings[j];
        strings[j] = temp;
      }
    }
  }
  return strings;
}

class Solution {
public:
  vector<string> braceExpansionII(string expression) {

    Lexer lexer = Lexer(expression);

    vector<Lexer::Token> tokens = lexer.scan();
    // for (int i = 0; i < tokens.size(); i++) {
    //   cout << tokens[i].type << " " << tokens[i].lexeme << "\n";
    // }

    Parser parser = Parser(tokens);
    unique_ptr<ASTNode> node = parser.parse();

    // node->print(0);

    vector<string> strings = node->evaluate();

    return sortStrings(strings);
  }

private:
};

int main(int argc, char **argv) {

  vector<string> strings = Solution().braceExpansionII(argv[1]);

  for (int i = 0; i < strings.size(); i++) {
    cout << "\"" << strings[i] << "\", ";
  }

  cout << "\n";
}
