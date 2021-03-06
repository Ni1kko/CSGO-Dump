/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_MISC_TEXTHELPER_HPP
#define OSHGUI_MISC_TEXTHELPER_HPP

#include <memory>
#include "../Exports.hpp"
#include "Strings.hpp"
#include "../Drawing/Font.hpp"
#include "../Drawing/Point.hpp"
#include "../Drawing/Size.hpp"

namespace OSHGui {
	namespace Misc {
		/**
		 * Verwaltet eine Zeichenkette und deren Ausma?e.
		 */
		class OSHGUI_EXPORT TextHelper {
		public:
			/**
			 * Erstellt ein TextHelper-Objekt mit der entsprechenden Schriftart.
			 *
			 * \param font
			 */
			TextHelper( const Drawing::FontPtr &font );

			/**
			 * Legt die Schriftart fest.
			 *
			 * \param font darf nicht 0 sein
			 */
			void SetFont( const Drawing::FontPtr &font );
			/**
			 * Legt den Text fest.
			 *
			 * \param text
			 */
			void SetText( const AnsiString &text );
			/**
			 * H?ngt das Zeichen an den Text an.
			 *
			 * \param character
			 */
			void Append( const AnsiChar character );
			/**
			 * H?ngt die Zeichenkette an den Text an.
			 *
			 * \param text
			 */
			void Append( const AnsiString &text );
			/**
			 * F?gt das Zeichen an der entsprechenden Position im Text ein.
			 *
			 * \param position
			 * \param character
			 */
			void Insert( int position, const AnsiChar character );
			/**
			 * F?gt die Zeichenkette an der entsprechenden Position im Text ein.
			 *
			 * \param position
			 * \param text
			 */
			void Insert( int position, const AnsiString &text );
			/**
			 * L?scht den kompletten Text.
			 */
			void Clear();
			/**
			 * L?scht das Zeichen an der entsprechenden Stelle.
			 *
			 * \param index die Stelle ab der gel?scht werden soll
			 * \param length Anzahl der zu l?schenden Zeichen (default: 1)
			 */
			void Remove( int index, int length = 1 );

			/**
			 * Ruft die L?nge der Zeichenkette ab.
			 *
			 * \return length
			 */
			int GetLength() const;
			/**
			 * Ruft die Zeichenkette ab.
			 *
			 * \return text
			 */
			const AnsiString &GetText() const;
			/**
			 * Ruft die Ma?e der Zeichenkette in Pixel ab.
			 *
			 * \return size
			 */
			const Drawing::SizeF &GetSize() const;

			/**
			 * Berechnet die Ma?e neu.
			 */
			void RefreshSize();

			/**
			 * Gibt die Position des Zeichens an der entsprechenden Stelle zur?ck.
			 *
			 * \param index
			 * \param trailing linker Rand des Zeichens / rechter Rand des Zeichens
			 * \return position
			 */
			Drawing::PointF GetCharacterPosition( int index, bool trailing = false ) const;
			/**
			 * Gibt die Gr??e eines Teilstrings zur?ck.
			 *
			 * \param index der Startindex
			 * \param size die L?nge der Zeichenkette. -1 = bis zum Ende (default: -1)
			 * \return die Gr??e
			 */
			Drawing::SizeF GetStringSize( int index, int size = -1 ) const;
			/**
			 * Gibt den Index des Zeichens zur?ck, das der Position am n?chsten steht.
			 *
			 * \param position
			 * \return index
			 */
			int GetClosestCharacterIndex( const Drawing::PointF &position ) const;

		private:
			AnsiString text_;

			Drawing::SizeF size_;

			Drawing::FontPtr font_;
		};
	}
}

#endif
