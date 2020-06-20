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

#include <iostream>

#include "../types.h"

#ifndef CNGE_VECTOR_FORWARD
#define CNGE_VECTOR_FORWARD

template <u32 N, typename T>
class Vector;

#endif // CNGE_VECTOR_FORWARD

#ifndef CNGE_VECTOR3_FORWARD
#define CNGE_VECTOR3_FORWARD

template <typename T>
class Vector<3, T>;

#endif // CNGE_VECTOR3_FORWARD

#ifndef CNGE_VECTOR4_FORWARD
#define CNGE_VECTOR4_FORWARD

template <typename T>
class Vector<4, T>;

#endif // CNGE_VECTOR4_FORWARD

#ifndef CNGE_VECTOR2
#define CNGE_VECTOR2

#include <ostream>
#include <cmath>

#include "Vector.h"
#include "Vector3.h"
#include "Vector4.h"
#include "VectorDimensions.h"

namespace VectorDimensions {
	extern VectorDimension<0> x;
	extern VectorDimension<1> y;
}

template <typename T>
class Vector<2, T> {
	private:
		bool owner = true;

		void clean() {
			if (owner)
				delete [] v;
		}

	public:
		static Vector<2, T> * newArray(u32 length) {
			T * v = new T[2 * length];
			Vector<2, T> * ret = new Vector<2, T>[length];
			ret[0] = Vector<2, T>(v);
			for (u32 i = 1; i < length; ++i)
				ret[i] = Vector<2, T>(v + i * 2, false);
			return ret;
		}

		T * v;
		
		Vector() :
			v(new T[2]()) {}

		Vector(T * v, bool owner = true) noexcept :
			owner(owner),
			v(v) {}

		explicit Vector(T value) :
			v(new T[2] { value, value }) {}

		Vector(T x, T y) :
			v(new T[2] { x, y }) {}

		template <typename T2>
		Vector(const Vector<2, T2> & vector) :
			v(new T[2] {
				(T) vector.v[0],
				(T) vector.v[1]
			}) {}

		Vector(const Vector<2, T> & vector) :
			v(new T[2] {
				vector.v[0],
				vector.v[1]
			}) {}

		Vector(Vector<2, T> && vector) noexcept :
			owner(vector.owner),
			v(vector.v) {
			vector.owner = false;
		}

		Vector<2, T> & operator=(const Vector<2, T> & vector) {
			v[0] = vector.v[0];
			v[1] = vector.v[1];
			return *this;
		}

		Vector<2, T> & operator=(Vector<2, T> && vector) noexcept {
			if (owner) {
				clean();
				owner = vector.owner;
				v = vector.v;
				vector.owner = false;
			}
			else {
				v[0] = vector.v[0];
				v[1] = vector.v[1];
			}
			return *this;
		}

		Vector<2, T> & operator+=(const Vector<2, T> & vector) {
			v[0] += vector.v[0];
			v[1] += vector.v[1];
			return *this;
		}

		Vector<2, T> & operator-=(const Vector<2, T> & vector) {
			v[0] -= vector.v[0];
			v[1] -= vector.v[1];
			return *this;
		}

		Vector<2, T> & operator*=(T scalar) {
			v[0] *= scalar;
			v[1] *= scalar;
			return *this;
		}

		Vector<2, T> & operator*=(const Vector<2, T> & vector) {
			v[0] *= vector.v[0];
			v[1] *= vector.v[1];
			return *this;
		}

		Vector<2, T> & operator/=(T scalar) {
			v[0] /= scalar;
			v[1] /= scalar;
			return *this;
		}

		Vector<2, T> & operator/=(const Vector<2, T> & vector) {
			v[0] /= vector.v[0];
			v[1] /= vector.v[1];
			return *this;
		}
		
		T & operator[](u32 index) noexcept {
			return v[index];
		}

		const T & operator[](u32 index) const noexcept {
			return v[index];
		}

		template <u32 N>
		T & operator[](const VectorDimensions::VectorDimension<N> & dimension) noexcept {
			static_assert(VectorDimensions::VectorDimension<N>::dimension != 2, "Vector<2, T> has no dimension z!");
			static_assert(VectorDimensions::VectorDimension<N>::dimension != 3, "Vector<2, T> has no dimension w!");
			static_assert(VectorDimensions::VectorDimension<N>::dimension  < 4, "Vector<2, T> has no dimension beyond y!");
			return v[VectorDimensions::VectorDimension<N>::dimension];
		}

