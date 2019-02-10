#include "CompilerDriver.h"
#include <time.h>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/optional.hpp>

struct CompilerOptions
{
    std::string inputPath;
    std::string outputPath;
};

boost::optional<CompilerOptions> parse_args(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    std::srand(static_cast<unsigned>(time(nullptr)));

    try
    {
        auto options = parse_args(argc, argv);
        if (options)
        {
            CCompilerDriver driver(std::cerr);
            if (!driver.Compile(options->inputPath, options->outputPath))
            {
                throw std::runtime_error("fatal error: compilation failed");
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

boost::optional<CompilerOptions> parse_args(int argc, char* argv[])
{
    using namespace boost::program_options;

    options_description desc("Usage");
    CompilerOptions result;
    desc.add_options()
        ("help,h", "print usage message")
        ("input,i", value<std::string>(), "pathname for input")
        ("output,o", value<std::string>()->default_value("program.o"), "pathname for output (optional)");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);
    if (vm.count("help"))
    {  
        std::cout << desc << "\n";
        return boost::none;
    }
    result.inputPath = vm["input"].as<std::string>();
    result.outputPath = vm["output"].as<std::string>();
    if (result.inputPath.empty())
    {
        throw std::runtime_error("missing input file (-i option)");
    }

    return result;
}
