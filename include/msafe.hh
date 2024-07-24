#pragma once

#include <vector>

using namespace std;

template <typename T>
extern vector<T> setv(vector<T> data, int index, T value, int line);

template <typename T>
extern T getv(vector<T> data, int index, int line);

