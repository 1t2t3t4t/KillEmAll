#pragma once

#include <typeinfo>
#include <string>

namespace Utils
{
	template<class T>
	T* NullCheck(T* Obj)
	{
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
		if (Obj == nullptr)
		{
			const FString Name = typeid(T).name();
			UE_LOG(LogTemp, Error, TEXT("Object of type %s is null"), *Name);
		}
#endif
		return Obj;
	}
	
}