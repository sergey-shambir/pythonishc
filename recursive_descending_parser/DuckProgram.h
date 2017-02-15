#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>

// ! Дурной код !
// Не следует помещать реализации методов в заголовочном файле.
// Следует детальнее выделять иерархию классов, например,
//  используя паттерны Interpreter (Интерпретатор) или Strategy (Стратегия)
class DuckProgram
{
public:
    using Operation = std::function<void()>;

    void SetName(const std::string &name)
    {
        m_name = name;
    }

    void SetType(const std::string &type)
    {
        m_type = type;
    }

    void RunProgram(std::ostream &out)
    {
        out << "I'm " << m_type << " duck '" << m_name << "'" << std::endl;
        for (const Operation &op : m_operations)
        {
            op();
        }
    }

    void AddQuackCommand()
    {
        // ! Дурной код !
        // Сейчас любая операция всегда выполняется с глобальным контекстом,
        //  нельзя даже перенаправить вывод в другой поток.
        // Помочь может паттерн проектирования Interpreter (Интерпретатор)
        m_operations.push_back([] {
            std::cerr << "quack" << std::endl;
        });
    }

    void AddTimedFlyCommand(const std::chrono::seconds &duration)
    {
        // ! Дурной код !
        // Сейчас любая операция всегда выполняется с глобальным контекстом,
        //  нельзя даже перенаправить вывод в другой поток.
        // Помочь может паттерн проектирования Interpreter (Интерпретатор)
        m_operations.push_back([duration] {
            std::cout << "I'm flying!!" << std::endl;
            std::this_thread::sleep_for(duration);
            std::cout << "I did ground" << std::endl;
        });
    }

    void AddFlyCommandWithMeters(int64_t meters)
    {
        // ! Дурной код !
        // Switch по строкам как минимум лучше заменить на enum.
        // ООП предлагает более мощное средство - иерархию
        //  подклассов IDuck, имеющих разное поведение.
        // Функциональный подход - использовать тип данных variant.
        if (m_type == "toy")
        {
            throw std::runtime_error("toy duck cannot fly out of bathroom");
        }
        else if (m_type == "mallard")
        {
            const uint64_t speed = 2; // meters per second.
            AddTimedFlyCommand(std::chrono::seconds(meters / speed));
        }
        else if (m_type == "robot")
        {
            const uint64_t speed = 4; // meters per second.
            AddTimedFlyCommand(std::chrono::seconds(meters / speed));
        }
    }

private:
    std::vector<Operation> m_operations;

    std::string m_name;
    std::string m_type;
};
