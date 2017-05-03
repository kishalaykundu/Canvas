/**
 * @file Matrix.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The 3x3 and 4x4-element matrix template class for Canvas framework.
 */
#pragma once

#include <cstring>
#include <cmath>

#include "Preprocess.h"
#include "Vector.h"

namespace Sim {

	template <int row, int col> class Matrix {
		protected:
			Real _m [row*col];

		public:
			inline Matrix ()
			{
				for (int i = 0; i < row*col; ++i){
					_m [i] = 0.;
				}
			}
			inline ~Matrix () = default;
			inline Matrix (const Matrix& m) = default;
			inline Matrix& operator = (const Matrix& m) = default;

			inline Matrix (const Real* m)
			{
				memcpy (_m, m, row*col*sizeof (Real));
			}
			inline Matrix (const Real m)
			{
				for (int i = 0; i <row*col; ++i){
					_m [i] = m;
				}
			}

			// copy operator
			inline Matrix& operator = (const Real val)
			{
				for (int i = 0; i <row*col; ++i){
					_m [i] = val;
				}
				return *this;
			}

			// access operators (mutative and non-mutative)
			inline Real& operator [] (int index)
			{
#				ifndef NDEBUG
				if (index >= row*col){
					LOG_ERROR ("Illegal matrix index accessed" << index);
					return _m [row*col - 1];
				}
#				endif
				return _m [index];
			}
			inline Real operator [] (int index) const
			{
#				ifndef NDEBUG
				if (index >= row*col){
					LOG_ERROR ("Illegal matrix index accessed" << index);
					return 0.;
				}
#				endif
				return _m [index];
			}
			inline Real& operator () (int x, int y)
			{
#				ifndef NDEBUG
				if (x >= row || y >= col){
					LOG_ERROR ("Illegal matrix index accessed [" << x << ", " << y << "]");
					return _m [row*col - 1];
				}
#				endif
				return _m [x*row + y];
			}
			inline Real operator () (int x, int y) const
			{
#				ifndef NDEBUG
				if (x >= row || y >= col){
					LOG_ERROR ("Illegal matrix index accessed [" << x << ", " << y << "]");
					return 0.;
				}
#				endif
				return _m [x*row + y];
			}

			// non-mutative arithmetic operators
			inline Matrix operator + (const Matrix& m) const
			{
				Matrix result;
				for (int i = 0; i < row*col; ++i){
					result._m [i] = _m [i] + m._m [i];
				}
				return result;
			}
			inline Matrix operator - (const Matrix& m) const
			{
				Matrix result;
				for (int i = 0; i < row*col; ++i){
					result._m [i] = _m [i] - m._m [i];
				}
				return result;
			}
			inline Matrix operator * (Real val) const
			{
				Matrix result;
				for (int i = 0; i < row*col; ++i){
					result._m [i] = val*_m [i];
				}
				return result;
			}
			inline Matrix operator / (Real val) const
			{
#				ifndef NDEBUG
				if (Absolute (val) < EPSILON){
					LOG_ERROR ("Invalid argument for matrix scalar division");
					Matrix m;
					return m;
				}
#				endif
				Real reciprocal = 1./val;
				Matrix result;
				for (int i = 0; i < row*col; ++i){
					result._m [i] = reciprocal*_m [i];
				}
				return result;
			}

			// mutative arithmetic operators
			inline Matrix& operator += (const Matrix& m)
			{
				for (int i = 0; i < row*col; ++i){
					_m [i] += m._m [i];
				}
				return *this;
			}
			inline Matrix& operator += (Real val)
			{
				for (int i = 0; i < row*col; ++i){
					_m [i] += val;
				}
				return *this;
			}
			inline Matrix& operator -= (const Matrix& m)
			{
				for (int i = 0; i < row*col; ++i){
					_m [i] -= m._m [i];
				}
				return *this;
			}
			inline Matrix& operator -= (Real val)
			{
				for (int i = 0; i < row*col; ++i){
					_m [i] -= val;
				}
				return *this;
			}
			inline Matrix& operator *= (Real val)
			{
				for (int i = 0; i < row*col; ++i){
					_m [i] *= val;
				}
				return *this;
			}
			inline Matrix& operator /= (Real val)
			{
#				ifndef NDEBUG
				if (Absolute (val) < EPSILON){
					LOG_ERROR ("Invalid argument for matrix scalar division");
					return *this;
				}
#				endif
				Real reciprocal = 1./ val;
				for (int i = 0; i < row*col; ++i){
					_m [i] *=  reciprocal;
				}
				return *this;
			}

			// self-transpose operator (only works for square matrix)
			inline void Transpose ()
			{
#				ifndef NDEBUG
				if (row != col){
					LOG_ERROR ("Self transpose not defined for non-square matrix");
					return;
				}
#				endif
				for (int i = 0; i < row; ++i){
					for (int j = i + 1; j < col; ++j){
						Real tmp = _m [row*i + j];
						_m [row*i + j] = _m [row*j + i];
						_m [row*j + i] = tmp;
					}
				}
			}

			// generalized transpose
			inline Matrix Transpose () const
			{
				Matrix <col, row> result;
				for (int i = 0; i < row; ++i){
					for (int j = 0; j < col; ++j){
						result._m [col*j + i] = _m [row*i + j];
					}
				}
				return result;
			}
	};

