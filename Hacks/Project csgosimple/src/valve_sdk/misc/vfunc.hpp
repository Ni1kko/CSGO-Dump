#pragma once

template<typename FuncType>
__forceinline static FuncType CallVFunction(void* ppClass, int index)
{
	return (*static_cast<FuncType**>(ppClass))[index];
}