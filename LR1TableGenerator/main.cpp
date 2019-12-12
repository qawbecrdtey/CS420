#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
/*

input format given as:

nonterminal ::= symbol ... symbol | symbol ... symbol | ... | symbol ... symbol >.<
...
nonterminal ::= ... >.<

start_symbol >.<

WARNING : The input file should be LF, not CRLF.

*/

struct product {
	std::string from;
	std::vector<std::string> to;

	product(std::string from) : from(from) {}
};

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

	std::ifstream in(input_file_name);
	// If there is no input file with such name, exit the program.
	if (!in) {
		std::cerr << "Couldn't find file " << input_file_name << "!" << std::endl;
		return 1;
	}
	// Saves the input into source.
	in.seekg(0, std::ios::end);
	source.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&source[0], source.size());
	in.close();

	/* FILE INPUT END */

	std::cout << source << std::endl;

	std::stringstream ss;
	ss << source;
	std::string start_symbol;
	std::vector<product> product_vector;
    while(true) {
		std::string nonterm, sym;
		ss >> nonterm >> sym;
		std::cout << nonterm << ' ' << sym << std::endl;
		if (sym == ">.<") {
			start_symbol = nonterm;
			std::cout << "HI" << std::endl;
			break;
		}
		else if (sym != "::=") {
			return 1;
		}
		std::string const from = nonterm;
		std::vector<std::string> v;
		while (true) {
			ss >> nonterm;
			if (nonterm == "|") {
				product prod(from);
				std::swap(prod.to, v);
				product_vector.emplace_back(prod);
				continue;
			}
			else if (nonterm == ">.<") {
				product prod(from);
				std::swap(prod.to, v);
				product_vector.emplace_back(prod);
				break;
			}
			v.emplace_back(nonterm);
		}
    }

	std::cout << "Number of productions : " << product_vector.size() << std::endl;
	for (auto& now : product_vector) {
		std::cout << now.from << " ::= ";
		for (auto& cur : now.to) {
			std::cout << cur << ' ';
		}
		std::cout << std::endl;
	}
}