	// specialized class for 3x3 matrix
	class Matrix3x3 : public Matrix <3, 3> {
		public:
			inline Matrix3x3 (): Matrix <3, 3> () {}
			inline Matrix3x3 (const Matrix3x3& m): Matrix <3, 3> (m._m) {}
			inline Matrix3x3 (const Real* m): Matrix <3, 3> (m) {}
			inline Matrix3x3 (const Real m): Matrix <3, 3> (m) {}
			inline Matrix3x3 (Real m00, Real m01, Real m02, Real m10, Real m11, Real m12, Real m20, Real m21, Real m22)
			{
				_m [0] = m00; _m [1] = m01; _m [2] = m02;
				_m [3] = m10; _m [4] = m11; _m [5] = m12;
				_m [6] = m20; _m [7] = m21; _m [8] = m22;
			}

			static const Matrix3x3 ZERO;
			static const Matrix3x3 ONE;
			static const Matrix3x3 IDENTITY;

			// non-mutative multiplication operator
			inline Matrix3x3 operator * (const Matrix3x3& m) const
			{
				return Matrix3x3 (_m [0] * m [0] + _m [1] * m [3] + _m [2] * m [6],
													_m [0] * m [1] + _m [1] * m [4] + _m [2] * m [7],
													_m [0] * m [2] + _m [1] * m [5] + _m [2] * m [8],
													_m [3] * m [0] + _m [4] * m [3] + _m [5] * m [6],
													_m [3] * m [1] + _m [4] * m [4] + _m [5] * m [7],
													_m [3] * m [2] + _m [4] * m [5] + _m [5] * m [8],
													_m [6] * m [0] + _m [7] * m [3] + _m [8] * m [6],
													_m [6] * m [1] + _m [7] * m [4] + _m [8] * m [7],
													_m [6] * m [2] + _m [7] * m [5] + _m [8] * m [8]);
			}
			// mutative multiplication operator
			inline Matrix3x3& operator *= (const Matrix3x3& m)
			{
				Real tmp [9];
				tmp [0] = _m [0] * m [0] + _m [1] * m [3] + _m [2] * m [6];
				tmp [1] = _m [0] * m [1] + _m [1] * m [4] + _m [2] * m [7];
				tmp [2] = _m [0] * m [2] + _m [1] * m [5] + _m [2] * m [8];
				tmp [3] = _m [3] * m [0] + _m [4] * m [3] + _m [5] * m [6];
				tmp [4] = _m [3] * m [1] + _m [4] * m [4] + _m [5] * m [7];
				tmp [5] = _m [3] * m [2] + _m [4] * m [5] + _m [5] * m [8];
				tmp [6] = _m [6] * m [0] + _m [7] * m [3] + _m [8] * m [6];
				tmp [7] = _m [6] * m [1] + _m [7] * m [4] + _m [8] * m [7];
				tmp [8] = _m [6] * m [2] + _m [7] * m [5] + _m [8] * m [8];
				memcpy (_m, tmp, 9 * sizeof (Real));
				return *this;
			}

