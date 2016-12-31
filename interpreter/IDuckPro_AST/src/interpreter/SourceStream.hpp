#ifndef SOURCE_STREAM_H_d5631977_5e75_4ac8_b6b5_d1ba92936d11
#define SOURCE_STREAM_H_d5631977_5e75_4ac8_b6b5_d1ba92936d11

#include <string>

class SourceStream
{
public:
    explicit SourceStream(std::string const& source)
        :_source(source)
        ,_position(0)
        ,_line(1)
        ,_column(1)
    {
    }

    inline void move()
    {
        if (peek() == '\n')
        {
            ++_line;
            _column = 1;
        }
        else
        {
            ++_column;
        }
        ++_position;
    }

    inline bool isEnd() const
    {
        return (_position >= _source.size());
    }

    inline char peek() const
    {
        if (_position >= _source.size()) {
            return 0;
        }
        return _source[_position];
    }

    inline size_t line() const
    {
        return _line;
    }

    inline size_t column() const
    {
        return _column;
    }

private:
    std::string const& _source;
    size_t _position;
    size_t _line;
    size_t _column;
};

#endif // SOURCE_STREAM_H_d5631977_5e75_4ac8_b6b5_d1ba92936d11
