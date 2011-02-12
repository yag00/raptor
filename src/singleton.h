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