/**
 * @file Geometry.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The geometry component interface for the Asset class in the Canvas
 * class. It's derived from the generic Component interface. Geometry
 * holds all the vertices and face-indices for any asset.
 */
#pragma once

#include <memory>

#include "Vector.h"
#include "AxisAlignedBox.h"
#include "Asset/Component.h"

namespace Sim {
	namespace Assets {

		auto toggle = [=] (int val) {return val ^ (0 ^ 1);};

		class Geometry : public Component {

		protected:
			class SpatialSubset {

				friend class Geometry;

			protected:
				unsigned int _voffset = 0;
				unsigned int _ioffset = 0;
				unsigned int _isize = 0;
				AxisAlignedBox _bound;

			public:
				SpatialSubset () = default;
				~SpatialSubset () = default;

				// udpate the axis-aligned bounding box for subset
				void UpdateBound (Vector* vertices, unsigned int* faces)
				{
					Vector min (vertices [faces [0] - _voffset]);
					Vector max (min);
					for (unsigned int i = 1; i < 3*_isize; ++i){
						for (unsigned int j = 0; j < 3; ++j){
							if (min [j] > vertices [faces [i] - _voffset][j]){
								min [j] = vertices [faces [i] - _voffset][j];
							}
						}
						for (unsigned int j = 0; j < 3; ++j){
							if (max [j] < vertices [faces [i] - _voffset][j]){
								max [j] = vertices [faces [i] - _voffset][j];
							}
						}
					}
					_bound.Update (min, max);
				}
			};

		protected:
			AxisAlignedBox _bounds;

			int _offsetIndex = 0;
			unsigned int _offsetSize = 0;
      unsigned int _numVertices = 0;
      unsigned int _numSurfaceVertices = 0;
      std::unique_ptr <Vector []> _vertices;

			unsigned int _numFaces = 0;
			std::unique_ptr <unsigned int []> _faces;

      unsigned int _numSubsets = 1;
      std::unique_ptr <SpatialSubset []> _subsets;

		public:
			Geometry () = default;
			virtual ~Geometry () {Cleanup ();}

			AssetComponentType Type () const final {return AssetComponentType::Geometry;}

			virtual bool Initialize (tinyxml2::XMLElement& config, Asset* asset) override;
			virtual void Update () override;
			virtual void Cleanup () override;

			unsigned int VertexCount () const {return _numVertices;}
			unsigned int SurfaceVertexCount () const {return _numSurfaceVertices;}
			Vector* PreviousVertexBuffer () {return &(_vertices.get () [toggle (_offsetIndex)*_numVertices]);}
			Vector* CurrentVertexBuffer () {return &(_vertices.get () [_offsetIndex*_numVertices]);}

			unsigned int FaceIndexCount () const {return _numFaces;}
			unsigned int* FaceIndexBuffer () {return _faces.get ();}
			unsigned int* FaceIndexBuffer (unsigned int index)
			{
#					ifndef NDEBUG
				if (index >= _numSubsets){
					LOG_ERROR ("Accessing invalid index buffer no" << index << " (count: " << _numSubsets << ")");
					return nullptr;
				} else {
#					endif
					return &(_faces.get () [_subsets.get () [index]._ioffset]);
#					ifndef NDEBUG
				}
#					endif
			}

		protected:
			bool ReadVertexFile (const char* file);
			bool ReadIndexFiles (const char* prefix);
			void UpdateSurfaceVertexCount ();
		};
	}
}
