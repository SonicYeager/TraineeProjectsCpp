#pragma once
#include <vector>
#include <string>
#include <functional>

struct Range
{
	int begin;
	int end;


	inline operator bool()
	{
		return begin < end;
	}

	Range() = default;
	Range(int beg, int end) 
		: begin(beg)
		, end(end)
	{}
};

using PrimeNumber = int;
using PrimeNumbers = std::vector<PrimeNumber>;

template<typename... Args>
using Event = std::function<void(Args...)>;
