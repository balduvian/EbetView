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

#ifndef CNGE_VECTOR2_FORWARD
#define CNGE_VECTOR2_FORWARD

template <typename T>
class Vector<2, T>;

#endif // CNGE_VECTOR2_FORWARD

#ifndef CNGE_VECTOR4_FORWARD
#define CNGE_VECTOR4_FORWARD

template <typename T>
class Vector<4, T>;

#endif // CNGE_VECTOR4_FORWARD

#ifndef CNGE_VECTOR3
#define CNGE_VECTOR3

#include <ostream>
#include <cmath>

#include "Vector.h"
#include "Vector2.h"
#include "Vector4.h"
#include "VectorDimensions.h"

namespace VectorDimensions {
	extern VectorDimension<2> z;
}

template <typename T>
class Vector<3, T> {
	private:
		bool owner = true;

		void clean() {
			if (owner)
				delete [] v;
		}

	public:
		static Vector<3, T> * newArray(u32 length) {
			T * v = new T[3 * length];
			Vector<3, T> * ret = new Vector<3, T>[length];
			ret[0] = Vector<3, T>(v);
			for (u32 i = 1; i < length; ++i)
				ret[i] = Vector<3, T>(v + i * 3, false);
			return ret;
		}

		T * v;

		Vector() :
			v(new T[3]()) {}
		
		Vector(T * v, bool owner = true) noexcept :
			owner(owner),
			v(v) {}
		
		explicit Vector(T value) :
			v(new T[3] { value, value, value }) {}

		Vector(T x, T y, T z) :
			v(new T[3] { x, y, z }) {}

		template <typename T2>
		Vector(const Vector<3, T2> & vector) :
			v(new T[3] {
				(T) vector.v[0],
				(T) vector.v[1],
				(T) vector.v[2]
			}) {}

		Vector(const Vector<3, T> & vector) :
			v(new T[3] {
				vector.v[0],
				vector.v[1],
				vector.v[2]
			}) {}

		Vector(Vector<3, T> && vector) :
			owner(vector.owner),
			v(vector.v) {
			vector.owner = false;
		}

		Vector<3, T> & operator=(const Vector<3, T> & vector) {
			v[0] = vector.v[0];
			v[1] = vector.v[1];
			v[2] = vector.v[2];
			return *this;
		}

		Vector<3, T> & operator=(Vector<3, T> && vector) {
			if (owner) {
				clean();
				owner = vector.owner;
				v = vector.v;
				vector.owner = false;
			}
			else {
				v[0] = vector.v[0];
				v[1] = vector.v[1];
				v[2] = vector.v[2];
			}
			return *this;
		}

		Vector<3, T> & operator+=(const Vector<3, T> & vector) {
			v[0] += vector.v[0];
			v[1] += vector.v[1];
			v[2] += vector.v[2];
			return *this;
		}

		Vector<3, T> & operator-=(const Vector<3, T> & vector) {
			v[0] -= vector.v[0];
			v[1] -= vector.v[1];
			v[2] -= vector.v[2];
			return *this;
		}

		Vector<3, T> & operator*=(T scalar) {
			v[0] *= scalar;
			v[1] *= scalar;
			v[2] *= scalar;
			return *this;
		}

		Vector<3, T> & operator*=(const Vector<3, T> & vector) {
			v[0] *= vector.v[0];
			v[1] *= vector.v[1];
			v[2] *= vector.v[2];
			return *this;
		}

		Vector<3, T> & operator/=(T scalar) {
			v[0] /= scalar;
			v[1] /= scalar;
			v[2] /= scalar;
			return *this;
		}

		Vector<3, T> & operator/=(const Vector<3, T> & vector) {
			v[0] /= vector.v[0];
			v[1] /= vector.v[1];
			v[2] /= vector.v[2];
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
			static_assert(VectorDimensions::VectorDimension<N>::dimension != 3, "Vector<3, T> has no dimension w!");
			static_assert(VectorDimensions::VectorDimension<N>::dimension  < 4, "Vector<3, T> has no dimension beyond z!");
			return v[VectorDimensions::VectorDimension<N>::dimension];
		}

