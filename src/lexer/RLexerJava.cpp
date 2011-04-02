#include <QtGui>
#include "RLexerJava.h"


// The ctor.
RLexerJava::RLexerJava(QObject *parent) : RLexerCPP(parent) {
}

// The dtor.
RLexerJava::~RLexerJava() {
}


// Returns the language name.
const char *RLexerJava::language() const {
    return "Java";
}


// Returns the set of keywords.
const char *RLexerJava::keywords(int set) const {
	if (set == 1)
		return								
			"instanceof assert if else switch case default break goto return for "
			"while do continue new throw throws try catch finally this super extends "
			"implements import true false null ";
			
	if (set == 2)
        return
			"package transient strictfp void char short int long double float const "
			"static volatile byte boolean class interface native private protected "
			"public final abstract synchronized enum ";
	if (set == 3)
        return	
           " future generic inner operator outer rest var ";
		   
	return 0;
}
