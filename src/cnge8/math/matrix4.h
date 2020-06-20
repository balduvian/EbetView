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

#include "../Types.h"

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

#ifndef CNGE_MATRIX_FORWARD
#define CNGE_MATRIX_FORWARD

template <u32 N, u32 M, typename T>
class Matrix;

#endif // CNGE_MATRIX_FORWARD

#ifndef CNGE_MATRIXN_FORWARD
#define CNGE_MATRIXN_FORWARD

template <u32 N, typename T>
class Matrix<N, N, T>;

#endif // CNGE_MATRIXN_FORWARD

#ifndef CNGE_MATRIX4
#define CNGE_MATRIX4

#include <ostream>
#include <cmath>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "MatrixN.h"

template <typename T>
class Matrix<4, 4, T> {
	private:
		bool owner = true;

		void clean() {
			if (owner)
				delete [] m;
		}
	public:
		T * m;

		static Matrix<4, 4, T> makeIdentity() {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeOrthographic(T left, T right, T bottom, T top, T near, T far) {
			return new T[4 * 4] {
				2 / (right - left), 0, 0, 0,
				0, 2 / (top - bottom), 0, 0,
				0, 0, 2 / (far  -  near), 0,
				(left + right) / (left - right), (bottom + top) / (bottom - top), (near + far) / (near - far), 1
			};
		}

		static Matrix<4, 4, T> makePerspective(T fovY, T aspectRatio, T near, T far) {
			auto vertical = 1 / tan(fovY / 2);
			return new T[4 * 4] {
				vertical / aspectRatio, 0, 0, 0,
				0, vertical, 0, 0,
				0, 0, (near + far) / (near - far), -1,
				0, 0, (2 * far * near) / (near - far), 0
			};
		}
		
		static Matrix<4, 4, T> makeShear2D(T axisAngle, T shearAngle) {
			auto sinAxis = sin(axisAngle);
			auto cosAxis = cos(axisAngle);
			auto tanShear = tan(shearAngle);
			return new T[4 * 4] {
				sinAxis * tanShear * cosAxis, -cosAxis * tanShear * cosAxis, 0, 0,
				sinAxis * tanShear * sinAxis, -cosAxis * tanShear * sinAxis, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		
		static Matrix<4, 4, T> makeShear2D(const Vector<2, T> & axis, T shearAngle) {
			auto unitAxis = normalize(axis); 
			auto tanShear = tan(shearAngle);
			return new T[4 * 4] {
				unitAxis.v[1] * tanShear * unitAxis.v[0], -unitAxis.v[0] * tanShear * unitAxis.v[0], 0, 0,
				unitAxis.v[1] * tanShear * unitAxis.v[1], -unitAxis.v[0] * tanShear * unitAxis.v[1], 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		
		static Matrix<4, 4, T> makeShear2D(T axisAngle, const Vector<2, T> & shear) {
			auto sinAxis = sin(axisAngle);
			auto cosAxis = cos(axisAngle);
			auto tanShear = shear.v[1] / shear.v[0];
			return new T[4 * 4] {
				sinAxis * tanShear * cosAxis, -cosAxis * tanShear * cosAxis, 0, 0,
				sinAxis * tanShear * sinAxis, -cosAxis * tanShear * sinAxis, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		
		static Matrix<4, 4, T> makeShear2D(const Vector<2, T> & axis, const Vector<2, T> & shear) {
			auto unitAxis = normalize(axis);
			auto tanShear = shear.v[1] / shear.v[0];
			return new T[4 * 4] {
				unitAxis.v[1] * tanShear * unitAxis.v[0], -unitAxis.v[0] * tanShear * unitAxis.v[0], 0, 0,
				unitAxis.v[1] * tanShear * unitAxis.v[1], -unitAxis.v[0] * tanShear * unitAxis.v[1], 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		
		static Matrix<4, 4, T> makeShear2DX(T angle) {
			return new T[4 * 4] {
				1, -tan(angle), 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		
		static Matrix<4, 4, T> makeShear2DX(const Vector<2, T> & shear) {
			return new T[4 * 4] {
				1, -shear.v[1] / shear.v[0], 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		
		static Matrix<4, 4, T> makeShear2DY(T angle) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				tan(angle), 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		
		static Matrix<4, 4, T> makeShear2DY(const Vector<2, T> & shear) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				shear.v[1] / shear.v[0], 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeTranslate2D(T translateX, T translateY) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translateX, translateY, 0, 1
			};
		}

		static Matrix<4, 4, T> makeTranslate2D(const Vector<2, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[0], translate.v[1], 0, 1
			};
		}

		static Matrix<4, 4, T> makeTranslate(T translateX, T translateY, T translateZ) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translateX, translateY, translateZ, 1
			};
		}

		static Matrix<4, 4, T> makeTranslate(const Vector<3, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[0], translate.v[1], translate.v[2], 1
			};
		}

		static Matrix<4, 4, T> makeRotate2D(T rotation) {
			auto sinZ = sin(rotation);
			auto cosZ = cos(rotation);
			return new T[4 * 4] {
				cosZ, sinZ, 0, 0,
				-sinZ, cosZ, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScale2D(T scale) {
			return new T[4 * 4] {
				scale, 0, 0, 0,
				0, scale, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScale2D(T scaleX, T scaleY) {
			return new T[4 * 4] {
				scaleX, 0, 0, 0,
				0, scaleY, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScale2D(const Vector<2, T> & scale) {
			return new T[4 * 4] {
				scale.v[0], 0, 0, 0,
				0, scale.v[1], 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScale(T scale) {
			return new T[4 * 4] {
				scale, 0, 0, 0,
				0, scale, 0, 0,
				0, 0, scale, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScale(T scaleX, T scaleY, T scaleZ) {
			return new T[4 * 4] {
				scaleX, 0, 0, 0,
				0, scaleY, 0, 0,
				0, 0, scaleZ, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScale(const Vector<3, T> & scale) {
			return new T[4 * 4] {
				scale.v[0], 0, 0, 0,
				0, scale.v[1], 0, 0,
				0, 0, scale.v[2], 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeTranslateX(T translateX) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translateX, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeTranslateY(T translateY) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, translateY, 0, 1
			};
		}

		static Matrix<4, 4, T> makeTranslateZ(T translateZ) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, translateZ, 1
			};
		}

		static Matrix<4, 4, T> makeTranslateXY(T translateX, T translateY) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translateX, translateY, 0, 1
			};
		}

		static Matrix<4, 4, T> makeTranslateXY(const Vector<2, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[0], translate.v[1], 0, 1
			};
		}

		static Matrix<4, 4, T> makeTranslateXZ(T translateX, T translateZ) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translateX, 0, translateZ, 1
			};
		}

		static Matrix<4, 4, T> makeTranslateXZ(const Vector<2, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[0], 0, translate.v[1], 1
			};
		}

		static Matrix<4, 4, T> makeTranslateYX(const Vector<2, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[1], translate.v[0], 0, 1
			};
		}

		static Matrix<4, 4, T> makeTranslateYZ(T translateY, T translateZ) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, translateY, translateZ, 1
			};
		}

		static Matrix<4, 4, T> makeTranslateYZ(const Vector<2, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, translate.v[0], translate.v[1], 1
			};
		}

		static Matrix<4, 4, T> makeTranslateZX(const Vector<2, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[1], 0, translate.v[0], 1
			};
		}

		static Matrix<4, 4, T> makeTranslateZY(const Vector<2, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, translate.v[1], translate.v[0], 1
			};
		}

		static Matrix<4, 4, T> makeTranslateXYZ(T translateX, T translateY, T translateZ) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translateX, translateY, translateZ, 1
			};
		}

		static Matrix<4, 4, T> makeTranslateXYZ(const Vector<3, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[0], translate.v[1], translate.v[2], 1
			};
		}

		static Matrix<4, 4, T> makeTranslateXZY(const Vector<3, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[0], translate.v[2], translate.v[1], 1
			};
		}

		static Matrix<4, 4, T> makeTranslateYXZ(const Vector<3, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[1], translate.v[0], translate.v[2], 1
			};
		}

