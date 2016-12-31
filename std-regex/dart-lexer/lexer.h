#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <boost/regex.hpp>

enum TokenKind {
    T_COMMENT,
    T_NUMBER,
    T_STRING,
    T_IDENTIFIER,
    T_ACTUAL_KEYWORD,
    T_PSEUDO_KEYWORD,
    T_OPERATOR,

    T_EOF_SYMBOL,
    T_ERROR
};

class Lexer
{
public:
    Lexer(const std::string &text);

    /**
     * @brief read reads token and changes lastPosition, lastText, etc. properties
     * @return kind of token that lexer have read
     */
    TokenKind read();

    unsigned lastPosition() const;
    unsigned lastLength() const;
    unsigned lastEnd() const;
    const std::string &lastText() const;

private:
    /**
     * @brief match matches lexical stream with given regexp and overwrites
     * "_result" field
     * @param pattern
     * @return true if stream content matches given pattern
     */
    bool match(const boost::regex &pattern);

    /**
     * @brief matchAndStore matches lexical stream with given regexp, stores
     * matched text, its position and length on success
     * @param pattern - regular expression to match
     * @return true if stream content matches given pattern
     */
    bool matchAndStore(const boost::regex &pattern);

    const std::string _text;

    std::string _lastMatchedText;
    unsigned _position;
    unsigned _lastMatchedPosition;
    boost::match_results<const char*> _result;

    const boost::regex _exprComment;
    const boost::regex _exprNumber;
    const boost::regex _exprString;
    const boost::regex _exprIdentifier;
    const boost::regex _exprActualKeyword;
    const boost::regex _exprPseudoKeyword;
    const boost::regex _exprWhitespace;
};

#endif // LEXER_H
