/**
 * @file AxisAlignedBox.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The axis-aligned bounding box class for the Canvas framework
 */
#pragma once

#include "Preprocess.h"
#include "Vector.h"

namespace Sim {

	class AxisAlignedBox {

	protected:
		Vector _v[2] = {Vector::ZERO, Vector::ZERO};
		Vector _center = Vector::ZERO;
		Vector _halflength = Vector::ZERO;

	public:
		// default constructor
		inline AxisAlignedBox () = default;
		// destructor
		inline ~AxisAlignedBox () = default;

		// copy constructor
		inline AxisAlignedBox (const AxisAlignedBox &bv)
		: _center (bv._center), _halflength (bv._halflength)
		{
			for (int i = 0; i < 2; ++i){
				_v [i] = bv._v [i];
			}
		}
		// overloaded constructor
		inline AxisAlignedBox (const Vector &v0, const Vector &v1)
		{
			_v [0] = v0;
			_v [1] = v1;
			Update ();
		}
		// overloaded constructor
		inline AxisAlignedBox (const Real* v0, const Real* v1)
		{
			_v [0] = Vector (v0);
			_v [1] = Vector (v1);
			Update ();
		}

		// assignment operator
		inline AxisAlignedBox &operator = (const AxisAlignedBox &bv)
		{
			for (int i = 0; i < 2; ++i){
				_v [i] = bv._v [i];
			}
			_center = bv._center;
			_halflength = bv._halflength;
			return *this;
		}

		// corner accessor
		inline Vector operator [] (const int i) const
		{
#			ifndef NDEBUG
			if (i < 0 || i > 8){
				LOG_ERROR ("Invalid index for axis aligned box");
				return Vector (_v [1]);
			}
#			endif
			switch (i){
				case 0:
					return Vector (_v [0]);
				case 1:
					return Vector (_v [1][0], _v [0][1], _v [0][2]);
				case 2:
					return Vector (_v [0][0], _v [1][1], _v [0][2]);
				case 3:
					return Vector (_v [1][0], _v [1][1], _v [0][2]);
				case 4:
					return Vector (_v [0][0], _v [0][1], _v [1][2]);
				case 5:
					return Vector (_v [1][0], _v [0][1], _v [1][2]);
				case 6:
					return Vector (_v [0][0], _v [1][1], _v [1][2]);
				case 7: // goes out of the switch body and returns _v[1]
					break;
			}
			return Vector (_v [1]);
		}

		inline const Vector& Center () const
		{
			return _center;
		}
		inline const Vector& Halflength () const
		{
			return _halflength;
		}

		// update method to update center and halflength
		inline void Update ()
		{
			_center = _v[0]; _center += _v[1];
			_center *= 0.5;
			_halflength = _center; _halflength -= _v[0];
		}

		// update method with min and max specified
		inline void Update (const Vector& min, const Vector& max)
		{
			_v [0] = min; _v [1] = max;
			Update ();
		}

		// bounding box inside another bounding box test
		inline bool Inside (const AxisAlignedBox &bv) const
		{
			return (bv._v[0][0] >= _v[0][0]) & (bv._v[1][0] <= _v[1][0]) & (bv._v[0][1] >= _v[0][1]) &
					(bv._v[1][1] <= _v[1][1]) & (bv._v[0][2] >= _v[0][2]) & (bv._v[1][2] <= _v[1][2]);
		}
	};
}
