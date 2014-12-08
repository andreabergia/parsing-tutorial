#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType
{
    OPERATOR,
    NUMBER,
    IDENTIFIER,
    END_OF_INPUT
};

class Token
{
public:
    Token(TokenType tokenType, std::string content)
        : tokenType_(tokenType), content_(content)
    {
    }

    inline TokenType getTokenType() const
    {
        return tokenType_;
    }

    inline std::string getContent() const
    {
        return content_;
    }

    inline bool operator ==(const Token& other) const
    {
        return tokenType_ == other.tokenType_
            && content_ == other.content_;
    }

private:
    TokenType tokenType_;
    std::string content_;
};

#endif