		template <u32 N>
		const T & operator[](const VectorDimensions::VectorDimension<N> & dimension) const noexcept {
			static_assert(VectorDimensions::VectorDimension<N>::dimension != 2, "Vector<2, T> has no dimension z!");
			static_assert(VectorDimensions::VectorDimension<N>::dimension != 3, "Vector<2, T> has no dimension w!");
			static_assert(VectorDimensions::VectorDimension<N>::dimension  < 4, "Vector<2, T> has no dimension beyond y!");
			return v[VectorDimensions::VectorDimension<N>::dimension];
		}

		T & x() noexcept {
			return v[0];
		}

		const T & x() const noexcept {
			return v[0];
		}

		T & y() noexcept {
			return v[1];
		}

		const T & y() const noexcept {
			return v[1];
		}

		T getLength() const {
			return sqrt(
				v[0] * v[0] +
				v[1] * v[1]
			);
		}

		T getLengthSquared() const {
			return (
				v[0] * v[0] +
				v[1] * v[1]
			);
		}

		Vector<2, T> & setLength(T length) {
			auto scalar = length / getLength();
			v[0] *= scalar;
			v[1] *= scalar;
			return *this;
		}

		Vector<2, T> & normalize() {
			auto scalar = 1 / getLength();
			v[0] *= scalar;
			v[1] *= scalar;
			return *this;
		}

		T getAngle() const {
			return atan2(v[1], v[0]);
		}
		
		Vector<2, T> & setAngle(T angle) {
			auto length = getLength();
			v[0] = cos(angle) * length;
			v[1] = sin(angle) * length;
			return *this;
		}

		Vector<2, T> & rotate(T angle) {
			auto   sine = sin(angle);
			auto cosine = cos(angle);
			auto x = v[0];
			v[0] = v[0] * cosine - v[1] *   sine;
			v[1] = x    *   sine + v[1] * cosine;
			return *this;
		}

		Vector<2, T> & negate() {
			v[0] = -v[0];
			v[1] = -v[1];
			return *this;
		}

		Vector<2, T> & project(const Vector<2, T> & projection) {
			auto scalar = dot(*this, projection) / projection.getLengthSquared();
			v[0] /= scalar;
			v[1] /= scalar;
			return *this;
		}

		Vector<2, T> & shear(T axisAngle, T shearAngle) {
			auto sinAxis = sin(axisAngle);
			auto cosAxis = cos(axisAngle);
			auto scalar  = v[0] * sinAxis - v[1] * cosAxis;
			     scalar *= tan(shearAngle);
			v[0] += scalar * cosAxis;
			v[1] += scalar * sinAxis;
			return *this;
		}

		Vector<2, T> & shear(const Vector<2, T> & axis, T shearAngle) {
			auto unitAxis = normalize(axis); 
			auto scalar  = v[0] * unitAxis.v[1] - v[1] * unitAxis.v[0];
			     scalar *= tan(shearAngle);
			v[0] += scalar * unitAxis.v[0];
			v[1] += scalar * unitAxis.v[1];
			return *this;
		}

		Vector<2, T> & shear(T axisAngle, const Vector<2, T> & shear) {
			auto sinAxis = sin(axisAngle);
			auto cosAxis = cos(axisAngle);
			auto scalar  = v[0] * sinAxis - v[1] * cosAxis;
			     scalar *= shear.v[1] / shear.v[0];
			v[0] += scalar * cosAxis;
			v[1] += scalar * sinAxis;
			return *this;
		}

		Vector<2, T> & shear(const Vector<2, T> & axis, const Vector<2, T> & shear) {
			auto unitAxis = normalize(axis); 
			auto scalar  = v[0] * unitAxis.v[1] - v[1] * unitAxis.v[0];
			     scalar *= shear.v[1] / shear.v[0];
			v[0] += scalar * unitAxis.v[0];
			v[1] += scalar * unitAxis.v[1];
			return *this;
		}

		Vector<2, T> & shearX(T angle) {
			v[0] -= v[1] * tan(angle);
			return *this;
		}