			// vector multiplication
			inline Vector3 operator * (Vector3 &v)
			{
				return Vector3 (v [0]*_m [0] + v [1]*_m [1] + v [2]*_m [2],
						v [0]*_m [3] + v [1]*_m [4] + v [2]*_m [5],
						v [0]*_m [6] + v [1]*_m [7] + v [2]*_m [8]);
			}
			inline Vector4 operator * (Vector4 &v)
			{
				return Vector4 (v [0]*_m [0] + v [1]*_m [1] + v [2]*_m [2],
						v [0]*_m [3] + v [1]*_m [4] + v [2]*_m [5],
						v [0]*_m [6] + v [1]*_m [7] + v [2]*_m [8], 1.);
			}

			// determinant calculator
			inline Real Determinant () const
			{
				return _m [0] * (_m [4]*_m [8] - _m [5]*_m [7]) +
						_m [1] * (_m [5]*_m [6] - _m [3]*_m [8]) +
						_m [2] * (_m [3]*_m [7] - _m [4]*_m [6]);
			}

			// inverse operator
			inline void Invert ()
			{
				Real tmp [9];
				tmp [0] = _m [4] * _m [8] - _m [5] * _m [7];
				tmp [1] = _m [2] * _m [7] - _m [1] * _m [8];
				tmp [2] = _m [1] * _m [5] - _m [2] * _m [4];
				tmp [3] = _m [5] * _m [6] - _m [3] * _m [8];
				tmp [4] = _m [0] * _m [8] - _m [2] * _m [6];
				tmp [5] = _m [2] * _m [3] - _m [0] * _m [5];
				tmp [6] = _m [3] * _m [7] - _m [4] * _m [6];
				tmp [7] = _m [1] * _m [6] - _m [0] * _m [7];
				tmp [8] = _m [0] * _m [4] - _m [1] * _m [3];

				Real det = Determinant ();
#				ifndef NDEBUG
				if (Absolute (det) < EPSILON){
					LOG_ERROR ("Degenerate matrix: determinant 0");
					return;
				}
#				endif
				det = 1./ det;
				for (int i = 0; i < 9; ++i){
					_m [i] = det * tmp [i];
				}
			}
	};

	// specialized class for 4x4 matrix
	class Matrix4x4 : public Matrix <4, 4> {
		public:
			inline Matrix4x4 (): Matrix <4, 4> () {_m [15] = 1.;}
			inline Matrix4x4 (const Matrix4x4& m): Matrix <4, 4> (m._m) {}
			inline Matrix4x4 (const Matrix3x3& m)
			: Matrix <4,4> ()
			{
				for (unsigned int i = 0; i < 3; ++i){
					_m [i] = m [i];
				}
				for (unsigned int i = 0; i < 3; ++i){
					_m [i + 4] = m [i + 3];
				}
				for (unsigned int i = 0; i < 3; ++i){
					_m [i + 8] = m [i + 6];
				}
			}
			inline Matrix4x4 (const Real* m): Matrix <4, 4> (m) {}
			inline Matrix4x4 (const Real m): Matrix <4, 4> (m) {_m [15] = 1.;}
			inline Matrix4x4 (Real m00, Real m01, Real m02, Real m10, Real m11, Real m12, Real m20, Real m21, Real m22)
			{
				_m [0] = m00; _m [1] = m01; _m [2] = m02; _m [3] = 0.;
				_m [4] = m10; _m [5] = m11; _m [6] = m12; _m [7] = 0.;
				_m [8] = m20; _m [9] = m21; _m [10] = m22; _m [11] = 0.;
				_m [12] = 0.; _m [13] = 0.; _m [14] = 0.; _m [15] = 1.;
			}
			inline Matrix4x4 (Real m00, Real m01, Real m02, Real m03, Real m10, Real m11, Real m12, Real m13,
					Real m20, Real m21, Real m22, Real m23, Real m30, Real m31, Real m32, Real m33)
			{
				_m [0] = m00; _m [1] = m01; _m [2] = m02; _m [3] = m03;
				_m [4] = m10; _m [5] = m11; _m [6] = m12; _m [7] = m13;
				_m [8] = m20; _m [9] = m21; _m [10] = m22; _m [11] = m23;
				_m [12] = m30; _m [13] = m31; _m [14] = m32; _m [15] = m33;
			}

