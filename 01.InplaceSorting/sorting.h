#ifndef SORTING_H
#define SORTING_H

// Header-only

#include <utility>

// Для swap
#include "collvalue.h"

// ----------------------------- Пузырёк (например) --------------------------------------
template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end)
{
    if (begin == end)
    {
        return;
    }

    for (Iterator i = begin; i != end; ++i)
    {
        for (Iterator j = begin; j + 1 != end; ++j)
        {
            if (*(j + 1) < *j)
            {
                using std::swap;
                swap(*j, *(j + 1));
            }
        }
    }
}

// --------------------------- QuickSort (например) ---------------------------------------
template <typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    if (end - begin <= 1)
    {
        return;
    }

    Iterator left = begin;
    Iterator right = end - 1;
    auto mid = begin + (end - begin) / 2;
    auto pivot = *mid;

    while (left <= right)
    {
        while (*left < pivot)
        {
            ++left;
        }
        while (pivot < *right)
        {
            --right;
        }
        if (left <= right)
        {
            using std::swap;
            swap(*left, *right);
            ++left;
            --right;
        }
    }

    if (begin < right + 1)
    {
        quick_sort(begin, right + 1);
    }
    if (left < end)
    {
        quick_sort(left, end);
    }
}

#endif // SORTING_H
