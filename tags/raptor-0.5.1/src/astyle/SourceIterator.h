/**
 * This file is part of the raptor project.
 *
 * Copyright (C) 2011 Christophe Duvernois <christophe.duvernois@gmail.com>
 *
 * Raptor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Raptor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef __SOURCE_ITERATOR_H__
#define __SOURCE_ITERATOR_H__

#include <astyle.h>

//----------------------------------------------------------------------------
// SStreamIterator class (extract from astyle_main.h/cpp)
// typename will be istringstream for GUI and istream otherwise
// ASSourceIterator is an abstract class defined in astyle.h
//----------------------------------------------------------------------------

template<typename T>
class SStreamIterator : public astyle::ASSourceIterator
{
	public:
		bool checkForEmptyLine;

		// function declarations
		SStreamIterator(T *in);
		virtual ~SStreamIterator();
		bool getLineEndChange(int lineEndFormat) const;
		string nextLine(bool emptyLineWasDeleted);
		string peekNextLine();
		void peekReset();
		void saveLastInputLine();

	private:
		T * inStream;          // pointer to the input stream
		string buffer;         // current input line
		string prevBuffer;     // previous input line
		int eolWindows;        // number of Windows line endings, CRLF
		int eolLinux;          // number of Linux line endings, LF
		int eolMacOld;         // number of old Mac line endings. CR
		char outputEOL[4];     // next output end of line char
		streamoff peekStart;   // starting position for peekNextLine
		bool prevLineDeleted;  // the previous input line was deleted

	public:	// inline functions
		bool compareToInputBuffer(const string &nextLine) const
		{ return (nextLine == prevBuffer); }
		const char* getOutputEOL() const { return outputEOL; }
		bool hasMoreLines() const { return !inStream->eof(); }
};

#include "SourceIterator.tpp"

#endif // __SOURCE_ITERATOR_H__

