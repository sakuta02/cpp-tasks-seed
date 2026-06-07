#include "Gauss_solve.h"

#include <cmath>
#include <stdexcept>

namespace
{
constexpr double kEpsilon = 1e-12;
}

GaussVector Gauss_solve(GaussMatrix& ab)
{
    if (ab.cols() != ab.rows() + 1)
    {
        throw std::invalid_argument("Expected an augmented matrix of size N x (N + 1)");
    }

    const Eigen::Index n = ab.rows();

    for (Eigen::Index pivot_col = 0; pivot_col < n; ++pivot_col)
    {
        Eigen::Index pivot_row = pivot_col;
        double pivot_abs = std::abs(ab(pivot_row, pivot_col));

        for (Eigen::Index row = pivot_col + 1; row < n; ++row)
        {
            const double candidate_abs = std::abs(ab(row, pivot_col));
            if (candidate_abs > pivot_abs)
            {
                pivot_abs = candidate_abs;
                pivot_row = row;
            }
        }

        if (pivot_abs < kEpsilon)
        {
            throw std::runtime_error("Singular matrix");
        }

        if (pivot_row != pivot_col)
        {
            ab.row(pivot_col).swap(ab.row(pivot_row));
        }

        for (Eigen::Index row = pivot_col + 1; row < n; ++row)
        {
            const double factor = ab(row, pivot_col) / ab(pivot_col, pivot_col);
            ab.row(row).segment(pivot_col, ab.cols() - pivot_col) -=
                factor * ab.row(pivot_col).segment(pivot_col, ab.cols() - pivot_col);
            ab(row, pivot_col) = 0.0;
        }
    }

    GaussVector solution(n);
    for (Eigen::Index row = n - 1; row >= 0; --row)
    {
        double rhs = ab(row, n);
        if (row + 1 < n)
        {
            rhs -= ab.row(row).segment(row + 1, n - row - 1).dot(solution.segment(row + 1, n - row - 1));
        }

        const double diagonal = ab(row, row);
        if (std::abs(diagonal) < kEpsilon)
        {
            throw std::runtime_error("Singular matrix");
        }

        solution(row) = rhs / diagonal;
        if (row == 0)
        {
            break;
        }
    }

    return solution;
}
