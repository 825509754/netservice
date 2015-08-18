#pragma once

#ifndef SINGLETON_H
#define SINGLETON_H


template <class T>
class Singleton
{
protected:
	Singleton(){}
	~Singleton(){}

public:
	static T *get_instance()
	{
		if (instance == NULL)
		{
			instance = new T;
		}

		return instance;
	}

private:
	static T *instance;

};

template<class T> 
T* Singleton<T>::instance = 0;


#endif