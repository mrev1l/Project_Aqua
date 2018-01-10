#pragma once

namespace aqua
{
namespace patterns
{

template<class T>
class Singleton
{
public:
	static void CreateInstance()
	{
		Singleton<T>::s_instance = new T();
	}

	static T* GetInstance()
	{
		return Singleton<T>::s_instance;
	}

	virtual bool Initialize()
	{
		return true;
	}

	virtual bool Shutdown()
	{
		return true;
	}

	static void DestroyInstance()
	{
		if (Singleton<T>::s_instance)
		{
			delete Singleton<T>::s_instance;
			Singleton<T>::s_instance = nullptr;
		}
	}

protected:
	Singleton() = default;
	virtual ~Singleton() = default;
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	Singleton(const Singleton&) = default;
	Singleton& operator =(const Singleton&) = default;

	static T* s_instance;
};

}
}