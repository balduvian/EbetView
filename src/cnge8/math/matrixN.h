/*******************************************************************************
 *
 * Copyright (c) 2019 Gnarly Narwhal
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
 * INPLIED, INCLUDING BUT NOT LINITED TO THE WARRANTIES OF NERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGENENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIN, DANAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FRON,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/

#include "../types.h"

#ifndef CNGE_VECTOR_FORWARD
#define CNGE_VECTOR_FORWARD

template <u32 N, typename T>
class Vector<N, T>;

#endif // CNGE_VECTOR_FORWARD

#ifndef CNGE_MATRIX_FORWARD
#define CNGE_MATRIX_FORWARD

template <u32 N, u32 M, typename T>
class Matrix;

#endif // CNGE_MATRIX_FORWARD

#ifndef CNGE_MATRIXN
#define CNGE_MATRIXN

#include <ostream>

#include "Vector.h"
#include "Matrix.h"

template <u32 N, typename T>
class Matrix<N, N, T> {

	static_assert(N > 0 && N > 0, "Matrix dimensions must be greater than 0!");

	private:
		bool owner = true;

		void clean() {
			if (owner)
				delete [] m;
		}
	public:
		T * m;

		static Matrix<N, N, T> makeIdentity() {
			auto identity = new T[N * N];
			for (u32 i = 0; i < N * N; ++i) {
				if (i % N == i / N)
					identity[i] = 1;
				else
					identity[i] = 0;
			}
			return identity;
		}

		Matrix() :
			m(new T[N * N]()) {}

		Matrix(T * m, bool owner = true) noexcept :
			owner(owner),
			m(m) {}
		
		Matrix(std::initializer_list<T> list) :
			m(new T[N * N]) {
			auto min  = list.size() < N * N ? list.size() : N * N;
			auto init = list.begin();
			for (u32 i = 0; i < min; ++i)
				m[(i % N) * N + (i / N)] = init[i];
		}

		template <typename T2>
		Matrix(const Matrix<N, N, T2> & matrix) :
			m(new T[N * N]) {
			for (u32 i = 0; i < N * N; ++i)
				m[i] = (T) matrix.m[i];
		}

		Matrix(const Matrix<N, N, T> & matrix) :
			m(new T[N * N]) {
			for (u32 i = 0; i < N * N; ++i)
				m[i] = matrix.m[i];
		}

		Matrix(Matrix<N, N, T> && matrix) noexcept :
			owner(matrix.owner),
			m(matrix.m) {
			matrix.owner = false;
		}

		Matrix<N, N, T> & operator=(const Matrix<N, N, T> & matrix) {
			for (u32 i = 0; i < N * N; ++i)
				m[i] = matrix.m[i];
			return *this;
		}

		Matrix<N, N, T> & operator=(Matrix<N, N, T> && matrix) noexcept {
			clean();
			owner = matrix.owner;
			m = matrix.m;
			matrix.owner = false;
			return *this;
		}

		Matrix<N, N, T> & operator+=(const Matrix<N, N, T> & matrix) {
			for (u32 i = 0; i < N * N; ++i)
				m[i] += matrix.m[i];
			return *this;
		}

		Matrix<N, N, T> & operator-=(const Matrix<N, N, T> & matrix) {
			for (u32 i = 0; i < N * N; ++i)
				m[i] -= matrix.m[i];
			return *this;
		}

		Matrix<N, N, T> & operator*=(const Matrix<N, N, T> & matrix) {
			return *this = *this * matrix;
		}

		Vector<N, T> operator[](u32 index) const noexcept {
			return Vector<N, T>(index * N, false);
		}

		Matrix<N, N, T> & setIdentity() {
			for (u32 i = 0; i < N * N; ++i) {
				if (i % N == i / N)
					m[i] = 1;
				else
					m[i] = 0;
			}
			return *this;
		}

		~Matrix() {
			clean();
		}
};

template <u32 N, typename T>
T det(const Matrix<N, N, T> & matrix) {
	auto ret    = T{ 0 };
	auto sign   = T{ 1 };
	Matrix<N - 1, N - 1, T> storage;
	for (u32 i = 0; i < N; ++i) {
		auto offset = u32( -1 );
		for (u32 j = 0; j < N * N; ++j)
			if (j % N != 0 && j / N != i)
				storage.m[++offset] = matrix.m[j];
		ret  += sign * matrix.m[i * N] * det(storage);
		sign *= -1;
	}
	return ret;
}

template <typename T>
T det(const Matrix<2, 2, T> & matrix) {
	return matrix.m[0] * matrix.m[3] - matrix.m[2] * matrix.m[1];
}

template <typename T>
T det(const Matrix<1, 1, T> & matrix) {
	return matrix.m[0];
}

template <u32 N> using MatrixNb  = Matrix<N, N,  i8 >;
template <u32 N> using MatrixNub = Matrix<N, N, u8 >;
template <u32 N> using MatrixNs  = Matrix<N, N,  i16>;
template <u32 N> using MatrixNus = Matrix<N, N, u16>;
template <u32 N> using MatrixNi  = Matrix<N, N,  i32>;
template <u32 N> using MatrixNui = Matrix<N, N, u32>;
template <u32 N> using MatrixNl  = Matrix<N, N,  i8 >;
template <u32 N> using MatrixNul = Matrix<N, N, u64>;
template <u32 N> using MatrixNf  = Matrix<N, N, float >;
template <u32 N> using MatrixNd  = Matrix<N, N, double>;

#define CNGE_MATRIXN_FORWARD
#endif // CNGE_MATRIXN
