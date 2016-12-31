#include "lexer.h"

const char ACTUAL_KEYWORDS_PATTERN[] =
        "break|case|catch|const|continue|default"
        "|do|else|false|final|finally|for|if|in|new"
        "|null|return|super|switch|this|throw|true"
        "|try|var|void|while";

const char PSEUDO_KEYWORDS_PATTERN[] =
        "abstract|assert|class|extends|factory|get"
        "|implements|import|interface|library|is|native"
        "|negate|operator|set|source|static|typedef";

const char NUMBER_PATTERN[] = // exp: [eE][+-]?[0-9]+
        "0"
        "|[1-9][0-9]*"
        "|([0-9]*\\.?[0-9]+|[0-9]+\\.)([eE][+-]?[0-9]+)?"
        "|0[oO]?[0-7]+"
        "|0[bB]?[01]+"
        "|0[xX][0-9a-fA-F]+"; // hexi

const char WHITESPACE_PATTERN[] = "[ \\t\\r\\n]*";

const char COMMENT_PATTERN[] =
        "#!.*"      // unix shell comment at start of file
        "|//.*"
        "|/\\*.*?\\*/";

/** NOTE: This lexer determines only identifiers with dollars,
 * but Dart also has special "identifier without dollar"
 */
const char IDENTIFIER_PATTERN[] = "[\\$a-zA-Z\\_][\\$a-zA-Z\\_0-9]*";

/** NOTE: escape codes in multiline strings not supported yet
 * However, official dart grammar won't support this case too
 * See https://dart.googlecode.com/svn/branches/bleeding_edge/dart/language/grammar/Dart.g
 */
const char STRING_PATTERN[] =
        "\\@?\\\"\\\"\\\"(.|\n)*?\\\"\\\"\\\""
        "|\\@?\\\"\\\"\\\"(.|\n)*?\\\"\\\"\\\""
        "|\\@?\\'([^'\\\\\\n]*(\\\\(.|\n)[^'\\\\\\n]*)*)\\'"
        "|\\@?\\\"([^\"\\\\\\n]*(\\\\(.|\\n)[^\"\\\\\\n]*)*)\\\""
        ;

Lexer::Lexer(const std::string &text)
    :_text(text)
    ,_position(0)
    ,_lastMatchedPosition(0)
    ,_exprComment(COMMENT_PATTERN)
    ,_exprNumber(NUMBER_PATTERN)
    ,_exprString(STRING_PATTERN)
    ,_exprIdentifier(IDENTIFIER_PATTERN)
    ,_exprActualKeyword(ACTUAL_KEYWORDS_PATTERN)
    ,_exprPseudoKeyword(PSEUDO_KEYWORDS_PATTERN)
    ,_exprWhitespace(WHITESPACE_PATTERN)
{
}

TokenKind Lexer::read()
{
    if (match(_exprWhitespace))
        _position += _result.length();

    if (_position >= _text.size())
        return T_EOF_SYMBOL;

    if (matchAndStore(_exprPseudoKeyword))
        return T_PSEUDO_KEYWORD;
    if (matchAndStore(_exprActualKeyword))
        return T_ACTUAL_KEYWORD;
    if (matchAndStore(_exprIdentifier))
        return T_IDENTIFIER;
    if (matchAndStore(_exprString))
        return T_STRING;
    if (matchAndStore(_exprNumber))
        return T_NUMBER;
    if (matchAndStore(_exprComment))
        return T_COMMENT;

    _lastMatchedPosition = _position;
    ++_position;
    _lastMatchedText = _text.at(_lastMatchedPosition);
    return T_OPERATOR;
}

unsigned Lexer::lastPosition() const
{
    return _lastMatchedPosition;
}

unsigned Lexer::lastLength() const
{
    return _position - _lastMatchedPosition;
}

unsigned Lexer::lastEnd() const
{
    return _position;
}

const std::string &Lexer::lastText() const
{
    return _lastMatchedText;
}

bool Lexer::match(const boost::regex &pattern)
{
    boost::regex_constants::match_flags flags =
            boost::regex_constants::match_continuous;

    return boost::regex_search(_text.c_str() + _position, _result, pattern, flags);
}

bool Lexer::matchAndStore(const boost::regex &pattern)
{
    if (match(pattern)) {
        _lastMatchedPosition = _position;
        _position += _result.length();
        _lastMatchedText = _result.str();
        return true;
    }
    return false;
}
