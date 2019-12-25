#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

#include "Parser/Grammar.hpp"
//#include "Parser/Action.hpp"
#include "Parser/Error.hpp"
#include "Parser/Node.hpp"
#include "Interpreter.hpp"

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

	std::cout << source << std::endl;
	tao::pegtl::memory_input in(source, "");
	try {
		//Parser::Identifier_map_stack ims;
		//Parser::Identifier_storage_vector isv;
		auto const root = tao::pegtl::parse_tree::parse<Parser::grammar, Parser::node, Parser::selector, tao::pegtl::nothing, Parser::control>(in);
		if (root) {
			tao::pegtl::parse_tree::print_dot(std::cout, *root);
			root->root_dfs();
		}
		
		//auto b = tao::pegtl::parse<Parser::grammar, Parser::action>(in, ims, isv);
		//std::cout << (b ? "true" : "false") << std::endl;
	}
	catch (tao::pegtl::parse_error& e) {
		const auto p = e.positions.front();
		std::cout << e.what() << std::endl << in.line_at(p) << std::endl;
		std::cout << std::string(p.byte_in_line, ' ') << '^' << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}