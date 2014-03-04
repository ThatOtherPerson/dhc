#include "lexer/lexer.hpp"
#include <iostream>
#include <fstream>

#include <sstream>
#include <unicode/unistr.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Hello world, you usage-ignoring meanie!" << std::endl;
        return -1;
    }

    std::ifstream file (argv[1], std::ios::in | std::ios::binary | std::ios::ate);
    std::string utf8;

    if (!file.is_open()) {
        std::cerr << "I can't open that file. I hate you too." << std::endl;
        return -1;
    }

    utf8.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    utf8.assign((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());

    file.close();

    UErrorCode e = U_ZERO_ERROR;
    UChar utf16[512];

    u_strFromUTF8(utf16, sizeof(utf16), NULL, utf8.c_str(), -1, &e);
    if (U_FAILURE(e))
    {
        std::cerr << "error " << u_errorName(e) << std::endl;
    }

    icu::UnicodeString source(utf16);

    dhc::lexer::lexer lex(source);

    std::stringstream ss;
    ss  << "<html>\n<head>\n<title>" << argv[1] << "</title>\n"
        << "<style type=\"text/css\">\n"
        << "body { background-color: #000000 }\n"
        << ".whitespace { color: #aaaaaa }\n"
        << ".qvarid { font-weight: bolder; color: #ffff00 }\n"
        << ".qconid { color: #00ffff }\n"
        << ".qvarsym { color: #ff00ff }\n"
        << ".qconsym { color: #ab0010 }\n"
        << ".literal { color: #ffffff }\n"
        << ".special { color: #ff0000 }\n"
        << ".reservedop { color: #00ab10 }\n"
        << ".reservedid { color: #00ff00 }\n"
        << "</style>" << std::endl << "<body>\n"
        << "<pre>" << std::endl;

    while (!lex.finished()) {
        dhc::lexer::match_ptr token (lex.next());

        if (token) {
            std::string flat;
            token->flatten().toUTF8String(flat);
            ss << "<span class=\"" << lex.typenames[token->type] << "\">" << flat << "</span>";
        } else {
            std::cerr << lex.error(argv[1]) << std::endl;
            return -1;
        }
    }

    ss << "</pre></body></html>" << std::endl;

    std::cout << ss.str();

    return 0;
}
