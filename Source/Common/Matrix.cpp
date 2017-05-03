/**
 * @file Matrix.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See Matrix.h.
 */

#include "Matrix.h"

namespace Sim {

	const Matrix3x3 Matrix3x3::ZERO (0., 0., 0., 0., 0., 0., 0., 0., 0.);
	const Matrix3x3 Matrix3x3::ONE (1., 1., 1., 1., 1., 1., 1., 1., 1.);
	const Matrix3x3 Matrix3x3::IDENTITY (1., 0., 0., 0., 1., 0., 0., 0., 1.);

	const Matrix4x4 Matrix4x4::ZERO (
			0., 0., 0., 0.,
			0., 0., 0., 0.,
			0., 0., 0., 0.,
			0., 0., 0., 1.);
	const Matrix4x4 Matrix4x4::ONE (
			1., 1., 1., 1.,
			1., 1., 1., 1.,
			1., 1., 1., 1.,
			1., 1., 1., 1.);
	const Matrix4x4 Matrix4x4::IDENTITY (
			1., 0., 0., 0.,
			0., 1., 0., 0.,
			0., 0., 1., 0.,
			0., 0., 0., 1.);
}
