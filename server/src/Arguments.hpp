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
	void getValue(Dart_NativeArguments args, int index, T* value)
	{
		Dart_Handle handle = DartGetNativeArgument(args, index);

		// \todo Is this correct?
		assert(Dart_IsClass(handle));

		std::intptr_t ptr;
		Dart_GetNativeInstanceField(handle, 0, &ptr);

		value = static_cast<T>(ptr);
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
