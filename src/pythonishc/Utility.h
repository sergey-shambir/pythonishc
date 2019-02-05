#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/adaptor/reversed.hpp>

// Реализует приём "пул строк" (англ. string interning, string pool)
// Пул строк позволяет однозначно сопоставить строку и её целочисленный ID.
// - ID можно использовать в стеке LALR парсера, созданного Lemon. Lemon генерирует код с 'union',
//   в котором можно хранить unsigned и нельзя хранить std::string.
// - Также ID немного ускорит компиляцию, для него быстрее работает копирование, сравнение
//   и подсчёт хеша.
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

// Цепочка областей видимостей для символов (чаще всего переменных).
// TSymbol - один атрибут или структура с атрибутам символа.
template <class TSymbol>
class CScopeChain
{
public:
    using Scope = std::unordered_map<unsigned, TSymbol>;

    void PushScope()
    {
        m_scopes.push_back(Scope());
    }

    void PopScope()
    {
        m_scopes.pop_back();
    }

    bool HasSymbol(unsigned nameId)const
    {
        return GetSymbol(nameId).is_initialized();
    }

    // Возвращает значение для символа с заданным именем.
    boost::optional<TSymbol> GetSymbol(unsigned nameId)const
    {
        for (const Scope &scope : boost::adaptors::reverse(m_scopes))
        {
            auto it = scope.find(nameId);
            if (it != scope.end())
            {
                return it->second;
            }
        }
        return boost::none;
    }

    // Возвращает true и устанавливает значение для символа, если он был ранее объявлен.
    // Иначе возвращает false.
    bool SetSymbol(unsigned nameId, const TSymbol &value)
    {
        for (Scope &scope : boost::adaptors::reverse(m_scopes))
        {
            auto it = scope.find(nameId);
            if (it != scope.end())
            {
                it->second = value;
                return true;
            }
        }
        return false;
    }

    void DefineSymbol(unsigned nameId, const TSymbol &value)
    {
        m_scopes.back()[nameId] = value;
    }

private:
    std::vector<Scope> m_scopes;
};
