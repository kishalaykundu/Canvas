/**
 * @file MsdRender.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The OpenGL render component for the CuglMsd data type. It is
 * extended from component type.
 */
#pragma once

#include "tinyxml2.h"

#include "Types.h"
#include "GL/GLUtils.h"
#include "Asset/Asset.h"
#include "Asset/Component.h"

namespace Sim {
	namespace Assets {

		class MsdRender : public Component {

			friend class MsdPhysics;

		protected:
			GLuint _vertexArray = 0;

			// vertex positions
			GLuint _positionBuffer = 0;

			/**
			 * Buffer objects specifications:
			 * 1. Normal texture coordinates
			 * 2. Color texture coordinates
			 */
			GLuint _texCoordBuffers [2] = {0, 0};

			GLuint _colorTextur = 0;
			GLuint _indexBuffer = 0;

			unsigned int _normalFramebufferDimensions [2] = {0, 0};
			GLuint _normalFramebuffer = 0;
			GLuint _normalTexture = 0;

			/**
			 * Program ID specifications:
			 * 1. Normal calculation program
			 * 2. Rendering program
			 */
			GLuint _program [2] = {0, 0};

			bool _colorTextureExists = false;

		public:
			MsdRender () = default;
			~MsdRender () {Cleanup ();}

			MsdRender (const MsdRender&) = delete;
			MsdRender& operator = (const MsdRender&) = delete;

			AssetComponentType Type () const final {return AssetComponentType::Render;}

			bool Initialize (tinyxml2::XMLElement& config, Asset* asset) override;
			void Update () override {}
			void Cleanup () override;

		protected:
			bool LoadPrograms (tinyxml2::XMLElement&);
		};
	}
}
