/**
 * @file Vector.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The 2,3,4-element vector template class for Canvas framework.
 */
#pragma once

#include <cstring>
#include <cmath>

#include "Preprocess.h"
#include "Log.h"

namespace Sim {

	template <int size> class SimVector {
		protected:
			Real _v [size];

		public:
			inline SimVector ()
			{
				for (int i = 0; i < size; ++i){
					_v [i] = 0.;
				}
			}
			inline ~SimVector () = default;
			inline SimVector (const SimVector& v) = default;
			inline SimVector& operator = (const SimVector& v) = default;

			inline SimVector (const Real* v)
			{
				memcpy (_v, v, size*sizeof (Real));
			}
			inline SimVector (const Real v)
			{
				for (int i = 0; i <size; ++i){
					_v [i] = v;
				}
			}

			inline SimVector& operator = (const Real v)
			{
				for (int i = 0; i <size; ++i){
					_v [i] = v;
				}
				return *this;
			}

			inline Real& operator [] (int index)
			{
#				ifndef NDEBUG
				if (index > size){
					LOG_ERROR ("Illegal index accessed: " << index);
					return _v [size - 1];
				}
#				endif
				return _v [index];
			}
			inline Real operator [] (int index) const
			{
#				ifndef NDEBUG
				if (index > size){
					LOG_ERROR ("Illegal index accessed: " << index);
					return 0.;
				}
#				endif
				return _v [index];
			}

			// unary negative operator
			inline SimVector operator - () const
			{
				Real v [size];
				for (int i = 0; i < size; ++i){
					v [i] = -_v [i];
				}
				return SimVector (v);
			}

			// equality and non-equality operators
			inline bool operator == (const SimVector& v) const
			{
				for (int i = 0; i < size; ++i){
					if (ABS (_v [i] - v._v [i]) > EPSILON){
						return false;
					}
				}
				return true;
			}
			inline bool operator != (const SimVector& v) const {return !(*this == v);}

			// non-mutating arithmetic operators
			inline SimVector operator + (const SimVector& v) const
			{
				Real result [size];
				for (int i = 0; i < size; ++i){
					result [i] = _v [i] + v._v [i];
				}
				return SimVector (result);
			}
			inline SimVector operator - (const SimVector& v) const
			{
				Real result [size];
				for (int i = 0; i < size; ++i){
					result [i] = _v [i] - v._v [i];
				}
				return SimVector (result);
			}
			inline SimVector operator * (const SimVector& v) const
			{
				Real result [size];
				for (int i = 0; i < size; ++i){
					result [i] = _v [i] * v._v [i];
				}
				return SimVector (result);
			}
			inline SimVector operator / (const SimVector& v) const
			{
				Real result [size];
				for (int i = 0; i < size; ++i){
					result [i] = _v [i] / v._v [i];
				}
				return SimVector (result);
			}
			inline SimVector operator * (Real v) const
			{
				Real result [size];
				for (int i = 0; i < size; ++i){
					result [i] = _v [i] * v;
				}
				return SimVector (result);
			}
			inline SimVector operator / (Real v) const
			{
				Real result [size];
				Real rec = 1./v;
				for (int i = 0; i < size; ++i){
					result [i] = _v [i] * rec;
				}
				return SimVector (result);
			}

			// mutating arithmetic operators
			inline SimVector& operator += (const SimVector& v)
			{
				for (int i = 0; i < size; ++i){
					_v [i] += v._v [i];
				}
				return *this;
			}
			inline SimVector& operator -= (const SimVector& v)
			{
				for (int i = 0; i < size; ++i){
					_v [i] -= v [i];
				}
				return *this;
			}
			inline SimVector& operator *= (const SimVector& v)
			{
				for (int i = 0; i < size; ++i){
					_v [i] *= v._v [i];
				}
				return *this;
			}
			inline SimVector& operator /= (const SimVector& v)
			{
				for (int i = 0; i < size; ++i){
					_v [i] /= v._v [i];
				}
				return *this;
			}
			inline SimVector& operator += (Real v)
			{
				for (int i = 0; i < size; ++i){
					_v [i] += v;
				}
				return *this;
			}
			inline SimVector& operator -= (Real v)
			{
				for (int i = 0; i < size; ++i){
					_v [i] -= v;
				}
				return *this;
			}
			inline SimVector& operator *= (Real v)
			{
				for (int i = 0; i < size; ++i){
					_v [i] *= v;
				}
				return *this;
			}
			inline SimVector& operator /= (Real v)
			{
				Real rec = 1./v;
				for (int i = 0; i < size; ++i){
					_v [i] *= rec;
				}
				return *this;
			}

			inline Real LengthSquared () const
			{
				Real result = 0;
				for (int i = 0; i < size; ++i){
					result += _v [i] * _v [i];
				}
				return result;
			}
			inline Real Length () const
			{
				return static_cast <Real> (sqrt (LengthSquared ()));
			}

			inline void Normalize ()
			{
				Real len = Length ();
#				ifndef NDEBUG
				if (len < EPSILON){
					LOG_ERROR ("Cannot normalize vector. Length close to 0 ");
					return;
				}
#				endif
				Real rec = 1. / len;
				for (int i = 0; i < size; ++i){
					_v [i] *= rec;
				}
			}

			inline Real DistanceSquared (const SimVector& v) const
			{
				SimVector vec (_v);
				vec -= v;
				return v.LengthSquared ();
			}
			inline Real Distance (const SimVector& v) const
			{
				return static_cast <Real> (sqrt (DistanceSquared (v)));
			}

			inline Real Dot (const SimVector& v) const
			{
				Real result = 0.;
				for (int i = 0; i < size; ++i){
					result = _v [i] * v._v [i];
				}
				return result;
			}
			inline Real Angle (const SimVector& v) const
			{
				Real mag = static_cast <Real> (sqrt (LengthSquared () * v.LengthSquared ()));
#				ifndef NDEBUG
				if (mag < EPSILON){
					LOG_ERROR ("Cannot find angle with vector. One or more vectors close to 0 length");
					return 0.;
				}
#				endif
				return Dot (v) / mag;
			}

			inline Real& Largest ()
			{
				int index = 0;
				Real val, max = ABS (_v [0]);
				for (int i = 1; i < size; ++i){
					val = ABS (_v [i]);
					if (max < val){
						max = val;
						index = i;
					}
				}
				return _v [index];
			}
			inline Real& Smallest ()
			{
				int index = 0;
				Real val, min = ABS (_v [0]);
				for (int i = 1; i < size; ++i){
					val = ABS (_v [i]);
					if (min > val){
						min = val;
						index = i;
					}
				}
				return _v [index];
			}
	};

