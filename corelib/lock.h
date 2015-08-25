#pragma once

#ifndef _LOCK_HEAD_H_
#define _LOCK_HEAD_H_

#include <pthread.h>

class Lock
{
public:
	Lock()
	{
		pthread_mutex_init(&m_mutex, nullptr);
	}
	
	~Lock()
	{
		pthread_mutex_destroy(&m_mutex);
	}
	
public:
	void lock()
	{
		pthread_mutex_lock(&m_mutex);
	}
	
	void unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}
	
private:
	pthread_mutex_t m_mutex;	
};

#endif