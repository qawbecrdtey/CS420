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
#define DEBUG(str1, str2) std::cout << str1 << " : " << str2 << std::endl
#else
#define DEBUG(str1, str2)
#endif

/*
    Before parsing, use only one whitespace instead of continued whitespace to regularize the input.
    
    This does not change the result.

    //////////// e is a null symbol ////////////

    <whitespaces> ::= <whitespace>*                     whitespaces

    <digit> ::= [0-9]                                   digit

    <digits> ::= <digit>D                               digits
    
    D ::= <digit>D | e                                  right recursion of digits

    <id> ::= [a-zA-Z]                                   identifier

    S ::= <whitespaces>E<whitespaces>                   Start symbol
    
    E ::= T<whitespaces>J                               expression

    J ::= U<whitespaces>E | e                           right recursion of expression

    U ::= <+> | <->                                     symbols

    T ::= F<whitespaces>K                               term

    K ::= V<whitespaces>T | e                           right recursion of term

    V ::= <*> | </>                                     symbols

    F ::= <digits> | <id>                               factor
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

        void dfs(std::ofstream &ofs) {
            ofs << data.substr(pos, length) << '\n';
            for(auto &&p : children) {
                p->dfs(ofs);
            }
        }
    };

    std::ostream &operator<<(std::ostream &os, Node const &node) {
        os << node.data.substr(node.pos, node.length);
        return os;
    }

    std::ostream &operator<<(std::ostream &os, std::unique_ptr<Node> &&node) {
        os << node->data.substr(node->pos, node->length);
        return os;
    }

    struct whitespaces : Node { // <whitespaces>
        explicit whitespaces(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
DEBUG("parse", "whitespaces");
            auto cur_pos = pos;

            if(pos > data.length()) {
                std::stringstream ss;
                ss << "At position " << pos << ": expected <whitespace>, found EOL.";
                throw std::exception(ss.str().c_str());
            }
            while(cur_pos < data.length() && (data[cur_pos] < 33 || data[cur_pos] == 127)) cur_pos++;

            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("rearrange", "whitespaces");
        }
    };

    struct digit : Node { // <digit>
        explicit digit(std::string_view original, std::size_t pos) : Node(original, pos, 1) {}

        std::size_t parse() {
DEBUG("parse", "digit");
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
DEBUG("rearrange", "digit");
        }
    };

    struct D : Node { // D ::= <digit>D | e
        explicit D(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
DEBUG("parse", "D");
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
DEBUG("rearrange", "D");
            if(is_leaf()) return;

            std::vector<std::unique_ptr<Node> > temp;
            temp.reserve(children.size());

            for(auto &&p : children) {
                p->rearrange();
                temp.emplace_back(std::move(p));
            }

            children.clear();
            children.reserve(1);

            children.emplace_back(std::move(temp[0]));
            for(auto &&p : temp[1]->children) {
                children.emplace_back(std::move(p));
            }
        }
    };

    struct digits : Node { // <digits> ::= <digit>D
        explicit digits(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
DEBUG("parse", "digits");
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
DEBUG("rearrange", "digits");

            std::vector<std::unique_ptr<Node> > temp;
            temp.reserve(children.size());

            for(auto &&p : children) {
                p->rearrange();
                temp.emplace_back(std::move(p));
            }

            children.clear();
            children.reserve(1);

            children.emplace_back(std::move(temp[0]));
            for(auto &&p : temp[1]->children) {
                children.emplace_back(std::move(p));
            }
        }
    };

    struct identifier : Node { // <identifier>
        explicit identifier(std::string_view original, std::size_t pos) : Node(original, pos, 1) {}

        std::size_t parse() {
DEBUG("parse", "identifier");
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
DEBUG("rearrange", "identifier");
        }
    };

    struct F : Node { // F ::= <digits> | <id>
        explicit F(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
DEBUG("parse", "F");
            auto cur_pos = pos;

    /// or<digits, identifier>

        /// digits
            if(data[cur_pos] >= '0' && data[cur_pos] <= '9') {
                children.reserve(1);
                children.emplace_back(std::make_unique<digits>(data, cur_pos));
                cur_pos = children[0]->parse();
            }
        /// identifier
            else if((data[cur_pos] >= 'a' && data[cur_pos] <= 'z') || (data[cur_pos] >= 'A' && data[cur_pos] <= 'Z')) {
                children.reserve(1);
                children.emplace_back(std::make_unique<identifier>(data, cur_pos));
                cur_pos = children[0]->parse();
            }
        /// or<> == false
            else {
                std::stringstream ss;
                ss << "At position " << pos << ": expected F, found EOL.";
                throw std::exception(ss.str().c_str());
            }
        
            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("rearrange", "F");
            if(is_leaf()) return;

            std::vector<std::unique_ptr<Node> > temp;
            temp.reserve(children.size());

            for(auto &&p : children) {
                p->rearrange();
                temp.emplace_back(std::move(p));
            }

            children.clear();
            children.reserve(1);

            children.emplace_back(std::move(temp[0]));
        }
    };

    struct V : Node { // V ::= <*> | </>
        explicit V(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
DEBUG("parse", "V");
            auto cur_pos = pos;

    /// or<'*', '/'>
    /// one<'*', '/'>
            if(data[cur_pos] == '*' || data[cur_pos] == '/') {
                cur_pos++;
            }
        /// or<> == false
            else {
                std::stringstream ss;
                ss << "At position " << pos << ": expected V, found " << data[cur_pos] << '.' << std::endl;
                throw std::exception(ss.str().c_str());
            }

            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("rearragne", "V");
        }
    };

    struct T;

    struct K : Node { // K ::= V<whitespaces>T | e
        explicit K(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
DEBUG("parse", "K");
            auto cur_pos = pos;

    /// or<seq<V, whitespaces, T>, e>
    
        /// seq<V, whitespaces, T>
            if(cur_pos < data.length() && (data[cur_pos] == '*' || data[cur_pos] == '/')) {
                children.reserve(3);
            /// V
                children.emplace_back(std::make_unique<V>(data, cur_pos));
                cur_pos = children[0]->parse();
            /// whitespaces
                children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
                cur_pos = children[1]->parse();
            /// T
                children.emplace_back(std::make_unique<T>(data, cur_pos));
                cur_pos = children[2]->parse();    
            }
        /// e
            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("rearrange", "K");
            if(is_leaf()) return;

            std::vector<std::unique_ptr<Node> > temp;
            temp.reserve(children.size());

            for(auto &&p : children) {
                p->rearrange();
                temp.emplace_back(std::move(p));
            }

            children.clear();
            children.reserve(2);

            children.emplace_back(std::move(temp[0]));
            children.emplace_back(std::move(temp[2]));
        }
    };

    struct T : Node { // T ::= F<whitespaces>K
        explicit T(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
DEBUG("parse", "T");
            auto cur_pos = pos;

    /// seq<F, whitespaces, K>
            children.reserve(3);
        /// F
            children.emplace_back(std::make_unique<F>(data, cur_pos));
            cur_pos = children[0]->parse();
        /// whitespaces
            children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
            cur_pos = children[1]->parse();
        /// K
            children.emplace_back(std::make_unique<K>(data, cur_pos));
            cur_pos = children[2]->parse();

            length = cur_pos - pos;

            return cur_pos;
        }

        void rearrange() {
DEBUG("rearrange", "T");
            std::vector<std::unique_ptr<Node> > temp;
            temp.reserve(children.size());

            for(auto &&p : children) {
                p->rearrange();
                temp.emplace_back(std::move(p));
            }

            children.clear();

            if(temp[2]->is_leaf()) {
                children.reserve(1);
                children.emplace_back(std::move(temp[0]));
            }
            else {
                children.reserve(1);
                children.emplace_back(std::move(temp[2]->children[0]));
                children[0]->children.emplace_back(std::move(temp[0]));
                children[0]->children.emplace_back(std::move(temp[2]->children[1]));
            }
        }
    };

    struct U : Node { // U ::= <+> | <->
        explicit U(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
DEBUG("parse", "U");
            auto cur_pos = pos;

    /// or<'+', '-'>
    /// one<'+', '-'>
            if(data[cur_pos] == '+' || data[cur_pos] == '-') {
                cur_pos++;
            }
        /// or<> == false
            else {
                std::stringstream ss;
                ss << "At position " << pos << ": expected U, found " << data[cur_pos] << '.' << std::endl;
                throw std::exception(ss.str().c_str());
            }

            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("parse", "U");
        }
    };

    struct E;

    struct J : Node { // J ::= U<whitespaces>E | e
        explicit J(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
DEBUG("parse", "J");
            auto cur_pos = pos;

    /// or<seq<U, whitespace, E>, e>
    
        /// seq<U, whitespaces, E>
            if(cur_pos < data.length() && (data[cur_pos] == '+' || data[cur_pos] == '-')) {
                children.reserve(3);
            /// U
                children.emplace_back(std::make_unique<U>(data, cur_pos));
                cur_pos = children[0]->parse();
            /// whitespaces
                children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
                cur_pos = children[1]->parse();
            /// E
                children.emplace_back(std::make_unique<E>(data, cur_pos));
                cur_pos = children[2]->parse();    
            }
        /// e
            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("rearrange", "J");
            if(is_leaf()) return;

            std::vector<std::unique_ptr<Node> > temp;
            temp.reserve(children.size());

            for(auto &&p : children) {
                p->rearrange();
                temp.emplace_back(std::move(p));
            }

            children.clear();
            children.reserve(2);

            children.emplace_back(std::move(temp[0]));
            children.emplace_back(std::move(temp[2]));
        }
    };

    struct E : Node { // E ::= T<whitespaces>J
        explicit E(std::string_view original, std::size_t pos) : Node(original, pos, 0) {}

        std::size_t parse() {
DEBUG("parse", "E");
            auto cur_pos = pos;

    /// seq<T, whitespaces, J>
            children.reserve(3);
        /// T
            children.emplace_back(std::make_unique<T>(data, cur_pos));
            cur_pos = children[0]->parse();
        /// whitespaces
            children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
            cur_pos = children[1]->parse();
        /// J
            children.emplace_back(std::make_unique<J>(data, cur_pos));
            cur_pos = children[2]->parse();

            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("rearrange", "E");
            std::vector<std::unique_ptr<Node> > temp;
            temp.reserve(children.size());

            for(auto &&p : children) {
                p->rearrange();
                temp.emplace_back(std::move(p));
            }

            children.clear();

            if(temp[2]->is_leaf()) {
                children.reserve(1);
                children.emplace_back(std::move(temp[0]));
            }
            else {
                children.reserve(1);
                children.emplace_back(std::move(temp[2]->children[0]));
                children[0]->children.emplace_back(std::move(temp[0]));
                children[0]->children.emplace_back(std::move(temp[2]->children[1]));
            }
        }
    };

    struct S : Node { // S ::= <whitespaces>E<whitespaces>
        explicit S(std::string_view original, std::size_t pos = 0) : Node(original, pos, 0) {}
        
        std::size_t parse() {
DEBUG("parse", "S");
            auto cur_pos = pos;

    /// must<whitespaces, E, whitespaces, EOL>
            children.reserve(3);

        /// whitespaces
            children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
            cur_pos = children[0]->parse();
        /// E
            children.emplace_back(std::make_unique<E>(data, cur_pos));
            cur_pos = children[1]->parse();
        /// whitespaces
            children.emplace_back(std::make_unique<whitespaces>(data, cur_pos));
            cur_pos = children[2]->parse();
            
        /// must<EOL>
            if(cur_pos != data.length()) {
                std::stringstream ss;
                ss << "At position " << cur_pos << ": expected EOL, found " << data[cur_pos] << '.' << std::endl;
                throw std::exception(ss.str().c_str());
            }

            length = cur_pos - pos;
            return cur_pos;
        }

        void rearrange() {
DEBUG("rearrange", "S");
            std::vector<std::unique_ptr<Node> > temp;
            temp.reserve(children.size());

            for(auto &&p : children) {
                p->rearrange();
                temp.emplace_back(std::move(p));
            }

            children.clear();
            children.reserve(1);

            children.emplace_back(std::move(temp[1]));
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
            // buffer.erase(std::remove_if(buffer.begin(), buffer.end(), ::isspace), buffer.end());
            NONAME_NAMESPACE::S s(buffer);
            s.parse();
            s.rearrange();
            s.dfs(out_file);
            std::cout << "PASS" << std::endl;
            buffer.clear();
        }
        catch(std::exception &e) {
            std::cout << e.what() << std::endl;
            out_file << "incorrect syntax\n";
            buffer.clear();
        }
    }
}