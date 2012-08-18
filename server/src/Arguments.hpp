/**
 * \file Arguments.hpp
 *
 * \section COPYRIGHT
 *
 * Dart Embedding Example
 *
 * ---------------------------------------------------------------------
 *
 * Copyright (c) 2012 Don Olmstead
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software
 *   in a product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be
 *   misrepresented as being the original software.
 *
 *   3. This notice may not be removed or altered from any source
 *   distribution.
 */

#ifndef DART_EMBED_ARGUMENTS_HPP_INCLUDED
#define DART_EMBED_ARGUMENTS_HPP_INCLUDED

#include "dart_api.h"
#include <cassert>

namespace DartEmbed
{
	template <typename T>
	void getNativeField(Dart_NativeArguments args, int index, T** value)
	{
		Dart_Handle handle = Dart_GetNativeArgument(args, index);

		assert(Dart_IsInstance(handle));

		std::intptr_t ptr;
		Dart_GetNativeInstanceField(handle, 0, &ptr);

		*value = reinterpret_cast<T*>(ptr);
	}

	//----------------------------------------------------------------------

	template <typename T>
	void getValue(Dart_NativeArguments args, int index, T* value);

	//----------------------------------------------------------------------

	template <>
	inline void getValue<std::int32_t>(Dart_NativeArguments args, int index, std::int32_t* value)
	{
		Dart_Handle handle = Dart_GetNativeArgument(args, index);

		assert(Dart_IsInteger(handle));

		std::int64_t temp;
		Dart_IntegerToInt64(handle, &temp);

		*value = static_cast<std::int32_t>(temp);
	}

	//----------------------------------------------------------------------

	template <>
	inline void getValue<float>(Dart_NativeArguments args, int index, float* value)
	{
		Dart_Handle handle = Dart_GetNativeArgument(args, index);

		assert(Dart_IsDouble(handle));

		double temp;
		Dart_DoubleValue(handle, &temp);

		*value = static_cast<float>(temp);
	}

	//----------------------------------------------------------------------

	template <>
	inline void getValue<double>(Dart_NativeArguments args, int index, double* value)
	{
		Dart_Handle handle = Dart_GetNativeArgument(args, index);

		assert(Dart_IsDouble(handle));

		Dart_DoubleValue(handle, value);
	}
} // end namespace DartEmbed

#endif // end DART_EMBED_ARGUMENTS_HPP_INCLUDED
