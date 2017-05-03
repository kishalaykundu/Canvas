/**
 * @file Vector.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See Vector.h
 */

#include "Vector.h"

namespace Sim {
	const Vector2 Vector2::ZERO (0., 0.);
	const Vector2 Vector2::UNIT (1., 1.);

	const Vector2 Vector2::UNIT_X (1., 0.);
	const Vector2 Vector2::UNIT_Y (0., 1.);

	const Vector2 Vector2::NEG_UNIT_X (-1., 0.);
	const Vector2 Vector2::NEG_UNIT_Y (0., -1.);


	const Vector3 Vector3::ZERO (0., 0., 0.);
	const Vector3 Vector3::UNIT (1., 1., 1.);

	const Vector3 Vector3::UNIT_X (1., 0., 0.);
	const Vector3 Vector3::UNIT_Y (0., 1., 0.);
	const Vector3 Vector3::UNIT_Z (0., 0., 1.);

	const Vector3 Vector3::NEG_UNIT_X (-1., 0., 0.);
	const Vector3 Vector3::NEG_UNIT_Y (0., -1., 0.);
	const Vector3 Vector3::NEG_UNIT_Z (0., 0., -1.);


	const Vector4 Vector4::ZERO (0., 0., 0., 1.);
	const Vector4 Vector4::UNIT (1., 1., 1., 1.);

	const Vector4 Vector4::UNIT_X (1., 0., 0., 1.);
	const Vector4 Vector4::UNIT_Y (0., 1., 0., 1.);
	const Vector4 Vector4::UNIT_Z (0., 0., 1., 1.);

	const Vector4 Vector4::NEG_UNIT_X (-1., 0., 0., 1.);
	const Vector4 Vector4::NEG_UNIT_Y (0., -1., 0., 1.);
	const Vector4 Vector4::NEG_UNIT_Z (0., 0., -1., 1.);
}
