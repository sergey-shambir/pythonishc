#ifndef SOURCE_STREAM_H_d5631977_5e75_4ac8_b6b5_d1ba92936d11
#define SOURCE_STREAM_H_d5631977_5e75_4ac8_b6b5_d1ba92936d11

#include <string>

class SourceStream
{
public:
    explicit SourceStream(std::string const& source)
        :_source(source)
        ,_position(0)
    {
    }

    inline void move()
    {
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

private:
    std::string _source;
    size_t _position;
};

#endif // SOURCE_STREAM_H_d5631977_5e75_4ac8_b6b5_d1ba92936d11
