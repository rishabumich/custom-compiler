#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

// TOKENS:
// RETURN
// INT_LIT => 80
// SEMI

enum class TokenType
{
    _return,
    int_lit,
    semi
};

struct Token
{
    TokenType type;
    std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string &str)
{
    std::string buf = "";
    std::vector<Token> tokens;
    for (int i = 0; i < str.length(); ++i)
    {
        char c = str.at(i);
        if (std::isalpha(c))
        {
            buf.push_back(c);
            i++;
            while (std::isalnum(str.at(i)))
            {
                buf.push_back(str.at(i));
                i++;
            }
            i--;

            if (buf == "return")
            {
                tokens.push_back({.type = TokenType::_return});
                buf.clear();
                continue;
            }
            else
            {
                std::cerr << "Didn't work" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (std::isdigit(c))
        {
            buf.push_back(c);
            i++;
            while (std::isdigit(str.at(i)))
            {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({.type = TokenType::int_lit, .value = buf});
            buf.clear();
        }
        else if (c == ';')
        {
            tokens.push_back({.type = TokenType::semi});
        }
        else if (std::isspace(c))
        {
            continue;
        }
        else
        {
            std::cerr << "Didn't work" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return tokens;
}

std::string tokens_to_asm(const std::vector<Token> &tokens)
{
    std::stringstream output;
    output << "global _start\n_start:\n";
    for (int i = 0; i < tokens.size(); ++i)
    {
        const Token &token = tokens.at(i);

        if (token.type == TokenType::_return)
        {
            if (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::int_lit)
            {
                if (i + 2 < tokens.size() && tokens[i + 2].type == TokenType::semi)
                {
                    output << "     mov rax, 60\n";
                    output << "     mov rdi, " << tokens[i + 1].value.value() << "\n";
                    output << "     syscall";
                }
            }
        }
    }
    return output.str();
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
        std::cerr << "hydro <input.hy" << std::endl;
        return EXIT_FAILURE;
    }

    std::fstream input(argv[1], std::ios::in);

    std::stringstream contents_stream;
    contents_stream << input.rdbuf();

    std::string contents = contents_stream.str();

    std::vector<Token> tokens = tokenize(contents);

    {
        std::fstream output("./out.asm", std::ios::out);
        output << tokens_to_asm(tokens) << std::endl;
    }

    return EXIT_SUCCESS;
}