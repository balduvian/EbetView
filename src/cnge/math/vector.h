/*******************************************************************************
 *
 * Copyright (c) 2019 Gnarwhal
 *
 * -----------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/

#ifndef CNGE_VECTOR
#define CNGE_VECTOR

#include <ostream>
#include <cmath>
#include <initializer_list>

#include "types.h"

template <u32 N, typename T>
class Vector {

	static_assert(N > 0, "Vector dimensions must be greater than 0!");

	private:
		bool owner = true;

		void clean() {
			if (owner)
				delete [] v;
		}

	public:
		static Vector<N, T> * newArray(u32 length) {
			T * v = new T[N * length];
			Vector<N, T> * ret = new Vector<N, T>[length];
			ret[0] = Vector<N, T>(v);
			for (u32 i = 1; i < length; ++i)
				ret[i] = Vector<N, T>(v + i * N, false);
			return ret;
		}

		T * v;

		Vector() :
			v(new T[N]()) {}

		Vector(T * v, bool owner = true) noexcept :
			owner(owner),
			v(v) {}
		
		explicit Vector(T value) :
			v(new T[N]) {
			for (u32 i = 0; i < N; ++i)
				v[i] = value;
		}

		Vector(std::initializer_list<T> list) :
			v(new T[N]) {
			auto min  = list.size() < N ? list.size() : N;
			auto init = list.begin();
			for (u32 i = 0; i < min; ++i)
				v[i] = init[i];
		}

		template <typename T2>
		Vector(const Vector<N, T2> & vector) :
			v(new T[N]) {
			for (u32 i = 0; i < N; ++i)
				v[i] = (T) vector.v[i];
		}
		
		Vector(const Vector<N, T> & vector) :
			v(new T[N]) {
			for (u32 i = 0; i < N; ++i)
				v[i] = vector.v[i];
		}
		
		Vector(Vector<N, T> && vector) noexcept :
			owner(vector.owner),
			v(vector.v) {
			vector.owner = false;
		}

		Vector<N, T> & operator=(const Vector<N, T> & vector) {
			for (u32 i = 0; i < N; ++i)
				v[i] = vector.v[i];
			return *this;
		}
		
		Vector<N, T> & operator=(Vector<N, T> && vector) noexcept {
			if (owner) {
				clean();
				owner = vector.owner;
				v = vector.v;
				vector.owner = false;
			}
			else for (u32 i = 0; i < N; ++i)
				v[i] = vector.v[i];
			return *this;
		}

		Vector<N, T> & operator+=(const Vector<N, T> & vector) {
			for (u32 i = 0; i < N; ++i)
				v[i] += vector.v[i];
			return *this;
		}

		Vector<N, T> & operator-=(const Vector<N, T> & vector) {
			for (u32 i = 0; i < N; ++i)
				v[i] -= vector.v[i];
			return *this;
		}

		Vector<N, T> & operator*=(T scalar) {
			for (u32 i = 0; i < N; ++i)
				v[i] *= scalar;
			return *this;
		}

		Vector<N, T> & operator*=(const Vector<N, T> & vector) {
			for (u32 i = 0; i < N; ++i)
				v[i] *= vector.v[i];
			return *this;
		}

		Vector<N, T> & operator/=(T scalar) {
			for (u32 i = 0; i < N; ++i)
				v[i] /= scalar;
			return *this;
		}

		Vector<N, T> & operator/=(const Vector<N, T> & vector) {
			for (u32 i = 0; i < N; ++i)
				v[i] /= vector.v[i];
			return *this;
		}

		T & operator[](u32 index) noexcept {
			return v[index];
		}

		const T & operator[](u32 index) const noexcept {
			return v[index];
		}
		
		T getLength() const {
			auto lengthSquared = 0;
			for (u32 i = 0; i < N; ++i)
				lengthSquared += v[i] * v[i];
			return sqrt(lengthSquared);
		}

		T getLengthSquared() const {
			auto lengthSquared = 0;
			for (u32 i = 0; i < N; ++i)
				lengthSquared += v[i] * v[i];
			return lengthSquared;
		}

		Vector<N, T> & setLength(T length) {
			auto scalar = length / getLength();
			for (u32 i = 0; i < N; ++i)
				v[i] *= scalar;
			return *this;
		}

		Vector<N, T> & normalize() {
			auto scalar = 1 / getLength();
			for (u32 i = 0; i < N; ++i)
				v[i] *= scalar;
			return *this;
		}

		Vector<N, T> & negate() {
			for (u32 i = 0; i < N; ++i)
				v[i] = -v[i];
			return *this;
		}

		Vector<N, T> & project(const Vector<N, T> & projection) {
			auto scalar = dot(*this, projection) / projection.getLengthSquared();
			for (u32 i = 0; i < N; ++i)
				v[i] = scalar * projection.v[i];
			return *this;
		}

		Vector<N, T> & set(const Vector<N, T> & set) {
			for (u32 i = 0; i < N; ++i)
				v[i] = set.v[i];
			return *this;
		}

		Vector<N, T> & translate(const Vector<N, T> & translate) {
			for (u32 i = 0; i < N; ++i)
				v[i] += translate.v[i];
			return *this;
		}

		Vector<N, T> & scale(T scale) {
			for (u32 i = 0; i < N; ++i)
				v[i] *= scale;
			return *this;
		}

		Vector<N, T> & scale(const Vector<N, T> & scale) {
			for (u32 i = 0; i < N; ++i)
				v[i] *= scale.v[i];
			return *this;
		}

		virtual ~Vector() noexcept {
			clean();
		}
};

template <u32 N, typename T>
Vector<N, T> operator-(const Vector<N, T> & vector) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = -vector.v[i];
	return newV;
}

template <u32 N, typename T>
Vector<N, T> operator+(const Vector<N, T> & left, const Vector<N, T> & right) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = left.v[i] + right.v[i];
	return newV;
}

template <u32 N, typename T>
Vector<N, T> operator-(const Vector<N, T> & left, const Vector<N, T> & right) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = left.v[i] - right.v[i];
	return newV;
}

template <u32 N, typename T>
Vector<N, T> operator*(const Vector<N, T> & left, T right) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = left.v[i] * right;
	return newV;
}

template <u32 N, typename T>
Vector<N, T> operator*(T left, const Vector<N, T> & right) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = left * right.v[i];
	return newV;
}

template <u32 N, typename T>
Vector<N, T> operator*(const Vector<N, T> & left, const Vector<N, T> & right) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = left.v[i] * right.v[i];
	return newV;
}

template <u32 N, typename T>
Vector<N, T> operator/(const Vector<N, T> & left, T right) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = left.v[i] / right;
	return newV;
}

template <u32 N, typename T>
Vector<N, T> operator/(const Vector<N, T> & left, const Vector<N, T> & right) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = left.v[i] / right.v[i];
	return newV;
}

template <u32 N, typename T>
Vector<N + 1, T> operator|(const Vector<N, T> & left, T right) {
	auto newV = new T[N + 1];
	for (u32 i = 0; i < N + 1; ++i)
		newV[i] = left.v[i];
	newV[N] = right;
	return newV;
}

template <u32 N, typename T>
Vector<N + 1, T> operator|(T left, const Vector<N, T> & right) {
	auto newV = new T[N + 1];
	newV[0] = left;
	for (u32 i = 0; i < N + 1; ++i)
		newV[i + 1] = right.v[i];
	return newV;
}

template <u32 N, u32 N2, typename T>
Vector<N + N2, T> operator|(const Vector<N, T> & left, const Vector<N2, T> & right) {
	auto newV = new T[N + N2];
	auto vRight = newV + N;
	for (u32 i = 0; i < N; ++i)
		newV[i] = left.v[i];
	for (u32 i = 0; i < N2; ++i)
		vRight[i] = right.v[i];
	return newV;	
}

template <u32 N, typename T>
Vector<N, T> setLength(const Vector<N, T> & vector, T length) {
	auto scalar = length / vector.getLength();
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = vector.v[i] * scalar;
	return newV;
}

template <u32 N, typename T>
Vector<N, T> & setLength(const Vector<N, T> & vector, T length, const Vector<N, T> & target) {
	auto scalar = length / vector.getLength();
	for (u32 i = 0; i < N; ++i)
		target.v[i] = vector.v[i] * scalar;
	return target;
}

template <u32 N, typename T>
Vector<N, T> normalize(const Vector<N, T> & vector) {
	auto scalar = 1 / vector.getLength();
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = vector.v[i] * scalar;
	return newV;
}

template <u32 N, typename T>
Vector<N, T> & normalize(const Vector<N, T> & vector, const Vector<N, T> & target) {
	auto scalar = 1 / vector.getLength();
	for (u32 i = 0; i < N; ++i)
		target.v[i] = vector.v[i] * scalar;
	return target;
}

template <u32 N, typename T>
Vector<N, T> negate(const Vector<N, T> & vector) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = -vector.v[i];
	return newV;
}

template <u32 N, typename T>
Vector<N, T> & negate(const Vector<N, T> & vector, Vector<N, T> & target) {
	for (u32 i = 0; i < N; ++i)
		target.v[i] = -vector.v[i];
	return target;
}

template <u32 N, typename T>
T dot(const Vector<N, T> & left, const Vector<N, T> & right) {
	auto product = T();
	for (u32 i = 0; i < N; ++i)
		product += left.v[i] * right.v[i];
	return product;
}

template <u32 N, typename T>
Vector<N, T> project(const Vector<N, T> & vector, const Vector<N, T> & projection) {
	auto scalar = dot(vector, projection) / projection.getLengthSquared();
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = scalar * projection.v[i];
	return newV;
}

template <u32 N, typename T>
Vector<N, T> & project(const Vector<N, T> & vector, const Vector<N, T> & projection, Vector<N, T> & target) {
	auto scalar = dot(vector, projection) / projection.getLengthSquared();
	for (u32 i = 0; i < N; ++i)
		target.v[i] = scalar * projection.v[i];
	return target;
}

template <u32 N, typename T>
T angleBetween(const Vector<N, T> & vector1, const Vector<N, T> & vector2) {
	return acos(dot(vector1, vector2) / (vector1.getLength() * vector2.getLength()));
}

template <u32 N, typename T>
Vector<N, T> & translate(const Vector<N, T> & vector, const Vector<N, T> & translate) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = vector.v[i] + translate.v[i];
	return newV;
}


template <u32 N, typename T>
Vector<N, T> & translate(const Vector<N, T> & vector, const Vector<N, T> & translate, Vector<N, T> & target) {
	for (u32 i = 0; i < N; ++i)
		target.v[i] = vector.v[i] + translate.v[i];
	return target;
}

template <u32 N, typename T>
Vector<N, T> & scale(const Vector<N, T> & vector, T scale) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = vector.v[i] * scale;
	return newV;
}


template <u32 N, typename T>
Vector<N, T> & scale(const Vector<N, T> & vector, T scale, Vector<N, T> & target) {
	for (u32 i = 0; i < N; ++i)
		target.v[i] = vector.v[i] * scale;
	return target;
}

template <u32 N, typename T>
Vector<N, T> & scale(const Vector<N, T> & vector, const Vector<N, T> & scale) {
	auto newV = new T[N];
	for (u32 i = 0; i < N; ++i)
		newV[i] = vector.v[i] * scale.v[i];
	return newV;
}


template <u32 N, typename T>
Vector<N, T> & scale(const Vector<N, T> & vector, const Vector<N, T> & scale, Vector<N, T> & target) {
	for (u32 i = 0; i < N; ++i)
		target.v[i] = vector.v[i] * scale.v[i];
	return target;
}

template <u32 N, typename T>
std::ostream & operator<<(std::ostream & stream, const Vector<N, T> & vector) {
	stream << '<';
	for (u32 i = 0; i < N; ++i) {
		stream << vector.v[i];
		if (i < N - 1)
			stream << ", ";
	}
	return stream << '>';
}

template <u32 N> using VectorNb  = Vector<N,  i8 >;
template <u32 N> using VectorNub = Vector<N, u8 >;
template <u32 N> using VectorNs  = Vector<N,  i16>;
template <u32 N> using VectorNus = Vector<N, u16>;
template <u32 N> using VectorNi  = Vector<N,  i32>;
template <u32 N> using VectorNui = Vector<N, u32>;
template <u32 N> using VectorNl  = Vector<N,  i64>;
template <u32 N> using VectorNul = Vector<N, u64>;
template <u32 N> using VectorNf  = Vector<N, float >;
template <u32 N> using VectorNd  = Vector<N, double>;

#define CNGE_VECTOR_FORWARD
#endif // CNGE_VECTOR
