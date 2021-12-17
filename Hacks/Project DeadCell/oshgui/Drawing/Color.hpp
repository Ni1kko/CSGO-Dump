/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_COLOR_HPP
	#define OSHGUI_DRAWING_COLOR_HPP

#ifndef OSHGUI_EXPORTS_HPP
	#define OSHGUI_EXPORTS_HPP

#ifdef OSHGUI_COMPILE_DLL
#ifdef __GNUC__
	#define OSHGUI_EXPORT __attribute__((dllexport))
#else
	#define OSHGUI_EXPORT __declspec(dllexport)
#endif
#elif defined OSHGUI_COMPILE_DLL_IMPORT
#ifdef __GNUC__
	#define OSHGUI_EXPORT __attribute__((dllimport))
#else
	#define OSHGUI_EXPORT __declspec(dllimport)
#endif
#else
	#define OSHGUI_EXPORT
#endif
#endif

#include <cstdint>

namespace OSHGui {
	namespace Drawing {
		typedef uint32_t argb_t;

		/**
		 * Stellt eine ARGB-Farbe (Alpha, Rot, Gr�n, Blau) dar.
		 */
		class OSHGUI_EXPORT Color {
		public:
			/**
			 * Legt eine leere Farbe an (ARGB = 0)
			 */
			Color();
			/**
			 * Legt eine Farbe mit dem angegeben ARGB Wert an.
			 *
			 * \param argb
			 */
			explicit Color( argb_t argb );
			/**
			 * Legt eine Farbe mit den Werten f�r Rot, Gr�n und Blau an.
			 *
			 * \param red
			 * \param green
			 * \param blue
			 */
			explicit Color( float red, float green, float blue );
			/**
			 * Legt eine Farbe mit den Werten f�r Alpha, Rot, Gr�n und Blau an.
			 *
			 * \param alpha
			 * \param red
			 * \param green
			 * \param blue
			 */
			explicit Color( float alpha, float red, float green, float blue );

			static Color FromRGB( uint8_t red, uint8_t green, uint8_t blue );
			static Color FromARGB( uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue );

			static Color FromARGB( float *clr, float alpha );
			static Color FromARGB( float *clr );

			/**
			 * Gibt eine leere Farbe (ARGB = 0) zur�ck.
			 */
			static Color Empty();

			/**
			 * Gibt die Farbe Rot zur�ck (255, 255, 0, 0)
			 */
			static Color Red();
			/**
			 * Gibt die Farbe Lime zur�ck (255, 0, 255, 0)
			 */
			static Color Lime();
			/**
			 * Gibt die Farbe Blau zur�ck (255, 0, 0, 255)
			 */
			static Color Blue();

			/**
			 * Gibt die Farbe Schwarz zur�ck (255, 0, 0, 0)
			 */
			static Color Black();
			/**
			 * Gibt die Farbe Grau zur�ck (255, 128, 128, 128)
			 */
			static Color Gray();
			/**
			 * Gibt die Farbe Wei� zur�ck (255, 255, 255, 255)
			 */
			static Color White();

			/**
			 * Gibt die Farbe Gelb zur�ck (255, 255, 255, 0)
			 */
			static Color Yellow();
			/**
			 * Gibt die Farbe Fuchsia zur�ck (255, 255, 0, 255)
			 */
			static Color Fuchsia();
			/**
			 * Gibt die Farbe Cyan zur�ck (255, 0, 255, 255)
			 */
			static Color Cyan();
			/**
			 * Gibt die Farbe Orange zur�ck (255, 255, 128, 0)
			 */
			static Color Orange();

			/**
			 * Gibt die Farbe Maroon zur�ck (255, 128, 0, 0)
			 */
			static Color Maroon();
			/**
			 * Gibt die Farbe Gr�n zur�ck (255, 0, 128, 0)
			 */
			static Color Green();
			/**
			 * Gibt die Farbe Navy zur�ck (255, 0, 0, 128)
			 */
			static Color Navy();

			float GetRed() const;
			float GetGreen() const;
			float GetBlue() const;
			float GetAlpha() const;
			void SetAlpha( float alpha );
			argb_t GetARGB() const;

			bool IsTranslucent() const;

			/**
			 * Berechnet den Farbton der Farbe.
			 *
			 * \return der Farbton
			 */
			float Hue() const;
			/**
			 * Berechnet die S�ttigung der Farbe.
			 *
			 * \return die S�ttigung
			 */
			float Saturation() const;
			/**
			 * Berechnet die Helligkeit der Farbe.
			 *
			 * \return die Helligkeit
			 */
			float Brightness() const;

			/**
			 * Wandelt HSB Farbinformationen in ein Color-Objekt um.
			 *
			 * \param hue der Farbton
			 * \param saturation die S�ttigugn
			 * \param brightness die Helligkeit
			 * \return color
			 */
			static Color FromHSB( float hue, float saturation, float brightness );

			void Clamp();
			Color Clamped() const;

			friend bool OSHGUI_EXPORT operator==( const Color &lhs, const Color &rhs );
			friend Color OSHGUI_EXPORT operator+( const Color &lhs, const Color &rhs );
			friend Color OSHGUI_EXPORT operator-( const Color &lhs, const Color &rhs );
			friend Color OSHGUI_EXPORT operator*( const Color &lhs, const Color &rhs );
			friend Color OSHGUI_EXPORT operator*( const Color &lhs, float rhs );

		private:
			void SetARGB( argb_t argb );
			void CalculateARGB();

			float alpha_;
			float red_;
			float green_;
			float blue_;

			argb_t argb_;
		};

		bool OSHGUI_EXPORT operator ==( const Color &lhs, const Color &rhs );
		bool OSHGUI_EXPORT operator!=( const Color &lhs, const Color &rhs );
		Color OSHGUI_EXPORT operator+( const Color &lhs, const Color &rhs );
		Color OSHGUI_EXPORT operator-( const Color &lhs, const Color &rhs );
		Color OSHGUI_EXPORT operator*( const Color &lhs, const Color &rhs );
		Color OSHGUI_EXPORT operator*( const Color &lhs, float rhs );
	}
}

#endif
