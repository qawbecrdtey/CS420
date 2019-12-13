#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <tao/pegtl.hpp>

//#include "ASCII.hpp"
//#include "syntax_tree_node.hpp"
#include "Parser/Grammar.hpp"
#include "Parser/Action.hpp"

inline std::string_view substr(std::string_view base, std::uint64_t pos = 0, std::uint64_t len = std::string_view::npos) { return base.substr(pos, len); }

int main(int argc, char* argv[]) {
	using namespace std::literals;
	/* FILE INPUT */

	std::string input_file_name = "../input.txt";
	std::string source;
	// The first element denotes the input file.
	// If no first element exists, then the default is "input.txt".
	if (argc < 2) {
		std::cout << "Using input file ../input.txt as default." << std::endl;
	}
	else {
		std::cout << "Using input file " << (input_file_name = argv[1]) << '.' << std::endl;
	}

	std::ifstream in_f(input_file_name);
	// If there is no input file with such name, exit the program.
	if (!in_f) {
		std::cerr << "Couldn't find file " << input_file_name << "!" << std::endl;
		return 1;
	}
	// Saves the input into source.
	in_f.seekg(0, std::ios::end);
	source.resize(in_f.tellg());
	in_f.seekg(0, std::ios::beg);
	in_f.read(&source[0], source.size());
	in_f.close();

	/* FILE INPUT END */

    std::cout << source<< std::endl;

	tao::pegtl::memory_input in(source, "");
	tao::pegtl::parse<Parser::grammar, Parser::action>(in);
/*
	std::cout << source << std::endl;
	std::cout << "Length: " << source.length() << std::endl;
	std::vector<std::unique_ptr<syntax_tree_node>> token_vector;
	for (std::uint64_t i = 0; i < source.size();) {
		std::cout << i << ' ' << source[i] << std::endl;
		switch (determine_char(source[i])) {
		case ASCII::Minus: i++;  continue;
		case ASCII::Null: goto A;
		case ASCII::Control: i++; continue;
		case ASCII::Whitespace: while(determine_char(source[i]) == ASCII::Whitespace) i++; continue;
		case ASCII::Printable:
		{
			using P = std::pair<char, char>;
			bool flag = true;
			if (i + 1 < source.size()) {
				P const p = { source[i], source[i + 1] };
				flag = false;
				if (p.second == '=') {
					switch (p.first) {
					case '+':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("+="sv, SYMBOL::plusequal)); i += 2;
						break;
					case '-':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("-="sv, SYMBOL::minusequal)); i += 2;
						break;
					case '*':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("*="sv, SYMBOL::starequal)); i += 2;
						break;
					case '/':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("/="sv, SYMBOL::slashequal)); i += 2;
						break;
					case '%':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("%="sv, SYMBOL::percentequal)); i += 2;
						break;
					case '=':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("=="sv, SYMBOL::equalequal)); i += 2;
						break;
					case '!':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("!="sv, SYMBOL::notequal)); i += 2;
						break;
					case '<':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>("<="sv, SYMBOL::lessequal)); i += 2;
						break;
					case '>':
						token_vector.emplace_back(std::make_unique<syntax_tree_node>(">="sv, SYMBOL::greaterequal)); i += 2;
						break;
					default: flag = true;
					}
				}
				else if (p == P{ '+', '+' }) {
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("++"sv, SYMBOL::plusplus));
					i += 2;
				}
				else if (p == P{ '-', '-' }) {
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("--"sv, SYMBOL::minusminus));
					i += 2;
				}
				else if (p == P{ '/', '/' }) {
					uint64_t x = 1;
					while (source[i + x] != '\n' && source[i + x] != '\0') x++;
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(substr(source, i, x + 1), SYMBOL::slashslash));
					i += x + 1;
				}
				else if (p == P{ '&', '&' }) {
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("&&"sv, SYMBOL::andand));
					i += 2;
				}
				else if (p == P{ '|', '|' }) {
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("||"sv, SYMBOL::oror));
					i += 2;
				}
				else flag = true;
			}
			if (flag) {
				switch (source[i]) {
				case '=':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("="sv, SYMBOL::equal)); i++;
					break;
				case '+':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("+"sv, SYMBOL::plus)); i++;
					break;
				case '-':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("-"sv, SYMBOL::minus)); i++;
					break;
				case '*':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("*"sv, SYMBOL::star)); i++;
					break;
				case '/':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("/"sv, SYMBOL::slash)); i++;
					break;
				case '%':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("%"sv, SYMBOL::percent)); i++;
					break;
				case 92: // \ //
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("\\"sv, SYMBOL::backslash)); i++;
					break;
				case '!':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("!"sv, SYMBOL::exclamation)); i++;
					break;
				case '?':
                    token_vector.emplace_back(std::make_unique<syntax_tree_node>("?"sv, SYMBOL::question)); i++;
                    break;
                case ':':
                    token_vector.emplace_back(std::make_unique<syntax_tree_node>(":"sv, SYMBOL::colon)); i++;
                    break;
				case '<':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("<"sv, SYMBOL::less)); i++;
					break;
				case '>':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(">"sv, SYMBOL::greater)); i++;
					break;
				case '(':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("("sv, SYMBOL::openparen)); i++;
					break;
				case ')':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(")"sv, SYMBOL::closeparen)); i++;
					break;
				case '{':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("{"sv, SYMBOL::opencurly)); i++;
					break;
				case '}':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("}"sv, SYMBOL::closecurly)); i++;
					break;
				case '[':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("["sv, SYMBOL::openbrack)); i++;
					break;
				case ']':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("]"sv, SYMBOL::closebrack)); i++;
					break;
				case '.':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>("."sv, SYMBOL::dot)); i++;
					break;
				case ',':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(","sv, SYMBOL::comma)); i++;
					break;
				case ';':
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(";"sv, SYMBOL::semicolon)); i++;
					break;
				case (char)34: { // " //
					uint64_t x = 1;
					while (i + x < source.size() && !(source[i + x] == (char)34 && source[i + x - 1] != (char)92)) x++;
					token_vector.emplace_back(std::make_unique<syntax_tree_node>(substr(source, i, x + 1), SYMBOL::string_literal));
					i += x + 1;
				}
					break;
				default:
					if (is_identifier_first(source[i])) {
						uint64_t x = 1;
						while (is_identifier_middle(source[i + x])) x++;
						auto const sv = substr(source, i, x);
                        if (sv == "break"sv) token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::break_keyword));
                        else if (sv == "continue"sv) token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::continue_keyword));
                        else if (sv == "else"sv) token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::else_keyword));
						else if (sv == "float"sv) token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::float_keyword));
                        else if (sv == "for"sv) token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::for_keyword));
                        else if (sv == "if"sv) token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::if_keyword));
						else if (sv == "int"sv) token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::int_keyword));
                        else if (sv == "return"sv) token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::return_keyword));
                        else if (sv == "void"sv) token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::void_keyword));
                        else if (sv == "while"sv) token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::while_keyword));
						else token_vector.emplace_back(std::make_unique<syntax_tree_node>(sv, SYMBOL::identifier));
						i += x;
					}
					else if (is_number(source[i])) {
						uint64_t x = 1;
						while (is_number(source[i + x])) x++;
						token_vector.emplace_back(std::make_unique<syntax_tree_node>(substr(source, i, x), SYMBOL::number));
						i += x;
					}
					else {
						std::cout << "Unexpected character " << source[i] << std::endl;
						throw std::exception();
					}
				}
			}
		}
			break;
		default:
			std::cout << "Unexpected character " << source[i] << std::endl;
			throw std::exception();
		}
	}

A:
	std::cout << "Well done!" << std::endl;
	
	for (auto&& cur : token_vector) {
		std::cout << cur->data << ": "; print_SYMBOL(cur->symbol);
	}

	/* TOKENIZING COMPLETE *

*/
}