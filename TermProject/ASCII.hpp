#ifndef ASCII_hpp
#define ASCII_hpp

// Minus : unused, so ignore unless needed.
// Control : unused, so ignore unless needed.
// Whitespace : 0x20(space), 0x0A(newline) and 0x09(tab)
enum class ASCII { Minus, Null, Control, Whitespace, Printable };

ASCII determine_char(char c) {
	if(c < 0) return ASCII::Minus;
	if(c == 0) return ASCII::Null;
	if(c == 0x09 || c == 0x0A || c == 0x20) return ASCII::Whitespace;
	if(c < 0x20) return ASCII::Control;
	return ASCII::Printable;
}

bool is_identifier_first(char c) { return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_'; }
bool is_number(char c) { return '0' <= c && c <= '9'; }
bool is_identifier_middle(char c) { return is_identifier_first(c) || is_number(c); }

#endif