			static const Matrix4x4 ZERO;
			static const Matrix4x4 ONE;
			static const Matrix4x4 IDENTITY;

			// non-mutative multiplication operator
			inline Matrix4x4 operator * (const Matrix4x4& m) const
			{
				return Matrix4x4 (
						_m [0] * m [0] + _m [1] * m [4] + _m [2] * m [8] + _m [3] * m [12],
						_m [0] * m [1] + _m [1] * m [5] + _m [2] * m [9] + _m [3] * m [13],
						_m [0] * m [2] + _m [1] * m [6] + _m [2] * m [10] + _m [3] * m [14],
						_m [0] * m [3] + _m [1] * m [7] + _m [2] * m [11] + _m [3] * m [15],
						_m [4] * m [0] + _m [5] * m [4] + _m [6] * m [8] + _m [7] * m [12],
						_m [4] * m [1] + _m [5] * m [5] + _m [6] * m [9] + _m [7] * m [13],
						_m [4] * m [2] + _m [5] * m [6] + _m [6] * m [10] + _m [7] * m [14],
						_m [4] * m [3] + _m [5] * m [7] + _m [6] * m [11] + _m [7] * m [15],
						_m [8] * m [0] + _m [9] * m [4] + _m [10] * m [8] + _m [11] * m [12],
						_m [8] * m [1] + _m [9] * m [5] + _m [10] * m [9] + _m [11] * m [13],
						_m [8] * m [2] + _m [9] * m [6] + _m [10] * m [10] + _m [11] * m [14],
						_m [8] * m [3] + _m [9] * m [7] + _m [10] * m [11] + _m [11] * m [15],
						_m [12] * m [0] + _m [13] * m [4] + _m [14] * m [8] + _m [15] * m [12],
						_m [12] * m [1] + _m [13] * m [5] + _m [14] * m [9] + _m [15] * m [13],
						_m [12] * m [2] + _m [13] * m [6] + _m [14] * m [10] + _m [15] * m [14],
						_m [12] * m [3] + _m [13] * m [7] + _m [14] * m [11] + _m [15] * m [15]);
			}
			// mutative multiplication operator
			inline Matrix4x4& operator *= (const Matrix4x4& m)
			{
				Real tmp [16];
				tmp [0] = _m [0] * m [0] + _m [1] * m [4] + _m [2] * m [8] + _m [3] * m [12];
				tmp [1] = _m [0] * m [1] + _m [1] * m [5] + _m [2] * m [9] + _m [3] * m [13];
				tmp [2] = _m [0] * m [2] + _m [1] * m [6] + _m [2] * m [10] + _m [3] * m [14];
				tmp [3] = _m [0] * m [3] + _m [1] * m [7] + _m [2] * m [11] + _m [3] * m [15];
				tmp [4] = _m [4] * m [0] + _m [5] * m [4] + _m [6] * m [8] + _m [7] * m [12];
				tmp [5] = _m [4] * m [1] + _m [5] * m [5] + _m [6] * m [9] + _m [7] * m [13];
				tmp [6] = _m [4] * m [2] + _m [5] * m [6] + _m [6] * m [10] + _m [7] * m [14];
				tmp [7] = _m [4] * m [3] + _m [5] * m [7] + _m [6] * m [11] + _m [7] * m [15];
				tmp [8] = _m [8] * m [0] + _m [9] * m [4] + _m [10] * m [8] + _m [11] * m [12];
				tmp [9] = _m [8] * m [1] + _m [9] * m [5] + _m [10] * m [9] + _m [11] * m [13];
				tmp [10] = _m [8] * m [2] + _m [9] * m [6] + _m [10] * m [10] + _m [11] * m [14];
				tmp [11] = _m [8] * m [3] + _m [9] * m [7] + _m [10] * m [11] + _m [11] * m [15];
				tmp [12] = _m [12] * m [0] + _m [13] * m [4] + _m [14] * m [8] + _m [15] * m [12];
				tmp [13] = _m [12] * m [1] + _m [13] * m [5] + _m [14] * m [9] + _m [15] * m [13];
				tmp [14] = _m [12] * m [2] + _m [13] * m [6] + _m [14] * m [10] + _m [15] * m [14];
				tmp [15] = _m [12] * m [3] + _m [13] * m [7] + _m [14] * m [11] + _m [15] * m [15];
				memcpy (_m, tmp, 16*sizeof (Real));
				return *this;
			}

