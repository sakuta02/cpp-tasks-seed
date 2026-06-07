#include "util.h"

#include <iomanip>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <lazycsv.hpp>

namespace
{
bool try_parse_double(const std::string& text, double& value)
{
    size_t pos = 0;
    try
    {
        value = std::stod(text, &pos);
    }
    catch (const std::exception&)
    {
        return false;
    }
    return pos == text.size();
}

std::vector<double> parse_row(const std::vector<std::string>& row)
{
    std::vector<double> values;
    values.reserve(row.size());
    for (const auto& cell : row)
    {
        double value = 0.0;
        if (!try_parse_double(cell, value))
        {
            throw std::invalid_argument("CSV contains a non-numeric cell");
        }
        values.push_back(value);
    }
    return values;
}
} // namespace

GaussMatrix load_csv_to_matrix(const char *filename)
{
    std::vector<std::vector<std::string>> rows;
    lazycsv::parser parser(filename);

    for (const auto& csv_row : parser)
    {
        std::vector<std::string> row;
        for (const auto& cell : csv_row)
        {
            row.emplace_back(cell.raw());
        }
        if (!row.empty())
        {
            rows.push_back(std::move(row));
        }
    }

    if (rows.empty())
    {
        throw std::invalid_argument("CSV is empty");
    }

    size_t first_data_row = 0;
    {
        bool header_detected = false;
        for (const auto& cell : rows.front())
        {
            double value = 0.0;
            if (!try_parse_double(cell, value))
            {
                header_detected = true;
                break;
            }
        }
        if (header_detected)
        {
            first_data_row = 1;
        }
    }

    if (first_data_row >= rows.size())
    {
        throw std::invalid_argument("CSV has a header but no data rows");
    }

    const size_t cols = rows[first_data_row].size();
    if (cols == 0)
    {
        throw std::invalid_argument("CSV row is empty");
    }

    GaussMatrix matrix(static_cast<Eigen::Index>(rows.size() - first_data_row),
                       static_cast<Eigen::Index>(cols));

    for (size_t row_index = first_data_row; row_index < rows.size(); ++row_index)
    {
        if (rows[row_index].size() != cols)
        {
            throw std::invalid_argument("CSV rows have different lengths");
        }
        const auto values = parse_row(rows[row_index]);
        for (size_t col_index = 0; col_index < cols; ++col_index)
        {
            matrix(static_cast<Eigen::Index>(row_index - first_data_row),
                   static_cast<Eigen::Index>(col_index)) = values[col_index];
        }
    }

    return matrix;
}

void print_matrix_as_csv(std::ostream& out, const GaussMatrix& matrix, int prec)
{
    out << std::fixed << std::setprecision(prec);

    for (Eigen::Index col = 0; col < matrix.cols(); ++col)
    {
        out << "c" << col;
        if (col + 1 < matrix.cols())
        {
            out << ',';
        }
    }
    out << '\n';

    for (Eigen::Index row = 0; row < matrix.rows(); ++row)
    {
        for (Eigen::Index col = 0; col < matrix.cols(); ++col)
        {
            out << matrix(row, col);
            if (col + 1 < matrix.cols())
            {
                out << ',';
            }
        }
        out << '\n';
    }
}

void print_vector_as_csv(std::ostream& out, const GaussVector& vector, int prec)
{
    out << "x\n";
    out << std::fixed << std::setprecision(prec);
    for (Eigen::Index i = 0; i < vector.size(); ++i)
    {
        out << vector(i) << '\n';
    }
}
