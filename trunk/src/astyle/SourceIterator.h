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