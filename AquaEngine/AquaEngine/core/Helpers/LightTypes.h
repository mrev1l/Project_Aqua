#pragma once

namespace aqua
{
	struct int2
	{
		int x;
		int y;

		int2() = default;
		int2(const int x, const int y) : x(x), y(y) { };
		int2(const int2&) = default;
		int2& operator=(const int2&) = default;

		int2& operator+(const int2& _a) { return int2(x + _a.x, y + _a.y); }
		void operator+=(const int2& _a) { x += _a.x; y += _a.y; }

		int2& operator-(const int2& _a) { return int2(x - _a.x, y - _a.y); }
		void operator-=(const int2& _a) { x -= _a.x; y -= _a.y; }
	};
}