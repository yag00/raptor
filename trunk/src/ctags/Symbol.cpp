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

#include "Symbol.hpp"

Symbol::Symbol(TagEntryListItem* symbol_) : _symbol(symbol_){

}

Symbol::~Symbol(){
	while ( item != NULL ) {
		TagEntryListItem* temp = 0;
		tagEntryInfo* entry = &_symbol->tag;
		
		if ( entry->language )
			free( (char*)entry->language );
		
		if ( entry->sourceFileName )
			free( (char*)entry->sourceFileName );
		
		if ( entry->name)
			free( (char*)entry->name );
		
		if ( entry->kindName )
			free( (char*)entry->kindName );
		
		if ( entry->extensionFields.access )
			free( (char*)entry->extensionFields.access );
		
		if ( entry->extensionFields.fileScope )
			free( (char*)entry->extensionFields.fileScope );
		
		if ( entry->extensionFields.implementation )
			free( (char*)entry->extensionFields.implementation  );
		
		if ( entry->extensionFields.inheritance )
			free( (char*)entry->extensionFields.inheritance );
		
		if ( entry->extensionFields.scope[0] )
			free( (char*)entry->extensionFields.scope[0] );
		
		if ( entry->extensionFields.scope[1] )
			free( (char*)entry->extensionFields.scope[1] );
		
		if ( entry->extensionFields.typeRef[0] )
			free( (char*)entry->extensionFields.typeRef[0] );
		
		if ( entry->extensionFields.typeRef[1] )
			free( (char*)entry->extensionFields.typeRef[1] );
		
		if ( entry->extensionFields.signature )
			free( (char*)entry->extensionFields.signature );
		
		temp = item->next;
		free( item );
		item = temp;
	}
}

