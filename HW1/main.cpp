#include <algorithm>
#include <cassert>
#include <cctype>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#ifdef MODE_DEBUG
#define DEBUG(str) std::cout << str << std::endl
#else
#define DEBUG(str)
#endif

/*
    Before parsing, use only one whitespace instead of continued whitespace to regularize the input.
    
    This does not change the result.

    //////////// e is a null symbol ////////////

    <digit> ::= [0-9]                                   digit

    <digits> ::= <digit>D                               digits
    
    D ::= <digit>D | e                                  right recursion of digits

    <id> ::= [a-zA-Z]                                   identifier

    S ::= E                                             Start symbol

    F ::= <digits> | <id>                               factor

    T ::= FJ                                            term

    J ::= VT | e                                        right recursion of term

    V ::= <*> | </>                                     symbols

    E ::= TI                                            expression

    I ::= UE | e                                        right recursion of expression 
    
    U ::= <+> | <->                                     symbols
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

        virtual void rearrange() = 0;
    };

    std::ostream &operator<<(std::ostream &os, Node const &node) {
        os << node.data.substr(node.pos, node.length);
        return os;
    }

    std::ostream &operator<<(std::ostream &os, std::unique_ptr<Node> &&node) {
        os << node->data.substr(node->pos, node->length);
        return os;
    }

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
            return pos + 1;
        }

        void rearrange() {
DEBUG("digit");
            assert(is_leaf());
        }
    };

    struct D : Node {
        explicit D(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            if(cur_pos < data.length() && data[cur_pos] >= '0' && data[cur_pos] <= '9') {
                children.reserve(2);

                children.emplace_back(std::make_unique<digit>(data, cur_pos));
                cur_pos = children[0]->parse();

                children.emplace_back(std::make_unique<D>(data, cur_pos));
                cur_pos = children[1]->parse();
            }
            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("D");
            if(is_leaf()) return;
            assert(children.size() == 2);

            children[0]->rearrange();
            children[1]->rearrange();
            
            auto p1 = std::move(children[0]);
            auto p2 = std::move(children[1]);
            
            children.clear();
            children.reserve(p2->children.size() + 1);

            children.emplace_back(std::move(p1));
            if(!p2->is_leaf()) {
                for(std::size_t i = 0; i < p2->children.size(); i++) {
                    children.emplace_back(std::move(p2->children[i]));
                }
            }
        }
    };

    struct digits : Node {
        explicit digits(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(2);

            children.emplace_back(std::make_unique<digit>(data, cur_pos));
            cur_pos = children[0]->parse();

            children.emplace_back(std::make_unique<D>(data, cur_pos));
            cur_pos = children[1]->parse();

            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("digits");
            assert(children.size() == 2);

            children[0]->rearrange();
            children[1]->rearrange();

            auto p1 = std::move(children[0]);
            auto p2 = std::move(children[1]);
            
            children.clear();
            children.reserve(p2->children.size() + 1);

            children.emplace_back(std::move(p1));
            if(!p2->is_leaf()) {
                for(std::size_t i = 0; i < p2->children.size(); i++) {
                    children.emplace_back(std::move(p2->children[i]));
                }
            }
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
            return pos + 1;
        }

        void rearrange() {
DEBUG("identifier");
            assert(is_leaf());
        }
    };

    struct F : Node {
        explicit F(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            if(cur_pos >= data.length()) {
                std::stringstream ss;
                ss << "At position " << cur_pos << ": expected F, found EOL.";
                throw std::exception(ss.str().c_str());
            }
            if(data[cur_pos] >= '0' && data[cur_pos] <= '9') {
                children.reserve(1);

                children.emplace_back(std::make_unique<digits>(data, cur_pos));
                cur_pos = children[0]->parse();

                length = cur_pos - pos;
                return cur_pos;
            }
            if((data[pos] >= 'a' && data[pos] <= 'z') || (data[pos] >= 'A' && data[pos] <= 'Z')) {
                children.reserve(1);

                children.emplace_back(std::make_unique<identifier>(data, cur_pos));
                cur_pos = children[0]->parse();

                length = cur_pos - pos;
                return cur_pos;
            }
            std::stringstream ss;
            ss << "At position " << cur_pos << ": expected F, found " << data[cur_pos] << '.';
            throw std::exception(ss.str().c_str());
        }

        void rearrange() {
DEBUG("F");
            assert(children.size() == 1);

            children[0]->rearrange();
            
            auto p = std::move(children[0]);

            children.swap(p->children);
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
                ss << "At position " << pos << ": expected V, found " << data[pos] << '.' << std::endl;
                throw std::exception(ss.str().c_str());
            }
            return pos + 1;
        }

        void rearrange() {
DEBUG("V");
            assert(is_leaf());
        }
    };

    struct T;

    struct J : Node {
        explicit J(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            if(cur_pos > data.length()) {
                std::stringstream ss;
                ss << "At position " << cur_pos << ": expected J, found EOL.";
                throw std::exception(ss.str().c_str());
            }
            if(cur_pos < data.length() && (data[cur_pos] == '*' || data[cur_pos] == '/')) {
                children.reserve(2);

                children.emplace_back(std::make_unique<V>(data, cur_pos));
                cur_pos = children[0]->parse();

                children.emplace_back(std::make_unique<T>(data, cur_pos));
                cur_pos = children[1]->parse();
            }
            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("J");
            if(is_leaf()) return;

            children[0]->rearrange();
            children[1]->rearrange();

            auto p = std::move(children[1]->children[0]);
            
            children[1] = std::move(p);
        }
    };

    struct T : Node {
        explicit T(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(2);

            children.emplace_back(std::make_unique<F>(data, cur_pos));
            cur_pos = children[0]->parse();

            children.emplace_back(std::make_unique<J>(data, cur_pos));
            cur_pos = children[1]->parse();

            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("T");
            children[0]->rearrange(); // F
            children[1]->rearrange(); // J
            
            if(children[1]->is_leaf()) {
                auto p = std::move(children[0]);
                children.swap(p->children);
                return;
            }

            auto p1 = std::move(children[0]->children[0]); // T
            auto p2 = std::move(children[1]->children[0]); // V
            auto p3 = std::move(children[1]->children[1]); // T

            assert(p2->is_leaf());

            p2->children.reserve(2);
            p2->children.emplace_back(std::move(p1));
            p2->children.emplace_back(std::move(p3));
            
            children.clear();
            children.emplace_back(std::move(p2));
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
                ss << "At position " << pos << ": expected U, found " << data[pos] << '.' << std::endl;
                throw std::exception(ss.str().c_str());
            }
            return pos + 1;
        }

        void rearrange() {
DEBUG("U");
            assert(is_leaf());
        }
    };

    struct E;

    struct I : Node {
        explicit I(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            if(cur_pos > data.length()) {
                std::stringstream ss;
                ss << "At position " << cur_pos << ": expected I, found EOL.";
                throw std::exception(ss.str().c_str());
            }
            if(cur_pos < data.length() && (data[cur_pos] == '+' || data[cur_pos] == '-')) {
                children.reserve(2);

                children.emplace_back(std::make_unique<U>(data, cur_pos));
                cur_pos = children[0]->parse();

                children.emplace_back(std::make_unique<E>(data, cur_pos));
                cur_pos = children[1]->parse();
            }
            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("I");
            if(is_leaf()) return; // e

            children[0]->rearrange(); // U
            children[1]->rearrange(); // E

            auto p = std::move(children[1]->children[0]);
            
            children[1] = std::move(p);
        }
    };

    struct E : Node {
        explicit E(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(2);

            children.emplace_back(std::make_unique<T>(data, cur_pos));
            cur_pos = children[0]->parse();

            children.emplace_back(std::make_unique<I>(data, cur_pos));
            cur_pos = children[1]->parse();

            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("E");
            children[0]->rearrange(); // T
            children[1]->rearrange(); // I
            
            if(children[1]->is_leaf()) {
                auto p = std::move(children[0]);
                children.swap(p->children);
                return;
            }

            auto p1 = std::move(children[0]->children[0]); // E
            auto p2 = std::move(children[1]->children[0]); // U
            auto p3 = std::move(children[1]->children[1]); // E

            assert(p2->is_leaf());

            p2->children.reserve(2);
            p2->children.emplace_back(std::move(p1));
            p2->children.emplace_back(std::move(p3));
            
            children.clear();
            children.emplace_back(std::move(p2));
        }
    };

    struct S : Node {
        explicit S(std::string_view original, std::size_t pos = 0) : Node(original, pos, 0) {}
        
        std::size_t parse() {
            auto cur_pos = pos;
            children.reserve(1);

            children.emplace_back(std::make_unique<E>(data, cur_pos));
            cur_pos = children[0]->parse();

            length = cur_pos - pos;
            
            if(cur_pos != data.length()) {
                std::stringstream ss;
                ss << "At position " << cur_pos << ": expected EOL, found " << data[cur_pos] << '.' << std::endl;
                throw std::exception(ss.str().c_str());
            }

            return cur_pos;
        }

        void rearrange() {
DEBUG("S");
            children[0]->rearrange();

            auto p = std::move(children[0]);

            children.swap(p->children);
        }
    };
}

int main() {
    std::string buffer;
    std::ifstream in_file("input.txt");
    std::ofstream out_file("output.txt");
    std::size_t cnt = 0;
    while(std::getline(in_file, buffer)) {
        try {
            cnt++;
            std::cout << "Test case " << cnt << ": " << buffer << std::endl;
            std::cout << "Result: ";
            buffer.erase(std::remove_if(buffer.begin(), buffer.end(), ::isspace), buffer.end());
            NONAME_NAMESPACE::S s(buffer);
            s.parse();
            s.rearrange();
            std::cout << "PASS" << std::endl;
            buffer.clear();
        }
        catch(std::exception &e) {
            std::cout << e.what() << std::endl;
            buffer.clear();
        }
    }
}