		Vector<2, T> & shearX(const Vector<2, T> & shear) {
			v[0] -= v[1] * shear.v[1] / shear.v[0];
			return *this;
		}

		Vector<2, T> & shearY(T angle) {
			v[1] += v[0] * tan(angle);
			return *this;
		}

		Vector<2, T> & shearY(const Vector<2, T> & shear) {
			v[1] += v[0] * shear.v[1] / shear.v[0];
			return *this;
		}

		Vector<2, T> & set(const Vector<2, T> & set) {
			v[0] = set.v[0];
			v[1] = set.v[1];
			return *this;
		}

		Vector<2, T> & translate(const Vector<2, T> & translate) {
			v[0] += translate.v[0];
			v[1] += translate.v[1];
			return *this;
		}

		Vector<2, T> & scale(T scale) {
			v[0] *= scale;
			v[1] *= scale;
			return *this;
		}

		Vector<2, T> & scale(const Vector<2, T> & scale) {
			v[0] *= scale.v[0];
			v[1] *= scale.v[1];
			return *this;
		}

		Vector<2, T> & setX(T x) {
			v[0] = x;
			return *this;
		}

		Vector<2, T> & setY(T y) {
			v[1] = y;
			return *this;
		}

		Vector<2, T> & setXY(T x, T y) {
			v[0] = x;
			v[1] = y;
			return *this;
		}

		Vector<2, T> & setXY(const Vector<2, T> & set) {
			v[0] = set.v[0];
			v[1] = set.v[1];
			return *this;
		}

		Vector<2, T> & setYX(const Vector<2, T> & set) {
			v[1] = set.v[0];
			v[0] = set.v[1];
			return *this;
		}

		Vector<2, T> & translateX(T translateX) {
			v[0] += translateX;
			return *this;
		}

		Vector<2, T> & translateY(T translateY) {
			v[1] += translateY;
			return *this;
		}

		Vector<2, T> & translateXY(T translateX, T translateY) {
			v[0] += translateX;
			v[1] += translateY;
			return *this;
		}

		Vector<2, T> & translateXY(const Vector<2, T> & translate) {
			v[0] += translate.v[0];
			v[1] += translate.v[1];
			return *this;
		}

		Vector<2, T> & translateYX(const Vector<2, T> & translate) {
			v[1] += translate.v[0];
			v[0] += translate.v[1];
			return *this;
		}

		Vector<2, T> & scaleX(T scaleX) {
			v[0] *= scaleX;
		}

		Vector<2, T> & scaleY(T scaleY) {
			v[1] *= scaleY;
		}

		Vector<2, T> & scaleXY(T scale) {
			v[0] *= scale;
			v[1] *= scale;
		}

		Vector<2, T> & scaleXY(T scaleX, T scaleY) {
			v[0] *= scaleX;
			v[1] *= scaleY;
		}

		Vector<2, T> & scaleXY(const Vector<2, T> & scale) {
			v[0] *= scale.v[0];
			v[1] *= scale.v[1];
		}

		Vector<2, T> & scaleYX(const Vector<2, T> & scale) {
			v[1] *= scale.v[0];
			v[0] *= scale.v[1];
		}

		T getX() const {
			return v[0];
		}

		T getY() const {
			return v[1];
		}

		Vector<2, T> getXX() const {
			return { v[0], v[0] };
		}

		Vector<2, T> getXY() const {
			return { v[0], v[1] };
		}

		Vector<2, T> getYX() const {
			return { v[1], v[0] };
		}

		Vector<2, T> getYY() const {
			return { v[1], v[1] };
		}

		Vector<3, T> getXXX() const {
			return { v[0], v[0], v[0] };
		}

		Vector<3, T> getXXY() const {
			return { v[0], v[0], v[1] };
		}

		Vector<3, T> getXYX() const {
			return { v[0], v[1], v[0] };
		}

		Vector<3, T> getXYY() const {
			return { v[0], v[1], v[1] };
		}

		Vector<3, T> getYXX() const {
			return { v[1], v[0], v[0] };
		}

		Vector<3, T> getYXY() const {
			return { v[1], v[0], v[1] };
		}

		Vector<3, T> getYYX() const {
			return { v[1], v[1], v[0] };
		}

