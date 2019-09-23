#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

/* 
    //////////// e is a null symbol ////////////

    <digit> ::= [0-9]                                   digit

    <digits> ::= <digit>D                               digits
    
    D ::= <digit>D | e                                  right recursion of digits

    <id> ::= [a-zA-Z]                                   identifier

    <whitespaces> ::= <whitespace><whitespaces> | e     whitespaces

    S ::= <whitespaces>E<whitespaces>                   Start symbol

    F ::= <digits>                                      factor
        | <id>

    T ::= FI                                            term

    I ::= N | e                                         right recursion of term 1

    J ::= V<whitespaces>J                               right recursion of term 2

    N ::= <whitespace><whitespaces>J                    right recursion of term 3
        | J

    V ::= <*> | </>

    E ::= TK                                            expression

    K ::= M | e                                         right recursion of expression 1

    L ::= U<whitespaces>M                               right recursion of expression 2
        
    M ::= <whitespace><whitespaces>L                    right recursion of expression 3
        | L

    U ::= <+> | <->
*/

namespace NONAME_NAMESPACE {

    struct Node {
        std::vector<std::unique_ptr<Node> > children;
        std::string_view data;
        std::size_t pos;
        std::size_t length;

        explicit Node(std::string_view original, std::size_t pos = 0, std::size_t length = 0) : data(original), pos(pos), length(length) {}

        bool is_leaf() const noexcept { return children.empty(); }

        virtual std::size_t parse() = 0;
    };

    struct whitespace : Node {
        explicit whitespace(std::string_view original, std::size_t pos) : Node(original, pos, 1) {}

        std::size_t parse() {
            if(pos >= data.length()) {
                std::stringstream ss;
                ss << "At position " << pos << ": expected <whitespace>, found EOL.";
                throw std::exception(ss.str().c_str());
            }
            if(data[pos] > 32 && data[pos] != 127) {
                std::stringstream ss;
                ss << "At position " << pos << ": expected <whitespace>, found " << data[pos] << '.';
                throw std::exception(ss.str().c_str());
            }
std::cout << "whitespace -> " << data[pos] << std::endl;
            return pos + 1;
        }
    };

    struct whitespaces : Node {
        explicit whitespaces(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            if(cur_pos < data.length() && (data[cur_pos] < 33 || data[cur_pos] == 127)) {
                children.reserve(2);
                children.emplace_back(std::make_unique<whitespace>(data, cur_pos));
std::cout << "whitespaces -> whitespace" << std::endl;
                cur_pos = children[0]->parse();
                children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
std::cout << "whitespaces -> whitespaces" << std::endl;
                cur_pos = children[1]->parse();
            }
else std::cout << "whitespaces -> e" << std::endl;
            length = cur_pos - pos;
            return cur_pos;
        }
    };

    struct digit : Node {
        explicit digit(std::string_view original, std::size_t pos) : Node(original, pos, 1) {}

        std::size_t parse() {
            if(pos >= data.length()) {
                std::stringstream ss;
                ss << "At position " << pos << ": expected <digit>, found EOL.";
                throw std::exception(ss.str().c_str());
            }
            if(data[pos] < '0' || data[pos] > '9') {
                std::stringstream ss;
                ss << "At position " << pos << ": expected <digit>, found " << data[pos] << '.';
                throw std::exception(ss.str().c_str());
            }
std::cout << "digit -> " << data[pos] << std::endl;
            return pos + 1;
        }
    };

    struct D : Node {
        explicit D(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            if(cur_pos < data.length() && data[cur_pos] >= '0' && data[cur_pos] <= '9') {
                children.reserve(2);
                children.emplace_back(std::make_unique<digit>(data, cur_pos));
std::cout << "D -> digit" << std::endl;
                cur_pos = children[0]->parse();
                children.emplace_back(std::make_unique<D>(data, cur_pos));
std::cout << "D -> D" << std::endl;
                cur_pos = children[1]->parse();
            }
else std::cout << "D -> e" << std::endl;
            length = cur_pos - pos;
            return cur_pos;
        }
    };

    struct digits : Node {
        explicit digits(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(2);
            children.emplace_back(std::make_unique<digit>(data, cur_pos));
std::cout << "digits -> digit" << std::endl;
            cur_pos = children[0]->parse();
            children.emplace_back(std::make_unique<D>(data, cur_pos));
std::cout << "digits -> D" << std::endl;
            cur_pos = children[1]->parse();
            length = cur_pos - pos;
            return cur_pos;
        }
    };

    struct identifier : Node {
        explicit identifier(std::string_view original, std::size_t pos) : Node(original, pos, 1) {}

        std::size_t parse() {
            if(pos >= data.length()) {
                std::stringstream ss;
                ss << "At position " << pos << ": expected <identifier>, found EOL.";
                throw std::exception(ss.str().c_str());
            }
            if((data[pos] < 'a' || data[pos] > 'z') && (data[pos] < 'A' || data[pos] > 'Z')) {
                std::stringstream ss;
                ss << "At position " << pos << ": expected <identifier>, found " << data[pos] << '.';
                throw std::exception(ss.str().c_str());
            }
std::cout << "identifier -> " << data[pos] << std::endl;
            return pos + 1;
        }
    };

    struct factor : Node {
        explicit factor(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(1);
            if(cur_pos < data.length()) {
                if(data[cur_pos] >= '0' && data[cur_pos] <= '9') {
                    children.emplace_back(std::make_unique<digits>(data, cur_pos));
std::cout << "factor -> digits" << std::endl;
                }
                else {
                    children.emplace_back(std::make_unique<identifier>(data, cur_pos));
std::cout << "factor -> identifier" << std::endl;
                }
                cur_pos = children[0]->parse();
                length = cur_pos - pos;
                return cur_pos;
            }
            else {
                std::stringstream ss;
                ss << "At position " << pos << ": expected <factor>, found EOL.";
                throw std::exception(ss.str().c_str());
            }
        }
    };

