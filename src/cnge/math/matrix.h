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
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/

#include "types.h"

#ifndef CNGE_VECTOR_FORWARD
#define CNGE_VECTOR_FORWARD

template <u32 N, typename T>
class Vector;

#endif // CNGE_VECTOR_FORWARD

#ifndef CNGE_MATRIX
#define CNGE_MATRIX

#include <ostream>
#include <initializer_list>

#include "Vector.h"

template <u32 N, u32 M, typename T>
class Matrix {

	static_assert(N > 0 && M > 0, "Matrix dimensions must be greater than 0!");

	private:
		bool owner = true;

		void clean() {
			if (owner)
				delete [] m;
		}
	public:
		T * m;

		Matrix() :
			m(new T[N * M]()) {}

		Matrix(T * m, bool owner = true) noexcept :
			owner(owner),
			m(m) {}
		
		Matrix(std::initializer_list<T> list) :
			m(new T[N * M]) {
			auto min  = list.size() < N * M ? list.size() : N * M;
			auto init = list.begin();
			for (u32 i = 0; i < min; ++i)
				m[(i % M) * N + (i / M)] = init[i];
		}

		template <typename T2>
		Matrix(const Matrix<N, M, T2> & matrix) :
			m(new T[N * M]) {
			for (u32 i = 0; i < N * M; ++i)
				m[i] = (T) matrix.m[i];
		}

		Matrix(const Matrix<N, M, T> & matrix) :
			m(new T[N * M]) {
			for (u32 i = 0; i < N * M; ++i)
				m[i] = matrix.m[i];
		}

		Matrix(Matrix<N, M, T> && matrix) noexcept :
			owner(matrix.owner),
			m(matrix.m) {
			matrix.owner = false;
		}

		Matrix<N, M, T> & operator=(const Matrix<N, M, T> & matrix) {
			for (u32 i = 0; i < N * M; ++i)
				m[i] = matrix.m[i];
			return *this;
		}

		Matrix<N, M, T> & operator=(Matrix<N, M, T> && matrix) noexcept {
			clean();
			owner = matrix.owner;
			m = matrix.m;
			matrix.owner = false;
			return *this;
		}

		Matrix<N, M, T> & operator+=(const Matrix<N, M, T> & matrix) {
			for (u32 i = 0; i < N * M; ++i)
				m[i] += matrix.m[i];
			return *this;
		}

		Matrix<N, M, T> & operator-=(const Matrix<N, M, T> & matrix) {
			for (u32 i = 0; i < N * M; ++i)
				m[i] -= matrix.m[i];
			return *this;
		}

		Vector<M, T> operator[](u32 index) const noexcept {
			return Vector<M, T>(index * M, false);
		}

		~Matrix() {
			clean();
		}
};

template <u32 N, u32 M, typename T>
Matrix<N, M, T> operator+(const Matrix<N, M, T> & left, const Matrix<N, M, T> & right) {
	auto sum = new T[N * M];
	for (u32 i = 0; i < N * M; ++i)
		sum[i] = left.m[i] + right.m[i];
	return sum;
}

template <u32 N, u32 M, typename T>
Matrix<N, M, T> operator-(const Matrix<N, M, T> & left, const Matrix<N, M, T> & right) {
	auto difference = new T[N * M];
	for (u32 i = 0; i < N * M; ++i)
		difference[i] = left.m[i] - right.m[i];
	return difference;
}

template <u32 N, u32 N2, u32 M, typename T>
Matrix<N2, M, T> operator*(const Matrix<N, M, T> & left, const Matrix<N2, N, T> & right) {
	auto product = new T[N2 * M];
	for (u32 i = 0; i < M; ++i) {
		for (u32 j = 0; j < N2; ++j) {
			product[j * M + i] = 0;
			for (u32 k = 0; k < N; ++k)
				product[j * M + i] += left.m[k * M + i] * right.m[j * N + k];
		}
	}
	return product;
}

template <u32 N, u32 M, typename T>
Vector<M, T> operator*(const Matrix<N, M, T> & left, const Vector<N, T> & right) {
	auto product = new T[M];
	for (u32 i = 0; i < M; ++i) {
		product[i] = 0;
		for (u32 j = 0; j < N; ++j) 
			product[i] += left.m[j * M + i] * right.v[j];
	}
	return product;
}

template <u32 N, u32 M, typename T>
std::ostream & operator<<(std::ostream & stream, const Matrix<N, M, T> & matrix) {
	for (u32 i = 0; i < M; ++i) {
		stream << '[';
		for (u32 j = 0; j < N; ++j) {
			stream << matrix.m[j * M + i];
			if (j < N - 1)
				stream << ", ";
		}
		stream << "]\n";
	}
	return stream;
}

template <u32 N, u32 M> using MatrixNMb  = Matrix<N, M,  i8 >;
template <u32 N, u32 M> using MatrixNMub = Matrix<N, M, u8 >;
template <u32 N, u32 M> using MatrixNMs  = Matrix<N, M,  i16>;
template <u32 N, u32 M> using MatrixNMus = Matrix<N, M, u16>;
template <u32 N, u32 M> using MatrixNMi  = Matrix<N, M,  i32>;
template <u32 N, u32 M> using MatrixNMui = Matrix<N, M, u32>;
template <u32 N, u32 M> using MatrixNMl  = Matrix<N, M,  i8 >;
template <u32 N, u32 M> using MatrixNMul = Matrix<N, M, u64>;
template <u32 N, u32 M> using MatrixNMf  = Matrix<N, M, float >;
template <u32 N, u32 M> using MatrixNMd  = Matrix<N, M, double>;

#define CNGE_MATRIX_FORWARD
#endif // CNGE_MATRIX
