#include "parser/parser.hpp"
#include "lexer/layout.hpp"
#include <iostream>
#include <fstream>

#include <sstream>

#include <unicode/unistr.h>
#include <unicode/ucsdet.h>

void print_indent(int indent)
{
    for (int i = 0; i < indent; i++)
    {
        std::cout << ' ';
    }
    std::cout << "* ";
}

void print_tree(std::shared_ptr<dhc::graft::match::match> &root, int indent)
{
    if (root->type == static_cast<int>(dhc::lexer::type::WHITESPACE))
        return;

    auto tree = root->children();
    print_indent(indent);
    std::string flat;
    root->flatten().toUTF8String(flat);
    std::cout << '"' << flat << "\"" << std::endl;
    for (auto it = tree.begin(); it != tree.end(); ++it)
    {
        std::vector<std::shared_ptr<dhc::graft::match::match>> c = (*it)->children();
        if (c.size() == 0)
        {
            print_indent(indent + 4);
            std::string flat;
            (*it)->flatten().toUTF8String(flat);

            size_t pos = 0;
            while ((pos = flat.find("\n", pos)) != std::string::npos)
            {
                flat.replace(pos, 1, "\\n");
                pos += 2;
            }

            std::cout << '"' << flat << "\"" << ' ' << std::endl;
        }
        else
        {
            print_tree((*it), indent + 4);
        }
    }
}

int main(int argc, char** argv)
{
    UErrorCode e = U_ZERO_ERROR;
    std::string filename = argc > 1 ? argv[1] : "main.hs";

    std::ifstream file (filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "I can't open that file. I hate you too." << std::endl;
        return 1;
    }

    std::string utf8;
    utf8.reserve(file.tellg());

    file.seekg(0, std::ios::beg);
    utf8.assign((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

    file.close();

    UCharsetDetector *ucd = ucsdet_open(&e);

    ucsdet_setDeclaredEncoding(ucd, "UTF-8", -1, &e);
    ucsdet_setText(ucd, utf8.c_str(), utf8.size(), &e);
    const UCharsetMatch *ucm = ucsdet_detect(ucd, &e);
    if (U_FAILURE(e))
    {
        std::cerr << "Charset detection error: " << u_errorName(e) << std::endl;
        return e;
    }

    std::cout << "Charset detected: " << ucsdet_getName(ucm, &e) << " confidence: " << ucsdet_getConfidence(ucm, &e) << std::endl;
    if (U_FAILURE(e))
    {
        std::cerr << "Charset detection error: " << u_errorName(e) << std::endl;
        return e;
    }

    UChar *buf = new UChar[utf8.size()];

    int32_t out = ucsdet_getUChars(ucm, buf, utf8.size(), &e);
    if (U_FAILURE(e))
    {
        std::cerr << "Charset conversion error: " << u_errorName(e) << std::endl;
        return e;
    }

    buf[out] = 0;

    ucsdet_close(ucd);

    icu::UnicodeString source(buf);
    delete [] buf;

    source.append("\n");

    std::string heh;
    source.toUTF8String(heh);
    std::cout << "Read:" << std::endl << heh << std::endl;

    dhc::lexer::layout l(source);

    while (!l.finished()) {
        dhc::lexer::match_ptr token (l.next());

        if (token) {
            std::string flat;
            token->flatten().toUTF8String(flat);
            std::cout << flat << ' ';
        } else {
            std::cerr << filename << std::endl;
        }
    }

    std::cout << std::endl;

    dhc::parser::parser p(source);

    std::cout << "Created parser" << std::endl;

    if (!p.finished()) {
        dhc::lexer::match_ptr token (p.next());

        if (token) {
            print_tree(token, 0);
        } else {
            std::cerr << p.error(filename) << std::endl;
        }
    }

    return 0;
}

