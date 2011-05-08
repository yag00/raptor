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

#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <QObject>


class Symbol : public  QObject
{
	public:
		Symbol();
		~Symbol();
	
	private:
		std::string _name;
		std::string _filename;
	
		/* language of source file */
		std::string _language;
	
		/* line number of tag */
		uint32_t _line;

		bool isFileScope
	boolean     lineNumberEntry;  /* pattern or line number entry */

	boolean     isFileScope;      /* is tag visibile only within source file? */
	boolean     isFileEntry;      /* is this just an entry for a file name? */
	boolean     truncateLine;     /* truncate tag line at end of tag name? */
	const char *sourceFileName;   /* name of source file */
	const char *name;             /* name of the tag */
	const char *kindName;         /* kind of tag */
	char        kind;             /* single character representation of kind */
	struct {
		const char* access;
		const char* fileScope;
		const char* implementation;
		const char* inheritance;
		const char* scope [2];    /* value and key */
		const char* signature;

		/* type (union/struct/etc.) and name for a variable or typedef. */
		const char* typeRef [2];  /* e.g., "struct" and struct name */

	} extensionFields;  /* list of extension fields*/
	
};

#endif /* __SYMBOL_H__ */