	class Vector2: public SimVector <2> {
		public:
			inline Vector2 (): SimVector <2> () {}
			inline Vector2 (const Vector2& v): SimVector <2> (v._v) {}
			inline Vector2 (const Real* v): SimVector <2> (v) {}
			inline Vector2 (const Real v): SimVector <2> (v) {}
			inline Vector2 (Real v0, Real v1)
			{
				_v [0] = v0; _v [1] = v1;
			}

			static const Vector2 ZERO;
			static const Vector2 UNIT;
			static const Vector2 UNIT_X;
			static const Vector2 UNIT_Y;
			static const Vector2 NEG_UNIT_X;
			static const Vector2 NEG_UNIT_Y;
	};

	class Vector3: public SimVector <3> {
		public:
			inline Vector3 (): SimVector <3> () {}
			inline Vector3 (const Vector3& v): SimVector <3> (v._v) {}
			inline Vector3 (const Real* v): SimVector <3> (v) {}
			inline Vector3 (const Real v): SimVector <3> (v) {}
			inline Vector3 (Real v0, Real v1, Real v2)
			{
				_v [0] = v0; _v [1] = v1; _v [2] = v2;
			}

			static const Vector3 ZERO;
			static const Vector3 UNIT;
			static const Vector3 UNIT_X;
			static const Vector3 UNIT_Y;
			static const Vector3 UNIT_Z;
			static const Vector3 NEG_UNIT_X;
			static const Vector3 NEG_UNIT_Y;
			static const Vector3 NEG_UNIT_Z;

			inline void Cross (const Vector3& v)
			{
				Real tmp [3];
				tmp [0] = _v [1] * v [2] - _v [2] * v [1];
				tmp [1] = _v [2] * v [0] - _v [0] * v [2];
				tmp [2] = _v [0] * v [1] - _v [1] * v [0];
				memcpy (_v, tmp, 3*sizeof (Real));
			}
			inline void Cross (const Vector3& v, Vector3& r) const
			{
				r [0] = _v [1] * v._v [2] - _v [2] * v._v [1];
				r [1] = _v [2] * v._v [0] - _v [0] * v._v [2];
				r [2] = _v [0] * v._v [1] - _v [1] * v._v [0];
			}
			inline Vector3 Cross (const Vector3& v) const
			{
				return Vector3 (_v [1] * v._v [2] - _v [2] * v._v [1],
						_v [2] * v._v [0] - _v [0] * v._v [2], _v [0] * v._v [1] - _v [1] * v._v [0]);
			}

