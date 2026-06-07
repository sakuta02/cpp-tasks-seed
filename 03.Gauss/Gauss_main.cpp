#include <exception>
#include <iostream>

#include "Gauss_solve.h"
#include "util.h"

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./gauss <input.csv>\n";
        return 1;
    }

    try
    {
        auto ab = load_csv_to_matrix(argv[1]);
        const auto solution = Gauss_solve(ab);
        print_vector_as_csv(std::cout, solution);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        return 1;
    }

    return 0;
}
