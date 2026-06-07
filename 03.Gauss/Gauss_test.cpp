#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Gauss_solve.h"
#include "util.h"

TEST(GaussSolve, SolvesTwoByTwoSystem)
{
    GaussMatrix ab(2, 3);
    ab << 2.0, 1.0, 5.0,
    4.0, -6.0, -2.0;

    const auto x = Gauss_solve(ab);

    EXPECT_NEAR(x(0), 1.75, 1e-9);
    EXPECT_NEAR(x(1), 1.5, 1e-9);
}

TEST(GaussSolve, HandlesPivoting)
{
    GaussMatrix ab(2, 3);
    ab << 0.0, 2.0, 4.0,
    3.0, 1.0, 5.0;

    const auto x = Gauss_solve(ab);

    EXPECT_NEAR(x(0), 1.0, 1e-9);
    EXPECT_NEAR(x(1), 2.0, 1e-9);
}

TEST(GaussSolve, RejectsSingularMatrix)
{
    GaussMatrix ab(2, 3);
    ab << 1.0, 2.0, 3.0,
    2.0, 4.0, 6.0;

    EXPECT_THROW(Gauss_solve(ab), std::runtime_error);
}

TEST(GaussUtil, LoadsCsvWithHeader)
{
    const std::string filename = "gauss_load_test.csv";
    {
        std::ofstream out(filename);
        out << "a,b,c\n";
        out << "1,2,3\n";
        out << "4,5,6\n";
    }

    const auto matrix = load_csv_to_matrix(filename.c_str());
    std::remove(filename.c_str());

    ASSERT_EQ(matrix.rows(), 2);
    ASSERT_EQ(matrix.cols(), 3);
    EXPECT_DOUBLE_EQ(matrix(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(matrix(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(matrix(1, 2), 6.0);
}

TEST(GaussUtil, PrintsVectorAsCsv)
{
    GaussVector vector(3);
    vector << 1.0, -2.5, 3.25;

    std::ostringstream out;
    print_vector_as_csv(out, vector, 2);

    EXPECT_EQ(out.str(), "x\n1.00\n-2.50\n3.25\n");
}