			inline void NormalizedCross (const Vector3& v)
			{
				Cross (v);
				Real mag = Length ();
#				ifndef NDEBUG
				if (mag < EPSILON){
					LOG_ERROR ("Cannot normalize vector after cross operations. Length close to 0");
					return;
				}
#				endif
				for (int i = 0; i < 3; ++i){
					_v [i] /= mag;
				}
			}
			inline void NormalizedCross (const Vector3& v, Vector3& r) const
			{
				Cross (v, r);
				Real mag = r.Length ();
#				ifndef NDEBUG
				if (mag < EPSILON){
					LOG_ERROR ("Cannot normalize vector after cross operations. Length close to 0");
					return;
				}
#				endif
				r /= mag;
			}
			inline Vector3 NormalizedCross (const Vector3& v) const
			{
				Vector3 r;
				Cross (v, r);
				Real mag = r.Length ();
#				ifndef NDEBUG
				if (mag < EPSILON){
					LOG_ERROR ("Cannot normalize vector after cross operations. Length close to 0");
					return Vector3::ZERO;
				}
#				endif
				r /= mag;
				return r;
			}
	};

	class Vector4: public SimVector <4> {
		public:
			inline Vector4 (): SimVector <4> () {_v [3] = 1.;}
			inline Vector4 (const Vector4& v): SimVector <4> (v._v) {}
			inline Vector4 (const Real* v): SimVector <4> (v) {}
			inline Vector4 (const Real v): SimVector <4> (v) {}
			inline Vector4 (Real v0, Real v1, Real v2)
			{
				_v [0] = v0; _v [1] = v1; _v [2] = v2; _v [3] = 1.;
			}
			inline Vector4 (Real v0, Real v1, Real v2, Real v3)
			{
				_v [0] = v0; _v [1] = v1; _v [2] = v2; _v [3] = v3;
			}

			static const Vector4 ZERO;
			static const Vector4 UNIT;
			static const Vector4 UNIT_X;
			static const Vector4 UNIT_Y;
			static const Vector4 UNIT_Z;
			static const Vector4 NEG_UNIT_X;
			static const Vector4 NEG_UNIT_Y;
			static const Vector4 NEG_UNIT_Z;

			inline void Cross (const Vector4& v)
			{
				Real tmp [3];
				tmp [0] = _v [1] * v [2] - _v [2] * v [1];
				tmp [1] = _v [2] * v [0] - _v [0] * v [2];
				tmp [2] = _v [0] * v [1] - _v [1] * v [0];
				_v [3] = 1.;
				memcpy (_v, tmp, 3*sizeof (Real));
			}
			inline void Cross (const Vector4& v, Vector4& r) const
			{
				r [0] = _v [1] * v [2] - _v [2] * v [1];
				r [1] = _v [2] * v [0] - _v [0] * v [2];
				r [2] = _v [0] * v [1] - _v [1] * v [0];
			}
			inline Vector4 Cross (const Vector4& v) const
			{
				return Vector4 (_v [1] * v [2] - _v [2] * v [1],
						_v [2] * v [0] - _v [0] * v [2], _v [0] * v [1] - _v [1] * v [0], 1.);
			}

			inline void NormalizedCross (const Vector4& v)
			{
				Cross (v);
				Real mag = Length ();
#				ifndef NDEBUG
				if (mag < EPSILON){
					LOG_ERROR ("Cannot normalize vector after cross operations. Length close to 0");
					return;
				}
#				endif
				for (int i = 0; i < 3; ++i){
					_v [i] /= mag;
				}
			}
			inline void NormalizedCross (const Vector4& v, Vector4& r) const
			{
				Cross (v, r);
				Real mag = r.Length ();
#				ifndef NDEBUG
				if (mag < EPSILON){
					LOG_ERROR ("Cannot normalize vector after cross operations. Length close to 0");
					return;
				}
#				endif
				r /= mag;
			}
			inline Vector4 NormalizedCross (const Vector4& v) const
			{
				Vector4 r;
				Cross (v, r);
				Real mag = r.Length ();
#				ifndef NDEBUG
				if (mag < EPSILON){
					LOG_ERROR ("Cannot normalize vector after cross operations. Length close to 0");
					return Vector4::ZERO;
				}
#				endif
				r /= mag;
				return r;
			}
	};

#if defined (SIM_VECTOR4_ENABLED)
	#define SIM_VECTOR_SIZE 4
	typedef Vector4 Vector;
#elif defined (SIM_VECTOR3_ENABLED)
	#define SIM_VECTOR_SIZE 3
	typedef Vector3 Vector;
#endif

}