    struct V : Node {
        explicit V(std::string_view original, std::size_t pos) : Node(original, pos, 1) {}

        std::size_t parse() {
            if(pos >= data.length()) {
                std::stringstream ss;
                ss << "At position " << pos << ": expected V, found EOL.";
                throw std::exception(ss.str().c_str());
            }
            if(data[pos] != '*' && data[pos] != '/') {
                std::stringstream ss;
                ss << "At position " << pos << ": expected V, found " << data[pos] << '.';
                throw std::exception(ss.str().c_str());
            }
std::cout << "V -> " << data[pos] << std::endl;
            return pos + 1;
        }
    };

	struct I;

    struct J : Node {
        explicit J(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(3);
            children.emplace_back(std::make_unique<V>(data, cur_pos));
std::cout << "J -> V" << std::endl;
            cur_pos = children[0]->parse();
            children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
std::cout << "J -> whitespaces" << std::endl;
            cur_pos = children[1]->parse();
            children.emplace_back(std::make_unique<I>(data, cur_pos));
std::cout << "J -> I" << std::endl;
            cur_pos = children[2]->parse();
            length = cur_pos - pos;
            return cur_pos;
        }
    };

    struct I : Node {
        explicit I(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            if(cur_pos < data.length() && ) {
                
            }
        }
    };

    struct term : Node {
        explicit term(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(2);
            children.emplace_back(std::make_unique<factor>(data, cur_pos));
std::cout << "term -> factor" << std::endl;
            cur_pos = children[0]->parse();
            children.emplace_back(std::make_unique<I>(data, cur_pos));
std::cout << "term -> I" << std::endl;
            cur_pos = children[1]->parse();
            length = cur_pos - pos;
            return cur_pos;
        }
    };

    struct U : Node {
        explicit U(std::string_view original, std::size_t pos) : Node(original, pos, 1) {}

        std::size_t parse() {
            if(pos >= data.length()) {
                std::stringstream ss;
                ss << "At position " << pos << ": expected U, found EOL.";
                throw std::exception(ss.str().c_str());
            }
            if(data[pos] != '+' && data[pos] != '-') {
                std::stringstream ss;
                ss << "At position " << pos << ": expected U, found " << data[pos] << '.';
                throw std::exception(ss.str().c_str());
            }
std::cout << "U -> " << data[pos] << std::endl;
            return pos + 1;
        }
    };

	struct K;

    struct L : Node {
        explicit L(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(3);
            children.emplace_back(std::make_unique<U>(data, cur_pos));
std::cout << "L -> U" << std::endl;
            cur_pos = children[0]->parse();
            children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
std::cout << "L -> whitespaces" << std::endl;
            cur_pos = children[1]->parse();
            children.emplace_back(std::make_unique<K>(data, cur_pos));
std::cout << "L -> K" << std::endl;
            cur_pos = children[2]->parse();
            length = cur_pos - pos;
            return cur_pos;
        }
    };

    struct K : Node {
        explicit K(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
std::cout << cur_pos << std::endl;
std::cout <<data.length() << std::endl;
            if(cur_pos < data.length()) {
                if(data[cur_pos] < 33 || data[cur_pos] == 127) {
                    children.reserve(3);
                    children.emplace_back(std::make_unique<whitespace>(data, cur_pos));
std::cout << "K -> whitespace" << std::endl;
                    cur_pos = children[0]->parse();
                    children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
std::cout << "K -> whitespaces" << std::endl;
                    cur_pos = children[1]->parse();
                    children.emplace_back(std::make_unique<L>(data, cur_pos));
std::cout << "K -> L" << std::endl;
                    cur_pos = children[2]->parse();
                }
                else if(data[cur_pos] == '+' || data[cur_pos] == '-') {
                    children.reserve(1);
                    children.emplace_back(std::make_unique<L>(data, cur_pos));
std::cout << "K -> L" << std::endl;
                    cur_pos = children[0]->parse();
                }
else std::cout << "K -> e" << std::endl;
            }
else std::cout << "K -> e" << std::endl;
            length = cur_pos - pos;
            return cur_pos;
        }
    };

    struct expression : Node {
        explicit expression(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(2);
            children.emplace_back(std::make_unique<term>(data, cur_pos));
std::cout << "expression -> term" << std::endl;
            cur_pos = children[0]->parse();
            children.emplace_back(std::make_unique<K>(data, cur_pos));
std::cout << "expression -> K" << std::endl;
            cur_pos = children[1]->parse();
            length = cur_pos - pos;
            return cur_pos;
        }
    };

    struct S : Node {
        explicit S(std::string_view original, std::size_t pos = 0) : Node(original, pos, 0) {}
        
        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(3);
            children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
std::cout << "S -> whitespaces" << std::endl;
            cur_pos = children[0]->parse();
            children.emplace_back(std::make_unique<expression>(data, cur_pos));
std::cout << "S -> expression" << std::endl;
            cur_pos = children[1]->parse();
            children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
std::cout << "S -> whitespaces" << std::endl;
            cur_pos = children[2]->parse();
            length = cur_pos - pos;
            return cur_pos;
        }
    };
}

int main() {
    std::string buffer;
    while(std::getline(std::cin, buffer)) {
        try {
            NONAME_NAMESPACE::S s(buffer);
            s.parse();
            buffer.clear();
        }
        catch(std::exception &e) {
            std::cout << "HELLOTHERE" << std::endl;
            std::cout << e.what() << std::endl;
            buffer.clear();
        }
    }
}