		static Matrix<4, 4, T> makeTranslateYZX(const Vector<3, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[2], translate.v[0], translate.v[1], 1
			};
		}

		static Matrix<4, 4, T> makeTranslateZXY(const Vector<3, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[1], translate.v[2], translate.v[0], 1
			};
		}

		static Matrix<4, 4, T> makeTranslateZYX(const Vector<3, T> & translate) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.v[2], translate.v[1], translate.v[0], 1
			};
		}

		static Matrix<4, 4, T> makeRotateX(T rotateX) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, cosX, sinX, 0,
				0, -sinX, cosX, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateY(T rotateY) {
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			return new T[4 * 4] {
				cosY, 0, -sinY, 0,
				0, 1, 0, 0,
				sinY, 0, cosY, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateZ(T rotateZ) {
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			return new T[4 * 4] {
				cosZ, sinZ, 0, 0,
				-sinZ, cosZ, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateXY(T rotateX, T rotateY) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			return new T[4 * 4] {
				cosY, 0, -sinY, 0,
				sinY * sinX, cosX, cosY * sinX, 0,
				sinY * cosX, -sinX, cosY * cosX, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateXY(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			return new T[4 * 4] {
				cosY, 0, -sinY, 0,
				sinY * sinX, cosX, cosY * sinX, 0,
				sinY * cosX, -sinX, cosY * cosX, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateXZ(T rotateX, T rotateZ) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			return new T[4 * 4] {
				cosZ, sinZ, 0, 0,
				-sinZ * cosX, cosZ * cosX, sinX, 0,
				sinZ * sinX, -cosZ * sinX, cosX, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateXZ(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			return new T[4 * 4] {
				cosZ, sinZ, 0, 0,
				-sinZ * cosX, cosZ * cosX, sinX, 0,
				sinZ * sinX, -cosZ * sinX, cosX, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateYX(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			return new T[4 * 4] {
				cosY, sinX * sinY, -cosX * sinY, 0,
				0, cosX, sinX, 0,
				sinY, -sinX * cosY, cosX * cosY, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateYZ(T rotateY, T rotateZ) {
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			return new T[4 * 4] {
				cosZ * cosY, sinZ * cosY, -sinY, 0,
				-sinZ, cosZ, 0, 0,
				cosZ * sinY, sinZ * sinY, cosY, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateYZ(const Vector<2, T> & rotate) {
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			return new T[4 * 4] {
				cosZ * cosY, sinZ * cosY, -sinY, 0,
				-sinZ, cosZ, 0, 0,
				cosZ * sinY, sinZ * sinY, cosY, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateZX(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			return new T[4 * 4] {
				cosZ, cosX * sinZ, sinX * sinZ, 0,
				-sinZ, cosX * cosZ, sinX * cosZ, 0,
				0, -sinX, cosX, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateZY(const Vector<2, T> & rotate) {
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			return new T[4 * 4] {
				cosY * cosZ, sinZ, -sinY * cosZ, 0,
				-cosY * sinZ, cosZ, sinY * sinZ, 0,
				sinY, 0, cosY, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateXYZ(T rotateX, T rotateY, T rotateZ) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			return new T[4 * 4] {
				cosZ * cosY, sinZ * cosY, -sinY, 0,
				-sinZ * cosX + cosZ * sinY * sinX, cosZ * cosX + sinZ * sinY * sinX, cosY * sinX, 0,
				sinZ * sinX + cosZ * sinY * cosX, -cosZ * sinX + sinZ * sinY * cosX, cosY * cosX, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateXYZ(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[2]);
			auto cosZ = cos(rotate.v[2]);
			return new T[4 * 4] {
				cosZ * cosY, sinZ * cosY, -sinY, 0,
				-sinZ * cosX + cosZ * sinY * sinX, cosZ * cosX + sinZ * sinY * sinX, cosY * sinX, 0,
				sinZ * sinX + cosZ * sinY * cosX, -cosZ * sinX + sinZ * sinY * cosX, cosY * cosX, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateXZY(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[2]);
			auto cosY = cos(rotate.v[2]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			return new T[4 * 4] {
				cosY * cosZ, sinZ, -sinY * cosZ, 0,
				-cosY * sinZ * cosX + sinY * sinX, cosZ * cosX, sinY * sinZ * cosX + cosY * sinX, 0,
				cosY * sinZ * sinX + sinY * cosX, -cosZ * sinX, -sinY * sinZ * sinX + cosY * cosX, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateYXZ(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[2]);
			auto cosZ = cos(rotate.v[2]);
			return new T[4 * 4] {
				cosZ * cosY - sinZ * sinX * sinY, sinZ * cosY + cosZ * sinX * sinY, -cosX * sinY, 0,
				-sinZ * cosX, cosZ * cosX, sinX, 0,
				cosZ * sinY + sinZ * sinX * cosY, sinZ * sinY - cosZ * sinX * cosY, cosX * cosY, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateYZX(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[2]);
			auto cosX = cos(rotate.v[2]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			return new T[4 * 4] {
				cosZ * cosY, cosX * sinZ * cosY + sinX * sinY, sinX * sinZ * cosY - cosX * sinY, 0,
				-sinZ, cosX * cosZ, sinX * cosZ, 0,
				cosZ * sinY, cosX * sinZ * sinY - sinX * cosY, sinX * sinZ * sinY + cosX * cosY, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateZXY(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[2]);
			auto cosY = cos(rotate.v[2]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			return new T[4 * 4] {
				cosY * cosZ + sinY * sinX * sinZ, cosX * sinZ, -sinY * cosZ + cosY * sinX * sinZ, 0,
				-cosY * sinZ + sinY * sinX * cosZ, cosX * cosZ, sinY * sinZ + cosY * sinX * cosZ, 0,
				sinY * cosX, -sinX, cosY * cosX, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeRotateZYX(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[2]);
			auto cosX = cos(rotate.v[2]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			return new T[4 * 4] {
				cosY * cosZ, sinX * sinY * cosZ + cosX * sinZ, -cosX * sinY * cosZ + sinX * sinZ, 0,
				-cosY * sinZ, -sinX * sinY * sinZ + cosX * cosZ, cosX * sinY * sinZ + sinX * cosZ, 0,
				sinY, -sinX * cosY, cosX * cosY, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleX(T scaleX) {
			return new T[4 * 4] {
				scaleX, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleY(T scaleY) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, scaleY, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleZ(T scaleZ) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, scaleZ, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleXY(T scale) {
			return new T[4 * 4] {
				scale, 0, 0, 0,
				0, scale, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleXY(T scaleX, T scaleY) {
			return new T[4 * 4] {
				scaleX, 0, 0, 0,
				0, scaleY, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleXY(const Vector<2, T> & scale) {
			return new T[4 * 4] {
				scale.v[0], 0, 0, 0,
				0, scale.v[1], 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleXZ(T scale) {
			return new T[4 * 4] {
				scale, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, scale, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleXZ(T scaleX, T scaleZ) {
			return new T[4 * 4] {
				scaleX, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, scaleZ, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleXZ(const Vector<2, T> & scale) {
			return new T[4 * 4] {
				scale.v[0], 0, 0, 0,
				0, 1, 0, 0,
				0, 0, scale.v[1], 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleYX(const Vector<2, T> & scale) {
			return new T[4 * 4] {
				scale.v[1], 0, 0, 0,
				0, scale.v[0], 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleYZ(T scale) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, scale, 0, 0,
				0, 0, scale, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleYZ(T scaleY, T scaleZ) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, scaleY, 0, 0,
				0, 0, scaleZ, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleYZ(const Vector<2, T> & scale) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, scale.v[0], 0, 0,
				0, 0, scale.v[1], 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleZX(const Vector<2, T> & scale) {
			return new T[4 * 4] {
				scale.v[1], 0, 0, 0,
				0, 1, 0, 0,
				0, 0, scale.v[0], 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleZY(const Vector<2, T> & scale) {
			return new T[4 * 4] {
				1, 0, 0, 0,
				0, scale.v[1], 0, 0,
				0, 0, scale.v[0], 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleXYZ(T scale) {
			return new T[4 * 4] {
				scale, 0, 0, 0,
				0, scale, 0, 0,
				0, 0, scale, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleXYZ(T scaleX, T scaleY, T scaleZ) {
			return new T[4 * 4] {
				scaleX, 0, 0, 0,
				0, scaleY, 0, 0,
				0, 0, scaleZ, 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleXYZ(const Vector<3, T> & scale) {
			return new T[4 * 4] {
				scale.v[0], 0, 0, 0,
				0, scale.v[1], 0, 0,
				0, 0, scale.v[2], 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleXZY(const Vector<3, T> & scale) {
			return new T[4 * 4] {
				scale.v[0], 0, 0, 0,
				0, scale.v[2], 0, 0,
				0, 0, scale.v[1], 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleYXZ(const Vector<3, T> & scale) {
			return new T[4 * 4] {
				scale.v[1], 0, 0, 0,
				0, scale.v[0], 0, 0,
				0, 0, scale.v[2], 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleYZX(const Vector<3, T> & scale) {
			return new T[4 * 4] {
				scale.v[2], 0, 0, 0,
				0, scale.v[0], 0, 0,
				0, 0, scale.v[1], 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleZXY(const Vector<3, T> & scale) {
			return new T[4 * 4] {
				scale.v[1], 0, 0, 0,
				0, scale.v[2], 0, 0,
				0, 0, scale.v[0], 0,
				0, 0, 0, 1
			};
		}

		static Matrix<4, 4, T> makeScaleZYX(const Vector<3, T> & scale) {
			return new T[4 * 4] {
				scale.v[2], 0, 0, 0,
				0, scale.v[1], 0, 0,
				0, 0, scale.v[0], 0,
				0, 0, 0, 1
			};
		}

		Matrix() :
			m(new T[4 * 4]()) {}

		Matrix(T * m, bool owner = true) noexcept :
			owner(owner),
			m(m) {}
		
		Matrix(T a, T b, T c, T d,
				   T e, T f, T g, T h,
				   T i, T j, T k, T l,
				   T m, T n, T o, T p) :
			m(new T[4 * 4] {
				a, e, i, m,
				b, f, j, n,
				c, g, k, o,
				d, h, l, p
			}) {}

		template <typename T2>
		Matrix(const Matrix<4, 4, T2> & matrix) :
			m(new T[4 * 4]) {
			m[0 ] = (T) matrix.m[0 ];
			m[1 ] = (T) matrix.m[1 ];
			m[2 ] = (T) matrix.m[2 ];
			m[3 ] = (T) matrix.m[3 ];
			m[4 ] = (T) matrix.m[4 ];
			m[5 ] = (T) matrix.m[5 ];
			m[6 ] = (T) matrix.m[6 ];
			m[7 ] = (T) matrix.m[7 ];
			m[8 ] = (T) matrix.m[8 ];
			m[9 ] = (T) matrix.m[9 ];
			m[10] = (T) matrix.m[10];
			m[11] = (T) matrix.m[11];
			m[12] = (T) matrix.m[12];
			m[13] = (T) matrix.m[13];
			m[14] = (T) matrix.m[14];
			m[15] = (T) matrix.m[15];
		}

		Matrix(const Matrix<4, 4, T> & matrix) :
			m(new T[4 * 4]) {
			m[0 ] = matrix.m[0 ];
			m[1 ] = matrix.m[1 ];
			m[2 ] = matrix.m[2 ];
			m[3 ] = matrix.m[3 ];
			m[4 ] = matrix.m[4 ];
			m[5 ] = matrix.m[5 ];
			m[6 ] = matrix.m[6 ];
			m[7 ] = matrix.m[7 ];
			m[8 ] = matrix.m[8 ];
			m[9 ] = matrix.m[9 ];
			m[10] = matrix.m[10];
			m[11] = matrix.m[11];
			m[12] = matrix.m[12];
			m[13] = matrix.m[13];
			m[14] = matrix.m[14];
			m[15] = matrix.m[15];
		}

		Matrix(Matrix<4, 4, T> && matrix) noexcept :
			owner(matrix.owner),
			m(matrix.m) {
			matrix.owner = false;
		}

		Matrix<4, 4, T> & operator=(const Matrix<4, 4, T> & matrix) {
			m[0 ] = matrix.m[0 ];
			m[1 ] = matrix.m[1 ];
			m[2 ] = matrix.m[2 ];
			m[3 ] = matrix.m[3 ];
			m[4 ] = matrix.m[4 ];
			m[5 ] = matrix.m[5 ];
			m[6 ] = matrix.m[6 ];
			m[7 ] = matrix.m[7 ];
			m[8 ] = matrix.m[8 ];
			m[9 ] = matrix.m[9 ];
			m[10] = matrix.m[10];
			m[11] = matrix.m[11];
			m[12] = matrix.m[12];
			m[13] = matrix.m[13];
			m[14] = matrix.m[14];
			m[15] = matrix.m[15];
			return *this;
		}

		Matrix<4, 4, T> & operator=(Matrix<4, 4, T> && matrix) noexcept {
			clean();
			owner = matrix.owner;
			m = matrix.m;
			matrix.owner = false;
			return *this;
		}

		Matrix<4, 4, T> & operator+=(const Matrix<4, 4, T> & matrix) {
			m[0 ] += matrix.m[0 ];
			m[1 ] += matrix.m[1 ];
			m[2 ] += matrix.m[2 ];
			m[3 ] += matrix.m[3 ];
			m[4 ] += matrix.m[4 ];
			m[5 ] += matrix.m[5 ];
			m[6 ] += matrix.m[6 ];
			m[7 ] += matrix.m[7 ];
			m[8 ] += matrix.m[8 ];
			m[9 ] += matrix.m[9 ];
			m[10] += matrix.m[10];
			m[11] += matrix.m[11];
			m[12] += matrix.m[12];
			m[13] += matrix.m[13];
			m[14] += matrix.m[14];
			m[15] += matrix.m[15];
			return *this;
		}

		Matrix<4, 4, T> & operator-=(const Matrix<4, 4, T> & matrix) {
			m[0 ] -= matrix.m[0 ];
			m[1 ] -= matrix.m[1 ];
			m[2 ] -= matrix.m[2 ];
			m[3 ] -= matrix.m[3 ];
			m[4 ] -= matrix.m[4 ];
			m[5 ] -= matrix.m[5 ];
			m[6 ] -= matrix.m[6 ];
			m[7 ] -= matrix.m[7 ];
			m[8 ] -= matrix.m[8 ];
			m[9 ] -= matrix.m[9 ];
			m[10] -= matrix.m[10];
			m[11] -= matrix.m[11];
			m[12] -= matrix.m[12];
			m[13] -= matrix.m[13];
			m[14] -= matrix.m[14];
			m[15] -= matrix.m[15];
			return *this;
		}

		Matrix<4, 4, T> & operator*=(const Matrix<4, 4, T> & matrix) {
			return *this = *this * matrix;
		}

		Vector<4, T> operator[](u32 index) const noexcept {
			return Vector<4, T>(index * 4, false);
		}

		Matrix<4, 4, T> & setIdentity() {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setOrthographic(T left, T right, T bottom, T top, T near, T far) {
			m[0 ] = 2 / (right - left);
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 2 / (top - bottom);
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 2 / (far - near);
			m[11] = 0;
			m[12] = (left   + right) / (left   - right);
			m[13] = (bottom +   top) / (bottom -   top);
			m[14] = (near   +   far) / (near   -   far);
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setPerspective(T fovY, T aspectRatio, T near, T far) {
			auto vertical = 1 / tan(fovY / 2);
			m[0 ] = vertical / aspectRatio;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = vertical;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = (near + far) / (near - far);
			m[11] = -1;
			m[12] = 0;
			m[13] = 0;
			m[14] = (2 * far * near) / (near - far);
			m[15] = 0;
			return *this;
		}
		
		Matrix<4, 4, T> & setShear2D(T axisAngle, T shearAngle) {
			auto sinAxis = sin(axisAngle);
			auto cosAxis = cos(axisAngle);
			auto tanShear = tan(shearAngle);
			m[0 ] =  sinAxis * tanShear * cosAxis;
			m[1 ] = -cosAxis * tanShear * cosAxis;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] =  sinAxis * tanShear * sinAxis;
			m[5 ] = -cosAxis * tanShear * sinAxis;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}
		
		Matrix<4, 4, T> & setShear2D(const Vector<2, T> & axis, T shearAngle) {
			auto unitAxis = normalize(axis); 
			auto tanShear = tan(shearAngle);
			m[0 ] =  unitAxis.v[1] * tanShear * unitAxis.v[0];
			m[1 ] = -unitAxis.v[0] * tanShear * unitAxis.v[0];
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] =  unitAxis.v[1] * tanShear * unitAxis.v[1];
			m[5 ] = -unitAxis.v[0] * tanShear * unitAxis.v[1];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}
		
		Matrix<4, 4, T> & setShear2D(T axisAngle, const Vector<2, T> & shear) {
			auto sinAxis = sin(axisAngle);
			auto cosAxis = cos(axisAngle);
			auto tanShear = shear.v[1] / shear.v[0];
			m[0 ] =  sinAxis * tanShear * cosAxis;
			m[1 ] = -cosAxis * tanShear * cosAxis;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] =  sinAxis * tanShear * sinAxis;
			m[5 ] = -cosAxis * tanShear * sinAxis;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}
		
		Matrix<4, 4, T> & setShear2D(const Vector<2, T> & axis, const Vector<2, T> & shear) {
			auto unitAxis = normalize(axis);
			auto tanShear = shear.v[1] / shear.v[0];
			m[0 ] =  unitAxis.v[1] * tanShear * unitAxis.v[0];
			m[1 ] = -unitAxis.v[0] * tanShear * unitAxis.v[0];
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] =  unitAxis.v[1] * tanShear * unitAxis.v[1];
			m[5 ] = -unitAxis.v[0] * tanShear * unitAxis.v[1];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}
		
		Matrix<4, 4, T> & setShear2DX(T angle) {
			m[0 ] = 1;
			m[1 ] = -tan(angle);
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}
		
		Matrix<4, 4, T> & setShear2DX(const Vector<2, T> & shear) {
			m[0 ] = 1;
			m[1 ] = -shear.v[1] / shear.v[0];
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}
		
		Matrix<4, 4, T> & setShear2DY(T angle) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = tan(angle);
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}
		
		Matrix<4, 4, T> & setShear2DY(const Vector<2, T> & shear) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = shear.v[1] / shear.v[0];
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslate2D(T translateX, T translateY) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translateX;
			m[13] = translateY;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslate2D(const Vector<2, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[0];
			m[13] = translate.v[1];
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslate(T translateX, T translateY, T translateZ) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translateX;
			m[13] = translateY;
			m[14] = translateZ;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslate(const Vector<3, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[0];
			m[13] = translate.v[1];
			m[14] = translate.v[2];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotate2D(T rotation) {
			auto sinZ = sin(rotation);
			auto cosZ = cos(rotation);
			m[0 ] = cosZ;
			m[1 ] = sinZ;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = -sinZ;
			m[5 ] = cosZ;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScale2D(T scale) {
			m[0 ] = scale;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScale2D(T scaleX, T scaleY) {
			m[0 ] = scaleX;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scaleY;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScale2D(const Vector<2, T> & scale) {
			m[0 ] = scale.v[0];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[1];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScale(T scale) {
			m[0 ] = scale;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScale(T scaleX, T scaleY, T scaleZ) {
			m[0 ] = scaleX;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scaleY;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scaleZ;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScale(const Vector<3, T> & scale) {
			m[0 ] = scale.v[0];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[1];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[2];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateX(T translateX) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translateX;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateY(T translateY) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = translateY;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateZ(T translateZ) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = translateZ;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateXY(T translateX, T translateY) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translateX;
			m[13] = translateY;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateXY(const Vector<2, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[0];
			m[13] = translate.v[1];
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateXZ(T translateX, T translateZ) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translateX;
			m[13] = 0;
			m[14] = translateZ;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateXZ(const Vector<2, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[0];
			m[13] = 0;
			m[14] = translate.v[1];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateYX(const Vector<2, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[1];
			m[13] = translate.v[0];
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateYZ(T translateY, T translateZ) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = translateY;
			m[14] = translateZ;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateYZ(const Vector<2, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = translate.v[0];
			m[14] = translate.v[1];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateZX(const Vector<2, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[1];
			m[13] = 0;
			m[14] = translate.v[0];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateZY(const Vector<2, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = translate.v[1];
			m[14] = translate.v[0];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateXYZ(T translateX, T translateY, T translateZ) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translateX;
			m[13] = translateY;
			m[14] = translateZ;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateXYZ(const Vector<3, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[0];
			m[13] = translate.v[1];
			m[14] = translate.v[2];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateXZY(const Vector<3, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[0];
			m[13] = translate.v[2];
			m[14] = translate.v[1];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateYXZ(const Vector<3, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[1];
			m[13] = translate.v[0];
			m[14] = translate.v[2];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateYZX(const Vector<3, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[2];
			m[13] = translate.v[0];
			m[14] = translate.v[1];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateZXY(const Vector<3, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[1];
			m[13] = translate.v[2];
			m[14] = translate.v[0];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setTranslateZYX(const Vector<3, T> & translate) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = translate.v[2];
			m[13] = translate.v[1];
			m[14] = translate.v[0];
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateX(T rotateX) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = cosX;
			m[6 ] = sinX;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = -sinX;
			m[10] = cosX;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateY(T rotateY) {
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			m[0 ] = cosY;
			m[1 ] = 0;
			m[2 ] = -sinY;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = sinY;
			m[9 ] = 0;
			m[10] = cosY;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateZ(T rotateZ) {
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			m[0 ] = cosZ;
			m[1 ] = sinZ;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = -sinZ;
			m[5 ] = cosZ;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateXY(T rotateX, T rotateY) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			m[0 ] = cosY;
			m[1 ] = 0;
			m[2 ] = -sinY;
			m[3 ] = 0;
			m[4 ] = sinY * sinX;
			m[5 ] = cosX;
			m[6 ] = cosY * sinX;
			m[7 ] = 0;
			m[8 ] = sinY * cosX;
			m[9 ] = -sinX;
			m[10] = cosY * cosX;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateXY(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			m[0 ] = cosY;
			m[1 ] = 0;
			m[2 ] = -sinY;
			m[3 ] = 0;
			m[4 ] = sinY * sinX;
			m[5 ] = cosX;
			m[6 ] = cosY * sinX;
			m[7 ] = 0;
			m[8 ] = sinY * cosX;
			m[9 ] = -sinX;
			m[10] = cosY * cosX;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateXZ(T rotateX, T rotateZ) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			m[0 ] = cosZ;
			m[1 ] = sinZ;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = -sinZ * cosX;
			m[5 ] = cosZ * cosX;
			m[6 ] = sinX;
			m[7 ] = 0;
			m[8 ] = sinZ * sinX;
			m[9 ] = -cosZ * sinX;
			m[10] = cosX;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateXZ(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			m[0 ] = cosZ;
			m[1 ] = sinZ;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = -sinZ * cosX;
			m[5 ] = cosZ * cosX;
			m[6 ] = sinX;
			m[7 ] = 0;
			m[8 ] = sinZ * sinX;
			m[9 ] = -cosZ * sinX;
			m[10] = cosX;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateYX(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			m[0 ] = cosY;
			m[1 ] = sinX * sinY;
			m[2 ] = -cosX * sinY;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = cosX;
			m[6 ] = sinX;
			m[7 ] = 0;
			m[8 ] = sinY;
			m[9 ] = -sinX * cosY;
			m[10] = cosX * cosY;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateYZ(T rotateY, T rotateZ) {
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			m[0 ] = cosZ * cosY;
			m[1 ] = sinZ * cosY;
			m[2 ] = -sinY;
			m[3 ] = 0;
			m[4 ] = -sinZ;
			m[5 ] = cosZ;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = cosZ * sinY;
			m[9 ] = sinZ * sinY;
			m[10] = cosY;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateYZ(const Vector<2, T> & rotate) {
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			m[0 ] = cosZ * cosY;
			m[1 ] = sinZ * cosY;
			m[2 ] = -sinY;
			m[3 ] = 0;
			m[4 ] = -sinZ;
			m[5 ] = cosZ;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = cosZ * sinY;
			m[9 ] = sinZ * sinY;
			m[10] = cosY;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateZX(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			m[0 ] = cosZ;
			m[1 ] = cosX * sinZ;
			m[2 ] = sinX * sinZ;
			m[3 ] = 0;
			m[4 ] = -sinZ;
			m[5 ] = cosX * cosZ;
			m[6 ] = sinX * cosZ;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = -sinX;
			m[10] = cosX;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateZY(const Vector<2, T> & rotate) {
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			m[0 ] = cosY * cosZ;
			m[1 ] = sinZ;
			m[2 ] = -sinY * cosZ;
			m[3 ] = 0;
			m[4 ] = -cosY * sinZ;
			m[5 ] = cosZ;
			m[6 ] = sinY * sinZ;
			m[7 ] = 0;
			m[8 ] = sinY;
			m[9 ] = 0;
			m[10] = cosY;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateXYZ(T rotateX, T rotateY, T rotateZ) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			m[0 ] = cosZ * cosY;
			m[1 ] = sinZ * cosY;
			m[2 ] = -sinY;
			m[3 ] = 0;
			m[4 ] = -sinZ * cosX + cosZ * sinY * sinX;
			m[5 ] = cosZ * cosX + sinZ * sinY * sinX;
			m[6 ] = cosY * sinX;
			m[7 ] = 0;
			m[8 ] = sinZ * sinX + cosZ * sinY * cosX;
			m[9 ] = -cosZ * sinX + sinZ * sinY * cosX;
			m[10] = cosY * cosX;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateXYZ(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[2]);
			auto cosZ = cos(rotate.v[2]);
			m[0 ] = cosZ * cosY;
			m[1 ] = sinZ * cosY;
			m[2 ] = -sinY;
			m[3 ] = 0;
			m[4 ] = -sinZ * cosX + cosZ * sinY * sinX;
			m[5 ] = cosZ * cosX + sinZ * sinY * sinX;
			m[6 ] = cosY * sinX;
			m[7 ] = 0;
			m[8 ] = sinZ * sinX + cosZ * sinY * cosX;
			m[9 ] = -cosZ * sinX + sinZ * sinY * cosX;
			m[10] = cosY * cosX;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateXZY(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[2]);
			auto cosY = cos(rotate.v[2]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			m[0 ] = cosY * cosZ;
			m[1 ] = sinZ;
			m[2 ] = -sinY * cosZ;
			m[3 ] = 0;
			m[4 ] = -cosY * sinZ * cosX + sinY * sinX;
			m[5 ] = cosZ * cosX;
			m[6 ] = sinY * sinZ * cosX + cosY * sinX;
			m[7 ] = 0;
			m[8 ] = cosY * sinZ * sinX + sinY * cosX;
			m[9 ] = -cosZ * sinX;
			m[10] = -sinY * sinZ * sinX + cosY * cosX;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateYXZ(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[2]);
			auto cosZ = cos(rotate.v[2]);
			m[0 ] = cosZ * cosY - sinZ * sinX * sinY;
			m[1 ] = sinZ * cosY + cosZ * sinX * sinY;
			m[2 ] = -cosX * sinY;
			m[3 ] = 0;
			m[4 ] = -sinZ * cosX;
			m[5 ] = cosZ * cosX;
			m[6 ] = sinX;
			m[7 ] = 0;
			m[8 ] = cosZ * sinY + sinZ * sinX * cosY;
			m[9 ] = sinZ * sinY - cosZ * sinX * cosY;
			m[10] = cosX * cosY;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateYZX(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[2]);
			auto cosX = cos(rotate.v[2]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			m[0 ] = cosZ * cosY;
			m[1 ] = cosX * sinZ * cosY + sinX * sinY;
			m[2 ] = sinX * sinZ * cosY - cosX * sinY;
			m[3 ] = 0;
			m[4 ] = -sinZ;
			m[5 ] = cosX * cosZ;
			m[6 ] = sinX * cosZ;
			m[7 ] = 0;
			m[8 ] = cosZ * sinY;
			m[9 ] = cosX * sinZ * sinY - sinX * cosY;
			m[10] = sinX * sinZ * sinY + cosX * cosY;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateZXY(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[2]);
			auto cosY = cos(rotate.v[2]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			m[0 ] = cosY * cosZ + sinY * sinX * sinZ;
			m[1 ] = cosX * sinZ;
			m[2 ] = -sinY * cosZ + cosY * sinX * sinZ;
			m[3 ] = 0;
			m[4 ] = -cosY * sinZ + sinY * sinX * cosZ;
			m[5 ] = cosX * cosZ;
			m[6 ] = sinY * sinZ + cosY * sinX * cosZ;
			m[7 ] = 0;
			m[8 ] = sinY * cosX;
			m[9 ] = -sinX;
			m[10] = cosY * cosX;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setRotateZYX(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[2]);
			auto cosX = cos(rotate.v[2]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			m[0 ] = cosY * cosZ;
			m[1 ] = sinX * sinY * cosZ + cosX * sinZ;
			m[2 ] = -cosX * sinY * cosZ + sinX * sinZ;
			m[3 ] = 0;
			m[4 ] = -cosY * sinZ;
			m[5 ] = -sinX * sinY * sinZ + cosX * cosZ;
			m[6 ] = cosX * sinY * sinZ + sinX * cosZ;
			m[7 ] = 0;
			m[8 ] = sinY;
			m[9 ] = -sinX * cosY;
			m[10] = cosX * cosY;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleX(T scaleX) {
			m[0 ] = scaleX;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleY(T scaleY) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scaleY;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleZ(T scaleZ) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scaleZ;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleXY(T scale) {
			m[0 ] = scale;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleXY(T scaleX, T scaleY) {
			m[0 ] = scaleX;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scaleY;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleXY(const Vector<2, T> & scale) {
			m[0 ] = scale.v[0];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[1];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleXZ(T scale) {
			m[0 ] = scale;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleXZ(T scaleX, T scaleZ) {
			m[0 ] = scaleX;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scaleZ;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleXZ(const Vector<2, T> & scale) {
			m[0 ] = scale.v[0];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[1];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleYX(const Vector<2, T> & scale) {
			m[0 ] = scale.v[1];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[0];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = 1;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleYZ(T scale) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleYZ(T scaleY, T scaleZ) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scaleY;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scaleZ;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleYZ(const Vector<2, T> & scale) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[0];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[1];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleZX(const Vector<2, T> & scale) {
			m[0 ] = scale.v[1];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = 1;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[0];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleZY(const Vector<2, T> & scale) {
			m[0 ] = 1;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[1];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[0];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleXYZ(T scale) {
			m[0 ] = scale;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleXYZ(T scaleX, T scaleY, T scaleZ) {
			m[0 ] = scaleX;
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scaleY;
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scaleZ;
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleXYZ(const Vector<3, T> & scale) {
			m[0 ] = scale.v[0];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[1];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[2];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleXZY(const Vector<3, T> & scale) {
			m[0 ] = scale.v[0];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[2];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[1];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleYXZ(const Vector<3, T> & scale) {
			m[0 ] = scale.v[1];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[0];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[2];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleYZX(const Vector<3, T> & scale) {
			m[0 ] = scale.v[2];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[0];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[1];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleZXY(const Vector<3, T> & scale) {
			m[0 ] = scale.v[1];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[2];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[0];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & setScaleZYX(const Vector<3, T> & scale) {
			m[0 ] = scale.v[2];
			m[1 ] = 0;
			m[2 ] = 0;
			m[3 ] = 0;
			m[4 ] = 0;
			m[5 ] = scale.v[1];
			m[6 ] = 0;
			m[7 ] = 0;
			m[8 ] = 0;
			m[9 ] = 0;
			m[10] = scale.v[0];
			m[11] = 0;
			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;
			return *this;
		}

		Matrix<4, 4, T> & projectOrthographic(T left, T right, T bottom, T top, T near, T far) {
			auto m0  = 2 / (right -   left);
			auto m5  = 2 / (top   - bottom);
			auto m10 = 2 / (far   -   near);
			auto m12 = (left   + right) / (left   - right);
			auto m13 = (bottom +   top) / (bottom -   top);
			auto m14 = (near   +   far) / (near   -   far);
			T newM[] = {
				m[0] * m0,
				m[1] * m0,
				m[2] * m0,
				m[3] * m0,
				m[4] * m5,
				m[5] * m5,
				m[6] * m5,
				m[7] * m5,
				m[8] * m10,
				m[9] * m10,
				m[10] * m10,
				m[11] * m10,
				m[0] * m12 + m[4] * m13 + m[8] * m14 + m[12],
				m[1] * m12 + m[5] * m13 + m[9] * m14 + m[13],
				m[2] * m12 + m[6] * m13 + m[10] * m14 + m[14],
				m[3] * m12 + m[7] * m13 + m[11] * m14 + m[15]
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			m[12] = newM[12]; m[13] = newM[13]; m[14] = newM[14]; m[15] = newM[15];
			return *this;
		}

		Matrix<4, 4, T> & projectPerspective(T fovY, T aspectRatio, T near, T far) {
			auto vertical = 1 / tan(fovY / 2);
			auto m0  = vertical / aspectRatio;
			auto m10 = (near + far) / (near - far);
			auto m14 = (2 * far * near) / (near - far);
			T newM[] = {
				m[0] * m0,
				m[1] * m0,
				m[2] * m0,
				m[3] * m0,
				m[4] * vertical,
				m[5] * vertical,
				m[6] * vertical,
				m[7] * vertical,
				m[8] * m10 - m[12],
				m[9] * m10 - m[13],
				m[10] * m10 - m[14],
				m[11] * m10 - m[15],
				m[8] * m14,
				m[9] * m14,
				m[10] * m14,
				m[11] * m14
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			m[12] = newM[12]; m[13] = newM[13]; m[14] = newM[14]; m[15] = newM[15];
			return *this;
		}

		Matrix<4, 4, T> & shear2D(T axisAngle, T shearAngle) {
			auto sinAxis = sin(axisAngle);
			auto cosAxis = cos(axisAngle);
			auto tanShear = tan(shearAngle);
			auto m0  = sinAxis * tanShear * cosAxis;
			auto m1  = -cosAxis * tanShear * cosAxis;
			auto m4  = sinAxis * tanShear * sinAxis;
			auto m5  = -cosAxis * tanShear * sinAxis;
			T newM[] = {
				m[0] * m0 + m[4] * m1,
				m[1] * m0 + m[5] * m1,
				m[2] * m0 + m[6] * m1,
				m[3] * m0 + m[7] * m1,
				m[0] * m4 + m[4] * m5,
				m[1] * m4 + m[5] * m5,
				m[2] * m4 + m[6] * m5,
				m[3] * m4 + m[7] * m5,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & shear2D(const Vector<2, T> & axis, T shearAngle) {
			auto unitAxis = normalize(axis);
			auto tanShear = tan(shearAngle);
			auto m0  = unitAxis.v[1] * tanShear * unitAxis.v[0];
			auto m1  = -unitAxis.v[0] * tanShear * unitAxis.v[0];
			auto m4  = unitAxis.v[1] * tanShear * unitAxis.v[1];
			auto m5  = -unitAxis.v[0] * tanShear * unitAxis.v[1];
			T newM[] = {
				m[0] * m0 + m[4] * m1,
				m[1] * m0 + m[5] * m1,
				m[2] * m0 + m[6] * m1,
				m[3] * m0 + m[7] * m1,
				m[0] * m4 + m[4] * m5,
				m[1] * m4 + m[5] * m5,
				m[2] * m4 + m[6] * m5,
				m[3] * m4 + m[7] * m5,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & shear2D(T axisAngle,const Vector<2, T> & shear) {
			auto sinAxis = sin(axisAngle);
			auto cosAxis = cos(axisAngle);
			auto tanShear = shear.v[1] / shear.v[0];
			auto m0  = sinAxis * tanShear * cosAxis;
			auto m1  = -cosAxis * tanShear * cosAxis;
			auto m4  = sinAxis * tanShear * sinAxis;
			auto m5  = -cosAxis * tanShear * sinAxis;
			T newM[] = {
				m[0] * m0 + m[4] * m1,
				m[1] * m0 + m[5] * m1,
				m[2] * m0 + m[6] * m1,
				m[3] * m0 + m[7] * m1,
				m[0] * m4 + m[4] * m5,
				m[1] * m4 + m[5] * m5,
				m[2] * m4 + m[6] * m5,
				m[3] * m4 + m[7] * m5,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & shear2D(const Vector<2, T> & axis, const Vector<2, T> & shear) {
			auto unitAxis = normalize(axis);
			auto tanShear = shear.v[1] / shear.v[0];
			auto m0  = unitAxis.v[1] * tanShear * unitAxis.v[0];
			auto m1  = -unitAxis.v[0] * tanShear * unitAxis.v[0];
			auto m4  = unitAxis.v[1] * tanShear * unitAxis.v[1];
			auto m5  = -unitAxis.v[0] * tanShear * unitAxis.v[1];
			T newM[] = {
				m[0] * m0 + m[4] * m1,
				m[1] * m0 + m[5] * m1,
				m[2] * m0 + m[6] * m1,
				m[3] * m0 + m[7] * m1,
				m[0] * m4 + m[4] * m5,
				m[1] * m4 + m[5] * m5,
				m[2] * m4 + m[6] * m5,
				m[3] * m4 + m[7] * m5,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & shear2DX(T angle) {
			auto m1  = -tan(angle);
			T newM[] = {
				m[0] + m[4] * m1,
				m[1] + m[5] * m1,
				m[2] + m[6] * m1,
				m[3] + m[7] * m1,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & shear2DX(const Vector<2, T> & shear) {
			auto m1  = -shear.v[1] / shear.v[0];
			T newM[] = {
				m[0] + m[4] * m1,
				m[1] + m[5] * m1,
				m[2] + m[6] * m1,
				m[3] + m[7] * m1,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & shear2DY(T angle) {
			auto m4  = tan(angle);
			T newM[] = {
				m[0] * m4 + m[4],
				m[1] * m4 + m[5],
				m[2] * m4 + m[6],
				m[3] * m4 + m[7],
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			m[4 ] = newM[0 ]; m[5 ] = newM[1 ]; m[6 ] = newM[2 ]; m[7 ] = newM[3 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & shear2DY(const Vector<2, T> & shear) {
			auto m4  = shear.v[1] / shear.v[0];
			T newM[] = {
				m[0] * m4 + m[4],
				m[1] * m4 + m[5],
				m[2] * m4 + m[6],
				m[3] * m4 + m[7],
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			m[4 ] = newM[0 ]; m[5 ] = newM[1 ]; m[6 ] = newM[2 ]; m[7 ] = newM[3 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & translate2D(T translateX, T translateY) {
			T newM[] = {
				m[0] * translateX + m[4] * translateY + m[12],
				m[1] * translateX + m[5] * translateY + m[13],
				m[2] * translateX + m[6] * translateY + m[14],
				m[3] * translateX + m[7] * translateY + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translate2D(const Vector<2, T> & translate) {
			T newM[] = {
				m[0] * translate.v[0] + m[4] * translate.v[1] + m[12],
				m[1] * translate.v[0] + m[5] * translate.v[1] + m[13],
				m[2] * translate.v[0] + m[6] * translate.v[1] + m[14],
				m[3] * translate.v[0] + m[7] * translate.v[1] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translate(T translateX, T translateY, T translateZ) {
			T newM[] = {
				m[0] * translateX + m[4] * translateY + m[8] * translateZ + m[12],
				m[1] * translateX + m[5] * translateY + m[9] * translateZ + m[13],
				m[2] * translateX + m[6] * translateY + m[10] * translateZ + m[14],
				m[3] * translateX + m[7] * translateY + m[11] * translateZ + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translate(const Vector<3, T> & translate) {
			T newM[] = {
				m[0] * translate.v[0] + m[4] * translate.v[1] + m[8] * translate.v[2] + m[12],
				m[1] * translate.v[0] + m[5] * translate.v[1] + m[9] * translate.v[2] + m[13],
				m[2] * translate.v[0] + m[6] * translate.v[1] + m[10] * translate.v[2] + m[14],
				m[3] * translate.v[0] + m[7] * translate.v[1] + m[11] * translate.v[2] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & rotate2D(T rotation) {
			auto sinZ = sin(rotation);
			auto cosZ = cos(rotation);
			T newM[] = {
				m[0] * cosZ + m[4] * sinZ,
				m[1] * cosZ + m[5] * sinZ,
				m[2] * cosZ + m[6] * sinZ,
				m[3] * cosZ + m[7] * sinZ,
				m[0] * -sinZ + m[4] * cosZ,
				m[1] * -sinZ + m[5] * cosZ,
				m[2] * -sinZ + m[6] * cosZ,
				m[3] * -sinZ + m[7] * cosZ,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scale2D(T scale) {
			T newM[] = {
				m[0] * scale,
				m[1] * scale,
				m[2] * scale,
				m[3] * scale,
				m[4] * scale,
				m[5] * scale,
				m[6] * scale,
				m[7] * scale,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scale2D(T scaleX, T scaleY) {
			T newM[] = {
				m[0] * scaleX,
				m[1] * scaleX,
				m[2] * scaleX,
				m[3] * scaleX,
				m[4] * scaleY,
				m[5] * scaleY,
				m[6] * scaleY,
				m[7] * scaleY,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scale2D(const Vector<2, T> & scale) {
			T newM[] = {
				m[0] * scale.v[0],
				m[1] * scale.v[0],
				m[2] * scale.v[0],
				m[3] * scale.v[0],
				m[4] * scale.v[1],
				m[5] * scale.v[1],
				m[6] * scale.v[1],
				m[7] * scale.v[1],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scale(T scale) {
			T newM[] = {
				m[0] * scale,
				m[1] * scale,
				m[2] * scale,
				m[3] * scale,
				m[4] * scale,
				m[5] * scale,
				m[6] * scale,
				m[7] * scale,
				m[8] * scale,
				m[9] * scale,
				m[10] * scale,
				m[11] * scale,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scale(T scaleX, T scaleY, T scaleZ) {
			T newM[] = {
				m[0] * scaleX,
				m[1] * scaleX,
				m[2] * scaleX,
				m[3] * scaleX,
				m[4] * scaleY,
				m[5] * scaleY,
				m[6] * scaleY,
				m[7] * scaleY,
				m[8] * scaleZ,
				m[9] * scaleZ,
				m[10] * scaleZ,
				m[11] * scaleZ,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scale(const Vector<3, T> & scale) {
			T newM[] = {
				m[0] * scale.v[0],
				m[1] * scale.v[0],
				m[2] * scale.v[0],
				m[3] * scale.v[0],
				m[4] * scale.v[1],
				m[5] * scale.v[1],
				m[6] * scale.v[1],
				m[7] * scale.v[1],
				m[8] * scale.v[2],
				m[9] * scale.v[2],
				m[10] * scale.v[2],
				m[11] * scale.v[2],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & translateX(T translateX) {
			T newM[] = {
				m[0] * translateX + m[12],
				m[1] * translateX + m[13],
				m[2] * translateX + m[14],
				m[3] * translateX + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateY(T translateY) {
			T newM[] = {
				m[4] * translateY + m[12],
				m[5] * translateY + m[13],
				m[6] * translateY + m[14],
				m[7] * translateY + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateZ(T translateZ) {
			T newM[] = {
				m[8] * translateZ + m[12],
				m[9] * translateZ + m[13],
				m[10] * translateZ + m[14],
				m[11] * translateZ + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateXY(T translateX, T translateY) {
			T newM[] = {
				m[0] * translateX + m[4] * translateY + m[12],
				m[1] * translateX + m[5] * translateY + m[13],
				m[2] * translateX + m[6] * translateY + m[14],
				m[3] * translateX + m[7] * translateY + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateXY(const Vector<2, T> & translate) {
			T newM[] = {
				m[0] * translate.v[0] + m[4] * translate.v[1] + m[12],
				m[1] * translate.v[0] + m[5] * translate.v[1] + m[13],
				m[2] * translate.v[0] + m[6] * translate.v[1] + m[14],
				m[3] * translate.v[0] + m[7] * translate.v[1] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateXZ(T translateX, T translateZ) {
			T newM[] = {
				m[0] * translateX + m[8] * translateZ + m[12],
				m[1] * translateX + m[9] * translateZ + m[13],
				m[2] * translateX + m[10] * translateZ + m[14],
				m[3] * translateX + m[11] * translateZ + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateXZ(const Vector<2, T> & translate) {
			T newM[] = {
				m[0] * translate.v[0] + m[8] * translate.v[1] + m[12],
				m[1] * translate.v[0] + m[9] * translate.v[1] + m[13],
				m[2] * translate.v[0] + m[10] * translate.v[1] + m[14],
				m[3] * translate.v[0] + m[11] * translate.v[1] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateYX(const Vector<2, T> & translate) {
			T newM[] = {
				m[0] * translate.v[1] + m[4] * translate.v[0] + m[12],
				m[1] * translate.v[1] + m[5] * translate.v[0] + m[13],
				m[2] * translate.v[1] + m[6] * translate.v[0] + m[14],
				m[3] * translate.v[1] + m[7] * translate.v[0] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateYZ(T translateY, T translateZ) {
			T newM[] = {
				m[4] * translateY + m[8] * translateZ + m[12],
				m[5] * translateY + m[9] * translateZ + m[13],
				m[6] * translateY + m[10] * translateZ + m[14],
				m[7] * translateY + m[11] * translateZ + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateYZ(const Vector<2, T> & translate) {
			T newM[] = {
				m[4] * translate.v[0] + m[8] * translate.v[1] + m[12],
				m[5] * translate.v[0] + m[9] * translate.v[1] + m[13],
				m[6] * translate.v[0] + m[10] * translate.v[1] + m[14],
				m[7] * translate.v[0] + m[11] * translate.v[1] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateZX(const Vector<2, T> & translate) {
			T newM[] = {
				m[0] * translate.v[1] + m[8] * translate.v[0] + m[12],
				m[1] * translate.v[1] + m[9] * translate.v[0] + m[13],
				m[2] * translate.v[1] + m[10] * translate.v[0] + m[14],
				m[3] * translate.v[1] + m[11] * translate.v[0] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateZY(const Vector<2, T> & translate) {
			T newM[] = {
				m[4] * translate.v[1] + m[8] * translate.v[0] + m[12],
				m[5] * translate.v[1] + m[9] * translate.v[0] + m[13],
				m[6] * translate.v[1] + m[10] * translate.v[0] + m[14],
				m[7] * translate.v[1] + m[11] * translate.v[0] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateXYZ(T translateX, T translateY, T translateZ) {
			T newM[] = {
				m[0] * translateX + m[4] * translateY + m[8] * translateZ + m[12],
				m[1] * translateX + m[5] * translateY + m[9] * translateZ + m[13],
				m[2] * translateX + m[6] * translateY + m[10] * translateZ + m[14],
				m[3] * translateX + m[7] * translateY + m[11] * translateZ + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateXYZ(const Vector<3, T> & translate) {
			T newM[] = {
				m[0] * translate.v[0] + m[4] * translate.v[1] + m[8] * translate.v[2] + m[12],
				m[1] * translate.v[0] + m[5] * translate.v[1] + m[9] * translate.v[2] + m[13],
				m[2] * translate.v[0] + m[6] * translate.v[1] + m[10] * translate.v[2] + m[14],
				m[3] * translate.v[0] + m[7] * translate.v[1] + m[11] * translate.v[2] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateXZY(const Vector<3, T> & translate) {
			T newM[] = {
				m[0] * translate.v[0] + m[4] * translate.v[2] + m[8] * translate.v[1] + m[12],
				m[1] * translate.v[0] + m[5] * translate.v[2] + m[9] * translate.v[1] + m[13],
				m[2] * translate.v[0] + m[6] * translate.v[2] + m[10] * translate.v[1] + m[14],
				m[3] * translate.v[0] + m[7] * translate.v[2] + m[11] * translate.v[1] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateYXZ(const Vector<3, T> & translate) {
			T newM[] = {
				m[0] * translate.v[1] + m[4] * translate.v[0] + m[8] * translate.v[2] + m[12],
				m[1] * translate.v[1] + m[5] * translate.v[0] + m[9] * translate.v[2] + m[13],
				m[2] * translate.v[1] + m[6] * translate.v[0] + m[10] * translate.v[2] + m[14],
				m[3] * translate.v[1] + m[7] * translate.v[0] + m[11] * translate.v[2] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateYZX(const Vector<3, T> & translate) {
			T newM[] = {
				m[0] * translate.v[2] + m[4] * translate.v[0] + m[8] * translate.v[1] + m[12],
				m[1] * translate.v[2] + m[5] * translate.v[0] + m[9] * translate.v[1] + m[13],
				m[2] * translate.v[2] + m[6] * translate.v[0] + m[10] * translate.v[1] + m[14],
				m[3] * translate.v[2] + m[7] * translate.v[0] + m[11] * translate.v[1] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateZXY(const Vector<3, T> & translate) {
			T newM[] = {
				m[0] * translate.v[1] + m[4] * translate.v[2] + m[8] * translate.v[0] + m[12],
				m[1] * translate.v[1] + m[5] * translate.v[2] + m[9] * translate.v[0] + m[13],
				m[2] * translate.v[1] + m[6] * translate.v[2] + m[10] * translate.v[0] + m[14],
				m[3] * translate.v[1] + m[7] * translate.v[2] + m[11] * translate.v[0] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & translateZYX(const Vector<3, T> & translate) {
			T newM[] = {
				m[0] * translate.v[2] + m[4] * translate.v[1] + m[8] * translate.v[0] + m[12],
				m[1] * translate.v[2] + m[5] * translate.v[1] + m[9] * translate.v[0] + m[13],
				m[2] * translate.v[2] + m[6] * translate.v[1] + m[10] * translate.v[0] + m[14],
				m[3] * translate.v[2] + m[7] * translate.v[1] + m[11] * translate.v[0] + m[15]
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			m[12] = newM[0 ]; m[13] = newM[1 ]; m[14] = newM[2 ]; m[15] = newM[3 ];
			return *this;
		}

		Matrix<4, 4, T> & rotateX(T rotateX) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			T newM[] = {
				m[4] * cosX + m[8] * sinX,
				m[5] * cosX + m[9] * sinX,
				m[6] * cosX + m[10] * sinX,
				m[7] * cosX + m[11] * sinX,
				m[4] * -sinX + m[8] * cosX,
				m[5] * -sinX + m[9] * cosX,
				m[6] * -sinX + m[10] * cosX,
				m[7] * -sinX + m[11] * cosX,
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			m[4 ] = newM[0 ]; m[5 ] = newM[1 ]; m[6 ] = newM[2 ]; m[7 ] = newM[3 ];
			m[8 ] = newM[4 ]; m[9 ] = newM[5 ]; m[10] = newM[6 ]; m[11] = newM[7 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateY(T rotateY) {
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			T newM[] = {
				m[0] * cosY + m[8] * -sinY,
				m[1] * cosY + m[9] * -sinY,
				m[2] * cosY + m[10] * -sinY,
				m[3] * cosY + m[11] * -sinY,
				m[0] * sinY + m[8] * cosY,
				m[1] * sinY + m[9] * cosY,
				m[2] * sinY + m[10] * cosY,
				m[3] * sinY + m[11] * cosY,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			m[8 ] = newM[4 ]; m[9 ] = newM[5 ]; m[10] = newM[6 ]; m[11] = newM[7 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateZ(T rotateZ) {
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			T newM[] = {
				m[0] * cosZ + m[4] * sinZ,
				m[1] * cosZ + m[5] * sinZ,
				m[2] * cosZ + m[6] * sinZ,
				m[3] * cosZ + m[7] * sinZ,
				m[0] * -sinZ + m[4] * cosZ,
				m[1] * -sinZ + m[5] * cosZ,
				m[2] * -sinZ + m[6] * cosZ,
				m[3] * -sinZ + m[7] * cosZ,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateXY(T rotateX, T rotateY) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto m4  = sinY * sinX;
			auto m6  = cosY * sinX;
			auto m8  = sinY * cosX;
			auto m10 = cosY * cosX;
			T newM[] = {
				m[0] * cosY + m[8] * -sinY,
				m[1] * cosY + m[9] * -sinY,
				m[2] * cosY + m[10] * -sinY,
				m[3] * cosY + m[11] * -sinY,
				m[0] * m4 + m[4] * cosX + m[8] * m6,
				m[1] * m4 + m[5] * cosX + m[9] * m6,
				m[2] * m4 + m[6] * cosX + m[10] * m6,
				m[3] * m4 + m[7] * cosX + m[11] * m6,
				m[0] * m8 + m[4] * -sinX + m[8] * m10,
				m[1] * m8 + m[5] * -sinX + m[9] * m10,
				m[2] * m8 + m[6] * -sinX + m[10] * m10,
				m[3] * m8 + m[7] * -sinX + m[11] * m10,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateXY(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto m4  = sinY * sinX;
			auto m6  = cosY * sinX;
			auto m8  = sinY * cosX;
			auto m10 = cosY * cosX;
			T newM[] = {
				m[0] * cosY + m[8] * -sinY,
				m[1] * cosY + m[9] * -sinY,
				m[2] * cosY + m[10] * -sinY,
				m[3] * cosY + m[11] * -sinY,
				m[0] * m4 + m[4] * cosX + m[8] * m6,
				m[1] * m4 + m[5] * cosX + m[9] * m6,
				m[2] * m4 + m[6] * cosX + m[10] * m6,
				m[3] * m4 + m[7] * cosX + m[11] * m6,
				m[0] * m8 + m[4] * -sinX + m[8] * m10,
				m[1] * m8 + m[5] * -sinX + m[9] * m10,
				m[2] * m8 + m[6] * -sinX + m[10] * m10,
				m[3] * m8 + m[7] * -sinX + m[11] * m10,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateXZ(T rotateX, T rotateZ) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			auto m4  = -sinZ * cosX;
			auto m5  = cosZ * cosX;
			auto m8  = sinZ * sinX;
			auto m9  = -cosZ * sinX;
			T newM[] = {
				m[0] * cosZ + m[4] * sinZ,
				m[1] * cosZ + m[5] * sinZ,
				m[2] * cosZ + m[6] * sinZ,
				m[3] * cosZ + m[7] * sinZ,
				m[0] * m4 + m[4] * m5 + m[8] * sinX,
				m[1] * m4 + m[5] * m5 + m[9] * sinX,
				m[2] * m4 + m[6] * m5 + m[10] * sinX,
				m[3] * m4 + m[7] * m5 + m[11] * sinX,
				m[0] * m8 + m[4] * m9 + m[8] * cosX,
				m[1] * m8 + m[5] * m9 + m[9] * cosX,
				m[2] * m8 + m[6] * m9 + m[10] * cosX,
				m[3] * m8 + m[7] * m9 + m[11] * cosX,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateXZ(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			auto m4  = -sinZ * cosX;
			auto m5  = cosZ * cosX;
			auto m8  = sinZ * sinX;
			auto m9  = -cosZ * sinX;
			T newM[] = {
				m[0] * cosZ + m[4] * sinZ,
				m[1] * cosZ + m[5] * sinZ,
				m[2] * cosZ + m[6] * sinZ,
				m[3] * cosZ + m[7] * sinZ,
				m[0] * m4 + m[4] * m5 + m[8] * sinX,
				m[1] * m4 + m[5] * m5 + m[9] * sinX,
				m[2] * m4 + m[6] * m5 + m[10] * sinX,
				m[3] * m4 + m[7] * m5 + m[11] * sinX,
				m[0] * m8 + m[4] * m9 + m[8] * cosX,
				m[1] * m8 + m[5] * m9 + m[9] * cosX,
				m[2] * m8 + m[6] * m9 + m[10] * cosX,
				m[3] * m8 + m[7] * m9 + m[11] * cosX,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateYX(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto m1  = sinX * sinY;
			auto m2  = -cosX * sinY;
			auto m9  = -sinX * cosY;
			auto m10 = cosX * cosY;
			T newM[] = {
				m[0] * cosY + m[4] * m1 + m[8] * m2,
				m[1] * cosY + m[5] * m1 + m[9] * m2,
				m[2] * cosY + m[6] * m1 + m[10] * m2,
				m[3] * cosY + m[7] * m1 + m[11] * m2,
				m[4] * cosX + m[8] * sinX,
				m[5] * cosX + m[9] * sinX,
				m[6] * cosX + m[10] * sinX,
				m[7] * cosX + m[11] * sinX,
				m[0] * sinY + m[4] * m9 + m[8] * m10,
				m[1] * sinY + m[5] * m9 + m[9] * m10,
				m[2] * sinY + m[6] * m9 + m[10] * m10,
				m[3] * sinY + m[7] * m9 + m[11] * m10,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateYZ(T rotateY, T rotateZ) {
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			auto m0  = cosZ * cosY;
			auto m1  = sinZ * cosY;
			auto m8  = cosZ * sinY;
			auto m9  = sinZ * sinY;
			T newM[] = {
				m[0] * m0 + m[4] * m1 + m[8] * -sinY,
				m[1] * m0 + m[5] * m1 + m[9] * -sinY,
				m[2] * m0 + m[6] * m1 + m[10] * -sinY,
				m[3] * m0 + m[7] * m1 + m[11] * -sinY,
				m[0] * -sinZ + m[4] * cosZ,
				m[1] * -sinZ + m[5] * cosZ,
				m[2] * -sinZ + m[6] * cosZ,
				m[3] * -sinZ + m[7] * cosZ,
				m[0] * m8 + m[4] * m9 + m[8] * cosY,
				m[1] * m8 + m[5] * m9 + m[9] * cosY,
				m[2] * m8 + m[6] * m9 + m[10] * cosY,
				m[3] * m8 + m[7] * m9 + m[11] * cosY,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateYZ(const Vector<2, T> & rotate) {
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			auto m0  = cosZ * cosY;
			auto m1  = sinZ * cosY;
			auto m8  = cosZ * sinY;
			auto m9  = sinZ * sinY;
			T newM[] = {
				m[0] * m0 + m[4] * m1 + m[8] * -sinY,
				m[1] * m0 + m[5] * m1 + m[9] * -sinY,
				m[2] * m0 + m[6] * m1 + m[10] * -sinY,
				m[3] * m0 + m[7] * m1 + m[11] * -sinY,
				m[0] * -sinZ + m[4] * cosZ,
				m[1] * -sinZ + m[5] * cosZ,
				m[2] * -sinZ + m[6] * cosZ,
				m[3] * -sinZ + m[7] * cosZ,
				m[0] * m8 + m[4] * m9 + m[8] * cosY,
				m[1] * m8 + m[5] * m9 + m[9] * cosY,
				m[2] * m8 + m[6] * m9 + m[10] * cosY,
				m[3] * m8 + m[7] * m9 + m[11] * cosY,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateZX(const Vector<2, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			auto m1  = cosX * sinZ;
			auto m2  = sinX * sinZ;
			auto m5  = cosX * cosZ;
			auto m6  = sinX * cosZ;
			T newM[] = {
				m[0] * cosZ + m[4] * m1 + m[8] * m2,
				m[1] * cosZ + m[5] * m1 + m[9] * m2,
				m[2] * cosZ + m[6] * m1 + m[10] * m2,
				m[3] * cosZ + m[7] * m1 + m[11] * m2,
				m[0] * -sinZ + m[4] * m5 + m[8] * m6,
				m[1] * -sinZ + m[5] * m5 + m[9] * m6,
				m[2] * -sinZ + m[6] * m5 + m[10] * m6,
				m[3] * -sinZ + m[7] * m5 + m[11] * m6,
				m[4] * -sinX + m[8] * cosX,
				m[5] * -sinX + m[9] * cosX,
				m[6] * -sinX + m[10] * cosX,
				m[7] * -sinX + m[11] * cosX,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateZY(const Vector<2, T> & rotate) {
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			auto m0  = cosY * cosZ;
			auto m2  = -sinY * cosZ;
			auto m4  = -cosY * sinZ;
			auto m6  = sinY * sinZ;
			T newM[] = {
				m[0] * m0 + m[4] * sinZ + m[8] * m2,
				m[1] * m0 + m[5] * sinZ + m[9] * m2,
				m[2] * m0 + m[6] * sinZ + m[10] * m2,
				m[3] * m0 + m[7] * sinZ + m[11] * m2,
				m[0] * m4 + m[4] * cosZ + m[8] * m6,
				m[1] * m4 + m[5] * cosZ + m[9] * m6,
				m[2] * m4 + m[6] * cosZ + m[10] * m6,
				m[3] * m4 + m[7] * cosZ + m[11] * m6,
				m[0] * sinY + m[8] * cosY,
				m[1] * sinY + m[9] * cosY,
				m[2] * sinY + m[10] * cosY,
				m[3] * sinY + m[11] * cosY,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateXYZ(T rotateX, T rotateY, T rotateZ) {
			auto sinX = sin(rotateX);
			auto cosX = cos(rotateX);
			auto sinY = sin(rotateY);
			auto cosY = cos(rotateY);
			auto sinZ = sin(rotateZ);
			auto cosZ = cos(rotateZ);
			auto m0  = cosZ * cosY;
			auto m1  = sinZ * cosY;
			auto m4  = -sinZ * cosX + cosZ * sinY * sinX;
			auto m5  = cosZ * cosX + sinZ * sinY * sinX;
			auto m6  = cosY * sinX;
			auto m8  = sinZ * sinX + cosZ * sinY * cosX;
			auto m9  = -cosZ * sinX + sinZ * sinY * cosX;
			auto m10 = cosY * cosX;
			T newM[] = {
				m[0] * m0 + m[4] * m1 + m[8] * -sinY,
				m[1] * m0 + m[5] * m1 + m[9] * -sinY,
				m[2] * m0 + m[6] * m1 + m[10] * -sinY,
				m[3] * m0 + m[7] * m1 + m[11] * -sinY,
				m[0] * m4 + m[4] * m5 + m[8] * m6,
				m[1] * m4 + m[5] * m5 + m[9] * m6,
				m[2] * m4 + m[6] * m5 + m[10] * m6,
				m[3] * m4 + m[7] * m5 + m[11] * m6,
				m[0] * m8 + m[4] * m9 + m[8] * m10,
				m[1] * m8 + m[5] * m9 + m[9] * m10,
				m[2] * m8 + m[6] * m9 + m[10] * m10,
				m[3] * m8 + m[7] * m9 + m[11] * m10,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateXYZ(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[2]);
			auto cosZ = cos(rotate.v[2]);
			auto m0  = cosZ * cosY;
			auto m1  = sinZ * cosY;
			auto m4  = -sinZ * cosX + cosZ * sinY * sinX;
			auto m5  = cosZ * cosX + sinZ * sinY * sinX;
			auto m6  = cosY * sinX;
			auto m8  = sinZ * sinX + cosZ * sinY * cosX;
			auto m9  = -cosZ * sinX + sinZ * sinY * cosX;
			auto m10 = cosY * cosX;
			T newM[] = {
				m[0] * m0 + m[4] * m1 + m[8] * -sinY,
				m[1] * m0 + m[5] * m1 + m[9] * -sinY,
				m[2] * m0 + m[6] * m1 + m[10] * -sinY,
				m[3] * m0 + m[7] * m1 + m[11] * -sinY,
				m[0] * m4 + m[4] * m5 + m[8] * m6,
				m[1] * m4 + m[5] * m5 + m[9] * m6,
				m[2] * m4 + m[6] * m5 + m[10] * m6,
				m[3] * m4 + m[7] * m5 + m[11] * m6,
				m[0] * m8 + m[4] * m9 + m[8] * m10,
				m[1] * m8 + m[5] * m9 + m[9] * m10,
				m[2] * m8 + m[6] * m9 + m[10] * m10,
				m[3] * m8 + m[7] * m9 + m[11] * m10,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateXZY(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[0]);
			auto cosX = cos(rotate.v[0]);
			auto sinY = sin(rotate.v[2]);
			auto cosY = cos(rotate.v[2]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			auto m0  = cosY * cosZ;
			auto m2  = -sinY * cosZ;
			auto m4  = -cosY * sinZ * cosX + sinY * sinX;
			auto m5  = cosZ * cosX;
			auto m6  = sinY * sinZ * cosX + cosY * sinX;
			auto m8  = cosY * sinZ * sinX + sinY * cosX;
			auto m9  = -cosZ * sinX;
			auto m10 = -sinY * sinZ * sinX + cosY * cosX;
			T newM[] = {
				m[0] * m0 + m[4] * sinZ + m[8] * m2,
				m[1] * m0 + m[5] * sinZ + m[9] * m2,
				m[2] * m0 + m[6] * sinZ + m[10] * m2,
				m[3] * m0 + m[7] * sinZ + m[11] * m2,
				m[0] * m4 + m[4] * m5 + m[8] * m6,
				m[1] * m4 + m[5] * m5 + m[9] * m6,
				m[2] * m4 + m[6] * m5 + m[10] * m6,
				m[3] * m4 + m[7] * m5 + m[11] * m6,
				m[0] * m8 + m[4] * m9 + m[8] * m10,
				m[1] * m8 + m[5] * m9 + m[9] * m10,
				m[2] * m8 + m[6] * m9 + m[10] * m10,
				m[3] * m8 + m[7] * m9 + m[11] * m10,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateYXZ(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[2]);
			auto cosZ = cos(rotate.v[2]);
			auto m0  = cosZ * cosY - sinZ * sinX * sinY;
			auto m1  = sinZ * cosY + cosZ * sinX * sinY;
			auto m2  = -cosX * sinY;
			auto m4  = -sinZ * cosX;
			auto m5  = cosZ * cosX;
			auto m8  = cosZ * sinY + sinZ * sinX * cosY;
			auto m9  = sinZ * sinY - cosZ * sinX * cosY;
			auto m10 = cosX * cosY;
			T newM[] = {
				m[0] * m0 + m[4] * m1 + m[8] * m2,
				m[1] * m0 + m[5] * m1 + m[9] * m2,
				m[2] * m0 + m[6] * m1 + m[10] * m2,
				m[3] * m0 + m[7] * m1 + m[11] * m2,
				m[0] * m4 + m[4] * m5 + m[8] * sinX,
				m[1] * m4 + m[5] * m5 + m[9] * sinX,
				m[2] * m4 + m[6] * m5 + m[10] * sinX,
				m[3] * m4 + m[7] * m5 + m[11] * sinX,
				m[0] * m8 + m[4] * m9 + m[8] * m10,
				m[1] * m8 + m[5] * m9 + m[9] * m10,
				m[2] * m8 + m[6] * m9 + m[10] * m10,
				m[3] * m8 + m[7] * m9 + m[11] * m10,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateYZX(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[2]);
			auto cosX = cos(rotate.v[2]);
			auto sinY = sin(rotate.v[0]);
			auto cosY = cos(rotate.v[0]);
			auto sinZ = sin(rotate.v[1]);
			auto cosZ = cos(rotate.v[1]);
			auto m0  = cosZ * cosY;
			auto m1  = cosX * sinZ * cosY + sinX * sinY;
			auto m2  = sinX * sinZ * cosY - cosX * sinY;
			auto m5  = cosX * cosZ;
			auto m6  = sinX * cosZ;
			auto m8  = cosZ * sinY;
			auto m9  = cosX * sinZ * sinY - sinX * cosY;
			auto m10 = sinX * sinZ * sinY + cosX * cosY;
			T newM[] = {
				m[0] * m0 + m[4] * m1 + m[8] * m2,
				m[1] * m0 + m[5] * m1 + m[9] * m2,
				m[2] * m0 + m[6] * m1 + m[10] * m2,
				m[3] * m0 + m[7] * m1 + m[11] * m2,
				m[0] * -sinZ + m[4] * m5 + m[8] * m6,
				m[1] * -sinZ + m[5] * m5 + m[9] * m6,
				m[2] * -sinZ + m[6] * m5 + m[10] * m6,
				m[3] * -sinZ + m[7] * m5 + m[11] * m6,
				m[0] * m8 + m[4] * m9 + m[8] * m10,
				m[1] * m8 + m[5] * m9 + m[9] * m10,
				m[2] * m8 + m[6] * m9 + m[10] * m10,
				m[3] * m8 + m[7] * m9 + m[11] * m10,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateZXY(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[1]);
			auto cosX = cos(rotate.v[1]);
			auto sinY = sin(rotate.v[2]);
			auto cosY = cos(rotate.v[2]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			auto m0  = cosY * cosZ + sinY * sinX * sinZ;
			auto m1  = cosX * sinZ;
			auto m2  = -sinY * cosZ + cosY * sinX * sinZ;
			auto m4  = -cosY * sinZ + sinY * sinX * cosZ;
			auto m5  = cosX * cosZ;
			auto m6  = sinY * sinZ + cosY * sinX * cosZ;
			auto m8  = sinY * cosX;
			auto m10 = cosY * cosX;
			T newM[] = {
				m[0] * m0 + m[4] * m1 + m[8] * m2,
				m[1] * m0 + m[5] * m1 + m[9] * m2,
				m[2] * m0 + m[6] * m1 + m[10] * m2,
				m[3] * m0 + m[7] * m1 + m[11] * m2,
				m[0] * m4 + m[4] * m5 + m[8] * m6,
				m[1] * m4 + m[5] * m5 + m[9] * m6,
				m[2] * m4 + m[6] * m5 + m[10] * m6,
				m[3] * m4 + m[7] * m5 + m[11] * m6,
				m[0] * m8 + m[4] * -sinX + m[8] * m10,
				m[1] * m8 + m[5] * -sinX + m[9] * m10,
				m[2] * m8 + m[6] * -sinX + m[10] * m10,
				m[3] * m8 + m[7] * -sinX + m[11] * m10,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & rotateZYX(const Vector<3, T> & rotate) {
			auto sinX = sin(rotate.v[2]);
			auto cosX = cos(rotate.v[2]);
			auto sinY = sin(rotate.v[1]);
			auto cosY = cos(rotate.v[1]);
			auto sinZ = sin(rotate.v[0]);
			auto cosZ = cos(rotate.v[0]);
			auto m0  = cosY * cosZ;
			auto m1  = sinX * sinY * cosZ + cosX * sinZ;
			auto m2  = -cosX * sinY * cosZ + sinX * sinZ;
			auto m4  = -cosY * sinZ;
			auto m5  = -sinX * sinY * sinZ + cosX * cosZ;
			auto m6  = cosX * sinY * sinZ + sinX * cosZ;
			auto m9  = -sinX * cosY;
			auto m10 = cosX * cosY;
			T newM[] = {
				m[0] * m0 + m[4] * m1 + m[8] * m2,
				m[1] * m0 + m[5] * m1 + m[9] * m2,
				m[2] * m0 + m[6] * m1 + m[10] * m2,
				m[3] * m0 + m[7] * m1 + m[11] * m2,
				m[0] * m4 + m[4] * m5 + m[8] * m6,
				m[1] * m4 + m[5] * m5 + m[9] * m6,
				m[2] * m4 + m[6] * m5 + m[10] * m6,
				m[3] * m4 + m[7] * m5 + m[11] * m6,
				m[0] * sinY + m[4] * m9 + m[8] * m10,
				m[1] * sinY + m[5] * m9 + m[9] * m10,
				m[2] * sinY + m[6] * m9 + m[10] * m10,
				m[3] * sinY + m[7] * m9 + m[11] * m10,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleX(T scaleX) {
			T newM[] = {
				m[0] * scaleX,
				m[1] * scaleX,
				m[2] * scaleX,
				m[3] * scaleX,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleY(T scaleY) {
			T newM[] = {
				m[4] * scaleY,
				m[5] * scaleY,
				m[6] * scaleY,
				m[7] * scaleY,
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			m[4 ] = newM[0 ]; m[5 ] = newM[1 ]; m[6 ] = newM[2 ]; m[7 ] = newM[3 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleZ(T scaleZ) {
			T newM[] = {
				m[8] * scaleZ,
				m[9] * scaleZ,
				m[10] * scaleZ,
				m[11] * scaleZ,
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			m[8 ] = newM[0 ]; m[9 ] = newM[1 ]; m[10] = newM[2 ]; m[11] = newM[3 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleXY(T scale) {
			T newM[] = {
				m[0] * scale,
				m[1] * scale,
				m[2] * scale,
				m[3] * scale,
				m[4] * scale,
				m[5] * scale,
				m[6] * scale,
				m[7] * scale,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleXY(T scaleX, T scaleY) {
			T newM[] = {
				m[0] * scaleX,
				m[1] * scaleX,
				m[2] * scaleX,
				m[3] * scaleX,
				m[4] * scaleY,
				m[5] * scaleY,
				m[6] * scaleY,
				m[7] * scaleY,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleXY(const Vector<2, T> & scale) {
			T newM[] = {
				m[0] * scale.v[0],
				m[1] * scale.v[0],
				m[2] * scale.v[0],
				m[3] * scale.v[0],
				m[4] * scale.v[1],
				m[5] * scale.v[1],
				m[6] * scale.v[1],
				m[7] * scale.v[1],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleXZ(T scale) {
			T newM[] = {
				m[0] * scale,
				m[1] * scale,
				m[2] * scale,
				m[3] * scale,
				m[8] * scale,
				m[9] * scale,
				m[10] * scale,
				m[11] * scale,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			m[8 ] = newM[4 ]; m[9 ] = newM[5 ]; m[10] = newM[6 ]; m[11] = newM[7 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleXZ(T scaleX, T scaleZ) {
			T newM[] = {
				m[0] * scaleX,
				m[1] * scaleX,
				m[2] * scaleX,
				m[3] * scaleX,
				m[8] * scaleZ,
				m[9] * scaleZ,
				m[10] * scaleZ,
				m[11] * scaleZ,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			m[8 ] = newM[4 ]; m[9 ] = newM[5 ]; m[10] = newM[6 ]; m[11] = newM[7 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleXZ(const Vector<2, T> & scale) {
			T newM[] = {
				m[0] * scale.v[0],
				m[1] * scale.v[0],
				m[2] * scale.v[0],
				m[3] * scale.v[0],
				m[8] * scale.v[1],
				m[9] * scale.v[1],
				m[10] * scale.v[1],
				m[11] * scale.v[1],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			m[8 ] = newM[4 ]; m[9 ] = newM[5 ]; m[10] = newM[6 ]; m[11] = newM[7 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleYX(const Vector<2, T> & scale) {
			T newM[] = {
				m[0] * scale.v[1],
				m[1] * scale.v[1],
				m[2] * scale.v[1],
				m[3] * scale.v[1],
				m[4] * scale.v[0],
				m[5] * scale.v[0],
				m[6] * scale.v[0],
				m[7] * scale.v[0],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			/**** m[8 ] ****/ /**** m[9 ] ****/ /**** m[10] ****/ /**** m[11] ****/
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleYZ(T scale) {
			T newM[] = {
				m[4] * scale,
				m[5] * scale,
				m[6] * scale,
				m[7] * scale,
				m[8] * scale,
				m[9] * scale,
				m[10] * scale,
				m[11] * scale,
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			m[4 ] = newM[0 ]; m[5 ] = newM[1 ]; m[6 ] = newM[2 ]; m[7 ] = newM[3 ];
			m[8 ] = newM[4 ]; m[9 ] = newM[5 ]; m[10] = newM[6 ]; m[11] = newM[7 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleYZ(T scaleY, T scaleZ) {
			T newM[] = {
				m[4] * scaleY,
				m[5] * scaleY,
				m[6] * scaleY,
				m[7] * scaleY,
				m[8] * scaleZ,
				m[9] * scaleZ,
				m[10] * scaleZ,
				m[11] * scaleZ,
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			m[4 ] = newM[0 ]; m[5 ] = newM[1 ]; m[6 ] = newM[2 ]; m[7 ] = newM[3 ];
			m[8 ] = newM[4 ]; m[9 ] = newM[5 ]; m[10] = newM[6 ]; m[11] = newM[7 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleYZ(const Vector<2, T> & scale) {
			T newM[] = {
				m[4] * scale.v[0],
				m[5] * scale.v[0],
				m[6] * scale.v[0],
				m[7] * scale.v[0],
				m[8] * scale.v[1],
				m[9] * scale.v[1],
				m[10] * scale.v[1],
				m[11] * scale.v[1],
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			m[4 ] = newM[0 ]; m[5 ] = newM[1 ]; m[6 ] = newM[2 ]; m[7 ] = newM[3 ];
			m[8 ] = newM[4 ]; m[9 ] = newM[5 ]; m[10] = newM[6 ]; m[11] = newM[7 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleZX(const Vector<2, T> & scale) {
			T newM[] = {
				m[0] * scale.v[1],
				m[1] * scale.v[1],
				m[2] * scale.v[1],
				m[3] * scale.v[1],
				m[8] * scale.v[0],
				m[9] * scale.v[0],
				m[10] * scale.v[0],
				m[11] * scale.v[0],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			/**** m[4 ] ****/ /**** m[5 ] ****/ /**** m[6 ] ****/ /**** m[7 ] ****/
			m[8 ] = newM[4 ]; m[9 ] = newM[5 ]; m[10] = newM[6 ]; m[11] = newM[7 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleZY(const Vector<2, T> & scale) {
			T newM[] = {
				m[4] * scale.v[1],
				m[5] * scale.v[1],
				m[6] * scale.v[1],
				m[7] * scale.v[1],
				m[8] * scale.v[0],
				m[9] * scale.v[0],
				m[10] * scale.v[0],
				m[11] * scale.v[0],
			};
			/**** m[0 ] ****/ /**** m[1 ] ****/ /**** m[2 ] ****/ /**** m[3 ] ****/
			m[4 ] = newM[0 ]; m[5 ] = newM[1 ]; m[6 ] = newM[2 ]; m[7 ] = newM[3 ];
			m[8 ] = newM[4 ]; m[9 ] = newM[5 ]; m[10] = newM[6 ]; m[11] = newM[7 ];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleXYZ(T scale) {
			T newM[] = {
				m[0] * scale,
				m[1] * scale,
				m[2] * scale,
				m[3] * scale,
				m[4] * scale,
				m[5] * scale,
				m[6] * scale,
				m[7] * scale,
				m[8] * scale,
				m[9] * scale,
				m[10] * scale,
				m[11] * scale,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleXYZ(T scaleX, T scaleY, T scaleZ) {
			T newM[] = {
				m[0] * scaleX,
				m[1] * scaleX,
				m[2] * scaleX,
				m[3] * scaleX,
				m[4] * scaleY,
				m[5] * scaleY,
				m[6] * scaleY,
				m[7] * scaleY,
				m[8] * scaleZ,
				m[9] * scaleZ,
				m[10] * scaleZ,
				m[11] * scaleZ,
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleXYZ(const Vector<3, T> & scale) {
			T newM[] = {
				m[0] * scale.v[0],
				m[1] * scale.v[0],
				m[2] * scale.v[0],
				m[3] * scale.v[0],
				m[4] * scale.v[1],
				m[5] * scale.v[1],
				m[6] * scale.v[1],
				m[7] * scale.v[1],
				m[8] * scale.v[2],
				m[9] * scale.v[2],
				m[10] * scale.v[2],
				m[11] * scale.v[2],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleXZY(const Vector<3, T> & scale) {
			T newM[] = {
				m[0] * scale.v[0],
				m[1] * scale.v[0],
				m[2] * scale.v[0],
				m[3] * scale.v[0],
				m[4] * scale.v[2],
				m[5] * scale.v[2],
				m[6] * scale.v[2],
				m[7] * scale.v[2],
				m[8] * scale.v[1],
				m[9] * scale.v[1],
				m[10] * scale.v[1],
				m[11] * scale.v[1],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleYXZ(const Vector<3, T> & scale) {
			T newM[] = {
				m[0] * scale.v[1],
				m[1] * scale.v[1],
				m[2] * scale.v[1],
				m[3] * scale.v[1],
				m[4] * scale.v[0],
				m[5] * scale.v[0],
				m[6] * scale.v[0],
				m[7] * scale.v[0],
				m[8] * scale.v[2],
				m[9] * scale.v[2],
				m[10] * scale.v[2],
				m[11] * scale.v[2],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleYZX(const Vector<3, T> & scale) {
			T newM[] = {
				m[0] * scale.v[2],
				m[1] * scale.v[2],
				m[2] * scale.v[2],
				m[3] * scale.v[2],
				m[4] * scale.v[0],
				m[5] * scale.v[0],
				m[6] * scale.v[0],
				m[7] * scale.v[0],
				m[8] * scale.v[1],
				m[9] * scale.v[1],
				m[10] * scale.v[1],
				m[11] * scale.v[1],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleZXY(const Vector<3, T> & scale) {
			T newM[] = {
				m[0] * scale.v[1],
				m[1] * scale.v[1],
				m[2] * scale.v[1],
				m[3] * scale.v[1],
				m[4] * scale.v[2],
				m[5] * scale.v[2],
				m[6] * scale.v[2],
				m[7] * scale.v[2],
				m[8] * scale.v[0],
				m[9] * scale.v[0],
				m[10] * scale.v[0],
				m[11] * scale.v[0],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		Matrix<4, 4, T> & scaleZYX(const Vector<3, T> & scale) {
			T newM[] = {
				m[0] * scale.v[2],
				m[1] * scale.v[2],
				m[2] * scale.v[2],
				m[3] * scale.v[2],
				m[4] * scale.v[1],
				m[5] * scale.v[1],
				m[6] * scale.v[1],
				m[7] * scale.v[1],
				m[8] * scale.v[0],
				m[9] * scale.v[0],
				m[10] * scale.v[0],
				m[11] * scale.v[0],
			};
			m[0 ] = newM[0 ]; m[1 ] = newM[1 ]; m[2 ] = newM[2 ]; m[3 ] = newM[3 ];
			m[4 ] = newM[4 ]; m[5 ] = newM[5 ]; m[6 ] = newM[6 ]; m[7 ] = newM[7 ];
			m[8 ] = newM[8 ]; m[9 ] = newM[9 ]; m[10] = newM[10]; m[11] = newM[11];
			/**** m[12] ****/ /**** m[13] ****/ /**** m[14] ****/ /**** m[15] ****/
			return *this;
		}

		~Matrix() {
			clean();
		}
};

template <typename T>
Matrix<4, 4, T> operator+(const Matrix<4, 4, T> & left, const Matrix<4, 4, T> & right) {
	return new T[4 * 4] {
		left.m[0 ] + right.m[0 ], left.m[1 ] + right.m[1 ], left.m[2 ] + right.m[2 ], left.m[3 ] + right.m[3 ],
		left.m[4 ] + right.m[4 ], left.m[5 ] + right.m[5 ], left.m[6 ] + right.m[6 ], left.m[7 ] + right.m[7 ],
		left.m[8 ] + right.m[8 ], left.m[9 ] + right.m[9 ], left.m[10] + right.m[10], left.m[11] + right.m[11],
		left.m[12] + right.m[12], left.m[13] + right.m[13], left.m[14] + right.m[14], left.m[15] + right.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> operator-(const Matrix<4, 4, T> & left, const Matrix<4, 4, T> & right) {
	return new T[4 * 4] {
		left.m[0 ] - right.m[0 ], left.m[1 ] - right.m[1 ], left.m[2 ] - right.m[2 ], left.m[3 ] - right.m[3 ],
		left.m[4 ] - right.m[4 ], left.m[5 ] - right.m[5 ], left.m[6 ] - right.m[6 ], left.m[7 ] - right.m[7 ],
		left.m[8 ] - right.m[8 ], left.m[9 ] - right.m[9 ], left.m[10] - right.m[10], left.m[11] - right.m[11],
		left.m[12] - right.m[12], left.m[13] - right.m[13], left.m[14] - right.m[14], left.m[15] - right.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> operator*(const Matrix<4, 4, T> & left, const Matrix<4, 4, T> & right) {
	return new T[4 * 4] {
		left.m[0] * right.m[0 ] + left.m[4] * right.m[1 ] + left.m[8 ] * right.m[2 ] + left.m[12] * right.m[3 ],
		left.m[1] * right.m[0 ] + left.m[5] * right.m[1 ] + left.m[9 ] * right.m[2 ] + left.m[13] * right.m[3 ],
		left.m[2] * right.m[0 ] + left.m[6] * right.m[1 ] + left.m[10] * right.m[2 ] + left.m[14] * right.m[3 ],
		left.m[3] * right.m[0 ] + left.m[7] * right.m[1 ] + left.m[11] * right.m[2 ] + left.m[15] * right.m[3 ],
		left.m[0] * right.m[4 ] + left.m[4] * right.m[5 ] + left.m[8 ] * right.m[6 ] + left.m[12] * right.m[7 ],
		left.m[1] * right.m[4 ] + left.m[5] * right.m[5 ] + left.m[9 ] * right.m[6 ] + left.m[13] * right.m[7 ],
		left.m[2] * right.m[4 ] + left.m[6] * right.m[5 ] + left.m[10] * right.m[6 ] + left.m[14] * right.m[7 ],
		left.m[3] * right.m[4 ] + left.m[7] * right.m[5 ] + left.m[11] * right.m[6 ] + left.m[15] * right.m[7 ],
		left.m[0] * right.m[8 ] + left.m[4] * right.m[9 ] + left.m[8 ] * right.m[10] + left.m[12] * right.m[11],
		left.m[1] * right.m[8 ] + left.m[5] * right.m[9 ] + left.m[9 ] * right.m[10] + left.m[13] * right.m[11],
		left.m[2] * right.m[8 ] + left.m[6] * right.m[9 ] + left.m[10] * right.m[10] + left.m[14] * right.m[11],
		left.m[3] * right.m[8 ] + left.m[7] * right.m[9 ] + left.m[11] * right.m[10] + left.m[15] * right.m[11],
		left.m[0] * right.m[12] + left.m[4] * right.m[13] + left.m[8 ] * right.m[14] + left.m[12] * right.m[15],
		left.m[1] * right.m[12] + left.m[5] * right.m[13] + left.m[9 ] * right.m[14] + left.m[13] * right.m[15],
		left.m[2] * right.m[12] + left.m[6] * right.m[13] + left.m[10] * right.m[14] + left.m[14] * right.m[15],
		left.m[3] * right.m[12] + left.m[7] * right.m[13] + left.m[11] * right.m[14] + left.m[15] * right.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> projectOrthographic(const Matrix<4, 4, T> & matrix, T left, T right, T bottom, T top, T near, T far) {
	auto m0  = 2 / (right -   left);
	auto m5  = 2 / (top   - bottom);
	auto m10 = 2 / (far   -   near);
	auto m12 = (left   + right) / (left   - right);
	auto m13 = (bottom +   top) / (bottom -   top);
	auto m14 = (near   +   far) / (near   -   far);
	return new T[4 * 4] {
		matrix.m[0 ] * m0,
		matrix.m[1 ] * m0,
		matrix.m[2 ] * m0,
		matrix.m[3 ] * m0,
		matrix.m[4 ] * m5,
		matrix.m[5 ] * m5,
		matrix.m[6 ] * m5,
		matrix.m[7 ] * m5,
		matrix.m[8 ] * m10,
		matrix.m[9 ] * m10,
		matrix.m[10] * m10,
		matrix.m[11] * m10,
		matrix.m[0 ] * m12 + matrix.m[4] * m13 + matrix.m[8 ] * m14 + matrix.m[12],
		matrix.m[1 ] * m12 + matrix.m[5] * m13 + matrix.m[9 ] * m14 + matrix.m[13],
		matrix.m[2 ] * m12 + matrix.m[6] * m13 + matrix.m[10] * m14 + matrix.m[14],
		matrix.m[3 ] * m12 + matrix.m[7] * m13 + matrix.m[11] * m14 + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & projectOrthographic(const Matrix<4, 4, T> & matrix, T left, T right, T bottom, T top, T near, T far, Matrix<4, 4, T> & target) {
	auto m0  = 2 / (right -   left);
	auto m5  = 2 / (top   - bottom);
	auto m10 = 2 / (far   -   near);
	auto m12 = (left   + right) / (left   - right);
	auto m13 = (bottom +   top) / (bottom -   top);
	auto m14 = (near   +   far) / (near   -   far);
	target.m[0 ] = matrix.m[0 ] * m0;
	target.m[1 ] = matrix.m[1 ] * m0;
	target.m[2 ] = matrix.m[2 ] * m0;
	target.m[3 ] = matrix.m[3 ] * m0;
	target.m[4 ] = matrix.m[4 ] * m5;
	target.m[5 ] = matrix.m[5 ] * m5;
	target.m[6 ] = matrix.m[6 ] * m5;
	target.m[7 ] = matrix.m[7 ] * m5;
	target.m[8 ] = matrix.m[8 ] * m10;
	target.m[9 ] = matrix.m[9 ] * m10;
	target.m[10] = matrix.m[10] * m10;
	target.m[11] = matrix.m[11] * m10;
	target.m[12] = matrix.m[0 ] * m12 + matrix.m[4] * m13 + matrix.m[8 ] * m14 + matrix.m[12];
	target.m[13] = matrix.m[1 ] * m12 + matrix.m[5] * m13 + matrix.m[9 ] * m14 + matrix.m[13];
	target.m[14] = matrix.m[2 ] * m12 + matrix.m[6] * m13 + matrix.m[10] * m14 + matrix.m[14];
	target.m[15] = matrix.m[3 ] * m12 + matrix.m[7] * m13 + matrix.m[11] * m14 + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> projectPerspective(const Matrix<4, 4, T> & matrix, T fovY, T aspectRatio, T near, T far) {
	auto vertical = 1 / tan(fovY / 2);
	auto m0  = vertical / aspectRatio;
	auto m10 = (near + far) / (near - far);
	auto m14 = (2 * far * near) / (near - far);
	return new T[4 * 4] {
		matrix.m[0 ] * m0,
		matrix.m[1 ] * m0,
		matrix.m[2 ] * m0,
		matrix.m[3 ] * m0,
		matrix.m[4 ] * vertical,
		matrix.m[5 ] * vertical,
		matrix.m[6 ] * vertical,
		matrix.m[7 ] * vertical,
		matrix.m[8 ] * m10 - matrix.m[12],
		matrix.m[9 ] * m10 - matrix.m[13],
		matrix.m[10] * m10 - matrix.m[14],
		matrix.m[11] * m10 - matrix.m[15],
		matrix.m[8 ] * m14,
		matrix.m[9 ] * m14,
		matrix.m[10] * m14,
		matrix.m[11] * m14
	};
}

template <typename T>
Matrix<4, 4, T> & projectPerspective(const Matrix<4, 4, T> & matrix, T fovY, T aspectRatio, T near, T far, Matrix<4, 4, T> & target) {
	auto vertical = 1 / tan(fovY / 2);
	auto m0  = vertical / aspectRatio;
	auto m10 = (near + far) / (near - far);
	auto m14 = (2 * far * near) / (near - far);
	target.m[0 ] = matrix.m[0 ] * m0;
	target.m[1 ] = matrix.m[1 ] * m0;
	target.m[2 ] = matrix.m[2 ] * m0;
	target.m[3 ] = matrix.m[3 ] * m0;
	target.m[4 ] = matrix.m[4 ] * vertical;
	target.m[5 ] = matrix.m[5 ] * vertical;
	target.m[6 ] = matrix.m[6 ] * vertical;
	target.m[7 ] = matrix.m[7 ] * vertical;
	target.m[8 ] = matrix.m[8 ] * m10 - matrix.m[12];
	target.m[9 ] = matrix.m[9 ] * m10 - matrix.m[13];
	target.m[10] = matrix.m[10] * m10 - matrix.m[14];
	target.m[11] = matrix.m[11] * m10 - matrix.m[15];
	target.m[12] = matrix.m[8 ] * m14;
	target.m[13] = matrix.m[9 ] * m14;
	target.m[14] = matrix.m[10] * m14;
	target.m[15] = matrix.m[11] * m14;
	return target;
}

template <typename T>
Matrix<4, 4, T> shear2D(const Matrix<4, 4, T> & matrix, T axisAngle, T shearAngle) {
	auto sinAxis = sin(axisAngle);
	auto cosAxis = cos(axisAngle);
	auto tanShear = tan(shearAngle);
	auto m0  = sinAxis * tanShear * cosAxis;
	auto m1  = -cosAxis * tanShear * cosAxis;
	auto m4  = sinAxis * tanShear * sinAxis;
	auto m5  = -cosAxis * tanShear * sinAxis;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1,
		matrix.m[1] * m0 + matrix.m[5] * m1,
		matrix.m[2] * m0 + matrix.m[6] * m1,
		matrix.m[3] * m0 + matrix.m[7] * m1,
		matrix.m[0] * m4 + matrix.m[4] * m5,
		matrix.m[1] * m4 + matrix.m[5] * m5,
		matrix.m[2] * m4 + matrix.m[6] * m5,
		matrix.m[3] * m4 + matrix.m[7] * m5,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> shear2D(const Matrix<4, 4, T> & matrix, const Vector<2, T> & axis, T shearAngle) {
	auto unitAxis = normalize(axis);
	auto tanShear = tan(shearAngle);
	auto m0  = unitAxis.v[1] * tanShear * unitAxis.v[0];
	auto m1  = -unitAxis.v[0] * tanShear * unitAxis.v[0];
	auto m4  = unitAxis.v[1] * tanShear * unitAxis.v[1];
	auto m5  = -unitAxis.v[0] * tanShear * unitAxis.v[1];
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1,
		matrix.m[1] * m0 + matrix.m[5] * m1,
		matrix.m[2] * m0 + matrix.m[6] * m1,
		matrix.m[3] * m0 + matrix.m[7] * m1,
		matrix.m[0] * m4 + matrix.m[4] * m5,
		matrix.m[1] * m4 + matrix.m[5] * m5,
		matrix.m[2] * m4 + matrix.m[6] * m5,
		matrix.m[3] * m4 + matrix.m[7] * m5,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> shear2D(const Matrix<4, 4, T> & matrix, T axisAngle,const Vector<2, T> & shear) {
	auto sinAxis = sin(axisAngle);
	auto cosAxis = cos(axisAngle);
	auto tanShear = shear.v[1] / shear.v[0];
	auto m0  = sinAxis * tanShear * cosAxis;
	auto m1  = -cosAxis * tanShear * cosAxis;
	auto m4  = sinAxis * tanShear * sinAxis;
	auto m5  = -cosAxis * tanShear * sinAxis;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1,
		matrix.m[1] * m0 + matrix.m[5] * m1,
		matrix.m[2] * m0 + matrix.m[6] * m1,
		matrix.m[3] * m0 + matrix.m[7] * m1,
		matrix.m[0] * m4 + matrix.m[4] * m5,
		matrix.m[1] * m4 + matrix.m[5] * m5,
		matrix.m[2] * m4 + matrix.m[6] * m5,
		matrix.m[3] * m4 + matrix.m[7] * m5,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> shear2D(const Matrix<4, 4, T> & matrix, const Vector<2, T> & axis, const Vector<2, T> & shear) {
	auto unitAxis = normalize(axis);
	auto tanShear = shear.v[1] / shear.v[0];
	auto m0  = unitAxis.v[1] * tanShear * unitAxis.v[0];
	auto m1  = -unitAxis.v[0] * tanShear * unitAxis.v[0];
	auto m4  = unitAxis.v[1] * tanShear * unitAxis.v[1];
	auto m5  = -unitAxis.v[0] * tanShear * unitAxis.v[1];
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1,
		matrix.m[1] * m0 + matrix.m[5] * m1,
		matrix.m[2] * m0 + matrix.m[6] * m1,
		matrix.m[3] * m0 + matrix.m[7] * m1,
		matrix.m[0] * m4 + matrix.m[4] * m5,
		matrix.m[1] * m4 + matrix.m[5] * m5,
		matrix.m[2] * m4 + matrix.m[6] * m5,
		matrix.m[3] * m4 + matrix.m[7] * m5,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> shear2DX(const Matrix<4, 4, T> & matrix, T angle) {
	auto m1  = -tan(angle);
	return new T[4 * 4] {
		matrix.m[0] + matrix.m[4] * m1,
		matrix.m[1] + matrix.m[5] * m1,
		matrix.m[2] + matrix.m[6] * m1,
		matrix.m[3] + matrix.m[7] * m1,
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> shear2DX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & shear) {
	auto m1  = -shear.v[1] / shear.v[0];
	return new T[4 * 4] {
		matrix.m[0] + matrix.m[4] * m1,
		matrix.m[1] + matrix.m[5] * m1,
		matrix.m[2] + matrix.m[6] * m1,
		matrix.m[3] + matrix.m[7] * m1,
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> shear2DY(const Matrix<4, 4, T> & matrix, T angle) {
	auto m4  = tan(angle);
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[0] * m4 + matrix.m[4],
		matrix.m[1] * m4 + matrix.m[5],
		matrix.m[2] * m4 + matrix.m[6],
		matrix.m[3] * m4 + matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> shear2DY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & shear) {
	auto m4  = shear.v[1] / shear.v[0];
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[0] * m4 + matrix.m[4],
		matrix.m[1] * m4 + matrix.m[5],
		matrix.m[2] * m4 + matrix.m[6],
		matrix.m[3] * m4 + matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & shear2D(const Matrix<4, 4, T> & matrix, T axisAngle, T shearAngle, Matrix<4, 4, T> & target) {
	auto sinAxis = sin(axisAngle);
	auto cosAxis = cos(axisAngle);
	auto tanShear = tan(shearAngle);
	auto m0  = sinAxis * tanShear * cosAxis;
	auto m1  = -cosAxis * tanShear * cosAxis;
	auto m4  = sinAxis * tanShear * sinAxis;
	auto m5  = -cosAxis * tanShear * sinAxis;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & shear2D(const Matrix<4, 4, T> & matrix, const Vector<2, T> & axis, T shearAngle, Matrix<4, 4, T> & target) {
	auto unitAxis = normalize(axis);
	auto tanShear = tan(shearAngle);
	auto m0  = unitAxis.v[1] * tanShear * unitAxis.v[0];
	auto m1  = -unitAxis.v[0] * tanShear * unitAxis.v[0];
	auto m4  = unitAxis.v[1] * tanShear * unitAxis.v[1];
	auto m5  = -unitAxis.v[0] * tanShear * unitAxis.v[1];
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & shear2D(const Matrix<4, 4, T> & matrix, T axisAngle,const Vector<2, T> & shear, Matrix<4, 4, T> & target) {
	auto sinAxis = sin(axisAngle);
	auto cosAxis = cos(axisAngle);
	auto tanShear = shear.v[1] / shear.v[0];
	auto m0  = sinAxis * tanShear * cosAxis;
	auto m1  = -cosAxis * tanShear * cosAxis;
	auto m4  = sinAxis * tanShear * sinAxis;
	auto m5  = -cosAxis * tanShear * sinAxis;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & shear2D(const Matrix<4, 4, T> & matrix, const Vector<2, T> & axis, const Vector<2, T> & shear, Matrix<4, 4, T> & target) {
	auto unitAxis = normalize(axis);
	auto tanShear = shear.v[1] / shear.v[0];
	auto m0  = unitAxis.v[1] * tanShear * unitAxis.v[0];
	auto m1  = -unitAxis.v[0] * tanShear * unitAxis.v[0];
	auto m4  = unitAxis.v[1] * tanShear * unitAxis.v[1];
	auto m5  = -unitAxis.v[0] * tanShear * unitAxis.v[1];
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & shear2DX(const Matrix<4, 4, T> & matrix, T angle, Matrix<4, 4, T> & target) {
	auto m1  = -tan(angle);
	target.m[0] = matrix.m[0] + matrix.m[4] * m1;
	target.m[1] = matrix.m[1] + matrix.m[5] * m1;
	target.m[2] = matrix.m[2] + matrix.m[6] * m1;
	target.m[3] = matrix.m[3] + matrix.m[7] * m1;
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & shear2DX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & shear, Matrix<4, 4, T> & target) {
	auto m1  = -shear.v[1] / shear.v[0];
	target.m[0] = matrix.m[0] + matrix.m[4] * m1;
	target.m[1] = matrix.m[1] + matrix.m[5] * m1;
	target.m[2] = matrix.m[2] + matrix.m[6] * m1;
	target.m[3] = matrix.m[3] + matrix.m[7] * m1;
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & shear2DY(const Matrix<4, 4, T> & matrix, T angle, Matrix<4, 4, T> & target) {
	auto m4  = tan(angle);
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[0] * m4 + matrix.m[4];
	target.m[5] = matrix.m[1] * m4 + matrix.m[5];
	target.m[6] = matrix.m[2] * m4 + matrix.m[6];
	target.m[7] = matrix.m[3] * m4 + matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & shear2DY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & shear, Matrix<4, 4, T> & target) {
	auto m4  = shear.v[1] / shear.v[0];
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[0] * m4 + matrix.m[4];
	target.m[5] = matrix.m[1] * m4 + matrix.m[5];
	target.m[6] = matrix.m[2] * m4 + matrix.m[6];
	target.m[7] = matrix.m[3] * m4 + matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> translate2D(const Matrix<4, 4, T> & matrix, T translateX, T translateY) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translateX + matrix.m[4] * translateY + matrix.m[12],
		matrix.m[1] * translateX + matrix.m[5] * translateY + matrix.m[13],
		matrix.m[2] * translateX + matrix.m[6] * translateY + matrix.m[14],
		matrix.m[3] * translateX + matrix.m[7] * translateY + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translate2D(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[0] + matrix.m[4] * translate.v[1] + matrix.m[12],
		matrix.m[1] * translate.v[0] + matrix.m[5] * translate.v[1] + matrix.m[13],
		matrix.m[2] * translate.v[0] + matrix.m[6] * translate.v[1] + matrix.m[14],
		matrix.m[3] * translate.v[0] + matrix.m[7] * translate.v[1] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & translate2D(const Matrix<4, 4, T> & matrix, T translateX, T translateY, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translateX + matrix.m[4] * translateY + matrix.m[12];
	target.m[13] = matrix.m[1] * translateX + matrix.m[5] * translateY + matrix.m[13];
	target.m[14] = matrix.m[2] * translateX + matrix.m[6] * translateY + matrix.m[14];
	target.m[15] = matrix.m[3] * translateX + matrix.m[7] * translateY + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translate2D(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[0] + matrix.m[4] * translate.v[1] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[0] + matrix.m[5] * translate.v[1] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[0] + matrix.m[6] * translate.v[1] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[0] + matrix.m[7] * translate.v[1] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> translate(const Matrix<4, 4, T> & matrix, T translateX, T translateY, T translateZ) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translateX + matrix.m[4] * translateY + matrix.m[8] * translateZ + matrix.m[12],
		matrix.m[1] * translateX + matrix.m[5] * translateY + matrix.m[9] * translateZ + matrix.m[13],
		matrix.m[2] * translateX + matrix.m[6] * translateY + matrix.m[10] * translateZ + matrix.m[14],
		matrix.m[3] * translateX + matrix.m[7] * translateY + matrix.m[11] * translateZ + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translate(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[0] + matrix.m[4] * translate.v[1] + matrix.m[8] * translate.v[2] + matrix.m[12],
		matrix.m[1] * translate.v[0] + matrix.m[5] * translate.v[1] + matrix.m[9] * translate.v[2] + matrix.m[13],
		matrix.m[2] * translate.v[0] + matrix.m[6] * translate.v[1] + matrix.m[10] * translate.v[2] + matrix.m[14],
		matrix.m[3] * translate.v[0] + matrix.m[7] * translate.v[1] + matrix.m[11] * translate.v[2] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & translate(const Matrix<4, 4, T> & matrix, T translateX, T translateY, T translateZ, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translateX + matrix.m[4] * translateY + matrix.m[8] * translateZ + matrix.m[12];
	target.m[13] = matrix.m[1] * translateX + matrix.m[5] * translateY + matrix.m[9] * translateZ + matrix.m[13];
	target.m[14] = matrix.m[2] * translateX + matrix.m[6] * translateY + matrix.m[10] * translateZ + matrix.m[14];
	target.m[15] = matrix.m[3] * translateX + matrix.m[7] * translateY + matrix.m[11] * translateZ + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translate(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[0] + matrix.m[4] * translate.v[1] + matrix.m[8] * translate.v[2] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[0] + matrix.m[5] * translate.v[1] + matrix.m[9] * translate.v[2] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[0] + matrix.m[6] * translate.v[1] + matrix.m[10] * translate.v[2] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[0] + matrix.m[7] * translate.v[1] + matrix.m[11] * translate.v[2] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> rotate2D(const Matrix<4, 4, T> & matrix, T rotateZ) {
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	return new T[4 * 4] {
		matrix.m[0] * cosZ + matrix.m[4] * sinZ,
		matrix.m[1] * cosZ + matrix.m[5] * sinZ,
		matrix.m[2] * cosZ + matrix.m[6] * sinZ,
		matrix.m[3] * cosZ + matrix.m[7] * sinZ,
		matrix.m[0] * -sinZ + matrix.m[4] * cosZ,
		matrix.m[1] * -sinZ + matrix.m[5] * cosZ,
		matrix.m[2] * -sinZ + matrix.m[6] * cosZ,
		matrix.m[3] * -sinZ + matrix.m[7] * cosZ,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotate2D(const Matrix<4, 4, T> & matrix, T rotateZ, Matrix<4, 4, T> & target) {
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	target.m[0] = matrix.m[0] * cosZ + matrix.m[4] * sinZ;
	target.m[1] = matrix.m[1] * cosZ + matrix.m[5] * sinZ;
	target.m[2] = matrix.m[2] * cosZ + matrix.m[6] * sinZ;
	target.m[3] = matrix.m[3] * cosZ + matrix.m[7] * sinZ;
	target.m[4] = matrix.m[0] * -sinZ + matrix.m[4] * cosZ;
	target.m[5] = matrix.m[1] * -sinZ + matrix.m[5] * cosZ;
	target.m[6] = matrix.m[2] * -sinZ + matrix.m[6] * cosZ;
	target.m[7] = matrix.m[3] * -sinZ + matrix.m[7] * cosZ;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> scale2D(const Matrix<4, 4, T> & matrix, T scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale,
		matrix.m[1] * scale,
		matrix.m[2] * scale,
		matrix.m[3] * scale,
		matrix.m[4] * scale,
		matrix.m[5] * scale,
		matrix.m[6] * scale,
		matrix.m[7] * scale,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scale2D(const Matrix<4, 4, T> & matrix, T scaleX, T scaleY) {
	return new T[4 * 4] {
		matrix.m[0] * scaleX,
		matrix.m[1] * scaleX,
		matrix.m[2] * scaleX,
		matrix.m[3] * scaleX,
		matrix.m[4] * scaleY,
		matrix.m[5] * scaleY,
		matrix.m[6] * scaleY,
		matrix.m[7] * scaleY,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scale2D(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[0],
		matrix.m[1] * scale.v[0],
		matrix.m[2] * scale.v[0],
		matrix.m[3] * scale.v[0],
		matrix.m[4] * scale.v[1],
		matrix.m[5] * scale.v[1],
		matrix.m[6] * scale.v[1],
		matrix.m[7] * scale.v[1],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & scale2D(const Matrix<4, 4, T> & matrix, T scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale;
	target.m[1] = matrix.m[1] * scale;
	target.m[2] = matrix.m[2] * scale;
	target.m[3] = matrix.m[3] * scale;
	target.m[4] = matrix.m[4] * scale;
	target.m[5] = matrix.m[5] * scale;
	target.m[6] = matrix.m[6] * scale;
	target.m[7] = matrix.m[7] * scale;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scale2D(const Matrix<4, 4, T> & matrix, T scaleX, T scaleY, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scaleX;
	target.m[1] = matrix.m[1] * scaleX;
	target.m[2] = matrix.m[2] * scaleX;
	target.m[3] = matrix.m[3] * scaleX;
	target.m[4] = matrix.m[4] * scaleY;
	target.m[5] = matrix.m[5] * scaleY;
	target.m[6] = matrix.m[6] * scaleY;
	target.m[7] = matrix.m[7] * scaleY;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scale2D(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[0];
	target.m[1] = matrix.m[1] * scale.v[0];
	target.m[2] = matrix.m[2] * scale.v[0];
	target.m[3] = matrix.m[3] * scale.v[0];
	target.m[4] = matrix.m[4] * scale.v[1];
	target.m[5] = matrix.m[5] * scale.v[1];
	target.m[6] = matrix.m[6] * scale.v[1];
	target.m[7] = matrix.m[7] * scale.v[1];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> scale(const Matrix<4, 4, T> & matrix, T scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale,
		matrix.m[1] * scale,
		matrix.m[2] * scale,
		matrix.m[3] * scale,
		matrix.m[4] * scale,
		matrix.m[5] * scale,
		matrix.m[6] * scale,
		matrix.m[7] * scale,
		matrix.m[8] * scale,
		matrix.m[9] * scale,
		matrix.m[10] * scale,
		matrix.m[11] * scale,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scale(const Matrix<4, 4, T> & matrix, T scaleX, T scaleY, T scaleZ) {
	return new T[4 * 4] {
		matrix.m[0] * scaleX,
		matrix.m[1] * scaleX,
		matrix.m[2] * scaleX,
		matrix.m[3] * scaleX,
		matrix.m[4] * scaleY,
		matrix.m[5] * scaleY,
		matrix.m[6] * scaleY,
		matrix.m[7] * scaleY,
		matrix.m[8] * scaleZ,
		matrix.m[9] * scaleZ,
		matrix.m[10] * scaleZ,
		matrix.m[11] * scaleZ,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scale(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[0],
		matrix.m[1] * scale.v[0],
		matrix.m[2] * scale.v[0],
		matrix.m[3] * scale.v[0],
		matrix.m[4] * scale.v[1],
		matrix.m[5] * scale.v[1],
		matrix.m[6] * scale.v[1],
		matrix.m[7] * scale.v[1],
		matrix.m[8] * scale.v[2],
		matrix.m[9] * scale.v[2],
		matrix.m[10] * scale.v[2],
		matrix.m[11] * scale.v[2],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & scale(const Matrix<4, 4, T> & matrix, T scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale;
	target.m[1] = matrix.m[1] * scale;
	target.m[2] = matrix.m[2] * scale;
	target.m[3] = matrix.m[3] * scale;
	target.m[4] = matrix.m[4] * scale;
	target.m[5] = matrix.m[5] * scale;
	target.m[6] = matrix.m[6] * scale;
	target.m[7] = matrix.m[7] * scale;
	target.m[8] = matrix.m[8] * scale;
	target.m[9] = matrix.m[9] * scale;
	target.m[10] = matrix.m[10] * scale;
	target.m[11] = matrix.m[11] * scale;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scale(const Matrix<4, 4, T> & matrix, T scaleX, T scaleY, T scaleZ, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scaleX;
	target.m[1] = matrix.m[1] * scaleX;
	target.m[2] = matrix.m[2] * scaleX;
	target.m[3] = matrix.m[3] * scaleX;
	target.m[4] = matrix.m[4] * scaleY;
	target.m[5] = matrix.m[5] * scaleY;
	target.m[6] = matrix.m[6] * scaleY;
	target.m[7] = matrix.m[7] * scaleY;
	target.m[8] = matrix.m[8] * scaleZ;
	target.m[9] = matrix.m[9] * scaleZ;
	target.m[10] = matrix.m[10] * scaleZ;
	target.m[11] = matrix.m[11] * scaleZ;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scale(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[0];
	target.m[1] = matrix.m[1] * scale.v[0];
	target.m[2] = matrix.m[2] * scale.v[0];
	target.m[3] = matrix.m[3] * scale.v[0];
	target.m[4] = matrix.m[4] * scale.v[1];
	target.m[5] = matrix.m[5] * scale.v[1];
	target.m[6] = matrix.m[6] * scale.v[1];
	target.m[7] = matrix.m[7] * scale.v[1];
	target.m[8] = matrix.m[8] * scale.v[2];
	target.m[9] = matrix.m[9] * scale.v[2];
	target.m[10] = matrix.m[10] * scale.v[2];
	target.m[11] = matrix.m[11] * scale.v[2];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> translateX(const Matrix<4, 4, T> & matrix, T translateX) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translateX + matrix.m[12],
		matrix.m[1] * translateX + matrix.m[13],
		matrix.m[2] * translateX + matrix.m[14],
		matrix.m[3] * translateX + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateY(const Matrix<4, 4, T> & matrix, T translateY) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[4] * translateY + matrix.m[12],
		matrix.m[5] * translateY + matrix.m[13],
		matrix.m[6] * translateY + matrix.m[14],
		matrix.m[7] * translateY + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateZ(const Matrix<4, 4, T> & matrix, T translateZ) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[8] * translateZ + matrix.m[12],
		matrix.m[9] * translateZ + matrix.m[13],
		matrix.m[10] * translateZ + matrix.m[14],
		matrix.m[11] * translateZ + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateXY(const Matrix<4, 4, T> & matrix, T translateX, T translateY) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translateX + matrix.m[4] * translateY + matrix.m[12],
		matrix.m[1] * translateX + matrix.m[5] * translateY + matrix.m[13],
		matrix.m[2] * translateX + matrix.m[6] * translateY + matrix.m[14],
		matrix.m[3] * translateX + matrix.m[7] * translateY + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateXY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[0] + matrix.m[4] * translate.v[1] + matrix.m[12],
		matrix.m[1] * translate.v[0] + matrix.m[5] * translate.v[1] + matrix.m[13],
		matrix.m[2] * translate.v[0] + matrix.m[6] * translate.v[1] + matrix.m[14],
		matrix.m[3] * translate.v[0] + matrix.m[7] * translate.v[1] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateXZ(const Matrix<4, 4, T> & matrix, T translateX, T translateZ) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translateX + matrix.m[8] * translateZ + matrix.m[12],
		matrix.m[1] * translateX + matrix.m[9] * translateZ + matrix.m[13],
		matrix.m[2] * translateX + matrix.m[10] * translateZ + matrix.m[14],
		matrix.m[3] * translateX + matrix.m[11] * translateZ + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateXZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[0] + matrix.m[8] * translate.v[1] + matrix.m[12],
		matrix.m[1] * translate.v[0] + matrix.m[9] * translate.v[1] + matrix.m[13],
		matrix.m[2] * translate.v[0] + matrix.m[10] * translate.v[1] + matrix.m[14],
		matrix.m[3] * translate.v[0] + matrix.m[11] * translate.v[1] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateYX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[1] + matrix.m[4] * translate.v[0] + matrix.m[12],
		matrix.m[1] * translate.v[1] + matrix.m[5] * translate.v[0] + matrix.m[13],
		matrix.m[2] * translate.v[1] + matrix.m[6] * translate.v[0] + matrix.m[14],
		matrix.m[3] * translate.v[1] + matrix.m[7] * translate.v[0] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateYZ(const Matrix<4, 4, T> & matrix, T translateY, T translateZ) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[4] * translateY + matrix.m[8] * translateZ + matrix.m[12],
		matrix.m[5] * translateY + matrix.m[9] * translateZ + matrix.m[13],
		matrix.m[6] * translateY + matrix.m[10] * translateZ + matrix.m[14],
		matrix.m[7] * translateY + matrix.m[11] * translateZ + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateYZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[4] * translate.v[0] + matrix.m[8] * translate.v[1] + matrix.m[12],
		matrix.m[5] * translate.v[0] + matrix.m[9] * translate.v[1] + matrix.m[13],
		matrix.m[6] * translate.v[0] + matrix.m[10] * translate.v[1] + matrix.m[14],
		matrix.m[7] * translate.v[0] + matrix.m[11] * translate.v[1] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateZX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[1] + matrix.m[8] * translate.v[0] + matrix.m[12],
		matrix.m[1] * translate.v[1] + matrix.m[9] * translate.v[0] + matrix.m[13],
		matrix.m[2] * translate.v[1] + matrix.m[10] * translate.v[0] + matrix.m[14],
		matrix.m[3] * translate.v[1] + matrix.m[11] * translate.v[0] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateZY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[4] * translate.v[1] + matrix.m[8] * translate.v[0] + matrix.m[12],
		matrix.m[5] * translate.v[1] + matrix.m[9] * translate.v[0] + matrix.m[13],
		matrix.m[6] * translate.v[1] + matrix.m[10] * translate.v[0] + matrix.m[14],
		matrix.m[7] * translate.v[1] + matrix.m[11] * translate.v[0] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateXYZ(const Matrix<4, 4, T> & matrix, T translateX, T translateY, T translateZ) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translateX + matrix.m[4] * translateY + matrix.m[8] * translateZ + matrix.m[12],
		matrix.m[1] * translateX + matrix.m[5] * translateY + matrix.m[9] * translateZ + matrix.m[13],
		matrix.m[2] * translateX + matrix.m[6] * translateY + matrix.m[10] * translateZ + matrix.m[14],
		matrix.m[3] * translateX + matrix.m[7] * translateY + matrix.m[11] * translateZ + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateXYZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[0] + matrix.m[4] * translate.v[1] + matrix.m[8] * translate.v[2] + matrix.m[12],
		matrix.m[1] * translate.v[0] + matrix.m[5] * translate.v[1] + matrix.m[9] * translate.v[2] + matrix.m[13],
		matrix.m[2] * translate.v[0] + matrix.m[6] * translate.v[1] + matrix.m[10] * translate.v[2] + matrix.m[14],
		matrix.m[3] * translate.v[0] + matrix.m[7] * translate.v[1] + matrix.m[11] * translate.v[2] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateXZY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[0] + matrix.m[4] * translate.v[2] + matrix.m[8] * translate.v[1] + matrix.m[12],
		matrix.m[1] * translate.v[0] + matrix.m[5] * translate.v[2] + matrix.m[9] * translate.v[1] + matrix.m[13],
		matrix.m[2] * translate.v[0] + matrix.m[6] * translate.v[2] + matrix.m[10] * translate.v[1] + matrix.m[14],
		matrix.m[3] * translate.v[0] + matrix.m[7] * translate.v[2] + matrix.m[11] * translate.v[1] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateYXZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[1] + matrix.m[4] * translate.v[0] + matrix.m[8] * translate.v[2] + matrix.m[12],
		matrix.m[1] * translate.v[1] + matrix.m[5] * translate.v[0] + matrix.m[9] * translate.v[2] + matrix.m[13],
		matrix.m[2] * translate.v[1] + matrix.m[6] * translate.v[0] + matrix.m[10] * translate.v[2] + matrix.m[14],
		matrix.m[3] * translate.v[1] + matrix.m[7] * translate.v[0] + matrix.m[11] * translate.v[2] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateYZX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[2] + matrix.m[4] * translate.v[0] + matrix.m[8] * translate.v[1] + matrix.m[12],
		matrix.m[1] * translate.v[2] + matrix.m[5] * translate.v[0] + matrix.m[9] * translate.v[1] + matrix.m[13],
		matrix.m[2] * translate.v[2] + matrix.m[6] * translate.v[0] + matrix.m[10] * translate.v[1] + matrix.m[14],
		matrix.m[3] * translate.v[2] + matrix.m[7] * translate.v[0] + matrix.m[11] * translate.v[1] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateZXY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[1] + matrix.m[4] * translate.v[2] + matrix.m[8] * translate.v[0] + matrix.m[12],
		matrix.m[1] * translate.v[1] + matrix.m[5] * translate.v[2] + matrix.m[9] * translate.v[0] + matrix.m[13],
		matrix.m[2] * translate.v[1] + matrix.m[6] * translate.v[2] + matrix.m[10] * translate.v[0] + matrix.m[14],
		matrix.m[3] * translate.v[1] + matrix.m[7] * translate.v[2] + matrix.m[11] * translate.v[0] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> translateZYX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[0] * translate.v[2] + matrix.m[4] * translate.v[1] + matrix.m[8] * translate.v[0] + matrix.m[12],
		matrix.m[1] * translate.v[2] + matrix.m[5] * translate.v[1] + matrix.m[9] * translate.v[0] + matrix.m[13],
		matrix.m[2] * translate.v[2] + matrix.m[6] * translate.v[1] + matrix.m[10] * translate.v[0] + matrix.m[14],
		matrix.m[3] * translate.v[2] + matrix.m[7] * translate.v[1] + matrix.m[11] * translate.v[0] + matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & translateX(const Matrix<4, 4, T> & matrix, T translateX, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translateX + matrix.m[12];
	target.m[13] = matrix.m[1] * translateX + matrix.m[13];
	target.m[14] = matrix.m[2] * translateX + matrix.m[14];
	target.m[15] = matrix.m[3] * translateX + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateY(const Matrix<4, 4, T> & matrix, T translateY, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[4] * translateY + matrix.m[12];
	target.m[13] = matrix.m[5] * translateY + matrix.m[13];
	target.m[14] = matrix.m[6] * translateY + matrix.m[14];
	target.m[15] = matrix.m[7] * translateY + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateZ(const Matrix<4, 4, T> & matrix, T translateZ, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[8] * translateZ + matrix.m[12];
	target.m[13] = matrix.m[9] * translateZ + matrix.m[13];
	target.m[14] = matrix.m[10] * translateZ + matrix.m[14];
	target.m[15] = matrix.m[11] * translateZ + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateXY(const Matrix<4, 4, T> & matrix, T translateX, T translateY, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translateX + matrix.m[4] * translateY + matrix.m[12];
	target.m[13] = matrix.m[1] * translateX + matrix.m[5] * translateY + matrix.m[13];
	target.m[14] = matrix.m[2] * translateX + matrix.m[6] * translateY + matrix.m[14];
	target.m[15] = matrix.m[3] * translateX + matrix.m[7] * translateY + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateXY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[0] + matrix.m[4] * translate.v[1] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[0] + matrix.m[5] * translate.v[1] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[0] + matrix.m[6] * translate.v[1] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[0] + matrix.m[7] * translate.v[1] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateXZ(const Matrix<4, 4, T> & matrix, T translateX, T translateZ, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translateX + matrix.m[8] * translateZ + matrix.m[12];
	target.m[13] = matrix.m[1] * translateX + matrix.m[9] * translateZ + matrix.m[13];
	target.m[14] = matrix.m[2] * translateX + matrix.m[10] * translateZ + matrix.m[14];
	target.m[15] = matrix.m[3] * translateX + matrix.m[11] * translateZ + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateXZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[0] + matrix.m[8] * translate.v[1] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[0] + matrix.m[9] * translate.v[1] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[0] + matrix.m[10] * translate.v[1] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[0] + matrix.m[11] * translate.v[1] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateYX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[1] + matrix.m[4] * translate.v[0] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[1] + matrix.m[5] * translate.v[0] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[1] + matrix.m[6] * translate.v[0] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[1] + matrix.m[7] * translate.v[0] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateYZ(const Matrix<4, 4, T> & matrix, T translateY, T translateZ, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[4] * translateY + matrix.m[8] * translateZ + matrix.m[12];
	target.m[13] = matrix.m[5] * translateY + matrix.m[9] * translateZ + matrix.m[13];
	target.m[14] = matrix.m[6] * translateY + matrix.m[10] * translateZ + matrix.m[14];
	target.m[15] = matrix.m[7] * translateY + matrix.m[11] * translateZ + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateYZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[4] * translate.v[0] + matrix.m[8] * translate.v[1] + matrix.m[12];
	target.m[13] = matrix.m[5] * translate.v[0] + matrix.m[9] * translate.v[1] + matrix.m[13];
	target.m[14] = matrix.m[6] * translate.v[0] + matrix.m[10] * translate.v[1] + matrix.m[14];
	target.m[15] = matrix.m[7] * translate.v[0] + matrix.m[11] * translate.v[1] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateZX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[1] + matrix.m[8] * translate.v[0] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[1] + matrix.m[9] * translate.v[0] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[1] + matrix.m[10] * translate.v[0] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[1] + matrix.m[11] * translate.v[0] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateZY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[4] * translate.v[1] + matrix.m[8] * translate.v[0] + matrix.m[12];
	target.m[13] = matrix.m[5] * translate.v[1] + matrix.m[9] * translate.v[0] + matrix.m[13];
	target.m[14] = matrix.m[6] * translate.v[1] + matrix.m[10] * translate.v[0] + matrix.m[14];
	target.m[15] = matrix.m[7] * translate.v[1] + matrix.m[11] * translate.v[0] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateXYZ(const Matrix<4, 4, T> & matrix, T translateX, T translateY, T translateZ, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translateX + matrix.m[4] * translateY + matrix.m[8] * translateZ + matrix.m[12];
	target.m[13] = matrix.m[1] * translateX + matrix.m[5] * translateY + matrix.m[9] * translateZ + matrix.m[13];
	target.m[14] = matrix.m[2] * translateX + matrix.m[6] * translateY + matrix.m[10] * translateZ + matrix.m[14];
	target.m[15] = matrix.m[3] * translateX + matrix.m[7] * translateY + matrix.m[11] * translateZ + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateXYZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[0] + matrix.m[4] * translate.v[1] + matrix.m[8] * translate.v[2] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[0] + matrix.m[5] * translate.v[1] + matrix.m[9] * translate.v[2] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[0] + matrix.m[6] * translate.v[1] + matrix.m[10] * translate.v[2] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[0] + matrix.m[7] * translate.v[1] + matrix.m[11] * translate.v[2] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateXZY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[0] + matrix.m[4] * translate.v[2] + matrix.m[8] * translate.v[1] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[0] + matrix.m[5] * translate.v[2] + matrix.m[9] * translate.v[1] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[0] + matrix.m[6] * translate.v[2] + matrix.m[10] * translate.v[1] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[0] + matrix.m[7] * translate.v[2] + matrix.m[11] * translate.v[1] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateYXZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[1] + matrix.m[4] * translate.v[0] + matrix.m[8] * translate.v[2] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[1] + matrix.m[5] * translate.v[0] + matrix.m[9] * translate.v[2] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[1] + matrix.m[6] * translate.v[0] + matrix.m[10] * translate.v[2] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[1] + matrix.m[7] * translate.v[0] + matrix.m[11] * translate.v[2] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateYZX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[2] + matrix.m[4] * translate.v[0] + matrix.m[8] * translate.v[1] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[2] + matrix.m[5] * translate.v[0] + matrix.m[9] * translate.v[1] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[2] + matrix.m[6] * translate.v[0] + matrix.m[10] * translate.v[1] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[2] + matrix.m[7] * translate.v[0] + matrix.m[11] * translate.v[1] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateZXY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[1] + matrix.m[4] * translate.v[2] + matrix.m[8] * translate.v[0] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[1] + matrix.m[5] * translate.v[2] + matrix.m[9] * translate.v[0] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[1] + matrix.m[6] * translate.v[2] + matrix.m[10] * translate.v[0] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[1] + matrix.m[7] * translate.v[2] + matrix.m[11] * translate.v[0] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & translateZYX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & translate, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[0] * translate.v[2] + matrix.m[4] * translate.v[1] + matrix.m[8] * translate.v[0] + matrix.m[12];
	target.m[13] = matrix.m[1] * translate.v[2] + matrix.m[5] * translate.v[1] + matrix.m[9] * translate.v[0] + matrix.m[13];
	target.m[14] = matrix.m[2] * translate.v[2] + matrix.m[6] * translate.v[1] + matrix.m[10] * translate.v[0] + matrix.m[14];
	target.m[15] = matrix.m[3] * translate.v[2] + matrix.m[7] * translate.v[1] + matrix.m[11] * translate.v[0] + matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> rotateX(const Matrix<4, 4, T> & matrix, T rotateX) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4] * cosX + matrix.m[8] * sinX,
		matrix.m[5] * cosX + matrix.m[9] * sinX,
		matrix.m[6] * cosX + matrix.m[10] * sinX,
		matrix.m[7] * cosX + matrix.m[11] * sinX,
		matrix.m[4] * -sinX + matrix.m[8] * cosX,
		matrix.m[5] * -sinX + matrix.m[9] * cosX,
		matrix.m[6] * -sinX + matrix.m[10] * cosX,
		matrix.m[7] * -sinX + matrix.m[11] * cosX,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> rotateY(const Matrix<4, 4, T> & matrix, T rotateY) {
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	return new T[4 * 4] {
		matrix.m[0] * cosY + matrix.m[8] * -sinY,
		matrix.m[1] * cosY + matrix.m[9] * -sinY,
		matrix.m[2] * cosY + matrix.m[10] * -sinY,
		matrix.m[3] * cosY + matrix.m[11] * -sinY,
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[0] * sinY + matrix.m[8] * cosY,
		matrix.m[1] * sinY + matrix.m[9] * cosY,
		matrix.m[2] * sinY + matrix.m[10] * cosY,
		matrix.m[3] * sinY + matrix.m[11] * cosY,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> rotateZ(const Matrix<4, 4, T> & matrix, T rotateZ) {
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	return new T[4 * 4] {
		matrix.m[0] * cosZ + matrix.m[4] * sinZ,
		matrix.m[1] * cosZ + matrix.m[5] * sinZ,
		matrix.m[2] * cosZ + matrix.m[6] * sinZ,
		matrix.m[3] * cosZ + matrix.m[7] * sinZ,
		matrix.m[0] * -sinZ + matrix.m[4] * cosZ,
		matrix.m[1] * -sinZ + matrix.m[5] * cosZ,
		matrix.m[2] * -sinZ + matrix.m[6] * cosZ,
		matrix.m[3] * -sinZ + matrix.m[7] * cosZ,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> rotateXY(const Matrix<4, 4, T> & matrix, T rotateX, T rotateY) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	auto m4  = sinY * sinX;
	auto m6  = cosY * sinX;
	auto m8  = sinY * cosX;
	auto m10 = cosY * cosX;
	return new T[4 * 4] {
		matrix.m[0] * cosY + matrix.m[8] * -sinY,
		matrix.m[1] * cosY + matrix.m[9] * -sinY,
		matrix.m[2] * cosY + matrix.m[10] * -sinY,
		matrix.m[3] * cosY + matrix.m[11] * -sinY,
		matrix.m[0] * m4 + matrix.m[4] * cosX + matrix.m[8] * m6,
		matrix.m[1] * m4 + matrix.m[5] * cosX + matrix.m[9] * m6,
		matrix.m[2] * m4 + matrix.m[6] * cosX + matrix.m[10] * m6,
		matrix.m[3] * m4 + matrix.m[7] * cosX + matrix.m[11] * m6,
		matrix.m[0] * m8 + matrix.m[4] * -sinX + matrix.m[8] * m10,
		matrix.m[1] * m8 + matrix.m[5] * -sinX + matrix.m[9] * m10,
		matrix.m[2] * m8 + matrix.m[6] * -sinX + matrix.m[10] * m10,
		matrix.m[3] * m8 + matrix.m[7] * -sinX + matrix.m[11] * m10,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateXY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto m4  = sinY * sinX;
	auto m6  = cosY * sinX;
	auto m8  = sinY * cosX;
	auto m10 = cosY * cosX;
	return new T[4 * 4] {
		matrix.m[0] * cosY + matrix.m[8] * -sinY,
		matrix.m[1] * cosY + matrix.m[9] * -sinY,
		matrix.m[2] * cosY + matrix.m[10] * -sinY,
		matrix.m[3] * cosY + matrix.m[11] * -sinY,
		matrix.m[0] * m4 + matrix.m[4] * cosX + matrix.m[8] * m6,
		matrix.m[1] * m4 + matrix.m[5] * cosX + matrix.m[9] * m6,
		matrix.m[2] * m4 + matrix.m[6] * cosX + matrix.m[10] * m6,
		matrix.m[3] * m4 + matrix.m[7] * cosX + matrix.m[11] * m6,
		matrix.m[0] * m8 + matrix.m[4] * -sinX + matrix.m[8] * m10,
		matrix.m[1] * m8 + matrix.m[5] * -sinX + matrix.m[9] * m10,
		matrix.m[2] * m8 + matrix.m[6] * -sinX + matrix.m[10] * m10,
		matrix.m[3] * m8 + matrix.m[7] * -sinX + matrix.m[11] * m10,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> rotateXZ(const Matrix<4, 4, T> & matrix, T rotateX, T rotateZ) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	auto m4  = -sinZ * cosX;
	auto m5  = cosZ * cosX;
	auto m8  = sinZ * sinX;
	auto m9  = -cosZ * sinX;
	return new T[4 * 4] {
		matrix.m[0] * cosZ + matrix.m[4] * sinZ,
		matrix.m[1] * cosZ + matrix.m[5] * sinZ,
		matrix.m[2] * cosZ + matrix.m[6] * sinZ,
		matrix.m[3] * cosZ + matrix.m[7] * sinZ,
		matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * sinX,
		matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * sinX,
		matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * sinX,
		matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * sinX,
		matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * cosX,
		matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * cosX,
		matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * cosX,
		matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * cosX,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateXZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	auto m4  = -sinZ * cosX;
	auto m5  = cosZ * cosX;
	auto m8  = sinZ * sinX;
	auto m9  = -cosZ * sinX;
	return new T[4 * 4] {
		matrix.m[0] * cosZ + matrix.m[4] * sinZ,
		matrix.m[1] * cosZ + matrix.m[5] * sinZ,
		matrix.m[2] * cosZ + matrix.m[6] * sinZ,
		matrix.m[3] * cosZ + matrix.m[7] * sinZ,
		matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * sinX,
		matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * sinX,
		matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * sinX,
		matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * sinX,
		matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * cosX,
		matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * cosX,
		matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * cosX,
		matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * cosX,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateYX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto m1  = sinX * sinY;
	auto m2  = -cosX * sinY;
	auto m9  = -sinX * cosY;
	auto m10 = cosX * cosY;
	return new T[4 * 4] {
		matrix.m[0] * cosY + matrix.m[4] * m1 + matrix.m[8] * m2,
		matrix.m[1] * cosY + matrix.m[5] * m1 + matrix.m[9] * m2,
		matrix.m[2] * cosY + matrix.m[6] * m1 + matrix.m[10] * m2,
		matrix.m[3] * cosY + matrix.m[7] * m1 + matrix.m[11] * m2,
		matrix.m[4] * cosX + matrix.m[8] * sinX,
		matrix.m[5] * cosX + matrix.m[9] * sinX,
		matrix.m[6] * cosX + matrix.m[10] * sinX,
		matrix.m[7] * cosX + matrix.m[11] * sinX,
		matrix.m[0] * sinY + matrix.m[4] * m9 + matrix.m[8] * m10,
		matrix.m[1] * sinY + matrix.m[5] * m9 + matrix.m[9] * m10,
		matrix.m[2] * sinY + matrix.m[6] * m9 + matrix.m[10] * m10,
		matrix.m[3] * sinY + matrix.m[7] * m9 + matrix.m[11] * m10,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> rotateYZ(const Matrix<4, 4, T> & matrix, T rotateY, T rotateZ) {
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	auto m0  = cosZ * cosY;
	auto m1  = sinZ * cosY;
	auto m8  = cosZ * sinY;
	auto m9  = sinZ * sinY;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * -sinY,
		matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * -sinY,
		matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * -sinY,
		matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * -sinY,
		matrix.m[0] * -sinZ + matrix.m[4] * cosZ,
		matrix.m[1] * -sinZ + matrix.m[5] * cosZ,
		matrix.m[2] * -sinZ + matrix.m[6] * cosZ,
		matrix.m[3] * -sinZ + matrix.m[7] * cosZ,
		matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * cosY,
		matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * cosY,
		matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * cosY,
		matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * cosY,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateYZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate) {
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	auto m0  = cosZ * cosY;
	auto m1  = sinZ * cosY;
	auto m8  = cosZ * sinY;
	auto m9  = sinZ * sinY;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * -sinY,
		matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * -sinY,
		matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * -sinY,
		matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * -sinY,
		matrix.m[0] * -sinZ + matrix.m[4] * cosZ,
		matrix.m[1] * -sinZ + matrix.m[5] * cosZ,
		matrix.m[2] * -sinZ + matrix.m[6] * cosZ,
		matrix.m[3] * -sinZ + matrix.m[7] * cosZ,
		matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * cosY,
		matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * cosY,
		matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * cosY,
		matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * cosY,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateZX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	auto m1  = cosX * sinZ;
	auto m2  = sinX * sinZ;
	auto m5  = cosX * cosZ;
	auto m6  = sinX * cosZ;
	return new T[4 * 4] {
		matrix.m[0] * cosZ + matrix.m[4] * m1 + matrix.m[8] * m2,
		matrix.m[1] * cosZ + matrix.m[5] * m1 + matrix.m[9] * m2,
		matrix.m[2] * cosZ + matrix.m[6] * m1 + matrix.m[10] * m2,
		matrix.m[3] * cosZ + matrix.m[7] * m1 + matrix.m[11] * m2,
		matrix.m[0] * -sinZ + matrix.m[4] * m5 + matrix.m[8] * m6,
		matrix.m[1] * -sinZ + matrix.m[5] * m5 + matrix.m[9] * m6,
		matrix.m[2] * -sinZ + matrix.m[6] * m5 + matrix.m[10] * m6,
		matrix.m[3] * -sinZ + matrix.m[7] * m5 + matrix.m[11] * m6,
		matrix.m[4] * -sinX + matrix.m[8] * cosX,
		matrix.m[5] * -sinX + matrix.m[9] * cosX,
		matrix.m[6] * -sinX + matrix.m[10] * cosX,
		matrix.m[7] * -sinX + matrix.m[11] * cosX,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateZY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate) {
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	auto m0  = cosY * cosZ;
	auto m2  = -sinY * cosZ;
	auto m4  = -cosY * sinZ;
	auto m6  = sinY * sinZ;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * sinZ + matrix.m[8] * m2,
		matrix.m[1] * m0 + matrix.m[5] * sinZ + matrix.m[9] * m2,
		matrix.m[2] * m0 + matrix.m[6] * sinZ + matrix.m[10] * m2,
		matrix.m[3] * m0 + matrix.m[7] * sinZ + matrix.m[11] * m2,
		matrix.m[0] * m4 + matrix.m[4] * cosZ + matrix.m[8] * m6,
		matrix.m[1] * m4 + matrix.m[5] * cosZ + matrix.m[9] * m6,
		matrix.m[2] * m4 + matrix.m[6] * cosZ + matrix.m[10] * m6,
		matrix.m[3] * m4 + matrix.m[7] * cosZ + matrix.m[11] * m6,
		matrix.m[0] * sinY + matrix.m[8] * cosY,
		matrix.m[1] * sinY + matrix.m[9] * cosY,
		matrix.m[2] * sinY + matrix.m[10] * cosY,
		matrix.m[3] * sinY + matrix.m[11] * cosY,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> rotateXYZ(const Matrix<4, 4, T> & matrix, T rotateX, T rotateY, T rotateZ) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	auto m0  = cosZ * cosY;
	auto m1  = sinZ * cosY;
	auto m4  = -sinZ * cosX + cosZ * sinY * sinX;
	auto m5  = cosZ * cosX + sinZ * sinY * sinX;
	auto m6  = cosY * sinX;
	auto m8  = sinZ * sinX + cosZ * sinY * cosX;
	auto m9  = -cosZ * sinX + sinZ * sinY * cosX;
	auto m10 = cosY * cosX;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * -sinY,
		matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * -sinY,
		matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * -sinY,
		matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * -sinY,
		matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * m6,
		matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * m6,
		matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * m6,
		matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * m6,
		matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * m10,
		matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * m10,
		matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * m10,
		matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * m10,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateXYZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[2]);
	auto cosZ = cos(rotate.v[2]);
	auto m0  = cosZ * cosY;
	auto m1  = sinZ * cosY;
	auto m4  = -sinZ * cosX + cosZ * sinY * sinX;
	auto m5  = cosZ * cosX + sinZ * sinY * sinX;
	auto m6  = cosY * sinX;
	auto m8  = sinZ * sinX + cosZ * sinY * cosX;
	auto m9  = -cosZ * sinX + sinZ * sinY * cosX;
	auto m10 = cosY * cosX;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * -sinY,
		matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * -sinY,
		matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * -sinY,
		matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * -sinY,
		matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * m6,
		matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * m6,
		matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * m6,
		matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * m6,
		matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * m10,
		matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * m10,
		matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * m10,
		matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * m10,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateXZY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[2]);
	auto cosY = cos(rotate.v[2]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	auto m0  = cosY * cosZ;
	auto m2  = -sinY * cosZ;
	auto m4  = -cosY * sinZ * cosX + sinY * sinX;
	auto m5  = cosZ * cosX;
	auto m6  = sinY * sinZ * cosX + cosY * sinX;
	auto m8  = cosY * sinZ * sinX + sinY * cosX;
	auto m9  = -cosZ * sinX;
	auto m10 = -sinY * sinZ * sinX + cosY * cosX;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * sinZ + matrix.m[8] * m2,
		matrix.m[1] * m0 + matrix.m[5] * sinZ + matrix.m[9] * m2,
		matrix.m[2] * m0 + matrix.m[6] * sinZ + matrix.m[10] * m2,
		matrix.m[3] * m0 + matrix.m[7] * sinZ + matrix.m[11] * m2,
		matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * m6,
		matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * m6,
		matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * m6,
		matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * m6,
		matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * m10,
		matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * m10,
		matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * m10,
		matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * m10,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateYXZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[2]);
	auto cosZ = cos(rotate.v[2]);
	auto m0  = cosZ * cosY - sinZ * sinX * sinY;
	auto m1  = sinZ * cosY + cosZ * sinX * sinY;
	auto m2  = -cosX * sinY;
	auto m4  = -sinZ * cosX;
	auto m5  = cosZ * cosX;
	auto m8  = cosZ * sinY + sinZ * sinX * cosY;
	auto m9  = sinZ * sinY - cosZ * sinX * cosY;
	auto m10 = cosX * cosY;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * m2,
		matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * m2,
		matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * m2,
		matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * m2,
		matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * sinX,
		matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * sinX,
		matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * sinX,
		matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * sinX,
		matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * m10,
		matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * m10,
		matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * m10,
		matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * m10,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateYZX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[2]);
	auto cosX = cos(rotate.v[2]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	auto m0  = cosZ * cosY;
	auto m1  = cosX * sinZ * cosY + sinX * sinY;
	auto m2  = sinX * sinZ * cosY - cosX * sinY;
	auto m5  = cosX * cosZ;
	auto m6  = sinX * cosZ;
	auto m8  = cosZ * sinY;
	auto m9  = cosX * sinZ * sinY - sinX * cosY;
	auto m10 = sinX * sinZ * sinY + cosX * cosY;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * m2,
		matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * m2,
		matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * m2,
		matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * m2,
		matrix.m[0] * -sinZ + matrix.m[4] * m5 + matrix.m[8] * m6,
		matrix.m[1] * -sinZ + matrix.m[5] * m5 + matrix.m[9] * m6,
		matrix.m[2] * -sinZ + matrix.m[6] * m5 + matrix.m[10] * m6,
		matrix.m[3] * -sinZ + matrix.m[7] * m5 + matrix.m[11] * m6,
		matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * m10,
		matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * m10,
		matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * m10,
		matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * m10,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateZXY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[2]);
	auto cosY = cos(rotate.v[2]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	auto m0  = cosY * cosZ + sinY * sinX * sinZ;
	auto m1  = cosX * sinZ;
	auto m2  = -sinY * cosZ + cosY * sinX * sinZ;
	auto m4  = -cosY * sinZ + sinY * sinX * cosZ;
	auto m5  = cosX * cosZ;
	auto m6  = sinY * sinZ + cosY * sinX * cosZ;
	auto m8  = sinY * cosX;
	auto m10 = cosY * cosX;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * m2,
		matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * m2,
		matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * m2,
		matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * m2,
		matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * m6,
		matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * m6,
		matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * m6,
		matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * m6,
		matrix.m[0] * m8 + matrix.m[4] * -sinX + matrix.m[8] * m10,
		matrix.m[1] * m8 + matrix.m[5] * -sinX + matrix.m[9] * m10,
		matrix.m[2] * m8 + matrix.m[6] * -sinX + matrix.m[10] * m10,
		matrix.m[3] * m8 + matrix.m[7] * -sinX + matrix.m[11] * m10,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateZYX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate) {
	auto sinX = sin(rotate.v[2]);
	auto cosX = cos(rotate.v[2]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	auto m0  = cosY * cosZ;
	auto m1  = sinX * sinY * cosZ + cosX * sinZ;
	auto m2  = -cosX * sinY * cosZ + sinX * sinZ;
	auto m4  = -cosY * sinZ;
	auto m5  = -sinX * sinY * sinZ + cosX * cosZ;
	auto m6  = cosX * sinY * sinZ + sinX * cosZ;
	auto m9  = -sinX * cosY;
	auto m10 = cosX * cosY;
	return new T[4 * 4] {
		matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * m2,
		matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * m2,
		matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * m2,
		matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * m2,
		matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * m6,
		matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * m6,
		matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * m6,
		matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * m6,
		matrix.m[0] * sinY + matrix.m[4] * m9 + matrix.m[8] * m10,
		matrix.m[1] * sinY + matrix.m[5] * m9 + matrix.m[9] * m10,
		matrix.m[2] * sinY + matrix.m[6] * m9 + matrix.m[10] * m10,
		matrix.m[3] * sinY + matrix.m[7] * m9 + matrix.m[11] * m10,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & rotateX(const Matrix<4, 4, T> & matrix, T rotateX, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4] * cosX + matrix.m[8] * sinX;
	target.m[5] = matrix.m[5] * cosX + matrix.m[9] * sinX;
	target.m[6] = matrix.m[6] * cosX + matrix.m[10] * sinX;
	target.m[7] = matrix.m[7] * cosX + matrix.m[11] * sinX;
	target.m[8] = matrix.m[4] * -sinX + matrix.m[8] * cosX;
	target.m[9] = matrix.m[5] * -sinX + matrix.m[9] * cosX;
	target.m[10] = matrix.m[6] * -sinX + matrix.m[10] * cosX;
	target.m[11] = matrix.m[7] * -sinX + matrix.m[11] * cosX;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateY(const Matrix<4, 4, T> & matrix, T rotateY, Matrix<4, 4, T> & target) {
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	target.m[0] = matrix.m[0] * cosY + matrix.m[8] * -sinY;
	target.m[1] = matrix.m[1] * cosY + matrix.m[9] * -sinY;
	target.m[2] = matrix.m[2] * cosY + matrix.m[10] * -sinY;
	target.m[3] = matrix.m[3] * cosY + matrix.m[11] * -sinY;
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[0] * sinY + matrix.m[8] * cosY;
	target.m[9] = matrix.m[1] * sinY + matrix.m[9] * cosY;
	target.m[10] = matrix.m[2] * sinY + matrix.m[10] * cosY;
	target.m[11] = matrix.m[3] * sinY + matrix.m[11] * cosY;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateZ(const Matrix<4, 4, T> & matrix, T rotateZ, Matrix<4, 4, T> & target) {
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	target.m[0] = matrix.m[0] * cosZ + matrix.m[4] * sinZ;
	target.m[1] = matrix.m[1] * cosZ + matrix.m[5] * sinZ;
	target.m[2] = matrix.m[2] * cosZ + matrix.m[6] * sinZ;
	target.m[3] = matrix.m[3] * cosZ + matrix.m[7] * sinZ;
	target.m[4] = matrix.m[0] * -sinZ + matrix.m[4] * cosZ;
	target.m[5] = matrix.m[1] * -sinZ + matrix.m[5] * cosZ;
	target.m[6] = matrix.m[2] * -sinZ + matrix.m[6] * cosZ;
	target.m[7] = matrix.m[3] * -sinZ + matrix.m[7] * cosZ;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateXY(const Matrix<4, 4, T> & matrix, T rotateX, T rotateY, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	auto m4  = sinY * sinX;
	auto m6  = cosY * sinX;
	auto m8  = sinY * cosX;
	auto m10 = cosY * cosX;
	target.m[0] = matrix.m[0] * cosY + matrix.m[8] * -sinY;
	target.m[1] = matrix.m[1] * cosY + matrix.m[9] * -sinY;
	target.m[2] = matrix.m[2] * cosY + matrix.m[10] * -sinY;
	target.m[3] = matrix.m[3] * cosY + matrix.m[11] * -sinY;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * cosX + matrix.m[8] * m6;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * cosX + matrix.m[9] * m6;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * cosX + matrix.m[10] * m6;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * cosX + matrix.m[11] * m6;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * -sinX + matrix.m[8] * m10;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * -sinX + matrix.m[9] * m10;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * -sinX + matrix.m[10] * m10;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * -sinX + matrix.m[11] * m10;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateXY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto m4  = sinY * sinX;
	auto m6  = cosY * sinX;
	auto m8  = sinY * cosX;
	auto m10 = cosY * cosX;
	target.m[0] = matrix.m[0] * cosY + matrix.m[8] * -sinY;
	target.m[1] = matrix.m[1] * cosY + matrix.m[9] * -sinY;
	target.m[2] = matrix.m[2] * cosY + matrix.m[10] * -sinY;
	target.m[3] = matrix.m[3] * cosY + matrix.m[11] * -sinY;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * cosX + matrix.m[8] * m6;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * cosX + matrix.m[9] * m6;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * cosX + matrix.m[10] * m6;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * cosX + matrix.m[11] * m6;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * -sinX + matrix.m[8] * m10;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * -sinX + matrix.m[9] * m10;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * -sinX + matrix.m[10] * m10;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * -sinX + matrix.m[11] * m10;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateXZ(const Matrix<4, 4, T> & matrix, T rotateX, T rotateZ, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	auto m4  = -sinZ * cosX;
	auto m5  = cosZ * cosX;
	auto m8  = sinZ * sinX;
	auto m9  = -cosZ * sinX;
	target.m[0] = matrix.m[0] * cosZ + matrix.m[4] * sinZ;
	target.m[1] = matrix.m[1] * cosZ + matrix.m[5] * sinZ;
	target.m[2] = matrix.m[2] * cosZ + matrix.m[6] * sinZ;
	target.m[3] = matrix.m[3] * cosZ + matrix.m[7] * sinZ;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * sinX;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * sinX;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * sinX;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * sinX;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * cosX;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * cosX;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * cosX;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * cosX;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateXZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	auto m4  = -sinZ * cosX;
	auto m5  = cosZ * cosX;
	auto m8  = sinZ * sinX;
	auto m9  = -cosZ * sinX;
	target.m[0] = matrix.m[0] * cosZ + matrix.m[4] * sinZ;
	target.m[1] = matrix.m[1] * cosZ + matrix.m[5] * sinZ;
	target.m[2] = matrix.m[2] * cosZ + matrix.m[6] * sinZ;
	target.m[3] = matrix.m[3] * cosZ + matrix.m[7] * sinZ;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * sinX;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * sinX;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * sinX;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * sinX;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * cosX;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * cosX;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * cosX;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * cosX;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateYX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto m1  = sinX * sinY;
	auto m2  = -cosX * sinY;
	auto m9  = -sinX * cosY;
	auto m10 = cosX * cosY;
	target.m[0] = matrix.m[0] * cosY + matrix.m[4] * m1 + matrix.m[8] * m2;
	target.m[1] = matrix.m[1] * cosY + matrix.m[5] * m1 + matrix.m[9] * m2;
	target.m[2] = matrix.m[2] * cosY + matrix.m[6] * m1 + matrix.m[10] * m2;
	target.m[3] = matrix.m[3] * cosY + matrix.m[7] * m1 + matrix.m[11] * m2;
	target.m[4] = matrix.m[4] * cosX + matrix.m[8] * sinX;
	target.m[5] = matrix.m[5] * cosX + matrix.m[9] * sinX;
	target.m[6] = matrix.m[6] * cosX + matrix.m[10] * sinX;
	target.m[7] = matrix.m[7] * cosX + matrix.m[11] * sinX;
	target.m[8] = matrix.m[0] * sinY + matrix.m[4] * m9 + matrix.m[8] * m10;
	target.m[9] = matrix.m[1] * sinY + matrix.m[5] * m9 + matrix.m[9] * m10;
	target.m[10] = matrix.m[2] * sinY + matrix.m[6] * m9 + matrix.m[10] * m10;
	target.m[11] = matrix.m[3] * sinY + matrix.m[7] * m9 + matrix.m[11] * m10;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateYZ(const Matrix<4, 4, T> & matrix, T rotateY, T rotateZ, Matrix<4, 4, T> & target) {
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	auto m0  = cosZ * cosY;
	auto m1  = sinZ * cosY;
	auto m8  = cosZ * sinY;
	auto m9  = sinZ * sinY;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * -sinY;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * -sinY;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * -sinY;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * -sinY;
	target.m[4] = matrix.m[0] * -sinZ + matrix.m[4] * cosZ;
	target.m[5] = matrix.m[1] * -sinZ + matrix.m[5] * cosZ;
	target.m[6] = matrix.m[2] * -sinZ + matrix.m[6] * cosZ;
	target.m[7] = matrix.m[3] * -sinZ + matrix.m[7] * cosZ;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * cosY;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * cosY;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * cosY;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * cosY;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateYZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	auto m0  = cosZ * cosY;
	auto m1  = sinZ * cosY;
	auto m8  = cosZ * sinY;
	auto m9  = sinZ * sinY;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * -sinY;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * -sinY;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * -sinY;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * -sinY;
	target.m[4] = matrix.m[0] * -sinZ + matrix.m[4] * cosZ;
	target.m[5] = matrix.m[1] * -sinZ + matrix.m[5] * cosZ;
	target.m[6] = matrix.m[2] * -sinZ + matrix.m[6] * cosZ;
	target.m[7] = matrix.m[3] * -sinZ + matrix.m[7] * cosZ;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * cosY;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * cosY;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * cosY;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * cosY;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateZX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	auto m1  = cosX * sinZ;
	auto m2  = sinX * sinZ;
	auto m5  = cosX * cosZ;
	auto m6  = sinX * cosZ;
	target.m[0] = matrix.m[0] * cosZ + matrix.m[4] * m1 + matrix.m[8] * m2;
	target.m[1] = matrix.m[1] * cosZ + matrix.m[5] * m1 + matrix.m[9] * m2;
	target.m[2] = matrix.m[2] * cosZ + matrix.m[6] * m1 + matrix.m[10] * m2;
	target.m[3] = matrix.m[3] * cosZ + matrix.m[7] * m1 + matrix.m[11] * m2;
	target.m[4] = matrix.m[0] * -sinZ + matrix.m[4] * m5 + matrix.m[8] * m6;
	target.m[5] = matrix.m[1] * -sinZ + matrix.m[5] * m5 + matrix.m[9] * m6;
	target.m[6] = matrix.m[2] * -sinZ + matrix.m[6] * m5 + matrix.m[10] * m6;
	target.m[7] = matrix.m[3] * -sinZ + matrix.m[7] * m5 + matrix.m[11] * m6;
	target.m[8] = matrix.m[4] * -sinX + matrix.m[8] * cosX;
	target.m[9] = matrix.m[5] * -sinX + matrix.m[9] * cosX;
	target.m[10] = matrix.m[6] * -sinX + matrix.m[10] * cosX;
	target.m[11] = matrix.m[7] * -sinX + matrix.m[11] * cosX;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateZY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	auto m0  = cosY * cosZ;
	auto m2  = -sinY * cosZ;
	auto m4  = -cosY * sinZ;
	auto m6  = sinY * sinZ;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * sinZ + matrix.m[8] * m2;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * sinZ + matrix.m[9] * m2;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * sinZ + matrix.m[10] * m2;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * sinZ + matrix.m[11] * m2;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * cosZ + matrix.m[8] * m6;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * cosZ + matrix.m[9] * m6;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * cosZ + matrix.m[10] * m6;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * cosZ + matrix.m[11] * m6;
	target.m[8] = matrix.m[0] * sinY + matrix.m[8] * cosY;
	target.m[9] = matrix.m[1] * sinY + matrix.m[9] * cosY;
	target.m[10] = matrix.m[2] * sinY + matrix.m[10] * cosY;
	target.m[11] = matrix.m[3] * sinY + matrix.m[11] * cosY;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateXYZ(const Matrix<4, 4, T> & matrix, T rotateX, T rotateY, T rotateZ, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotateX);
	auto cosX = cos(rotateX);
	auto sinY = sin(rotateY);
	auto cosY = cos(rotateY);
	auto sinZ = sin(rotateZ);
	auto cosZ = cos(rotateZ);
	auto m0  = cosZ * cosY;
	auto m1  = sinZ * cosY;
	auto m4  = -sinZ * cosX + cosZ * sinY * sinX;
	auto m5  = cosZ * cosX + sinZ * sinY * sinX;
	auto m6  = cosY * sinX;
	auto m8  = sinZ * sinX + cosZ * sinY * cosX;
	auto m9  = -cosZ * sinX + sinZ * sinY * cosX;
	auto m10 = cosY * cosX;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * -sinY;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * -sinY;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * -sinY;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * -sinY;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * m6;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * m6;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * m6;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * m6;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * m10;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * m10;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * m10;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * m10;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateXYZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[2]);
	auto cosZ = cos(rotate.v[2]);
	auto m0  = cosZ * cosY;
	auto m1  = sinZ * cosY;
	auto m4  = -sinZ * cosX + cosZ * sinY * sinX;
	auto m5  = cosZ * cosX + sinZ * sinY * sinX;
	auto m6  = cosY * sinX;
	auto m8  = sinZ * sinX + cosZ * sinY * cosX;
	auto m9  = -cosZ * sinX + sinZ * sinY * cosX;
	auto m10 = cosY * cosX;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * -sinY;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * -sinY;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * -sinY;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * -sinY;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * m6;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * m6;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * m6;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * m6;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * m10;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * m10;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * m10;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * m10;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateXZY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotate.v[0]);
	auto cosX = cos(rotate.v[0]);
	auto sinY = sin(rotate.v[2]);
	auto cosY = cos(rotate.v[2]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	auto m0  = cosY * cosZ;
	auto m2  = -sinY * cosZ;
	auto m4  = -cosY * sinZ * cosX + sinY * sinX;
	auto m5  = cosZ * cosX;
	auto m6  = sinY * sinZ * cosX + cosY * sinX;
	auto m8  = cosY * sinZ * sinX + sinY * cosX;
	auto m9  = -cosZ * sinX;
	auto m10 = -sinY * sinZ * sinX + cosY * cosX;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * sinZ + matrix.m[8] * m2;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * sinZ + matrix.m[9] * m2;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * sinZ + matrix.m[10] * m2;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * sinZ + matrix.m[11] * m2;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * m6;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * m6;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * m6;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * m6;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * m10;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * m10;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * m10;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * m10;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateYXZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[2]);
	auto cosZ = cos(rotate.v[2]);
	auto m0  = cosZ * cosY - sinZ * sinX * sinY;
	auto m1  = sinZ * cosY + cosZ * sinX * sinY;
	auto m2  = -cosX * sinY;
	auto m4  = -sinZ * cosX;
	auto m5  = cosZ * cosX;
	auto m8  = cosZ * sinY + sinZ * sinX * cosY;
	auto m9  = sinZ * sinY - cosZ * sinX * cosY;
	auto m10 = cosX * cosY;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * m2;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * m2;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * m2;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * m2;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * sinX;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * sinX;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * sinX;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * sinX;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * m10;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * m10;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * m10;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * m10;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateYZX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotate.v[2]);
	auto cosX = cos(rotate.v[2]);
	auto sinY = sin(rotate.v[0]);
	auto cosY = cos(rotate.v[0]);
	auto sinZ = sin(rotate.v[1]);
	auto cosZ = cos(rotate.v[1]);
	auto m0  = cosZ * cosY;
	auto m1  = cosX * sinZ * cosY + sinX * sinY;
	auto m2  = sinX * sinZ * cosY - cosX * sinY;
	auto m5  = cosX * cosZ;
	auto m6  = sinX * cosZ;
	auto m8  = cosZ * sinY;
	auto m9  = cosX * sinZ * sinY - sinX * cosY;
	auto m10 = sinX * sinZ * sinY + cosX * cosY;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * m2;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * m2;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * m2;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * m2;
	target.m[4] = matrix.m[0] * -sinZ + matrix.m[4] * m5 + matrix.m[8] * m6;
	target.m[5] = matrix.m[1] * -sinZ + matrix.m[5] * m5 + matrix.m[9] * m6;
	target.m[6] = matrix.m[2] * -sinZ + matrix.m[6] * m5 + matrix.m[10] * m6;
	target.m[7] = matrix.m[3] * -sinZ + matrix.m[7] * m5 + matrix.m[11] * m6;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * m9 + matrix.m[8] * m10;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * m9 + matrix.m[9] * m10;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * m9 + matrix.m[10] * m10;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * m9 + matrix.m[11] * m10;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateZXY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotate.v[1]);
	auto cosX = cos(rotate.v[1]);
	auto sinY = sin(rotate.v[2]);
	auto cosY = cos(rotate.v[2]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	auto m0  = cosY * cosZ + sinY * sinX * sinZ;
	auto m1  = cosX * sinZ;
	auto m2  = -sinY * cosZ + cosY * sinX * sinZ;
	auto m4  = -cosY * sinZ + sinY * sinX * cosZ;
	auto m5  = cosX * cosZ;
	auto m6  = sinY * sinZ + cosY * sinX * cosZ;
	auto m8  = sinY * cosX;
	auto m10 = cosY * cosX;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * m2;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * m2;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * m2;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * m2;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * m6;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * m6;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * m6;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * m6;
	target.m[8] = matrix.m[0] * m8 + matrix.m[4] * -sinX + matrix.m[8] * m10;
	target.m[9] = matrix.m[1] * m8 + matrix.m[5] * -sinX + matrix.m[9] * m10;
	target.m[10] = matrix.m[2] * m8 + matrix.m[6] * -sinX + matrix.m[10] * m10;
	target.m[11] = matrix.m[3] * m8 + matrix.m[7] * -sinX + matrix.m[11] * m10;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & rotateZYX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & rotate, Matrix<4, 4, T> & target) {
	auto sinX = sin(rotate.v[2]);
	auto cosX = cos(rotate.v[2]);
	auto sinY = sin(rotate.v[1]);
	auto cosY = cos(rotate.v[1]);
	auto sinZ = sin(rotate.v[0]);
	auto cosZ = cos(rotate.v[0]);
	auto m0  = cosY * cosZ;
	auto m1  = sinX * sinY * cosZ + cosX * sinZ;
	auto m2  = -cosX * sinY * cosZ + sinX * sinZ;
	auto m4  = -cosY * sinZ;
	auto m5  = -sinX * sinY * sinZ + cosX * cosZ;
	auto m6  = cosX * sinY * sinZ + sinX * cosZ;
	auto m9  = -sinX * cosY;
	auto m10 = cosX * cosY;
	target.m[0] = matrix.m[0] * m0 + matrix.m[4] * m1 + matrix.m[8] * m2;
	target.m[1] = matrix.m[1] * m0 + matrix.m[5] * m1 + matrix.m[9] * m2;
	target.m[2] = matrix.m[2] * m0 + matrix.m[6] * m1 + matrix.m[10] * m2;
	target.m[3] = matrix.m[3] * m0 + matrix.m[7] * m1 + matrix.m[11] * m2;
	target.m[4] = matrix.m[0] * m4 + matrix.m[4] * m5 + matrix.m[8] * m6;
	target.m[5] = matrix.m[1] * m4 + matrix.m[5] * m5 + matrix.m[9] * m6;
	target.m[6] = matrix.m[2] * m4 + matrix.m[6] * m5 + matrix.m[10] * m6;
	target.m[7] = matrix.m[3] * m4 + matrix.m[7] * m5 + matrix.m[11] * m6;
	target.m[8] = matrix.m[0] * sinY + matrix.m[4] * m9 + matrix.m[8] * m10;
	target.m[9] = matrix.m[1] * sinY + matrix.m[5] * m9 + matrix.m[9] * m10;
	target.m[10] = matrix.m[2] * sinY + matrix.m[6] * m9 + matrix.m[10] * m10;
	target.m[11] = matrix.m[3] * sinY + matrix.m[7] * m9 + matrix.m[11] * m10;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> scaleX(const Matrix<4, 4, T> & matrix, T scaleX) {
	return new T[4 * 4] {
		matrix.m[0] * scaleX,
		matrix.m[1] * scaleX,
		matrix.m[2] * scaleX,
		matrix.m[3] * scaleX,
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleY(const Matrix<4, 4, T> & matrix, T scaleY) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4] * scaleY,
		matrix.m[5] * scaleY,
		matrix.m[6] * scaleY,
		matrix.m[7] * scaleY,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleZ(const Matrix<4, 4, T> & matrix, T scaleZ) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8] * scaleZ,
		matrix.m[9] * scaleZ,
		matrix.m[10] * scaleZ,
		matrix.m[11] * scaleZ,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleXY(const Matrix<4, 4, T> & matrix, T scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale,
		matrix.m[1] * scale,
		matrix.m[2] * scale,
		matrix.m[3] * scale,
		matrix.m[4] * scale,
		matrix.m[5] * scale,
		matrix.m[6] * scale,
		matrix.m[7] * scale,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleXY(const Matrix<4, 4, T> & matrix, T scaleX, T scaleY) {
	return new T[4 * 4] {
		matrix.m[0] * scaleX,
		matrix.m[1] * scaleX,
		matrix.m[2] * scaleX,
		matrix.m[3] * scaleX,
		matrix.m[4] * scaleY,
		matrix.m[5] * scaleY,
		matrix.m[6] * scaleY,
		matrix.m[7] * scaleY,
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleXY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[0],
		matrix.m[1] * scale.v[0],
		matrix.m[2] * scale.v[0],
		matrix.m[3] * scale.v[0],
		matrix.m[4] * scale.v[1],
		matrix.m[5] * scale.v[1],
		matrix.m[6] * scale.v[1],
		matrix.m[7] * scale.v[1],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleXZ(const Matrix<4, 4, T> & matrix, T scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale,
		matrix.m[1] * scale,
		matrix.m[2] * scale,
		matrix.m[3] * scale,
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8] * scale,
		matrix.m[9] * scale,
		matrix.m[10] * scale,
		matrix.m[11] * scale,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleXZ(const Matrix<4, 4, T> & matrix, T scaleX, T scaleZ) {
	return new T[4 * 4] {
		matrix.m[0] * scaleX,
		matrix.m[1] * scaleX,
		matrix.m[2] * scaleX,
		matrix.m[3] * scaleX,
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8] * scaleZ,
		matrix.m[9] * scaleZ,
		matrix.m[10] * scaleZ,
		matrix.m[11] * scaleZ,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleXZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[0],
		matrix.m[1] * scale.v[0],
		matrix.m[2] * scale.v[0],
		matrix.m[3] * scale.v[0],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8] * scale.v[1],
		matrix.m[9] * scale.v[1],
		matrix.m[10] * scale.v[1],
		matrix.m[11] * scale.v[1],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleYX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[1],
		matrix.m[1] * scale.v[1],
		matrix.m[2] * scale.v[1],
		matrix.m[3] * scale.v[1],
		matrix.m[4] * scale.v[0],
		matrix.m[5] * scale.v[0],
		matrix.m[6] * scale.v[0],
		matrix.m[7] * scale.v[0],
		matrix.m[8],
		matrix.m[9],
		matrix.m[10],
		matrix.m[11],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleYZ(const Matrix<4, 4, T> & matrix, T scale) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4] * scale,
		matrix.m[5] * scale,
		matrix.m[6] * scale,
		matrix.m[7] * scale,
		matrix.m[8] * scale,
		matrix.m[9] * scale,
		matrix.m[10] * scale,
		matrix.m[11] * scale,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleYZ(const Matrix<4, 4, T> & matrix, T scaleY, T scaleZ) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4] * scaleY,
		matrix.m[5] * scaleY,
		matrix.m[6] * scaleY,
		matrix.m[7] * scaleY,
		matrix.m[8] * scaleZ,
		matrix.m[9] * scaleZ,
		matrix.m[10] * scaleZ,
		matrix.m[11] * scaleZ,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleYZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4] * scale.v[0],
		matrix.m[5] * scale.v[0],
		matrix.m[6] * scale.v[0],
		matrix.m[7] * scale.v[0],
		matrix.m[8] * scale.v[1],
		matrix.m[9] * scale.v[1],
		matrix.m[10] * scale.v[1],
		matrix.m[11] * scale.v[1],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleZX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[1],
		matrix.m[1] * scale.v[1],
		matrix.m[2] * scale.v[1],
		matrix.m[3] * scale.v[1],
		matrix.m[4],
		matrix.m[5],
		matrix.m[6],
		matrix.m[7],
		matrix.m[8] * scale.v[0],
		matrix.m[9] * scale.v[0],
		matrix.m[10] * scale.v[0],
		matrix.m[11] * scale.v[0],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleZY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0],
		matrix.m[1],
		matrix.m[2],
		matrix.m[3],
		matrix.m[4] * scale.v[1],
		matrix.m[5] * scale.v[1],
		matrix.m[6] * scale.v[1],
		matrix.m[7] * scale.v[1],
		matrix.m[8] * scale.v[0],
		matrix.m[9] * scale.v[0],
		matrix.m[10] * scale.v[0],
		matrix.m[11] * scale.v[0],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleXYZ(const Matrix<4, 4, T> & matrix, T scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale,
		matrix.m[1] * scale,
		matrix.m[2] * scale,
		matrix.m[3] * scale,
		matrix.m[4] * scale,
		matrix.m[5] * scale,
		matrix.m[6] * scale,
		matrix.m[7] * scale,
		matrix.m[8] * scale,
		matrix.m[9] * scale,
		matrix.m[10] * scale,
		matrix.m[11] * scale,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleXYZ(const Matrix<4, 4, T> & matrix, T scaleX, T scaleY, T scaleZ) {
	return new T[4 * 4] {
		matrix.m[0] * scaleX,
		matrix.m[1] * scaleX,
		matrix.m[2] * scaleX,
		matrix.m[3] * scaleX,
		matrix.m[4] * scaleY,
		matrix.m[5] * scaleY,
		matrix.m[6] * scaleY,
		matrix.m[7] * scaleY,
		matrix.m[8] * scaleZ,
		matrix.m[9] * scaleZ,
		matrix.m[10] * scaleZ,
		matrix.m[11] * scaleZ,
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleXYZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[0],
		matrix.m[1] * scale.v[0],
		matrix.m[2] * scale.v[0],
		matrix.m[3] * scale.v[0],
		matrix.m[4] * scale.v[1],
		matrix.m[5] * scale.v[1],
		matrix.m[6] * scale.v[1],
		matrix.m[7] * scale.v[1],
		matrix.m[8] * scale.v[2],
		matrix.m[9] * scale.v[2],
		matrix.m[10] * scale.v[2],
		matrix.m[11] * scale.v[2],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleXZY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[0],
		matrix.m[1] * scale.v[0],
		matrix.m[2] * scale.v[0],
		matrix.m[3] * scale.v[0],
		matrix.m[4] * scale.v[2],
		matrix.m[5] * scale.v[2],
		matrix.m[6] * scale.v[2],
		matrix.m[7] * scale.v[2],
		matrix.m[8] * scale.v[1],
		matrix.m[9] * scale.v[1],
		matrix.m[10] * scale.v[1],
		matrix.m[11] * scale.v[1],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleYXZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[1],
		matrix.m[1] * scale.v[1],
		matrix.m[2] * scale.v[1],
		matrix.m[3] * scale.v[1],
		matrix.m[4] * scale.v[0],
		matrix.m[5] * scale.v[0],
		matrix.m[6] * scale.v[0],
		matrix.m[7] * scale.v[0],
		matrix.m[8] * scale.v[2],
		matrix.m[9] * scale.v[2],
		matrix.m[10] * scale.v[2],
		matrix.m[11] * scale.v[2],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleYZX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[2],
		matrix.m[1] * scale.v[2],
		matrix.m[2] * scale.v[2],
		matrix.m[3] * scale.v[2],
		matrix.m[4] * scale.v[0],
		matrix.m[5] * scale.v[0],
		matrix.m[6] * scale.v[0],
		matrix.m[7] * scale.v[0],
		matrix.m[8] * scale.v[1],
		matrix.m[9] * scale.v[1],
		matrix.m[10] * scale.v[1],
		matrix.m[11] * scale.v[1],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleZXY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[1],
		matrix.m[1] * scale.v[1],
		matrix.m[2] * scale.v[1],
		matrix.m[3] * scale.v[1],
		matrix.m[4] * scale.v[2],
		matrix.m[5] * scale.v[2],
		matrix.m[6] * scale.v[2],
		matrix.m[7] * scale.v[2],
		matrix.m[8] * scale.v[0],
		matrix.m[9] * scale.v[0],
		matrix.m[10] * scale.v[0],
		matrix.m[11] * scale.v[0],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> scaleZYX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale) {
	return new T[4 * 4] {
		matrix.m[0] * scale.v[2],
		matrix.m[1] * scale.v[2],
		matrix.m[2] * scale.v[2],
		matrix.m[3] * scale.v[2],
		matrix.m[4] * scale.v[1],
		matrix.m[5] * scale.v[1],
		matrix.m[6] * scale.v[1],
		matrix.m[7] * scale.v[1],
		matrix.m[8] * scale.v[0],
		matrix.m[9] * scale.v[0],
		matrix.m[10] * scale.v[0],
		matrix.m[11] * scale.v[0],
		matrix.m[12],
		matrix.m[13],
		matrix.m[14],
		matrix.m[15]
	};
}

template <typename T>
Matrix<4, 4, T> & scaleX(const Matrix<4, 4, T> & matrix, T scaleX, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scaleX;
	target.m[1] = matrix.m[1] * scaleX;
	target.m[2] = matrix.m[2] * scaleX;
	target.m[3] = matrix.m[3] * scaleX;
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleY(const Matrix<4, 4, T> & matrix, T scaleY, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4] * scaleY;
	target.m[5] = matrix.m[5] * scaleY;
	target.m[6] = matrix.m[6] * scaleY;
	target.m[7] = matrix.m[7] * scaleY;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleZ(const Matrix<4, 4, T> & matrix, T scaleZ, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8] * scaleZ;
	target.m[9] = matrix.m[9] * scaleZ;
	target.m[10] = matrix.m[10] * scaleZ;
	target.m[11] = matrix.m[11] * scaleZ;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleXY(const Matrix<4, 4, T> & matrix, T scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale;
	target.m[1] = matrix.m[1] * scale;
	target.m[2] = matrix.m[2] * scale;
	target.m[3] = matrix.m[3] * scale;
	target.m[4] = matrix.m[4] * scale;
	target.m[5] = matrix.m[5] * scale;
	target.m[6] = matrix.m[6] * scale;
	target.m[7] = matrix.m[7] * scale;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleXY(const Matrix<4, 4, T> & matrix, T scaleX, T scaleY, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scaleX;
	target.m[1] = matrix.m[1] * scaleX;
	target.m[2] = matrix.m[2] * scaleX;
	target.m[3] = matrix.m[3] * scaleX;
	target.m[4] = matrix.m[4] * scaleY;
	target.m[5] = matrix.m[5] * scaleY;
	target.m[6] = matrix.m[6] * scaleY;
	target.m[7] = matrix.m[7] * scaleY;
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleXY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[0];
	target.m[1] = matrix.m[1] * scale.v[0];
	target.m[2] = matrix.m[2] * scale.v[0];
	target.m[3] = matrix.m[3] * scale.v[0];
	target.m[4] = matrix.m[4] * scale.v[1];
	target.m[5] = matrix.m[5] * scale.v[1];
	target.m[6] = matrix.m[6] * scale.v[1];
	target.m[7] = matrix.m[7] * scale.v[1];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleXZ(const Matrix<4, 4, T> & matrix, T scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale;
	target.m[1] = matrix.m[1] * scale;
	target.m[2] = matrix.m[2] * scale;
	target.m[3] = matrix.m[3] * scale;
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8] * scale;
	target.m[9] = matrix.m[9] * scale;
	target.m[10] = matrix.m[10] * scale;
	target.m[11] = matrix.m[11] * scale;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleXZ(const Matrix<4, 4, T> & matrix, T scaleX, T scaleZ, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scaleX;
	target.m[1] = matrix.m[1] * scaleX;
	target.m[2] = matrix.m[2] * scaleX;
	target.m[3] = matrix.m[3] * scaleX;
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8] * scaleZ;
	target.m[9] = matrix.m[9] * scaleZ;
	target.m[10] = matrix.m[10] * scaleZ;
	target.m[11] = matrix.m[11] * scaleZ;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleXZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[0];
	target.m[1] = matrix.m[1] * scale.v[0];
	target.m[2] = matrix.m[2] * scale.v[0];
	target.m[3] = matrix.m[3] * scale.v[0];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8] * scale.v[1];
	target.m[9] = matrix.m[9] * scale.v[1];
	target.m[10] = matrix.m[10] * scale.v[1];
	target.m[11] = matrix.m[11] * scale.v[1];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleYX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[1];
	target.m[1] = matrix.m[1] * scale.v[1];
	target.m[2] = matrix.m[2] * scale.v[1];
	target.m[3] = matrix.m[3] * scale.v[1];
	target.m[4] = matrix.m[4] * scale.v[0];
	target.m[5] = matrix.m[5] * scale.v[0];
	target.m[6] = matrix.m[6] * scale.v[0];
	target.m[7] = matrix.m[7] * scale.v[0];
	target.m[8] = matrix.m[8];
	target.m[9] = matrix.m[9];
	target.m[10] = matrix.m[10];
	target.m[11] = matrix.m[11];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleYZ(const Matrix<4, 4, T> & matrix, T scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4] * scale;
	target.m[5] = matrix.m[5] * scale;
	target.m[6] = matrix.m[6] * scale;
	target.m[7] = matrix.m[7] * scale;
	target.m[8] = matrix.m[8] * scale;
	target.m[9] = matrix.m[9] * scale;
	target.m[10] = matrix.m[10] * scale;
	target.m[11] = matrix.m[11] * scale;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleYZ(const Matrix<4, 4, T> & matrix, T scaleY, T scaleZ, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4] * scaleY;
	target.m[5] = matrix.m[5] * scaleY;
	target.m[6] = matrix.m[6] * scaleY;
	target.m[7] = matrix.m[7] * scaleY;
	target.m[8] = matrix.m[8] * scaleZ;
	target.m[9] = matrix.m[9] * scaleZ;
	target.m[10] = matrix.m[10] * scaleZ;
	target.m[11] = matrix.m[11] * scaleZ;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleYZ(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4] * scale.v[0];
	target.m[5] = matrix.m[5] * scale.v[0];
	target.m[6] = matrix.m[6] * scale.v[0];
	target.m[7] = matrix.m[7] * scale.v[0];
	target.m[8] = matrix.m[8] * scale.v[1];
	target.m[9] = matrix.m[9] * scale.v[1];
	target.m[10] = matrix.m[10] * scale.v[1];
	target.m[11] = matrix.m[11] * scale.v[1];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleZX(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[1];
	target.m[1] = matrix.m[1] * scale.v[1];
	target.m[2] = matrix.m[2] * scale.v[1];
	target.m[3] = matrix.m[3] * scale.v[1];
	target.m[4] = matrix.m[4];
	target.m[5] = matrix.m[5];
	target.m[6] = matrix.m[6];
	target.m[7] = matrix.m[7];
	target.m[8] = matrix.m[8] * scale.v[0];
	target.m[9] = matrix.m[9] * scale.v[0];
	target.m[10] = matrix.m[10] * scale.v[0];
	target.m[11] = matrix.m[11] * scale.v[0];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleZY(const Matrix<4, 4, T> & matrix, const Vector<2, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0];
	target.m[1] = matrix.m[1];
	target.m[2] = matrix.m[2];
	target.m[3] = matrix.m[3];
	target.m[4] = matrix.m[4] * scale.v[1];
	target.m[5] = matrix.m[5] * scale.v[1];
	target.m[6] = matrix.m[6] * scale.v[1];
	target.m[7] = matrix.m[7] * scale.v[1];
	target.m[8] = matrix.m[8] * scale.v[0];
	target.m[9] = matrix.m[9] * scale.v[0];
	target.m[10] = matrix.m[10] * scale.v[0];
	target.m[11] = matrix.m[11] * scale.v[0];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleXYZ(const Matrix<4, 4, T> & matrix, T scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale;
	target.m[1] = matrix.m[1] * scale;
	target.m[2] = matrix.m[2] * scale;
	target.m[3] = matrix.m[3] * scale;
	target.m[4] = matrix.m[4] * scale;
	target.m[5] = matrix.m[5] * scale;
	target.m[6] = matrix.m[6] * scale;
	target.m[7] = matrix.m[7] * scale;
	target.m[8] = matrix.m[8] * scale;
	target.m[9] = matrix.m[9] * scale;
	target.m[10] = matrix.m[10] * scale;
	target.m[11] = matrix.m[11] * scale;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleXYZ(const Matrix<4, 4, T> & matrix, T scaleX, T scaleY, T scaleZ, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scaleX;
	target.m[1] = matrix.m[1] * scaleX;
	target.m[2] = matrix.m[2] * scaleX;
	target.m[3] = matrix.m[3] * scaleX;
	target.m[4] = matrix.m[4] * scaleY;
	target.m[5] = matrix.m[5] * scaleY;
	target.m[6] = matrix.m[6] * scaleY;
	target.m[7] = matrix.m[7] * scaleY;
	target.m[8] = matrix.m[8] * scaleZ;
	target.m[9] = matrix.m[9] * scaleZ;
	target.m[10] = matrix.m[10] * scaleZ;
	target.m[11] = matrix.m[11] * scaleZ;
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleXYZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[0];
	target.m[1] = matrix.m[1] * scale.v[0];
	target.m[2] = matrix.m[2] * scale.v[0];
	target.m[3] = matrix.m[3] * scale.v[0];
	target.m[4] = matrix.m[4] * scale.v[1];
	target.m[5] = matrix.m[5] * scale.v[1];
	target.m[6] = matrix.m[6] * scale.v[1];
	target.m[7] = matrix.m[7] * scale.v[1];
	target.m[8] = matrix.m[8] * scale.v[2];
	target.m[9] = matrix.m[9] * scale.v[2];
	target.m[10] = matrix.m[10] * scale.v[2];
	target.m[11] = matrix.m[11] * scale.v[2];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleXZY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[0];
	target.m[1] = matrix.m[1] * scale.v[0];
	target.m[2] = matrix.m[2] * scale.v[0];
	target.m[3] = matrix.m[3] * scale.v[0];
	target.m[4] = matrix.m[4] * scale.v[2];
	target.m[5] = matrix.m[5] * scale.v[2];
	target.m[6] = matrix.m[6] * scale.v[2];
	target.m[7] = matrix.m[7] * scale.v[2];
	target.m[8] = matrix.m[8] * scale.v[1];
	target.m[9] = matrix.m[9] * scale.v[1];
	target.m[10] = matrix.m[10] * scale.v[1];
	target.m[11] = matrix.m[11] * scale.v[1];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleYXZ(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[1];
	target.m[1] = matrix.m[1] * scale.v[1];
	target.m[2] = matrix.m[2] * scale.v[1];
	target.m[3] = matrix.m[3] * scale.v[1];
	target.m[4] = matrix.m[4] * scale.v[0];
	target.m[5] = matrix.m[5] * scale.v[0];
	target.m[6] = matrix.m[6] * scale.v[0];
	target.m[7] = matrix.m[7] * scale.v[0];
	target.m[8] = matrix.m[8] * scale.v[2];
	target.m[9] = matrix.m[9] * scale.v[2];
	target.m[10] = matrix.m[10] * scale.v[2];
	target.m[11] = matrix.m[11] * scale.v[2];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleYZX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[2];
	target.m[1] = matrix.m[1] * scale.v[2];
	target.m[2] = matrix.m[2] * scale.v[2];
	target.m[3] = matrix.m[3] * scale.v[2];
	target.m[4] = matrix.m[4] * scale.v[0];
	target.m[5] = matrix.m[5] * scale.v[0];
	target.m[6] = matrix.m[6] * scale.v[0];
	target.m[7] = matrix.m[7] * scale.v[0];
	target.m[8] = matrix.m[8] * scale.v[1];
	target.m[9] = matrix.m[9] * scale.v[1];
	target.m[10] = matrix.m[10] * scale.v[1];
	target.m[11] = matrix.m[11] * scale.v[1];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleZXY(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[1];
	target.m[1] = matrix.m[1] * scale.v[1];
	target.m[2] = matrix.m[2] * scale.v[1];
	target.m[3] = matrix.m[3] * scale.v[1];
	target.m[4] = matrix.m[4] * scale.v[2];
	target.m[5] = matrix.m[5] * scale.v[2];
	target.m[6] = matrix.m[6] * scale.v[2];
	target.m[7] = matrix.m[7] * scale.v[2];
	target.m[8] = matrix.m[8] * scale.v[0];
	target.m[9] = matrix.m[9] * scale.v[0];
	target.m[10] = matrix.m[10] * scale.v[0];
	target.m[11] = matrix.m[11] * scale.v[0];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Matrix<4, 4, T> & scaleZYX(const Matrix<4, 4, T> & matrix, const Vector<3, T> & scale, Matrix<4, 4, T> & target) {
	target.m[0] = matrix.m[0] * scale.v[2];
	target.m[1] = matrix.m[1] * scale.v[2];
	target.m[2] = matrix.m[2] * scale.v[2];
	target.m[3] = matrix.m[3] * scale.v[2];
	target.m[4] = matrix.m[4] * scale.v[1];
	target.m[5] = matrix.m[5] * scale.v[1];
	target.m[6] = matrix.m[6] * scale.v[1];
	target.m[7] = matrix.m[7] * scale.v[1];
	target.m[8] = matrix.m[8] * scale.v[0];
	target.m[9] = matrix.m[9] * scale.v[0];
	target.m[10] = matrix.m[10] * scale.v[0];
	target.m[11] = matrix.m[11] * scale.v[0];
	target.m[12] = matrix.m[12];
	target.m[13] = matrix.m[13];
	target.m[14] = matrix.m[14];
	target.m[15] = matrix.m[15];
	return target;
}

template <typename T>
Vector<4, T> operator*(const Matrix<4, 4, T> & left, const Vector<4, T> & right) {
	return new T[4] {
		left.m[0] * right.v[0] + left.m[4] * right.v[1] + left.m[8 ] * right.v[2] + left.m[12] * right.v[3],
		left.m[1] * right.v[0] + left.m[5] * right.v[1] + left.m[9 ] * right.v[2] + left.m[13] * right.v[3],
		left.m[2] * right.v[0] + left.m[6] * right.v[1] + left.m[10] * right.v[2] + left.m[14] * right.v[3],
		left.m[3] * right.v[0] + left.m[7] * right.v[1] + left.m[11] * right.v[2] + left.m[15] * right.v[3]
	};
}

template <typename T>
std::ostream & operator<<(std::ostream & stream, const Matrix<4, 4, T> & matrix) {
	return stream << '[' << matrix.m[0] << ", " << matrix.m[4] << ", " << matrix.m[8 ] << ", " << matrix.m[12] << "]\n"
		             "[" << matrix.m[1] << ", " << matrix.m[5] << ", " << matrix.m[9 ] << ", " << matrix.m[13] << "]\n"
		             "[" << matrix.m[2] << ", " << matrix.m[6] << ", " << matrix.m[10] << ", " << matrix.m[14] << "]\n"
		             "[" << matrix.m[3] << ", " << matrix.m[7] << ", " << matrix.m[11] << ", " << matrix.m[15] << "]\n";
}

using Matrix4b  = Matrix<4, 4,  i8 >;
using Matrix4ub = Matrix<4, 4, u8 >;
using Matrix4s  = Matrix<4, 4,  i16>;
using Matrix4us = Matrix<4, 4, u16>;
using Matrix4i  = Matrix<4, 4,  i32>;
using Matrix4ui = Matrix<4, 4, u32>;
using Matrix4l  = Matrix<4, 4,  i8 >;
using Matrix4ul = Matrix<4, 4, u64>;
using Matrix4f  = Matrix<4, 4, float >;
using Matrix4d  = Matrix<4, 4, double>;

#define CNGE_MATRIX4_FORWARD
#endif // CNGE_MATRIX4