		Vector<3, T> getYYY() const {
			return { v[1], v[1], v[1] };
		}

		Vector<4, T> getXXXX() const {
			return { v[0], v[0], v[0], v[0] };
		}

		Vector<4, T> getXXXY() const {
			return { v[0], v[0], v[0], v[1] };
		}

		Vector<4, T> getXXYX() const {
			return { v[0], v[0], v[1], v[0] };
		}

		Vector<4, T> getXXYY() const {
			return { v[0], v[0], v[1], v[1] };
		}

		Vector<4, T> getXYXX() const {
			return { v[0], v[1], v[0], v[0] };
		}

		Vector<4, T> getXYXY() const {
			return { v[0], v[1], v[0], v[1] };
		}

		Vector<4, T> getXYYX() const {
			return { v[0], v[1], v[1], v[0] };
		}

		Vector<4, T> getXYYY() const {
			return { v[0], v[1], v[1], v[1] };
		}

		Vector<4, T> getYXXX() const {
			return { v[1], v[0], v[0], v[0] };
		}

		Vector<4, T> getYXXY() const {
			return { v[1], v[0], v[0], v[1] };
		}

		Vector<4, T> getYXYX() const {
			return { v[1], v[0], v[1], v[0] };
		}

		Vector<4, T> getYXYY() const {
			return { v[1], v[0], v[1], v[1] };
		}

		Vector<4, T> getYYXX() const {
			return { v[1], v[1], v[0], v[0] };
		}

		Vector<4, T> getYYXY() const {
			return { v[1], v[1], v[0], v[1] };
		}

		Vector<4, T> getYYYX() const {
			return { v[1], v[1], v[1], v[0] };
		}

		Vector<4, T> getYYYY() const {
			return { v[1], v[1], v[1], v[1] };
		}

		virtual ~Vector() noexcept {
			clean();
		}
};

template <typename T>
Vector<2, T> operator-(const Vector<2, T> & vector) {
	return {
		-vector.v[0],
		-vector.v[1]
	};
}

template <typename T>
Vector<2, T> operator+(const Vector<2, T> & left, const Vector<2, T> & right) {
	return {
		left.v[0] + right.v[0],
		left.v[1] + right.v[1]
	};
}

template <typename T>
Vector<2, T> operator-(const Vector<2, T> & left, const Vector<2, T> & right) {
	return {
		left.v[0] - right.v[0],
		left.v[1] - right.v[1]
	};
}

template <typename T>
Vector<2, T> operator*(T left, const Vector<2, T> & right) {
	return {
		left * right.v[0],
		left * right.v[1]
	};
}

template <typename T>
Vector<2, T> operator*(const Vector<2, T> & left, T right) {
	return {
		left.v[0] * right,
		left.v[1] * right
	};
}

template <typename T>
Vector<2, T> operator*(const Vector<2, T> & left, const Vector<2, T> & right) {
	return {
		left.v[0] * right.v[0],
		left.v[1] * right.v[1]
	};
}

template <typename T>
Vector<2, T> operator/(const Vector<2, T> & left, T right) {
	return {
		left.v[0] / right,
		left.v[1] / right
	};
}

template <typename T>
Vector<2, T> operator/(const Vector<2, T> & left, const Vector<2, T> & right) {
	return {
		left.v[0] / right.v[0],
		left.v[1] / right.v[1]
	};
}

template <typename T>
Vector<3, T> operator|(T left, const Vector<2, T> & right) {
	return {
		left,
		right.v[0],
		right.v[1]
	};
}

template <typename T>
Vector<3, T> operator|(const Vector<2, T> & left, T right) {
	return {
		left.v[0],
		left.v[1],
		right
	};
}

template <typename T>
Vector<4, T> operator|(const Vector<2, T> & left, const Vector<2, T> & right) {
	return {
		left.v[0],
		left.v[1],
		right.v[0],
		right.v[1]
	};
}

template <typename T>
Vector<2, T> setLength(const Vector<2, T> & vector, T length) {
	auto scalar = length / vector.getLength();
	return {
		vector.v[0] * scalar,
		vector.v[1] * scalar
	};
}

