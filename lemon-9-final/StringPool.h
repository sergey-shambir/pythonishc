#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <boost/noncopyable.hpp>

class CStringPool : private boost::noncopyable
{
public:
    CStringPool();

    unsigned Insert(std::string const& str);
    std::string GetString(unsigned id)const;

private:
    std::unordered_map<std::string, unsigned> m_mapping;
    std::vector<std::string> m_pool;
};
