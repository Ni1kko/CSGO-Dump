/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_DIRECT3D8RENDERER_HPP
#define OSHGUI_DRAWING_DIRECT3D8RENDERER_HPP

#include "../Renderer.hpp"

#include <d3d8.h>
#include <vector>
#include <map>

namespace OSHGui {
	namespace Drawing {
		class Direct3D8Texture;
		class Direct3D8TextureTarget;
		class Direct3D8GeometryBuffer;

		/**
		 * Die Direct3D8 Variante des Renderer.
		 */
		class OSHGUI_EXPORT Direct3D8Renderer : public Renderer {
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * \param device Direct3D8 Device
			 */
			Direct3D8Renderer( LPDIRECT3DDEVICE8 device );
			~Direct3D8Renderer();

			/**
			 * Hilfsfunktion, die vor einem D3D Reset aufgerufen werden muss.
			 */
			void PreD3DReset();
			/**
			 * Hilfsfunktion, die nach einem D3D Reset aufgerufen werden muss.
			 */
			void PostD3DReset();

			/**
			 * Ruft das Direct3D8 Device ab.
			 */
			LPDIRECT3DDEVICE8 GetDevice() const;

			/**
			 * Pr?ft, ob nicht-quadratische Texturgr??en unterst?tzt werden.
			 */
			bool SupportsNonSquareTexture();

			/**
			 * Pr?ft, ob nicht-2^x Texturgr??en unterst?tzt werden.
			 */
			bool SupportsNPOTTextures();

			/**
			 * Rechnet die Gr??e in eine unterst?tzt Gr??e um.
			 *
			 * \param size
			 * \return umgewandelte Gr??e
			 * \sa SupportsNonSquareTexture(), SupportsNPOTTextures()
			 */
			SizeF GetAdjustedSize( const SizeF &size );

			virtual RenderTargetPtr &GetDefaultRenderTarget() override;
			virtual GeometryBufferPtr CreateGeometryBuffer() override;
			virtual TextureTargetPtr CreateTextureTarget() override;
			virtual TexturePtr CreateTexture() override;
			virtual TexturePtr CreateTexture( const Misc::AnsiString &filename ) override;
			virtual TexturePtr CreateTexture( const SizeF &size ) override;

			virtual void BeginRendering() override;
			virtual void EndRendering() override;
			virtual void SetDisplaySize( const SizeF &sz ) override;
			virtual const SizeF &GetDisplaySize() const override;
			virtual const PointF &GetDisplayDPI() const override;
			virtual uint32_t GetMaximumTextureSize() const override;

		private:
			/**
			 * Ruft die Gr??e des Viewport ab.
			 *
			 * \return Gr??e des Viewport
			 */
			SizeF GetViewportSize();
			/**
			 * Berechnet die n?chste Zweierpotenz, falss \a size keine Zweierpotenz ist.
			 *
			 * \param size
			 * \return n?chste Zweierpotenz
			 */
			float GetSizeNextPOT( float size ) const;

			/**
			 * Entfernt ung?ltige Referenzen zu Objekten, f?r die ein Reset ausgef?hrt werden muss.
			 */
			void RemoveWeakReferences();

			LPDIRECT3DDEVICE8 device;

			SizeF displaySize;
			PointF displayDPI;

			RenderTargetPtr defaultTarget;

			typedef std::vector< std::weak_ptr< Direct3D8TextureTarget > > TextureTargetList;
			TextureTargetList textureTargets;

			typedef std::vector< std::weak_ptr< Direct3D8Texture > > TextureList;
			TextureList textures;

			uint32_t maxTextureSize;
			bool supportNPOTTex;
			bool supportNonSquareTex;

			DWORD stateBlockHandle;
		};
	}
}

#endif