template <typename T>
Vector<2, T> & setLength(const Vector<2, T> & vector, T length, Vector<2, T> & target) {
	auto scalar = length / vector.getLength();
	target.v[0] = vector.v[0] * scalar;
	target.v[1] = vector.v[1] * scalar;
	return target;
}

template <typename T>
Vector<2, T> normalize(const Vector<2, T> & vector) {
	auto scalar = 1 / vector.getLength();
	return {
		vector.v[0] * scalar,
		vector.v[1] * scalar
	};
}

template <typename T>
Vector<2, T> & normalize(const Vector<2, T> & vector, Vector<2, T> & target) {
	auto scalar = 1 / vector.getLength();
	target.v[0] = vector.v[0] * scalar;
	target.v[1] = vector.v[1] * scalar;
	return target;
}

template <typename T>
Vector<2, T> setAngle(const Vector<2, T> & vector, T angle) {
	auto length = vector.getLength();
	return {
		cos(angle) * length,
		sin(angle) * length
	};
}

template <typename T>
Vector<2, T> & setAngle(const Vector<2, T> & vector, T angle, Vector<2, T> & target) {
	auto length = vector.getLength();
	target.v[0] = cos(angle) * length;
	target.v[1] = sin(angle) * length;
	return target;
}

template <typename T>
Vector<2, T> rotate(const Vector<2, T> & vector, T angle) {
	auto   sine = sin(angle);
	auto cosine = cos(angle);
	return {
		vector.v[0] * cosine - vector.v[1] *   sine,
		vector.v[0] *   sine + vector.v[1] * cosine
	};
}

template <typename T>
Vector<2, T> & rotate(const Vector<2, T> & vector, T angle, Vector<2, T> & target) {
	auto   sine = sin(angle);
	auto cosine = cos(angle);
	target.v[0] = vector.v[0] * cosine - vector.v[1] *   sine;
	target.v[1] = vector.v[0] *   sine + vector.v[1] * cosine;
	return target;
}

template <typename T>
Vector<2, T> negate(const Vector<2, T> & vector) {
	return {
		-vector.v[0],
		-vector.v[1]
	};
}

template <typename T>
Vector<2, T> & negate(const Vector<2, T> & vector, Vector<2, T> & target) {
	target.v[0] = -vector.v[0];
	target.v[1] = -vector.v[1];
	return target;
}

template <typename T>
T dot(const Vector<2, T> & left, const Vector<2, T> & right) {
	return (
		left.v[0] * right.v[0] +
		left.v[1] * right.v[1]
	);
}

template <typename T>
Vector<2, T> project(const Vector<2, T> & vector, const Vector<2, T> & projection) {
	auto scalar = dot(vector, projection) / projection.getLengthSquared();
	return {
		scalar * projection.v[0],
		scalar * projection.v[1]
	};
}

template <typename T>
Vector<2, T> & project(const Vector<2, T> & vector, const Vector<2, T> & projection, Vector<2, T> & target) {
	auto scalar = dot(vector, projection) / projection.getLengthSquared();
	target.v[0] = scalar * projection.v[0];
	target.v[1] = scalar * projection.v[1];
	return target;
}

template <typename T>
Vector<2, T> shear(const Vector<2, T> & vector, T axisAngle, T shearAngle) {
	auto sinAxis = sin(axisAngle);
	auto cosAxis = cos(axisAngle);
	auto scalar  = vector.v[0] * sinAxis - vector.v[1] * cosAxis;
	     scalar *= tan(shearAngle);
	return {
		vector.v[0] + scalar * cosAxis,
		vector.v[1] + scalar * sinAxis,
	};
}

template <typename T>
Vector<2, T> & shear(const Vector<2, T> & vector, T axisAngle, T shearAngle, Vector<2, T> & target) {
	auto sinAxis = sin(axisAngle);
	auto cosAxis = cos(axisAngle);
	auto scalar  = vector.v[0] * sinAxis - vector.v[1] * cosAxis;
	     scalar *= tan(shearAngle);
	target.v[0]  = vector.v[0] + scalar * cosAxis;
	target.v[1]  = vector.v[1] + scalar * sinAxis;
	return target;
}

template <typename T>
Vector<2, T> shearX(const Vector<2, T> & vector, T angle) {
	return {
		vector.v[0] - vector.v[1] * tan(angle),
		vector.v[1]
	};
}