		template <u32 N>
		const T & operator[](const VectorDimensions::VectorDimension<N> & dimension) const noexcept {
			static_assert(VectorDimensions::VectorDimension<N>::dimension != 3, "Vector<3, T> has no dimension w!");
			static_assert(VectorDimensions::VectorDimension<N>::dimension  < 4, "Vector<3, T> has no dimension beyond z!");
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

		T & z() noexcept {
			return v[2];
		}

		const T & z() const noexcept {
			return v[2];
		}

		T getLength() const {
			return sqrt(
				v[0] * v[0] +
				v[1] * v[1] +
				v[2] * v[2]
			);
		}

		T getLengthSquared() const {
			return (
				v[0] * v[0] +
				v[1] * v[1] +
				v[2] * v[2]
			);
		}

		Vector<3, T> & setLength(T length) {
			auto scalar = length / getLength();
			v[0] *= scalar;
			v[1] *= scalar;
			v[2] *= scalar;
			return *this;
		}

		Vector<3, T> & normalize() {
			auto scalar = 1 / getLength();
			v[0] *= scalar;
			v[1] *= scalar;
			v[2] *= scalar;
			return *this;
		}
		
		Vector<3, T> & negate() {
			v[0] = -v[0];
			v[1] = -v[1];
			v[2] = -v[2];
			return *this;
		}

		Vector<3, T> & project(const Vector<3, T> & projection) {
			auto scalar = dot(*this, projection) / projection.getLengthSquared();
			v[0] /= scalar;
			v[1] /= scalar;
			v[2] /= scalar;
			return *this;
		}

		Vector<3, T> & cross(const Vector<3, T> & cross) {
			T v0 = v[1] * cross.v[2] - v[2] * cross.v[1];
			T v1 = v[2] * cross.v[0] - v[0] * cross.v[2];
			v[2] = v[0] * cross.v[1] - v[1] * cross.v[0];
			v[0] = v0;
			v[1] = v1;
			return *this;
		}

		Vector<3, T> & set(const Vector<3, T> & set) {
			v[0] = set.v[0];
			v[1] = set.v[1];
			v[2] = set.v[2];
			return *this;
		}

		Vector<3, T> & translate(const Vector<3, T> & translate) {
			v[0] += translate.v[0];
			v[1] += translate.v[1];
			v[2] += translate.v[2];
			return *this;
		}
		
		Vector<3, T> & scale(T scale) {
			v[0] *= scale;
			v[1] *= scale;
			v[2] *= scale;
			return *this;
		}
		
		Vector<3, T> & scale(const Vector<3, T> & scale) {
			v[0] *= scale.v[0];
			v[1] *= scale.v[1];
			v[2] *= scale.v[2];
			return *this;
		}

		Vector<3, T> & setX(T x) {
			v[0] = x;
			return *this;
		}

		Vector<3, T> & setY(T y) {
			v[1] = y;
			return *this;
		}

		Vector<3, T> & setZ(T z) {
			v[2] = z;
			return *this;
		}

		Vector<3, T> & setXY(T x, T y) {
			v[0] = x;
			v[1] = y;
			return *this;
		}

		Vector<3, T> & setXY(const Vector<2, T> & set) {
			v[0] = set.v[0];
			v[1] = set.v[1];
			return *this;
		}

		Vector<3, T> & setXZ(T x, T z) {
			v[0] = x;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & setXZ(const Vector<2, T> & set) {
			v[0] = set.v[0];
			v[2] = set.v[1];
			return *this;
		}

		Vector<3, T> & setYX(const Vector<2, T> & set) {
			v[1] = set.v[0];
			v[0] = set.v[1];
			return *this;
		}

		Vector<3, T> & setYZ(T y, T z) {
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & setYZ(const Vector<2, T> & set) {
			v[1] = set.v[0];
			v[2] = set.v[1];
			return *this;
		}

		Vector<3, T> & setZX(const Vector<2, T> & set) {
			v[2] = set.v[0];
			v[0] = set.v[1];
			return *this;
		}

		Vector<3, T> & setZY(const Vector<2, T> & set) {
			v[2] = set.v[0];
			v[1] = set.v[1];
			return *this;
		}

		Vector<3, T> & setXYZ(T x, T y, T z) {
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & setXYZ(const Vector<3, T> & set) {
			v[0] = set.v[0];
			v[1] = set.v[1];
			v[2] = set.v[2];
			return *this;
		}

		Vector<3, T> & setXZY(const Vector<3, T> & set) {
			v[0] = set.v[0];
			v[2] = set.v[1];
			v[1] = set.v[2];
			return *this;
		}

		Vector<3, T> & setYXZ(const Vector<3, T> & set) {
			v[1] = set.v[0];
			v[0] = set.v[1];
			v[2] = set.v[2];
			return *this;
		}

		Vector<3, T> & setYZX(const Vector<3, T> & set) {
			v[1] = set.v[0];
			v[2] = set.v[1];
			v[0] = set.v[2];
			return *this;
		}

		Vector<3, T> & setZXY(const Vector<3, T> & set) {
			v[2] = set.v[0];
			v[0] = set.v[1];
			v[1] = set.v[2];
			return *this;
		}

		Vector<3, T> & setZYX(const Vector<3, T> & set) {
			v[2] = set.v[0];
			v[1] = set.v[1];
			v[0] = set.v[2];
			return *this;
		}

		Vector<3, T> & translateX(T translateX) {
			v[0] += translateX;
		}

		Vector<3, T> & translateY(T translateY) {
			v[1] += translateY;
		}

		Vector<3, T> & translateZ(T translateZ) {
			v[2] += translateZ;
		}

		Vector<3, T> & translateXY(T translateX, T translateY) {
			v[0] += translateX;
			v[1] += translateY;
		}

		Vector<3, T> & translateXY(const Vector<2, T> & translate) {
			v[0] += translate.v[0];
			v[1] += translate.v[1];
		}

		Vector<3, T> & translateXZ(T translateX, T translateZ) {
			v[0] += translateX;
			v[2] += translateZ;
		}

		Vector<3, T> & translateXZ(const Vector<2, T> & translate) {
			v[0] += translate.v[0];
			v[2] += translate.v[1];
		}

		Vector<3, T> & translateYX(const Vector<2, T> & translate) {
			v[1] += translate.v[0];
			v[0] += translate.v[1];
		}

		Vector<3, T> & translateYZ(T translateY, T translateZ) {
			v[1] += translateY;
			v[2] += translateZ;
		}

		Vector<3, T> & translateYZ(const Vector<2, T> & translate) {
			v[1] += translate.v[0];
			v[2] += translate.v[1];
		}

		Vector<3, T> & translateZX(const Vector<2, T> & translate) {
			v[2] += translate.v[0];
			v[0] += translate.v[1];
		}

		Vector<3, T> & translateZY(const Vector<2, T> & translate) {
			v[2] += translate.v[0];
			v[1] += translate.v[1];
		}

		Vector<3, T> & translateXYZ(T translateX, T translateY, T translateZ) {
			v[0] += translateX;
			v[1] += translateY;
			v[2] += translateZ;
		}

		Vector<3, T> & translateXYZ(const Vector<3, T> & translate) {
			v[0] += translate.v[0];
			v[1] += translate.v[1];
			v[2] += translate.v[2];
		}

		Vector<3, T> & translateXZY(const Vector<3, T> & translate) {
			v[0] += translate.v[0];
			v[2] += translate.v[1];
			v[1] += translate.v[2];
		}

		Vector<3, T> & translateYXZ(const Vector<3, T> & translate) {
			v[1] += translate.v[0];
			v[0] += translate.v[1];
			v[2] += translate.v[2];
		}

		Vector<3, T> & translateYZX(const Vector<3, T> & translate) {
			v[1] += translate.v[0];
			v[2] += translate.v[1];
			v[0] += translate.v[2];
		}

		Vector<3, T> & translateZXY(const Vector<3, T> & translate) {
			v[2] += translate.v[0];
			v[0] += translate.v[1];
			v[1] += translate.v[2];
		}

		Vector<3, T> & translateZYX(const Vector<3, T> & translate) {
			v[2] += translate.v[0];
			v[1] += translate.v[1];
			v[0] += translate.v[2];
		}

		Vector<3, T> & rotateX(T rotateX) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto y = v[1] * ( cosX ) + v[2] * (-sinX );
			auto z = v[1] * ( sinX ) + v[2] * ( cosX );
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateY(T rotateY) {
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto x = v[0] * ( cosY ) + v[2] * ( sinY );
			auto z = v[0] * (-sinY ) + v[2] * ( cosY );
			v[0] = x;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateZ(T rotateZ) {
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			auto x = v[0] * ( cosZ ) + v[1] * (-sinZ );
			auto y = v[0] * ( sinZ ) + v[1] * ( cosZ );
			v[0] = x;
			v[1] = y;
			return *this;
		}

		Vector<3, T> & rotateXY(T rotateX, T rotateY) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto x = v[0] * ( cosY ) + v[1] * ( sinY * sinX ) + v[2] * ( sinY * cosX );
			auto y = v[1] * ( cosX ) + v[2] * (-sinX );
			auto z = v[0] * (-sinY ) + v[1] * ( cosY * sinX ) + v[2] * ( cosY * cosX );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateXY(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto x = v[0] * ( cosY ) + v[1] * ( sinY * sinX ) + v[2] * ( sinY * cosX );
			auto y = v[1] * ( cosX ) + v[2] * (-sinX );
			auto z = v[0] * (-sinY ) + v[1] * ( cosY * sinX ) + v[2] * ( cosY * cosX );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateXZ(T rotateX, T rotateZ) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			auto x = v[0] * ( cosZ ) + v[1] * (-sinZ * cosX ) + v[2] * ( sinZ * sinX );
			auto y = v[0] * ( sinZ ) + v[1] * ( cosZ * cosX ) + v[2] * (-cosZ * sinX );
			auto z = v[1] * ( sinX ) + v[2] * ( cosX );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateXZ(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			auto x = v[0] * ( cosZ ) + v[1] * (-sinZ * cosX ) + v[2] * ( sinZ * sinX );
			auto y = v[0] * ( sinZ ) + v[1] * ( cosZ * cosX ) + v[2] * (-cosZ * sinX );
			auto z = v[1] * ( sinX ) + v[2] * ( cosX );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateYX(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto x = v[0] * ( cosY ) + v[2] * ( sinY );
			auto y = v[0] * ( sinX * sinY ) + v[1] * ( cosX ) + v[2] * (-sinX * cosY );
			auto z = v[0] * (-cosX * sinY ) + v[1] * ( sinX ) + v[2] * ( cosX * cosY );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateYZ(T rotateY, T rotateZ) {
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			auto x = v[0] * ( cosZ * cosY ) + v[1] * (-sinZ ) + v[2] * ( cosZ * sinY );
			auto y = v[0] * ( sinZ * cosY ) + v[1] * ( cosZ ) + v[2] * ( sinZ * sinY );
			auto z = v[0] * (-sinY ) + v[2] * ( cosY );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateYZ(const Vector<2, T> & rotate) {
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			auto x = v[0] * ( cosZ * cosY ) + v[1] * (-sinZ ) + v[2] * ( cosZ * sinY );
			auto y = v[0] * ( sinZ * cosY ) + v[1] * ( cosZ ) + v[2] * ( sinZ * sinY );
			auto z = v[0] * (-sinY ) + v[2] * ( cosY );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateZX(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			auto x = v[0] * ( cosZ ) + v[1] * (-sinZ );
			auto y = v[0] * ( cosX * sinZ ) + v[1] * ( cosX * cosZ ) + v[2] * (-sinX );
			auto z = v[0] * ( sinX * sinZ ) + v[1] * ( sinX * cosZ ) + v[2] * ( cosX );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateZY(const Vector<2, T> & rotate) {
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			auto x = v[0] * ( cosY * cosZ ) + v[1] * (-cosY * sinZ ) + v[2] * ( sinY );
			auto y = v[0] * ( sinZ ) + v[1] * ( cosZ );
			auto z = v[0] * (-sinY * cosZ ) + v[1] * ( sinY * sinZ ) + v[2] * ( cosY );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateXYZ(T rotateX, T rotateY, T rotateZ) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			auto x = v[0] * ( cosZ * cosY ) + v[1] * (-sinZ * cosX + cosZ * sinY * sinX ) + v[2] * ( sinZ * sinX + cosZ * sinY * cosX );
			auto y = v[0] * ( sinZ * cosY ) + v[1] * ( cosZ * cosX + sinZ * sinY * sinX ) + v[2] * (-cosZ * sinX + sinZ * sinY * cosX );
			auto z = v[0] * (-sinY ) + v[1] * ( cosY * sinX ) + v[2] * ( cosY * cosX );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateXYZ(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[2]);
			auto cosZ = cos(rotate.v[2]);
			auto x = v[0] * ( cosZ * cosY ) + v[1] * (-sinZ * cosX + cosZ * sinY * sinX ) + v[2] * ( sinZ * sinX + cosZ * sinY * cosX );
			auto y = v[0] * ( sinZ * cosY ) + v[1] * ( cosZ * cosX + sinZ * sinY * sinX ) + v[2] * (-cosZ * sinX + sinZ * sinY * cosX );
			auto z = v[0] * (-sinY ) + v[1] * ( cosY * sinX ) + v[2] * ( cosY * cosX );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateXZY(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[2]);
			auto cosY = cos(rotate.v[2]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			auto x = v[0] * ( cosY * cosZ ) + v[1] * (-cosY * sinZ * cosX + sinY * sinX ) + v[2] * ( cosY * sinZ * sinX + sinY * cosX );
			auto y = v[0] * ( sinZ ) + v[1] * ( cosZ * cosX ) + v[2] * (-cosZ * sinX );
			auto z = v[0] * (-sinY * cosZ ) + v[1] * ( sinY * sinZ * cosX + cosY * sinX ) + v[2] * (-sinY * sinZ * sinX + cosY * cosX );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateYXZ(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[2]);
			auto cosZ = cos(rotate.v[2]);
			auto x = v[0] * ( cosZ * cosY - sinZ * sinX * sinY ) + v[1] * (-sinZ * cosX ) + v[2] * ( cosZ * sinY + sinZ * sinX * cosY );
			auto y = v[0] * ( sinZ * cosY + cosZ * sinX * sinY ) + v[1] * ( cosZ * cosX ) + v[2] * ( sinZ * sinY - cosZ * sinX * cosY );
			auto z = v[0] * (-cosX * sinY ) + v[1] * ( sinX ) + v[2] * ( cosX * cosY );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateYZX(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[2]);
			auto cosX = cos(rotate.v[2]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			auto x = v[0] * ( cosZ * cosY ) + v[1] * (-sinZ ) + v[2] * ( cosZ * sinY );
			auto y = v[0] * ( cosX * sinZ * cosY + sinX * sinY ) + v[1] * ( cosX * cosZ ) + v[2] * ( cosX * sinZ * sinY - sinX * cosY );
			auto z = v[0] * ( sinX * sinZ * cosY - cosX * sinY ) + v[1] * ( sinX * cosZ ) + v[2] * ( sinX * sinZ * sinY + cosX * cosY );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateZXY(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[2]);
			auto cosY = cos(rotate.v[2]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			auto x = v[0] * ( cosY * cosZ + sinY * sinX * sinZ ) + v[1] * (-cosY * sinZ + sinY * sinX * cosZ ) + v[2] * ( sinY * cosX );
			auto y = v[0] * ( cosX * sinZ ) + v[1] * ( cosX * cosZ ) + v[2] * (-sinX );
			auto z = v[0] * (-sinY * cosZ + cosY * sinX * sinZ ) + v[1] * ( sinY * sinZ + cosY * sinX * cosZ ) + v[2] * ( cosY * cosX );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & rotateZYX(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[2]);
			auto cosX = cos(rotate.v[2]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			auto x = v[0] * ( cosY * cosZ ) + v[1] * (-cosY * sinZ ) + v[2] * ( sinY );
			auto y = v[0] * ( sinX * sinY * cosZ + cosX * sinZ ) + v[1] * (-sinX * sinY * sinZ + cosX * cosZ ) + v[2] * (-sinX * cosY );
			auto z = v[0] * (-cosX * sinY * cosZ + sinX * sinZ ) + v[1] * ( cosX * sinY * sinZ + sinX * cosZ ) + v[2] * ( cosX * cosY );
			v[0] = x;
			v[1] = y;
			v[2] = z;
			return *this;
		}

		Vector<3, T> & scaleX(T scaleX) {
			v[0] *= scaleX;
		}

		Vector<3, T> & scaleY(T scaleY) {
			v[1] *= scaleY;
		}

		Vector<3, T> & scaleZ(T scaleZ) {
			v[2] *= scaleZ;
		}

		Vector<3, T> & scaleXY(T scale) {
			v[0] *= scale;
			v[1] *= scale;
		}

		Vector<3, T> & scaleXY(T scaleX, T scaleY) {
			v[0] *= scaleX;
			v[1] *= scaleY;
		}

		Vector<3, T> & scaleXY(const Vector<2, T> & scale) {
			v[0] *= scale.v[0];
			v[1] *= scale.v[1];
		}

		Vector<3, T> & scaleXZ(T scale) {
			v[0] *= scale;
			v[2] *= scale;
		}

		Vector<3, T> & scaleXZ(T scaleX, T scaleZ) {
			v[0] *= scaleX;
			v[2] *= scaleZ;
		}

		Vector<3, T> & scaleXZ(const Vector<2, T> & scale) {
			v[0] *= scale.v[0];
			v[2] *= scale.v[1];
		}

		Vector<3, T> & scaleYX(const Vector<2, T> & scale) {
			v[1] *= scale.v[0];
			v[0] *= scale.v[1];
		}

		Vector<3, T> & scaleYZ(T scale) {
			v[1] *= scale;
			v[2] *= scale;
		}

		Vector<3, T> & scaleYZ(T scaleY, T scaleZ) {
			v[1] *= scaleY;
			v[2] *= scaleZ;
		}

		Vector<3, T> & scaleYZ(const Vector<2, T> & scale) {
			v[1] *= scale.v[0];
			v[2] *= scale.v[1];
		}

		Vector<3, T> & scaleZX(const Vector<2, T> & scale) {
			v[2] *= scale.v[0];
			v[0] *= scale.v[1];
		}

		Vector<3, T> & scaleZY(const Vector<2, T> & scale) {
			v[2] *= scale.v[0];
			v[1] *= scale.v[1];
		}

		Vector<3, T> & scaleXYZ(T scale) {
			v[0] *= scale;
			v[1] *= scale;
			v[2] *= scale;
		}

		Vector<3, T> & scaleXYZ(T scaleX, T scaleY, T scaleZ) {
			v[0] *= scaleX;
			v[1] *= scaleY;
			v[2] *= scaleZ;
		}

		Vector<3, T> & scaleXYZ(const Vector<3, T> & scale) {
			v[0] *= scale.v[0];
			v[1] *= scale.v[1];
			v[2] *= scale.v[2];
		}

		Vector<3, T> & scaleXZY(const Vector<3, T> & scale) {
			v[0] *= scale.v[0];
			v[2] *= scale.v[1];
			v[1] *= scale.v[2];
		}

		Vector<3, T> & scaleYXZ(const Vector<3, T> & scale) {
			v[1] *= scale.v[0];
			v[0] *= scale.v[1];
			v[2] *= scale.v[2];
		}

		Vector<3, T> & scaleYZX(const Vector<3, T> & scale) {
			v[1] *= scale.v[0];
			v[2] *= scale.v[1];
			v[0] *= scale.v[2];
		}

		Vector<3, T> & scaleZXY(const Vector<3, T> & scale) {
			v[2] *= scale.v[0];
			v[0] *= scale.v[1];
			v[1] *= scale.v[2];
		}

		Vector<3, T> & scaleZYX(const Vector<3, T> & scale) {
			v[2] *= scale.v[0];
			v[1] *= scale.v[1];
			v[0] *= scale.v[2];
		}

		T getX() const {
			return v[0];
		}

		T getY() const {
			return v[1];
		}

		T getZ() const {
			return v[2];
		}

		Vector<2, T> getXX() const {
			return { v[0], v[0] };
		}

		Vector<2, T> getXY() const {
			return { v[0], v[1] };
		}

		Vector<2, T> getXZ() const {
			return { v[0], v[2] };
		}

		Vector<2, T> getYX() const {
			return { v[1], v[0] };
		}

		Vector<2, T> getYY() const {
			return { v[1], v[1] };
		}

		Vector<2, T> getYZ() const {
			return { v[1], v[2] };
		}

		Vector<2, T> getZX() const {
			return { v[2], v[0] };
		}

		Vector<2, T> getZY() const {
			return { v[2], v[1] };
		}

		Vector<2, T> getZZ() const {
			return { v[2], v[2] };
		}

		Vector<3, T> getXXX() const {
			return { v[0], v[0], v[0] };
		}

		Vector<3, T> getXXY() const {
			return { v[0], v[0], v[1] };
		}

		Vector<3, T> getXXZ() const {
			return { v[0], v[0], v[2] };
		}

		Vector<3, T> getXYX() const {
			return { v[0], v[1], v[0] };
		}

		Vector<3, T> getXYY() const {
			return { v[0], v[1], v[1] };
		}

		Vector<3, T> getXYZ() const {
			return { v[0], v[1], v[2] };
		}

		Vector<3, T> getXZX() const {
			return { v[0], v[2], v[0] };
		}

		Vector<3, T> getXZY() const {
			return { v[0], v[2], v[1] };
		}

		Vector<3, T> getXZZ() const {
			return { v[0], v[2], v[2] };
		}

		Vector<3, T> getYXX() const {
			return { v[1], v[0], v[0] };
		}

		Vector<3, T> getYXY() const {
			return { v[1], v[0], v[1] };
		}

		Vector<3, T> getYXZ() const {
			return { v[1], v[0], v[2] };
		}

		Vector<3, T> getYYX() const {
			return { v[1], v[1], v[0] };
		}

		Vector<3, T> getYYY() const {
			return { v[1], v[1], v[1] };
		}

		Vector<3, T> getYYZ() const {
			return { v[1], v[1], v[2] };
		}

		Vector<3, T> getYZX() const {
			return { v[1], v[2], v[0] };
		}

		Vector<3, T> getYZY() const {
			return { v[1], v[2], v[1] };
		}

		Vector<3, T> getYZZ() const {
			return { v[1], v[2], v[2] };
		}

		Vector<3, T> getZXX() const {
			return { v[2], v[0], v[0] };
		}

		Vector<3, T> getZXY() const {
			return { v[2], v[0], v[1] };
		}

		Vector<3, T> getZXZ() const {
			return { v[2], v[0], v[2] };
		}

		Vector<3, T> getZYX() const {
			return { v[2], v[1], v[0] };
		}

		Vector<3, T> getZYY() const {
			return { v[2], v[1], v[1] };
		}

		Vector<3, T> getZYZ() const {
			return { v[2], v[1], v[2] };
		}

		Vector<3, T> getZZX() const {
			return { v[2], v[2], v[0] };
		}

		Vector<3, T> getZZY() const {
			return { v[2], v[2], v[1] };
		}

		Vector<3, T> getZZZ() const {
			return { v[2], v[2], v[2] };
		}

		Vector<4, T> getXXXX() const {
			return { v[0], v[0], v[0], v[0] };
		}

		Vector<4, T> getXXXY() const {
			return { v[0], v[0], v[0], v[1] };
		}

		Vector<4, T> getXXXZ() const {
			return { v[0], v[0], v[0], v[2] };
		}

		Vector<4, T> getXXYX() const {
			return { v[0], v[0], v[1], v[0] };
		}

		Vector<4, T> getXXYY() const {
			return { v[0], v[0], v[1], v[1] };
		}

		Vector<4, T> getXXYZ() const {
			return { v[0], v[0], v[1], v[2] };
		}

		Vector<4, T> getXXZX() const {
			return { v[0], v[0], v[2], v[0] };
		}

		Vector<4, T> getXXZY() const {
			return { v[0], v[0], v[2], v[1] };
		}

		Vector<4, T> getXXZZ() const {
			return { v[0], v[0], v[2], v[2] };
		}

		Vector<4, T> getXYXX() const {
			return { v[0], v[1], v[0], v[0] };
		}

		Vector<4, T> getXYXY() const {
			return { v[0], v[1], v[0], v[1] };
		}

		Vector<4, T> getXYXZ() const {
			return { v[0], v[1], v[0], v[2] };
		}

		Vector<4, T> getXYYX() const {
			return { v[0], v[1], v[1], v[0] };
		}

		Vector<4, T> getXYYY() const {
			return { v[0], v[1], v[1], v[1] };
		}

		Vector<4, T> getXYYZ() const {
			return { v[0], v[1], v[1], v[2] };
		}

		Vector<4, T> getXYZX() const {
			return { v[0], v[1], v[2], v[0] };
		}

		Vector<4, T> getXYZY() const {
			return { v[0], v[1], v[2], v[1] };
		}

		Vector<4, T> getXYZZ() const {
			return { v[0], v[1], v[2], v[2] };
		}

		Vector<4, T> getXZXX() const {
			return { v[0], v[2], v[0], v[0] };
		}

		Vector<4, T> getXZXY() const {
			return { v[0], v[2], v[0], v[1] };
		}

		Vector<4, T> getXZXZ() const {
			return { v[0], v[2], v[0], v[2] };
		}

		Vector<4, T> getXZYX() const {
			return { v[0], v[2], v[1], v[0] };
		}

		Vector<4, T> getXZYY() const {
			return { v[0], v[2], v[1], v[1] };
		}

		Vector<4, T> getXZYZ() const {
			return { v[0], v[2], v[1], v[2] };
		}

		Vector<4, T> getXZZX() const {
			return { v[0], v[2], v[2], v[0] };
		}

		Vector<4, T> getXZZY() const {
			return { v[0], v[2], v[2], v[1] };
		}

		Vector<4, T> getXZZZ() const {
			return { v[0], v[2], v[2], v[2] };
		}

		Vector<4, T> getYXXX() const {
			return { v[1], v[0], v[0], v[0] };
		}

		Vector<4, T> getYXXY() const {
			return { v[1], v[0], v[0], v[1] };
		}

		Vector<4, T> getYXXZ() const {
			return { v[1], v[0], v[0], v[2] };
		}

		Vector<4, T> getYXYX() const {
			return { v[1], v[0], v[1], v[0] };
		}

		Vector<4, T> getYXYY() const {
			return { v[1], v[0], v[1], v[1] };
		}

		Vector<4, T> getYXYZ() const {
			return { v[1], v[0], v[1], v[2] };
		}

		Vector<4, T> getYXZX() const {
			return { v[1], v[0], v[2], v[0] };
		}

		Vector<4, T> getYXZY() const {
			return { v[1], v[0], v[2], v[1] };
		}

		Vector<4, T> getYXZZ() const {
			return { v[1], v[0], v[2], v[2] };
		}

		Vector<4, T> getYYXX() const {
			return { v[1], v[1], v[0], v[0] };
		}

		Vector<4, T> getYYXY() const {
			return { v[1], v[1], v[0], v[1] };
		}

		Vector<4, T> getYYXZ() const {
			return { v[1], v[1], v[0], v[2] };
		}

		Vector<4, T> getYYYX() const {
			return { v[1], v[1], v[1], v[0] };
		}

		Vector<4, T> getYYYY() const {
			return { v[1], v[1], v[1], v[1] };
		}

		Vector<4, T> getYYYZ() const {
			return { v[1], v[1], v[1], v[2] };
		}

		Vector<4, T> getYYZX() const {
			return { v[1], v[1], v[2], v[0] };
		}

		Vector<4, T> getYYZY() const {
			return { v[1], v[1], v[2], v[1] };
		}

		Vector<4, T> getYYZZ() const {
			return { v[1], v[1], v[2], v[2] };
		}

		Vector<4, T> getYZXX() const {
			return { v[1], v[2], v[0], v[0] };
		}

		Vector<4, T> getYZXY() const {
			return { v[1], v[2], v[0], v[1] };
		}

		Vector<4, T> getYZXZ() const {
			return { v[1], v[2], v[0], v[2] };
		}

		Vector<4, T> getYZYX() const {
			return { v[1], v[2], v[1], v[0] };
		}

		Vector<4, T> getYZYY() const {
			return { v[1], v[2], v[1], v[1] };
		}

		Vector<4, T> getYZYZ() const {
			return { v[1], v[2], v[1], v[2] };
		}

		Vector<4, T> getYZZX() const {
			return { v[1], v[2], v[2], v[0] };
		}

		Vector<4, T> getYZZY() const {
			return { v[1], v[2], v[2], v[1] };
		}

		Vector<4, T> getYZZZ() const {
			return { v[1], v[2], v[2], v[2] };
		}

		Vector<4, T> getZXXX() const {
			return { v[2], v[0], v[0], v[0] };
		}

		Vector<4, T> getZXXY() const {
			return { v[2], v[0], v[0], v[1] };
		}

		Vector<4, T> getZXXZ() const {
			return { v[2], v[0], v[0], v[2] };
		}

		Vector<4, T> getZXYX() const {
			return { v[2], v[0], v[1], v[0] };
		}

		Vector<4, T> getZXYY() const {
			return { v[2], v[0], v[1], v[1] };
		}

		Vector<4, T> getZXYZ() const {
			return { v[2], v[0], v[1], v[2] };
		}

		Vector<4, T> getZXZX() const {
			return { v[2], v[0], v[2], v[0] };
		}

		Vector<4, T> getZXZY() const {
			return { v[2], v[0], v[2], v[1] };
		}

		Vector<4, T> getZXZZ() const {
			return { v[2], v[0], v[2], v[2] };
		}

		Vector<4, T> getZYXX() const {
			return { v[2], v[1], v[0], v[0] };
		}

		Vector<4, T> getZYXY() const {
			return { v[2], v[1], v[0], v[1] };
		}

		Vector<4, T> getZYXZ() const {
			return { v[2], v[1], v[0], v[2] };
		}

		Vector<4, T> getZYYX() const {
			return { v[2], v[1], v[1], v[0] };
		}

		Vector<4, T> getZYYY() const {
			return { v[2], v[1], v[1], v[1] };
		}

		Vector<4, T> getZYYZ() const {
			return { v[2], v[1], v[1], v[2] };
		}

		Vector<4, T> getZYZX() const {
			return { v[2], v[1], v[2], v[0] };
		}

		Vector<4, T> getZYZY() const {
			return { v[2], v[1], v[2], v[1] };
		}

		Vector<4, T> getZYZZ() const {
			return { v[2], v[1], v[2], v[2] };
		}

		Vector<4, T> getZZXX() const {
			return { v[2], v[2], v[0], v[0] };
		}

		Vector<4, T> getZZXY() const {
			return { v[2], v[2], v[0], v[1] };
		}

		Vector<4, T> getZZXZ() const {
			return { v[2], v[2], v[0], v[2] };
		}

		Vector<4, T> getZZYX() const {
			return { v[2], v[2], v[1], v[0] };
		}

		Vector<4, T> getZZYY() const {
			return { v[2], v[2], v[1], v[1] };
		}

		Vector<4, T> getZZYZ() const {
			return { v[2], v[2], v[1], v[2] };
		}

		Vector<4, T> getZZZX() const {
			return { v[2], v[2], v[2], v[0] };
		}

		Vector<4, T> getZZZY() const {
			return { v[2], v[2], v[2], v[1] };
		}

		Vector<4, T> getZZZZ() const {
			return { v[2], v[2], v[2], v[2] };
		}

		virtual ~Vector() noexcept {
			clean();
		}
};

template <typename T>
Vector<3, T> operator-(const Vector<3, T> & vector) {
	return {
		-vector.v[0],
		-vector.v[1],
		-vector.v[2]
	};
}

template <typename T>
Vector<3, T> operator+(const Vector<3, T> & left, const Vector<3, T> & right) {
	return {
		left.v[0] + right.v[0],
		left.v[1] + right.v[1],
		left.v[2] + right.v[2]
	};
}

template <typename T>
Vector<3, T> operator-(const Vector<3, T> & left, const Vector<3, T> & right) {
	return {
		left.v[0] - right.v[0],
		left.v[1] - right.v[1],
		left.v[2] - right.v[2]
	};
}

template <typename T>
Vector<3, T> operator*(T left, const Vector<3, T> & right) {
	return {
		left * right.v[0],
		left * right.v[1],
		left * right.v[2]
	};
}

template <typename T>
Vector<3, T> operator*(const Vector<3, T> & left, T right) {
	return {
		left.v[0] * right,
		left.v[1] * right,
		left.v[2] * right
	};
}

template <typename T>
Vector<3, T> operator*(const Vector<3, T> & left, const Vector<3, T> & right) {
	return {
		left.v[0] * right.v[0],
		left.v[1] * right.v[1],
		left.v[2] * right.v[2]
	};
}

template <typename T>
Vector<3, T> operator/(const Vector<3, T> & left, T right) {
	return {
		left.v[0] / right,
		left.v[1] / right,
		left.v[2] / right
	};
}

template <typename T>
Vector<3, T> operator/(const Vector<3, T> & left, const Vector<3, T> & right) {
	return {
		left.v[0] / right.v[0],
		left.v[1] / right.v[1],
		left.v[2] / right.v[2]
	};
}

template <typename T>
Vector<4, T> operator|(T left, const Vector<3, T> & right) {
	return {
		left,
		right.v[0],
		right.v[1],
		right.v[2]
	};
}

template <typename T>
Vector<4, T> operator|(const Vector<3, T> & left, T right) {
	return {
		left.v[0],
		left.v[1],
		left.v[2],
		right
	};
}

template <typename T>
Vector<3, T> setLength(const Vector<3, T> & vector, T length) {
	auto scalar = length / vector.getLength();
	return {
		vector.v[0] * scalar,
		vector.v[1] * scalar,
		vector.v[2] * scalar
	};
}

template <typename T>
Vector<3, T> & setLength(const Vector<3, T> & vector, T length, Vector<3, T> & target) {
	auto scalar = length / vector.getLength();
	target.v[0] = vector.v[0] * scalar;
	target.v[1] = vector.v[1] * scalar;
	target.v[2] = vector.v[2] * scalar;
	return target;
}

template <typename T>
Vector<3, T> normalize(const Vector<3, T> & vector) {
	auto scalar = 1 / vector.getLength();
	return {
		vector.v[0] * scalar,
		vector.v[1] * scalar,
		vector.v[2] * scalar
	};
}

template <typename T>
Vector<3, T> & normalize(const Vector<3, T> & vector, Vector<3, T> & target) {
	auto scalar = 1 / vector.getLength();
	target.v[0] = vector.v[0] * scalar;
	target.v[1] = vector.v[1] * scalar;
	target.v[2] = vector.v[2] * scalar;
	return target;
}

template <typename T>
Vector<3, T> negate(const Vector<3, T> & vector) {
	return {
		-vector.v[0],
		-vector.v[1],
		-vector.v[2]
	};
}

template <typename T>
Vector<3, T> & negate(const Vector<3, T> & vector, Vector<3, T> & target) {
	target.v[0] = -vector.v[0];
	target.v[1] = -vector.v[1];
	target.v[2] = -vector.v[2];
	return target;
}

template <typename T>
T dot(const Vector<3, T> & left, const Vector<3, T> & right) {
	return (
		left.v[0] * right.v[0] +
		left.v[1] * right.v[1] +
		left.v[2] * right.v[2]
	);
}

template <typename T>
Vector<3, T> project(const Vector<3, T> & vector, const Vector<3, T> & projection) {
	auto scalar = dot(vector, projection) / projection.getLengthSquared();
	return {
		scalar * projection.v[0],
		scalar * projection.v[1],
		scalar * projection.v[2]
	};
}

template <typename T>
Vector<3, T> & project(const Vector<3, T> & vector, const Vector<3, T> & projection, Vector<3, T> & target) {
	auto scalar = dot(vector, projection) / projection.getLengthSquared();
	target.v[0] = scalar * projection.v[0];
	target.v[1] = scalar * projection.v[1];
	target.v[2] = scalar * projection.v[2];
	return target;
}

template <typename T>
Vector<3, T> cross(const Vector<3, T> & vector, const Vector<3, T> & cross) {
	return {
		vector.v[1] * cross.v[2] - vector.v[2] * cross.v[1],
		vector.v[2] * cross.v[0] - vector.v[0] * cross.v[2],
		vector.v[0] * cross.v[1] - vector.v[1] * cross.v[0]
	};
}

template <typename T>
Vector<3, T> & cross(const Vector<3, T> & vector, const Vector<3, T> & cross, Vector<3, T> & target) {
	target.v[0] = vector.v[1] * cross.v[2] - vector.v[2] * cross.v[1];
	target.v[1] = vector.v[2] * cross.v[0] - vector.v[0] * cross.v[2];
	target.v[2] = vector.v[0] * cross.v[1] - vector.v[1] * cross.v[0];
	return target;
}

template <typename T>
Vector<3, T> translate(const Vector<3, T> & vector, const Vector<3, T> & translate) {
	return {
		vector.v[0]	+ translate.v[0],
		vector.v[1] + translate.v[1],
		vector.v[2] + translate.v[2]
	};
}

template <typename T>
Vector<3, T> & translate(const Vector<3, T> & vector, const Vector<3, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[0];
	target.v[1] = vector.v[1] + translate.v[1];
	target.v[2] = vector.v[2] + translate.v[2];
	return target;
}

template <typename T>
Vector<3, T> scale(const Vector<3, T> & vector, T scale) {
	return {
		vector.v[0] * scale,
		vector.v[1] * scale,
		vector.v[2] * scale
	};
}

template <typename T>
Vector<3, T> & scale(const Vector<3, T> & vector, T scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale;
	target.v[1] = vector.v[1] * scale;
	target.v[2] = vector.v[2] * scale;
	return target;
}

template <typename T>
Vector<3, T> scale(const Vector<3, T> & vector, const Vector<3, T> & scale) {
	return {
		vector.v[0] * scale.v[0],
		vector.v[1] * scale.v[1],
		vector.v[2] * scale.v[2]
	};
}

template <typename T>
Vector<3, T> & scale(const Vector<3, T> & vector, const Vector<3, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[0];
	target.v[1] = vector.v[1] * scale.v[1];
	target.v[2] = vector.v[2] * scale.v[2];
	return target;
}

template<typename T>
Vector<3, T> setX(const Vector<3, T> & vector, T x) {
	return {
		x,
		vector.v[1],
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> setY(const Vector<3, T> & vector, T y) {
	return {
		vector.v[0],
		y,
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> setZ(const Vector<3, T> & vector, T z) {
	return {
		vector.v[0],
		vector.v[1],
		z
	};
}

template<typename T>
Vector<3, T> setXY(const Vector<3, T> & vector, T x, T y) {
	return {
		x,
		y,
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> setXY(const Vector<3, T> & vector, const Vector<2, T> & set) {
	return {
		   set.v[0],
		   set.v[1],
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> setXZ(const Vector<3, T> & vector, T x, T z) {
	return {
		x,
		vector.v[1],
		z
	};
}

template<typename T>
Vector<3, T> setXZ(const Vector<3, T> & vector, const Vector<2, T> & set) {
	return {
		   set.v[0],
		vector.v[1],
		   set.v[1]
	};
}

template<typename T>
Vector<3, T> setYX(const Vector<3, T> & vector, const Vector<2, T> & set) {
	return {
		   set.v[1],
		   set.v[0],
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> setYZ(const Vector<3, T> & vector, T y, T z) {
	return {
		vector.v[0],
		y,
		z
	};
}

template<typename T>
Vector<3, T> setYZ(const Vector<3, T> & vector, const Vector<2, T> & set) {
	return {
		vector.v[0],
		   set.v[0],
		   set.v[1]
	};
}

template<typename T>
Vector<3, T> setZX(const Vector<3, T> & vector, const Vector<2, T> & set) {
	return {
		   set.v[1],
		vector.v[1],
		   set.v[0]
	};
}

template<typename T>
Vector<3, T> setZY(const Vector<3, T> & vector, const Vector<2, T> & set) {
	return {
		vector.v[0],
		   set.v[1],
		   set.v[0]
	};
}

template<typename T>
Vector<3, T> setX(const Vector<3, T> & vector, T x, Vector<3, T> & target) {
	target.v[0] = x;
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> setY(const Vector<3, T> & vector, T y, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = y;
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> setZ(const Vector<3, T> & vector, T z, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1];
	target.v[2] = z;
	return target;
}

template<typename T>
Vector<3, T> setXY(const Vector<3, T> & vector, T x, T y, Vector<3, T> & target) {
	target.v[0] = x;
	target.v[1] = y;
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> setXY(const Vector<3, T> & vector, const Vector<2, T> & set, Vector<3, T> & target) {
	target.v[0] =    set.v[0];
	target.v[1] =    set.v[1];
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> setXZ(const Vector<3, T> & vector, T x, T z, Vector<3, T> & target) {
	target.v[0] = x;
	target.v[1] = vector.v[1];
	target.v[2] = z;
	return target;
}

template<typename T>
Vector<3, T> setXZ(const Vector<3, T> & vector, const Vector<2, T> & set, Vector<3, T> & target) {
	target.v[0] =    set.v[0];
	target.v[1] = vector.v[1];
	target.v[2] =    set.v[1];
	return target;
}

template<typename T>
Vector<3, T> setYX(const Vector<3, T> & vector, const Vector<2, T> & set, Vector<3, T> & target) {
	target.v[0] =    set.v[1];
	target.v[1] =    set.v[0];
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> setYZ(const Vector<3, T> & vector, T y, T z, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = y;
	target.v[2] = z;
	return target;
}

template<typename T>
Vector<3, T> setYZ(const Vector<3, T> & vector, const Vector<2, T> & set, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] =    set.v[0];
	target.v[2] =    set.v[1];
	return target;
}

template<typename T>
Vector<3, T> setZX(const Vector<3, T> & vector, const Vector<2, T> & set, Vector<3, T> & target) {
	target.v[0] =    set.v[1];
	target.v[1] = vector.v[1];
	target.v[2] =    set.v[0];
	return target;
}

template<typename T>
Vector<3, T> setZY(const Vector<3, T> & vector, const Vector<2, T> & set, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] =    set.v[1];
	target.v[2] =    set.v[0];
	return target;
}

template<typename T>
Vector<3, T> translateX(const Vector<3, T> & vector, T translateX) {
	return {
		vector.v[0] + translateX,
		vector.v[1],
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> translateY(const Vector<3, T> & vector, T translateY) {
	return {
		vector.v[0],
		vector.v[1] + translateY,
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> translateZ(const Vector<3, T> & vector, T translateZ) {
	return {
		vector.v[0],
		vector.v[1],
		vector.v[2] + translateZ
	};
}

template<typename T>
Vector<3, T> translateXY(const Vector<3, T> & vector, T translateX, T translateY) {
	return {
		vector.v[0] + translateX,
		vector.v[1] + translateY,
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> translateXY(const Vector<3, T> & vector, const Vector<2, T> & translate) {
	return {
		vector.v[0] + translate.v[0],
		vector.v[1] + translate.v[1],
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> translateXZ(const Vector<3, T> & vector, T translateX, T translateZ) {
	return {
		vector.v[0] + translateX,
		vector.v[1],
		vector.v[2] + translateZ
	};
}

template<typename T>
Vector<3, T> translateXZ(const Vector<3, T> & vector, const Vector<2, T> & translate) {
	return {
		vector.v[0] + translate.v[0],
		vector.v[1],
		vector.v[2] + translate.v[1]
	};
}

template<typename T>
Vector<3, T> translateYX(const Vector<3, T> & vector, const Vector<2, T> & translate) {
	return {
		vector.v[0] + translate.v[1],
		vector.v[1] + translate.v[0],
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> translateYZ(const Vector<3, T> & vector, T translateY, T translateZ) {
	return {
		vector.v[0],
		vector.v[1] + translateY,
		vector.v[2] + translateZ
	};
}

template<typename T>
Vector<3, T> translateYZ(const Vector<3, T> & vector, const Vector<2, T> & translate) {
	return {
		vector.v[0],
		vector.v[1] + translate.v[0],
		vector.v[2] + translate.v[1]
	};
}

template<typename T>
Vector<3, T> translateZX(const Vector<3, T> & vector, const Vector<2, T> & translate) {
	return {
		vector.v[0] + translate.v[1],
		vector.v[1],
		vector.v[2] + translate.v[0]
	};
}

template<typename T>
Vector<3, T> translateZY(const Vector<3, T> & vector, const Vector<2, T> & translate) {
	return {
		vector.v[0],
		vector.v[1] + translate.v[1],
		vector.v[2] + translate.v[0]
	};
}

template<typename T>
Vector<3, T> translateXYZ(const Vector<3, T> & vector, T translateX, T translateY, T translateZ) {
	return {
		vector.v[0] + translateX,
		vector.v[1] + translateY,
		vector.v[2] + translateZ
	};
}

template<typename T>
Vector<3, T> translateXYZ(const Vector<3, T> & vector, const Vector<3, T> & translate) {
	return {
		vector.v[0] + translate.v[0],
		vector.v[1] + translate.v[1],
		vector.v[2] + translate.v[2]
	};
}

template<typename T>
Vector<3, T> translateXZY(const Vector<3, T> & vector, const Vector<3, T> & translate) {
	return {
		vector.v[0] + translate.v[0],
		vector.v[1] + translate.v[2],
		vector.v[2] + translate.v[1]
	};
}

template<typename T>
Vector<3, T> translateYXZ(const Vector<3, T> & vector, const Vector<3, T> & translate) {
	return {
		vector.v[0] + translate.v[1],
		vector.v[1] + translate.v[0],
		vector.v[2] + translate.v[2]
	};
}

template<typename T>
Vector<3, T> translateYZX(const Vector<3, T> & vector, const Vector<3, T> & translate) {
	return {
		vector.v[0] + translate.v[2],
		vector.v[1] + translate.v[0],
		vector.v[2] + translate.v[1]
	};
}

template<typename T>
Vector<3, T> translateZXY(const Vector<3, T> & vector, const Vector<3, T> & translate) {
	return {
		vector.v[0] + translate.v[1],
		vector.v[1] + translate.v[2],
		vector.v[2] + translate.v[0]
	};
}

template<typename T>
Vector<3, T> translateZYX(const Vector<3, T> & vector, const Vector<3, T> & translate) {
	return {
		vector.v[0] + translate.v[2],
		vector.v[1] + translate.v[1],
		vector.v[2] + translate.v[0]
	};
}

template<typename T>
Vector<3, T> translateX(const Vector<3, T> & vector, T translateX, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translateX;
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> translateY(const Vector<3, T> & vector, T translateY, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] + translateY;
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> translateZ(const Vector<3, T> & vector, T translateZ, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2] + translateZ;
	return target;
}

template<typename T>
Vector<3, T> translateXY(const Vector<3, T> & vector, T translateX, T translateY, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translateX;
	target.v[1] = vector.v[1] + translateY;
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> translateXY(const Vector<3, T> & vector, const Vector<2, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[0];
	target.v[1] = vector.v[1] + translate.v[1];
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> translateXZ(const Vector<3, T> & vector, T translateX, T translateZ, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translateX;
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2] + translateZ;
	return target;
}

template<typename T>
Vector<3, T> translateXZ(const Vector<3, T> & vector, const Vector<2, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[0];
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2] + translate.v[1];
	return target;
}

template<typename T>
Vector<3, T> translateYX(const Vector<3, T> & vector, const Vector<2, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[1];
	target.v[1] = vector.v[1] + translate.v[0];
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> translateYZ(const Vector<3, T> & vector, T translateY, T translateZ, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] + translateY;
	target.v[2] = vector.v[2] + translateZ;
	return target;
}

template<typename T>
Vector<3, T> translateYZ(const Vector<3, T> & vector, const Vector<2, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] + translate.v[0];
	target.v[2] = vector.v[2] + translate.v[1];
	return target;
}

template<typename T>
Vector<3, T> translateZX(const Vector<3, T> & vector, const Vector<2, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[1];
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2] + translate.v[0];
	return target;
}

template<typename T>
Vector<3, T> translateZY(const Vector<3, T> & vector, const Vector<2, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] + translate.v[1];
	target.v[2] = vector.v[2] + translate.v[0];
	return target;
}

template<typename T>
Vector<3, T> translateXYZ(const Vector<3, T> & vector, T translateX, T translateY, T translateZ, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translateX;
	target.v[1] = vector.v[1] + translateY;
	target.v[2] = vector.v[2] + translateZ;
	return target;
}

template<typename T>
Vector<3, T> translateXYZ(const Vector<3, T> & vector, const Vector<3, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[0];
	target.v[1] = vector.v[1] + translate.v[1];
	target.v[2] = vector.v[2] + translate.v[2];
	return target;
}

template<typename T>
Vector<3, T> translateXZY(const Vector<3, T> & vector, const Vector<3, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[0];
	target.v[1] = vector.v[1] + translate.v[2];
	target.v[2] = vector.v[2] + translate.v[1];
	return target;
}

template<typename T>
Vector<3, T> translateYXZ(const Vector<3, T> & vector, const Vector<3, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[1];
	target.v[1] = vector.v[1] + translate.v[0];
	target.v[2] = vector.v[2] + translate.v[2];
	return target;
}

template<typename T>
Vector<3, T> translateYZX(const Vector<3, T> & vector, const Vector<3, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[2];
	target.v[1] = vector.v[1] + translate.v[0];
	target.v[2] = vector.v[2] + translate.v[1];
	return target;
}

template<typename T>
Vector<3, T> translateZXY(const Vector<3, T> & vector, const Vector<3, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[1];
	target.v[1] = vector.v[1] + translate.v[2];
	target.v[2] = vector.v[2] + translate.v[0];
	return target;
}

template<typename T>
Vector<3, T> translateZYX(const Vector<3, T> & vector, const Vector<3, T> & translate, Vector<3, T> & target) {
	target.v[0] = vector.v[0] + translate.v[2];
	target.v[1] = vector.v[1] + translate.v[1];
	target.v[2] = vector.v[2] + translate.v[0];
	return target;
}

template<typename T>
Vector<3, T> & rotateX(const Vector<3, T> & vector, T rotateX) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	return {
		vector.v[0],
		vector.v[1] * ( cosX ) + vector.v[2] * (-sinX ),
		vector.v[1] * ( sinX ) + vector.v[2] * ( cosX )
	};
}

template<typename T>
Vector<3, T> & rotateY(const Vector<3, T> & vector, T rotateY) {
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	return {
		vector.v[0] * ( cosY ) + vector.v[2] * ( sinY ),
		vector.v[1],
		vector.v[0] * (-sinY ) + vector.v[2] * ( cosY )
	};
}

template<typename T>
Vector<3, T> & rotateZ(const Vector<3, T> & vector, T rotateZ) {
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	return {
		vector.v[0] * ( cosZ ) + vector.v[1] * (-sinZ ),
		vector.v[0] * ( sinZ ) + vector.v[1] * ( cosZ ),
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> & rotateXY(const Vector<3, T> & vector, T rotateX, T rotateY) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	return {
		vector.v[0] * ( cosY ) + vector.v[1] * ( sinY * sinX ) + vector.v[2] * ( sinY * cosX ),
		vector.v[1] * ( cosX ) + vector.v[2] * (-sinX ),
		vector.v[0] * (-sinY ) + vector.v[1] * ( cosY * sinX ) + vector.v[2] * ( cosY * cosX )
	};
}

template<typename T>
Vector<3, T> & rotateXY(const Vector<3, T> & vector, const Vector<2, T> & rotate) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	return {
		vector.v[0] * ( cosY ) + vector.v[1] * ( sinY * sinX ) + vector.v[2] * ( sinY * cosX ),
		vector.v[1] * ( cosX ) + vector.v[2] * (-sinX ),
		vector.v[0] * (-sinY ) + vector.v[1] * ( cosY * sinX ) + vector.v[2] * ( cosY * cosX )
	};
}

template<typename T>
Vector<3, T> & rotateXZ(const Vector<3, T> & vector, T rotateX, T rotateZ) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	return {
		vector.v[0] * ( cosZ ) + vector.v[1] * (-sinZ * cosX ) + vector.v[2] * ( sinZ * sinX ),
		vector.v[0] * ( sinZ ) + vector.v[1] * ( cosZ * cosX ) + vector.v[2] * (-cosZ * sinX ),
		vector.v[1] * ( sinX ) + vector.v[2] * ( cosX )
	};
}

template<typename T>
Vector<3, T> & rotateXZ(const Vector<3, T> & vector, const Vector<2, T> & rotate) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	return {
		vector.v[0] * ( cosZ ) + vector.v[1] * (-sinZ * cosX ) + vector.v[2] * ( sinZ * sinX ),
		vector.v[0] * ( sinZ ) + vector.v[1] * ( cosZ * cosX ) + vector.v[2] * (-cosZ * sinX ),
		vector.v[1] * ( sinX ) + vector.v[2] * ( cosX )
	};
}

template<typename T>
Vector<3, T> & rotateYX(const Vector<3, T> & vector, const Vector<2, T> & rotate) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	return {
		vector.v[0] * ( cosY ) + vector.v[2] * ( sinY ),
		vector.v[0] * ( sinX * sinY ) + vector.v[1] * ( cosX ) + vector.v[2] * (-sinX * cosY ),
		vector.v[0] * (-cosX * sinY ) + vector.v[1] * ( sinX ) + vector.v[2] * ( cosX * cosY )
	};
}

template<typename T>
Vector<3, T> & rotateYZ(const Vector<3, T> & vector, T rotateY, T rotateZ) {
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	return {
		vector.v[0] * ( cosZ * cosY ) + vector.v[1] * (-sinZ ) + vector.v[2] * ( cosZ * sinY ),
		vector.v[0] * ( sinZ * cosY ) + vector.v[1] * ( cosZ ) + vector.v[2] * ( sinZ * sinY ),
		vector.v[0] * (-sinY ) + vector.v[2] * ( cosY )
	};
}

template<typename T>
Vector<3, T> & rotateYZ(const Vector<3, T> & vector, const Vector<2, T> & rotate) {
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	return {
		vector.v[0] * ( cosZ * cosY ) + vector.v[1] * (-sinZ ) + vector.v[2] * ( cosZ * sinY ),
		vector.v[0] * ( sinZ * cosY ) + vector.v[1] * ( cosZ ) + vector.v[2] * ( sinZ * sinY ),
		vector.v[0] * (-sinY ) + vector.v[2] * ( cosY )
	};
}

template<typename T>
Vector<3, T> & rotateZX(const Vector<3, T> & vector, const Vector<2, T> & rotate) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	return {
		vector.v[0] * ( cosZ ) + vector.v[1] * (-sinZ ),
		vector.v[0] * ( cosX * sinZ ) + vector.v[1] * ( cosX * cosZ ) + vector.v[2] * (-sinX ),
		vector.v[0] * ( sinX * sinZ ) + vector.v[1] * ( sinX * cosZ ) + vector.v[2] * ( cosX )
	};
}

template<typename T>
Vector<3, T> & rotateZY(const Vector<3, T> & vector, const Vector<2, T> & rotate) {
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	return {
		vector.v[0] * ( cosY * cosZ ) + vector.v[1] * (-cosY * sinZ ) + vector.v[2] * ( sinY ),
		vector.v[0] * ( sinZ ) + vector.v[1] * ( cosZ ),
		vector.v[0] * (-sinY * cosZ ) + vector.v[1] * ( sinY * sinZ ) + vector.v[2] * ( cosY )
	};
}

template<typename T>
Vector<3, T> & rotateXYZ(const Vector<3, T> & vector, T rotateX, T rotateY, T rotateZ) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	return {
		vector.v[0] * ( cosZ * cosY ) + vector.v[1] * (-sinZ * cosX + cosZ * sinY * sinX ) + vector.v[2] * ( sinZ * sinX + cosZ * sinY * cosX ),
		vector.v[0] * ( sinZ * cosY ) + vector.v[1] * ( cosZ * cosX + sinZ * sinY * sinX ) + vector.v[2] * (-cosZ * sinX + sinZ * sinY * cosX ),
		vector.v[0] * (-sinY ) + vector.v[1] * ( cosY * sinX ) + vector.v[2] * ( cosY * cosX )
	};
}

template<typename T>
Vector<3, T> & rotateXYZ(const Vector<3, T> & vector, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[2]);
	auto cosZ = cos(rotate.v[2]);
	return {
		vector.v[0] * ( cosZ * cosY ) + vector.v[1] * (-sinZ * cosX + cosZ * sinY * sinX ) + vector.v[2] * ( sinZ * sinX + cosZ * sinY * cosX ),
		vector.v[0] * ( sinZ * cosY ) + vector.v[1] * ( cosZ * cosX + sinZ * sinY * sinX ) + vector.v[2] * (-cosZ * sinX + sinZ * sinY * cosX ),
		vector.v[0] * (-sinY ) + vector.v[1] * ( cosY * sinX ) + vector.v[2] * ( cosY * cosX )
	};
}

template<typename T>
Vector<3, T> & rotateXZY(const Vector<3, T> & vector, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[2]);
	auto cosY = cos(rotate.v[2]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	return {
		vector.v[0] * ( cosY * cosZ ) + vector.v[1] * (-cosY * sinZ * cosX + sinY * sinX ) + vector.v[2] * ( cosY * sinZ * sinX + sinY * cosX ),
		vector.v[0] * ( sinZ ) + vector.v[1] * ( cosZ * cosX ) + vector.v[2] * (-cosZ * sinX ),
		vector.v[0] * (-sinY * cosZ ) + vector.v[1] * ( sinY * sinZ * cosX + cosY * sinX ) + vector.v[2] * (-sinY * sinZ * sinX + cosY * cosX )
	};
}

template<typename T>
Vector<3, T> & rotateYXZ(const Vector<3, T> & vector, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[2]);
	auto cosZ = cos(rotate.v[2]);
	return {
		vector.v[0] * ( cosZ * cosY - sinZ * sinX * sinY ) + vector.v[1] * (-sinZ * cosX ) + vector.v[2] * ( cosZ * sinY + sinZ * sinX * cosY ),
		vector.v[0] * ( sinZ * cosY + cosZ * sinX * sinY ) + vector.v[1] * ( cosZ * cosX ) + vector.v[2] * ( sinZ * sinY - cosZ * sinX * cosY ),
		vector.v[0] * (-cosX * sinY ) + vector.v[1] * ( sinX ) + vector.v[2] * ( cosX * cosY )
	};
}

template<typename T>
Vector<3, T> & rotateYZX(const Vector<3, T> & vector, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[2]);
	auto cosX = cos(rotate.v[2]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	return {
		vector.v[0] * ( cosZ * cosY ) + vector.v[1] * (-sinZ ) + vector.v[2] * ( cosZ * sinY ),
		vector.v[0] * ( cosX * sinZ * cosY + sinX * sinY ) + vector.v[1] * ( cosX * cosZ ) + vector.v[2] * ( cosX * sinZ * sinY - sinX * cosY ),
		vector.v[0] * ( sinX * sinZ * cosY - cosX * sinY ) + vector.v[1] * ( sinX * cosZ ) + vector.v[2] * ( sinX * sinZ * sinY + cosX * cosY )
	};
}

template<typename T>
Vector<3, T> & rotateZXY(const Vector<3, T> & vector, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[2]);
	auto cosY = cos(rotate.v[2]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	return {
		vector.v[0] * ( cosY * cosZ + sinY * sinX * sinZ ) + vector.v[1] * (-cosY * sinZ + sinY * sinX * cosZ ) + vector.v[2] * ( sinY * cosX ),
		vector.v[0] * ( cosX * sinZ ) + vector.v[1] * ( cosX * cosZ ) + vector.v[2] * (-sinX ),
		vector.v[0] * (-sinY * cosZ + cosY * sinX * sinZ ) + vector.v[1] * ( sinY * sinZ + cosY * sinX * cosZ ) + vector.v[2] * ( cosY * cosX )
	};
}

template<typename T>
Vector<3, T> & rotateZYX(const Vector<3, T> & vector, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[2]);
	auto cosX = cos(rotate.v[2]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	return {
		vector.v[0] * ( cosY * cosZ ) + vector.v[1] * (-cosY * sinZ ) + vector.v[2] * ( sinY ),
		vector.v[0] * ( sinX * sinY * cosZ + cosX * sinZ ) + vector.v[1] * (-sinX * sinY * sinZ + cosX * cosZ ) + vector.v[2] * (-sinX * cosY ),
		vector.v[0] * (-cosX * sinY * cosZ + sinX * sinZ ) + vector.v[1] * ( cosX * sinY * sinZ + sinX * cosZ ) + vector.v[2] * ( cosX * cosY )
	};
}

template<typename T>
Vector<3, T> & rotateX(const Vector<3, T> & vector, T rotateX, Vector<3, T> & target) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] * ( cosX ) + vector.v[2] * (-sinX );
	target.v[2] = vector.v[1] * ( sinX ) + vector.v[2] * ( cosX );
	return target;
}

template<typename T>
Vector<3, T> & rotateY(const Vector<3, T> & vector, T rotateY, Vector<3, T> & target) {
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	target.v[0] = vector.v[0] * ( cosY ) + vector.v[2] * ( sinY );
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[0] * (-sinY ) + vector.v[2] * ( cosY );
	return target;
}

template<typename T>
Vector<3, T> & rotateZ(const Vector<3, T> & vector, T rotateZ, Vector<3, T> & target) {
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	target.v[0] = vector.v[0] * ( cosZ ) + vector.v[1] * (-sinZ );
	target.v[1] = vector.v[0] * ( sinZ ) + vector.v[1] * ( cosZ );
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> & rotateXY(const Vector<3, T> & vector, T rotateX, T rotateY, Vector<3, T> & target) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	target.v[0] = vector.v[0] * ( cosY ) + vector.v[1] * ( sinY * sinX ) + vector.v[2] * ( sinY * cosX );
	target.v[1] = vector.v[1] * ( cosX ) + vector.v[2] * (-sinX );
	target.v[2] = vector.v[0] * (-sinY ) + vector.v[1] * ( cosY * sinX ) + vector.v[2] * ( cosY * cosX );
	return target;
}

template<typename T>
Vector<3, T> & rotateXY(const Vector<3, T> & vector, const Vector<2, T> & rotate, Vector<3, T> & target) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	target.v[0] = vector.v[0] * ( cosY ) + vector.v[1] * ( sinY * sinX ) + vector.v[2] * ( sinY * cosX );
	target.v[1] = vector.v[1] * ( cosX ) + vector.v[2] * (-sinX );
	target.v[2] = vector.v[0] * (-sinY ) + vector.v[1] * ( cosY * sinX ) + vector.v[2] * ( cosY * cosX );
	return target;
}

template<typename T>
Vector<3, T> & rotateXZ(const Vector<3, T> & vector, T rotateX, T rotateZ, Vector<3, T> & target) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	target.v[0] = vector.v[0] * ( cosZ ) + vector.v[1] * (-sinZ * cosX ) + vector.v[2] * ( sinZ * sinX );
	target.v[1] = vector.v[0] * ( sinZ ) + vector.v[1] * ( cosZ * cosX ) + vector.v[2] * (-cosZ * sinX );
	target.v[2] = vector.v[1] * ( sinX ) + vector.v[2] * ( cosX );
	return target;
}

template<typename T>
Vector<3, T> & rotateXZ(const Vector<3, T> & vector, const Vector<2, T> & rotate, Vector<3, T> & target) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	target.v[0] = vector.v[0] * ( cosZ ) + vector.v[1] * (-sinZ * cosX ) + vector.v[2] * ( sinZ * sinX );
	target.v[1] = vector.v[0] * ( sinZ ) + vector.v[1] * ( cosZ * cosX ) + vector.v[2] * (-cosZ * sinX );
	target.v[2] = vector.v[1] * ( sinX ) + vector.v[2] * ( cosX );
	return target;
}

template<typename T>
Vector<3, T> & rotateYX(const Vector<3, T> & vector, const Vector<2, T> & rotate, Vector<3, T> & target) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	target.v[0] = vector.v[0] * ( cosY ) + vector.v[2] * ( sinY );
	target.v[1] = vector.v[0] * ( sinX * sinY ) + vector.v[1] * ( cosX ) + vector.v[2] * (-sinX * cosY );
	target.v[2] = vector.v[0] * (-cosX * sinY ) + vector.v[1] * ( sinX ) + vector.v[2] * ( cosX * cosY );
	return target;
}

template<typename T>
Vector<3, T> & rotateYZ(const Vector<3, T> & vector, T rotateY, T rotateZ, Vector<3, T> & target) {
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	target.v[0] = vector.v[0] * ( cosZ * cosY ) + vector.v[1] * (-sinZ ) + vector.v[2] * ( cosZ * sinY );
	target.v[1] = vector.v[0] * ( sinZ * cosY ) + vector.v[1] * ( cosZ ) + vector.v[2] * ( sinZ * sinY );
	target.v[2] = vector.v[0] * (-sinY ) + vector.v[2] * ( cosY );
	return target;
}

template<typename T>
Vector<3, T> & rotateYZ(const Vector<3, T> & vector, const Vector<2, T> & rotate, Vector<3, T> & target) {
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	target.v[0] = vector.v[0] * ( cosZ * cosY ) + vector.v[1] * (-sinZ ) + vector.v[2] * ( cosZ * sinY );
	target.v[1] = vector.v[0] * ( sinZ * cosY ) + vector.v[1] * ( cosZ ) + vector.v[2] * ( sinZ * sinY );
	target.v[2] = vector.v[0] * (-sinY ) + vector.v[2] * ( cosY );
	return target;
}

template<typename T>
Vector<3, T> & rotateZX(const Vector<3, T> & vector, const Vector<2, T> & rotate, Vector<3, T> & target) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	target.v[0] = vector.v[0] * ( cosZ ) + vector.v[1] * (-sinZ );
	target.v[1] = vector.v[0] * ( cosX * sinZ ) + vector.v[1] * ( cosX * cosZ ) + vector.v[2] * (-sinX );
	target.v[2] = vector.v[0] * ( sinX * sinZ ) + vector.v[1] * ( sinX * cosZ ) + vector.v[2] * ( cosX );
	return target;
}

template<typename T>
Vector<3, T> & rotateZY(const Vector<3, T> & vector, const Vector<2, T> & rotate, Vector<3, T> & target) {
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	target.v[0] = vector.v[0] * ( cosY * cosZ ) + vector.v[1] * (-cosY * sinZ ) + vector.v[2] * ( sinY );
	target.v[1] = vector.v[0] * ( sinZ ) + vector.v[1] * ( cosZ );
	target.v[2] = vector.v[0] * (-sinY * cosZ ) + vector.v[1] * ( sinY * sinZ ) + vector.v[2] * ( cosY );
	return target;
}

template<typename T>
Vector<3, T> & rotateXYZ(const Vector<3, T> & vector, T rotateX, T rotateY, T rotateZ, Vector<3, T> & target) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	target.v[0] = vector.v[0] * ( cosZ * cosY ) + vector.v[1] * (-sinZ * cosX + cosZ * sinY * sinX ) + vector.v[2] * ( sinZ * sinX + cosZ * sinY * cosX );
	target.v[1] = vector.v[0] * ( sinZ * cosY ) + vector.v[1] * ( cosZ * cosX + sinZ * sinY * sinX ) + vector.v[2] * (-cosZ * sinX + sinZ * sinY * cosX );
	target.v[2] = vector.v[0] * (-sinY ) + vector.v[1] * ( cosY * sinX ) + vector.v[2] * ( cosY * cosX );
	return target;
}

template<typename T>
Vector<3, T> & rotateXYZ(const Vector<3, T> & vector, const Vector<3, T> & rotate, Vector<3, T> & target) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[2]);
	auto cosZ = cos(rotate.v[2]);
	target.v[0] = vector.v[0] * ( cosZ * cosY ) + vector.v[1] * (-sinZ * cosX + cosZ * sinY * sinX ) + vector.v[2] * ( sinZ * sinX + cosZ * sinY * cosX );
	target.v[1] = vector.v[0] * ( sinZ * cosY ) + vector.v[1] * ( cosZ * cosX + sinZ * sinY * sinX ) + vector.v[2] * (-cosZ * sinX + sinZ * sinY * cosX );
	target.v[2] = vector.v[0] * (-sinY ) + vector.v[1] * ( cosY * sinX ) + vector.v[2] * ( cosY * cosX );
	return target;
}

template<typename T>
Vector<3, T> & rotateXZY(const Vector<3, T> & vector, const Vector<3, T> & rotate, Vector<3, T> & target) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[2]);
	auto cosY = cos(rotate.v[2]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	target.v[0] = vector.v[0] * ( cosY * cosZ ) + vector.v[1] * (-cosY * sinZ * cosX + sinY * sinX ) + vector.v[2] * ( cosY * sinZ * sinX + sinY * cosX );
	target.v[1] = vector.v[0] * ( sinZ ) + vector.v[1] * ( cosZ * cosX ) + vector.v[2] * (-cosZ * sinX );
	target.v[2] = vector.v[0] * (-sinY * cosZ ) + vector.v[1] * ( sinY * sinZ * cosX + cosY * sinX ) + vector.v[2] * (-sinY * sinZ * sinX + cosY * cosX );
	return target;
}

template<typename T>
Vector<3, T> & rotateYXZ(const Vector<3, T> & vector, const Vector<3, T> & rotate, Vector<3, T> & target) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[2]);
	auto cosZ = cos(rotate.v[2]);
	target.v[0] = vector.v[0] * ( cosZ * cosY - sinZ * sinX * sinY ) + vector.v[1] * (-sinZ * cosX ) + vector.v[2] * ( cosZ * sinY + sinZ * sinX * cosY );
	target.v[1] = vector.v[0] * ( sinZ * cosY + cosZ * sinX * sinY ) + vector.v[1] * ( cosZ * cosX ) + vector.v[2] * ( sinZ * sinY - cosZ * sinX * cosY );
	target.v[2] = vector.v[0] * (-cosX * sinY ) + vector.v[1] * ( sinX ) + vector.v[2] * ( cosX * cosY );
	return target;
}

template<typename T>
Vector<3, T> & rotateYZX(const Vector<3, T> & vector, const Vector<3, T> & rotate, Vector<3, T> & target) {
	auto sinX = sin(rotate.v[2]);
	auto cosX = cos(rotate.v[2]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	target.v[0] = vector.v[0] * ( cosZ * cosY ) + vector.v[1] * (-sinZ ) + vector.v[2] * ( cosZ * sinY );
	target.v[1] = vector.v[0] * ( cosX * sinZ * cosY + sinX * sinY ) + vector.v[1] * ( cosX * cosZ ) + vector.v[2] * ( cosX * sinZ * sinY - sinX * cosY );
	target.v[2] = vector.v[0] * ( sinX * sinZ * cosY - cosX * sinY ) + vector.v[1] * ( sinX * cosZ ) + vector.v[2] * ( sinX * sinZ * sinY + cosX * cosY );
	return target;
}

template<typename T>
Vector<3, T> & rotateZXY(const Vector<3, T> & vector, const Vector<3, T> & rotate, Vector<3, T> & target) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[2]);
	auto cosY = cos(rotate.v[2]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	target.v[0] = vector.v[0] * ( cosY * cosZ + sinY * sinX * sinZ ) + vector.v[1] * (-cosY * sinZ + sinY * sinX * cosZ ) + vector.v[2] * ( sinY * cosX );
	target.v[1] = vector.v[0] * ( cosX * sinZ ) + vector.v[1] * ( cosX * cosZ ) + vector.v[2] * (-sinX );
	target.v[2] = vector.v[0] * (-sinY * cosZ + cosY * sinX * sinZ ) + vector.v[1] * ( sinY * sinZ + cosY * sinX * cosZ ) + vector.v[2] * ( cosY * cosX );
	return target;
}

template<typename T>
Vector<3, T> & rotateZYX(const Vector<3, T> & vector, const Vector<3, T> & rotate, Vector<3, T> & target) {
	auto sinX = sin(rotate.v[2]);
	auto cosX = cos(rotate.v[2]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	target.v[0] = vector.v[0] * ( cosY * cosZ ) + vector.v[1] * (-cosY * sinZ ) + vector.v[2] * ( sinY );
	target.v[1] = vector.v[0] * ( sinX * sinY * cosZ + cosX * sinZ ) + vector.v[1] * (-sinX * sinY * sinZ + cosX * cosZ ) + vector.v[2] * (-sinX * cosY );
	target.v[2] = vector.v[0] * (-cosX * sinY * cosZ + sinX * sinZ ) + vector.v[1] * ( cosX * sinY * sinZ + sinX * cosZ ) + vector.v[2] * ( cosX * cosY );
	return target;
}

template<typename T>
Vector<3, T> scaleX(const Vector<3, T> & vector, T scaleX) {
	return {
		vector.v[0] * scaleX,
		vector.v[1],
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> scaleY(const Vector<3, T> & vector, T scaleY) {
	return {
		vector.v[0],
		vector.v[1] * scaleY,
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> scaleZ(const Vector<3, T> & vector, T scaleZ) {
	return {
		vector.v[0],
		vector.v[1],
		vector.v[2] * scaleZ
	};
}

template<typename T>
Vector<3, T> scaleXY(const Vector<3, T> & vector, T scale) {
	return {
		vector.v[0] * scale,
		vector.v[1] * scale,
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> scaleXY(const Vector<3, T> & vector, T scaleX, T scaleY) {
	return {
		vector.v[0] * scaleX,
		vector.v[1] * scaleY,
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> scaleXY(const Vector<3, T> & vector, const Vector<2, T> & scale) {
	return {
		vector.v[0] * scale.v[0],
		vector.v[1] * scale.v[1],
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> scaleXZ(const Vector<3, T> & vector, T scale) {
	return {
		vector.v[0] * scale,
		vector.v[1],
		vector.v[2] * scale
	};
}

template<typename T>
Vector<3, T> scaleXZ(const Vector<3, T> & vector, T scaleX, T scaleZ) {
	return {
		vector.v[0] * scaleX,
		vector.v[1],
		vector.v[2] * scaleZ
	};
}

template<typename T>
Vector<3, T> scaleXZ(const Vector<3, T> & vector, const Vector<2, T> & scale) {
	return {
		vector.v[0] * scale.v[0],
		vector.v[1],
		vector.v[2] * scale.v[1]
	};
}

template<typename T>
Vector<3, T> scaleYX(const Vector<3, T> & vector, const Vector<2, T> & scale) {
	return {
		vector.v[0] * scale.v[1],
		vector.v[1] * scale.v[0],
		vector.v[2]
	};
}

template<typename T>
Vector<3, T> scaleYZ(const Vector<3, T> & vector, T scale) {
	return {
		vector.v[0],
		vector.v[1] * scale,
		vector.v[2] * scale
	};
}

template<typename T>
Vector<3, T> scaleYZ(const Vector<3, T> & vector, T scaleY, T scaleZ) {
	return {
		vector.v[0],
		vector.v[1] * scaleY,
		vector.v[2] * scaleZ
	};
}

template<typename T>
Vector<3, T> scaleYZ(const Vector<3, T> & vector, const Vector<2, T> & scale) {
	return {
		vector.v[0],
		vector.v[1] * scale.v[0],
		vector.v[2] * scale.v[1]
	};
}

template<typename T>
Vector<3, T> scaleZX(const Vector<3, T> & vector, const Vector<2, T> & scale) {
	return {
		vector.v[0] * scale.v[1],
		vector.v[1],
		vector.v[2] * scale.v[0]
	};
}

template<typename T>
Vector<3, T> scaleZY(const Vector<3, T> & vector, const Vector<2, T> & scale) {
	return {
		vector.v[0],
		vector.v[1] * scale.v[1],
		vector.v[2] * scale.v[0]
	};
}

template<typename T>
Vector<3, T> scaleXYZ(const Vector<3, T> & vector, T scale) {
	return {
		vector.v[0] * scale,
		vector.v[1] * scale,
		vector.v[2] * scale
	};
}

template<typename T>
Vector<3, T> scaleXYZ(const Vector<3, T> & vector, T scaleX, T scaleY, T scaleZ) {
	return {
		vector.v[0] * scaleX,
		vector.v[1] * scaleY,
		vector.v[2] * scaleZ
	};
}

template<typename T>
Vector<3, T> scaleXYZ(const Vector<3, T> & vector, const Vector<3, T> & scale) {
	return {
		vector.v[0] * scale.v[0],
		vector.v[1] * scale.v[1],
		vector.v[2] * scale.v[2]
	};
}

template<typename T>
Vector<3, T> scaleXZY(const Vector<3, T> & vector, const Vector<3, T> & scale) {
	return {
		vector.v[0] * scale.v[0],
		vector.v[1] * scale.v[2],
		vector.v[2] * scale.v[1]
	};
}

template<typename T>
Vector<3, T> scaleYXZ(const Vector<3, T> & vector, const Vector<3, T> & scale) {
	return {
		vector.v[0] * scale.v[1],
		vector.v[1] * scale.v[0],
		vector.v[2] * scale.v[2]
	};
}

template<typename T>
Vector<3, T> scaleYZX(const Vector<3, T> & vector, const Vector<3, T> & scale) {
	return {
		vector.v[0] * scale.v[2],
		vector.v[1] * scale.v[0],
		vector.v[2] * scale.v[1]
	};
}

template<typename T>
Vector<3, T> scaleZXY(const Vector<3, T> & vector, const Vector<3, T> & scale) {
	return {
		vector.v[0] * scale.v[1],
		vector.v[1] * scale.v[2],
		vector.v[2] * scale.v[0]
	};
}

template<typename T>
Vector<3, T> scaleZYX(const Vector<3, T> & vector, const Vector<3, T> & scale) {
	return {
		vector.v[0] * scale.v[2],
		vector.v[1] * scale.v[1],
		vector.v[2] * scale.v[0]
	};
}

template<typename T>
Vector<3, T> scaleX(const Vector<3, T> & vector, T scaleX, const Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scaleX;
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> scaleY(const Vector<3, T> & vector, T scaleY, const Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] * scaleY;
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> scaleZ(const Vector<3, T> & vector, T scaleZ, const Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2] * scaleZ;
	return target;
}

template<typename T>
Vector<3, T> scaleXY(const Vector<3, T> & vector, T scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale;
	target.v[1] = vector.v[1] * scale;
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> scaleXY(const Vector<3, T> & vector, T scaleX, T scaleY, const Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scaleX;
	target.v[1] = vector.v[1] * scaleY;
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> scaleXY(const Vector<3, T> & vector, const Vector<2, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[0];
	target.v[1] = vector.v[1] * scale.v[1];
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> scaleXZ(const Vector<3, T> & vector, T scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale;
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2] * scale;
	return target;
}

template<typename T>
Vector<3, T> scaleXZ(const Vector<3, T> & vector, T scaleX, T scaleZ, const Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scaleX;
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2] * scaleZ;
	return target;
}

template<typename T>
Vector<3, T> scaleXZ(const Vector<3, T> & vector, const Vector<2, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[0];
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2] * scale.v[1];
	return target;
}

template<typename T>
Vector<3, T> scaleYX(const Vector<3, T> & vector, const Vector<2, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[1];
	target.v[1] = vector.v[1] * scale.v[0];
	target.v[2] = vector.v[2];
	return target;
}

template<typename T>
Vector<3, T> scaleYZ(const Vector<3, T> & vector, T scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] * scale;
	target.v[2] = vector.v[2] * scale;
	return target;
}

template<typename T>
Vector<3, T> scaleYZ(const Vector<3, T> & vector, T scaleY, T scaleZ, const Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] * scaleY;
	target.v[2] = vector.v[2] * scaleZ;
	return target;
}

template<typename T>
Vector<3, T> scaleYZ(const Vector<3, T> & vector, const Vector<2, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] * scale.v[0];
	target.v[2] = vector.v[2] * scale.v[1];
	return target;
}

template<typename T>
Vector<3, T> scaleZX(const Vector<3, T> & vector, const Vector<2, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[1];
	target.v[1] = vector.v[1];
	target.v[2] = vector.v[2] * scale.v[0];
	return target;
}

template<typename T>
Vector<3, T> scaleZY(const Vector<3, T> & vector, const Vector<2, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0];
	target.v[1] = vector.v[1] * scale.v[1];
	target.v[2] = vector.v[2] * scale.v[0];
	return target;
}

template<typename T>
Vector<3, T> scaleXYZ(const Vector<3, T> & vector, T scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale;
	target.v[1] = vector.v[1] * scale;
	target.v[2] = vector.v[2] * scale;
	return target;
}

template<typename T>
Vector<3, T> scaleXYZ(const Vector<3, T> & vector, T scaleX, T scaleY, T scaleZ, const Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scaleX;
	target.v[1] = vector.v[1] * scaleY;
	target.v[2] = vector.v[2] * scaleZ;
	return target;
}

template<typename T>
Vector<3, T> scaleXYZ(const Vector<3, T> & vector, const Vector<3, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[0];
	target.v[1] = vector.v[1] * scale.v[1];
	target.v[2] = vector.v[2] * scale.v[2];
	return target;
}

template<typename T>
Vector<3, T> scaleXZY(const Vector<3, T> & vector, const Vector<3, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[0];
	target.v[1] = vector.v[1] * scale.v[2];
	target.v[2] = vector.v[2] * scale.v[1];
	return target;
}

template<typename T>
Vector<3, T> scaleYXZ(const Vector<3, T> & vector, const Vector<3, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[1];
	target.v[1] = vector.v[1] * scale.v[0];
	target.v[2] = vector.v[2] * scale.v[2];
	return target;
}

template<typename T>
Vector<3, T> scaleYZX(const Vector<3, T> & vector, const Vector<3, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[2];
	target.v[1] = vector.v[1] * scale.v[0];
	target.v[2] = vector.v[2] * scale.v[1];
	return target;
}

template<typename T>
Vector<3, T> scaleZXY(const Vector<3, T> & vector, const Vector<3, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[1];
	target.v[1] = vector.v[1] * scale.v[2];
	target.v[2] = vector.v[2] * scale.v[0];
	return target;
}

template<typename T>
Vector<3, T> scaleZYX(const Vector<3, T> & vector, const Vector<3, T> & scale, Vector<3, T> & target) {
	target.v[0] = vector.v[0] * scale.v[2];
	target.v[1] = vector.v[1] * scale.v[1];
	target.v[2] = vector.v[2] * scale.v[0];
	return target;
}

template <typename T>
std::ostream & operator<<(std::ostream & stream, const Vector<3, T> & vector) {
	return stream << '<' << vector.v[0] << ", " << vector.v[1] << ", " << vector.v[2] << '>';
}


template <typename T> using Vector3 = Vector<3, T>;

using Vector3b  = Vector3< i8 >;
using Vector3ub = Vector3<u8 >;
using Vector3s  = Vector3< i16>;
using Vector3us = Vector3<u16>;
using Vector3i  = Vector3< i32>;
using Vector3ui = Vector3<u32>;
using Vector3l  = Vector3< i64>;
using Vector3ul = Vector3<u64>;
using Vector3f  = Vector3<float >;
using Vector3d  = Vector3<double>;

#define CNGE_VECTOR3_FORWARD
#endif // CNGE_VECTOR3
