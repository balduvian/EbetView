
#ifndef CNGE_MATH
#define CNGE_MATH

#include "types.h"

namespace CNGE {
	constexpr auto PI = 3.14159265359_f32;

	template<typename T>
	T mod(T a, T b) {
		return (b + (a % b)) % b;
	}

	template<>
	f32 mod<f32>(f32 a, f32 b);

	template<>
	f64 mod<f64>(f64 a, f64 b);

	template <typename T>
	i32 sign(T val) {
		return (T(0) < val) - (val < T(0));
	}

	template<typename T>
	auto interp(T start, T end, f32 along) -> T {
		return (end - start) * along + start;
	}

	template<typename T>
	auto invInterp(T start, T end, T value) -> T {
		return (value - start) / (end - start);
	}

	template<typename T>
	auto inclusiveRange(T low, T value, T high) -> bool {
		return (value >= low) && (value <= high);
	}

	template<typename T>
	auto exclusiveRange(T low, T value, T high) -> bool {
		return (value > low) && (value < high);
	}

	template<typename T>
	auto minMax(T& a, T& b) -> void {
		if (a > b) {
			auto temp = a;
			a = b;
			b = temp;
		}
	}
}

#endif