template <typename T>
Vector<2, T> & shearX(const Vector<2, T> & vector, T angle, Vector<2, T> & target) {
	target.v[0] = vector.v[0] - vector.v[1] * tan(angle);
	target.v[1] = vector.v[1];
	return target;
}

template <typename T>
Vector<2, T> shearY(const Vector<2, T> & vector, T angle) {
	return {
		vector.v[0],
		vector.v[1] + vector.v[0] * tan(angle)
	};
}

template <typename T>
Vector<2, T> & shearY(const Vector<2, T> & vector, T angle, Vector<2, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] + vector.v[0] * tan(angle);
	return target;
}

template <typename T>
Vector<2, T> translate(const Vector<2, T> & vector, const Vector<2, T> & translate) {
	return {
		vector.v[0]	+ translate.v[0],
		vector.v[1] + translate.v[1]
	};
}

template <typename T>
Vector<2, T> & translate(const Vector<2, T> & vector, const Vector<2, T> & translate, Vector<2, T> & target) {
	target.v[0] = vector.v[0] + translate.v[0];
	target.v[1] = vector.v[1] + translate.v[1];
	return target;
}

template <typename T>
Vector<2, T> scale(const Vector<2, T> & vector, T scale) {
	return {
		vector.v[0] * scale,
		vector.v[1] * scale
	};
}

template <typename T>
Vector<2, T> & scale(const Vector<2, T> & vector, T scale, Vector<2, T> & target) {
	target.v[0] = vector.v[0] * scale;
	target.v[1] = vector.v[1] * scale;
	return target;
}

template <typename T>
Vector<2, T> scale(const Vector<2, T> & vector, const Vector<2, T> & scale) {
	return {
		vector.v[0] * scale.v[0],
		vector.v[1] * scale.v[1]
	};
}

template <typename T>
Vector<2, T> & scale(const Vector<2, T> & vector, const Vector<2, T> & scale, Vector<2, T> & target) {
	target.v[0] = vector.v[0] * scale.v[0];
	target.v[1] = vector.v[1] * scale.v[1];
	return target;
}

template<typename T>
Vector<2, T> setX(const Vector<2, T> & vector, T x) {
	return {
		x,
		vector.v[1]
	};
}

template<typename T>
Vector<2, T> setY(const Vector<2, T> & vector, T y) {
	return {
		vector.v[0],
		y
	};
}

template<typename T>
Vector<2, T> setX(const Vector<2, T> & vector, T x, Vector<2, T> & target) {
	target.v[0] = x;
	target.v[1] = vector.v[1];
	return target;
}

template<typename T>
Vector<2, T> setY(const Vector<2, T> & vector, T y, Vector<2, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = y;
	return target;
}

template<typename T>
Vector<2, T> translateX(const Vector<2, T> & vector, T translateX) {
	return {
		vector.v[0] + translateX,
		vector.v[1]
	};
}

template<typename T>
Vector<2, T> translateY(const Vector<2, T> & vector, T translateY) {
	return {
		vector.v[0],
		vector.v[1] + translateY
	};
}

template<typename T>
Vector<2, T> translateXY(const Vector<2, T> & vector, T translateX, T translateY) {
	return {
		vector.v[0] + translateX,
		vector.v[1] + translateY
	};
}

template<typename T>
Vector<2, T> translateXY(const Vector<2, T> & vector, const Vector<2, T> & translate) {
	return {
		vector.v[0] + translate.v[0],
		vector.v[1] + translate.v[1]
	};
}

template<typename T>
Vector<2, T> translateYX(const Vector<2, T> & vector, const Vector<2, T> & translate) {
	return {
		vector.v[0] + translate.v[1],
		vector.v[1] + translate.v[0]
	};
}

template<typename T>
Vector<2, T> translateX(const Vector<2, T> & vector, T translateX, Vector<2, T> & target) {
	target.v[0] = vector.v[0] + translateX;
	target.v[1] = vector.v[1];
	return target;
}

template<typename T>
Vector<2, T> translateY(const Vector<2, T> & vector, T translateY, Vector<2, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] + translateY;
	return target;
}

template<typename T>
Vector<2, T> translateXY(const Vector<2, T> & vector, T translateX, T translateY, Vector<2, T> & target) {
	target.v[0] = vector.v[0] + translateX;
	target.v[1] = vector.v[1] + translateY;
	return target;
}