			// vector multiplication
			inline Vector4 operator * (Vector4 &v)
			{
				Real w = v [0]*_m [12] + v [1]*_m [13] + v [2]*_m [14] + v [3]*_m [15];
#				ifndef NDEBUG
				if (Absolute (w) < EPSILON){
					LOG_ERROR ("Malformed matrix-vector multiplication");
					return Vector4::ZERO;
				}
#				endif
				w = 1./w;
				return Vector4((v [0]*_m [0] + v [1]*_m [1] + v [2]*_m [2] + v [3]*_m [3]) * w,
						(v [0]*_m [4] + v [1]*_m [5] + v [2]*_m [6] + v [3]*_m [7]) * w,
						(v [0]*_m [8] + v [1]*_m [9] + v [2]*_m [10] + v [3]*_m [11]) * w, 1.);
			}

			// determinant
			inline Real Determinant () const
			{
				return
						(_m [3] * _m [6] - _m [2] * _m [7]) * _m [9] * _m [12] - (_m [3] * _m [5] + _m [1] * _m [7]) * _m [10] * _m [12] +
						(_m [2] * _m [5] - _m [1] * _m [6]) * _m [11] * _m [12] - (_m [3] * _m [6] + _m [2] * _m [7]) * _m [8] * _m [13] +
						(_m [3] * _m [4] - _m [0] * _m [7]) * _m [10] * _m [13] - (_m [2] * _m [4] + _m [0] * _m [6]) * _m [11] * _m [13] +
						(_m [3] * _m [5] - _m [1] * _m [7]) * _m [8] * _m [14] - (_m [3] * _m [4] + _m [0] * _m [7]) * _m [9] * _m [14] +
						(_m [1] * _m [4] - _m [0] * _m [5]) * _m [11] * _m [14] - (_m [2] * _m [5] + _m [1] * _m [6]) * _m [8] * _m [15] +
						(_m [2] * _m [4] - _m [0] * _m [6]) * _m [9] * _m [15] - (_m [1] * _m [4] + _m [0] * _m [5]) * _m [10] * _m [15];
			}

