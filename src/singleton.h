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

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <class T> class Singleton {
	public :
	
		/** @return unique instance of the class */
		static T& getInstance(){
			if (!_instance)
				_instance = new T;
			return *_instance;
		}

		/** destroy the instance of the class */
		static void destroy(){
			delete _instance;
			_instance = 0;
		}

	protected :    
		/** Constructor */
		Singleton() {}
		/** Destructor */
		~Singleton() {}

	private :
		/** instance of the class */
		static T* _instance;
  
		/** Copy constructor : copy forbidden */
		Singleton(Singleton&);
		/** copy forbidden */
		void operator =(Singleton&);
};

/** Static declaration */
template <class T> T* Singleton<T>::_instance = 0;

#endif /* __SINGLETON_H__ */