template<typename T>
Vector<2, T> translateXY(const Vector<2, T> & vector, const Vector<2, T> & translate, Vector<2, T> & target) {
	target.v[0] = vector.v[0] + translate.v[0];
	target.v[1] = vector.v[1] + translate.v[1];
	return target;
}

template<typename T>
Vector<2, T> translateYX(const Vector<2, T> & vector, const Vector<2, T> & translate, Vector<2, T> & target) {
	target.v[0] = vector.v[0] + translate.v[1];
	target.v[1] = vector.v[1] + translate.v[0];
	return target;
}

template<typename T>
Vector<2, T> scaleX(const Vector<2, T> & vector, T scaleX) {
	return {
		vector.v[0] * scaleX,
		vector.v[1]
	};
}

template<typename T>
Vector<2, T> scaleY(const Vector<2, T> & vector, T scaleY) {
	return {
		vector.v[0],
		vector.v[1] * scaleY
	};
}

template<typename T>
Vector<2, T> scaleXY(const Vector<2, T> & vector, T scale) {
	return {
		vector.v[0] * scale,
		vector.v[1] * scale
	};
}

template<typename T>
Vector<2, T> scaleXY(const Vector<2, T> & vector, T scaleX, T scaleY) {
	return {
		vector.v[0] * scaleX,
		vector.v[1] * scaleY
	};
}

template<typename T>
Vector<2, T> scaleXY(const Vector<2, T> & vector, const Vector<2, T> & scale) {
	return {
		vector.v[0] * scale.v[0],
		vector.v[1] * scale.v[1]
	};
}

template<typename T>
Vector<2, T> scaleYX(const Vector<2, T> & vector, const Vector<2, T> & scale) {
	return {
		vector.v[0] * scale.v[1],
		vector.v[1] * scale.v[0]
	};
}

template<typename T>
Vector<2, T> scaleX(const Vector<2, T> & vector, T scaleX, const Vector<2, T> & target) {
	target.v[0] = vector.v[0] * scaleX;
	target.v[1] = vector.v[1];
	return target;
}

template<typename T>
Vector<2, T> scaleY(const Vector<2, T> & vector, T scaleY, const Vector<2, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] * scaleY;
	return target;
}

template<typename T>
Vector<2, T> scaleXY(const Vector<2, T> & vector, T scale, Vector<2, T> & target) {
	target.v[0] = vector.v[0] * scale;
	target.v[1] = vector.v[1] * scale;
	return target;
}

template<typename T>
Vector<2, T> scaleXY(const Vector<2, T> & vector, T scaleX, T scaleY, const Vector<2, T> & target) {
	target.v[0] = vector.v[0] * scaleX;
	target.v[1] = vector.v[1] * scaleY;
	return target;
}

template<typename T>
Vector<2, T> scaleXY(const Vector<2, T> & vector, const Vector<2, T> & scale, Vector<2, T> & target) {
	target.v[0] = vector.v[0] * scale.v[0];
	target.v[1] = vector.v[1] * scale.v[1];
	return target;
}

template<typename T>
Vector<2, T> scaleYX(const Vector<2, T> & vector, const Vector<2, T> & scale, Vector<2, T> & target) {
	target.v[0] = vector.v[0] * scale.v[1];
	target.v[1] = vector.v[1] * scale.v[0];
	return target;
}

template <typename T>
std::ostream & operator<<(std::ostream & stream, const Vector<2, T> & vector) {
	return stream << '<' << vector.v[0] << ", " << vector.v[1] << '>';
}

template <typename T> using Vector2 = Vector<2, T>;

using Vector2b  = Vector2< i8 >;
using Vector2ub = Vector2<u8 >;
using Vector2s  = Vector2< i16>;
using Vector2us = Vector2<u16>;
using Vector2i  = Vector2< i32>;
using Vector2ui = Vector2<u32>;
using Vector2l  = Vector2< i64>;
using Vector2ul = Vector2<u64>;
using Vector2f  = Vector2<float >;
using Vector2d  = Vector2<double>;

#define CNGE_VECTOR2_FORWARD
#endif // CNGE_VECTOR2