			// inverse operator
			inline void Invert()
			{
				Real tmp [16];
				tmp [0] = _m [6] * _m [11] * _m [13] - _m [7] * _m [10] * _m [13] + _m [7] * _m [9] * _m [14] -
						_m [5] * _m [11] * _m [14] - _m [6] * _m [9] * _m [15] + _m [5] * _m [10] * _m [15];
				tmp [1] = _m [3] * _m [10] * _m [13] - _m [2] * _m [11] * _m [13] - _m [3] * _m [9] * _m [14] +
						_m [1] * _m [11] * _m [14] + _m [2] * _m [9] * _m [15] - _m [1] * _m [10] * _m [15];
				tmp [2] = _m [2] * _m [7] * _m [13] - _m [3] * _m [6] * _m [13] + _m [3] * _m [5] * _m [14] -
						_m [1] * _m [7] * _m [14] - _m [2] * _m [5] * _m [15] + _m [1] * _m [6] * _m [15];
				tmp [3] = _m [3] * _m [6] * _m [9] - _m [2] * _m [7] * _m [9] - _m [3] * _m [5] * _m [10] +
						_m [1] * _m [7] * _m [10] + _m [2] * _m [5] * _m [11] - _m [1] * _m [6] * _m [11];
				tmp [4] = _m [7] * _m [10] * _m [12] - _m [6] * _m [11] * _m [12] - _m [7] * _m [8] * _m [14] +
						_m [4] * _m [11] * _m [14] + _m [6] * _m [8] * _m [15] - _m [4] * _m [10] * _m [15];
				tmp [5] = _m [2] * _m [11] * _m [12] - _m [3] * _m [10] * _m [12] + _m [3] * _m [8] * _m [14] -
						_m [0] * _m [11] * _m [14] - _m [2] * _m [8] * _m [15] + _m [0] * _m [10] * _m [15];
				tmp [6] = _m [3] * _m [6] * _m [12] - _m [2] * _m [7] * _m [12] - _m [3] * _m [4] * _m [14] +
						_m [0] * _m [7] * _m [14] + _m [2] * _m [4] * _m [15] - _m [0] * _m [6] * _m [15];
				tmp [7] = _m [2] * _m [7] * _m [8] - _m [3] * _m [6] * _m [8] + _m [3] * _m [4] * _m [10] -
						_m [0] * _m [7] * _m [10] - _m [2] * _m [4] * _m [11] + _m [0] * _m [6] * _m [11];
				tmp [8] = _m [5] * _m [11] * _m [12] - _m [7] * _m [9] * _m [12] + _m [7] * _m [8] * _m [13] -
						_m [4] * _m [11] * _m [13] - _m [5] * _m [8] * _m [15] + _m [4] * _m [9] * _m [15];
				tmp [9] = _m [3] * _m [9] * _m [12] - _m [1] * _m [11] * _m [12] - _m [3] * _m [8] * _m [13] +
						_m [0] * _m [11] * _m [13] + _m [1] * _m [8] * _m [15] - _m [0] * _m [9] * _m [15];
				tmp [10] = _m [1] * _m [7] * _m [12] - _m [3] * _m [5] * _m [12] + _m [3] * _m [4] * _m [13] -
						_m [0] * _m [7] * _m [13] - _m [1] * _m [4] * _m [15] + _m [0] * _m [5] * _m [15];
				tmp [11] = _m [3] * _m [5] * _m [8] - _m [1] * _m [7] * _m [8] - _m [3] * _m [4] * _m [9] +
						_m [0] * _m [7] * _m [9] + _m [1] * _m [4] * _m [11] - _m [0] * _m [5] * _m [11];
				tmp [12] = _m [6] * _m [9] * _m [12] - _m [5] * _m [10] * _m [12] - _m [6] * _m [8] * _m [13] +
						_m [4] * _m [10] * _m [13] + _m [5] * _m [8] * _m [14] - _m [4] * _m [9] * _m [14];
				tmp [13] = _m [1] * _m [10] * _m [12] - _m [2] * _m [9] * _m [12] + _m [2] * _m [8] * _m [13] -
						_m [0] * _m [10] * _m [13] - _m [1] * _m [8] * _m [14] + _m [0] * _m [9] * _m [14];
				tmp [14] = _m [2] * _m [5] * _m [12] - _m [1] * _m [6] * _m [12] - _m [2] * _m [4] * _m [13] +
						_m [0] * _m [6] * _m [13] + _m [1] * _m [4] * _m [14] - _m [0] * _m [5] * _m [14];
				tmp [15] = _m [1] * _m [6] * _m [8] - _m [2] * _m [5] * _m [8] + _m [2] * _m [4] * _m [9] -
						_m [0] * _m [6] * _m [9] - _m [1] * _m [4] * _m [10] + _m [0] * _m [5] * _m [10];

				Real det = Determinant ();
#				ifndef NDEBUG
				if (Absolute (det) < EPSILON){
					LOG_ERROR ("Malformed matrix - determinant close to 0. Inverse could not be completed");
					return;
				}
#				endif
				det = 1./ det;
				for (int i = 0; i < 16; ++i){
					_m [i] = det * tmp [i];
				}
			}
	};
}
