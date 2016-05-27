#pragma once

//------------------------------------------------------------------------
//
//  Name:   Singleton.h
//
//  Desc:   super class for singleton class.
//			if you want to make a singleton class,
//			inherit this class with type template
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

template<typename T>
class Singleton {
public:
	static T& getInstance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() {}

private:
	Singleton(const Singleton&) = delete; // no copies
	Singleton& operator=(const Singleton&) = delete; // no self-assignments
};