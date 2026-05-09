#include <gtest/gtest.h>
#include <algorithm>
#include <vector>

// А вот тут collvalue.h не надо, берите просто инты
#include "sorting.h"

// Но проверьте, что сортировки таки работают...

TEST(FooSuite, FooTest)
{
    std::vector<int> a = {5, 4, 3, 2, 1};
    bubble_sort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(FooSuite, FooTest2)
{
    std::vector<int> a = {7};
    bubble_sort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(FooSuite, FooTest3)
{
    std::vector<int> a;
    bubble_sort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(FooSuite, FooTest4)
{
    std::vector<int> a = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    quick_sort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(FooSuite, FooTest5)
{
    std::vector<int> a = {3, 3, 2, 2, 1, 1, 3, 2, 1};
    quick_sort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(FooSuite, FooTest6)
{
    std::vector<int> a = {1, 2, 3, 4, 5, 6};
    quick_sort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}
