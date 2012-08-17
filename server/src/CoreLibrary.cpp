/**
 * \file CoreLibrary.cpp
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

#include "BuiltinLibraries.hpp"
#include "ScriptLibrary.hpp"
#include "NativeResolution.hpp"
using namespace DartEmbed;

//---------------------------------------------------------------------
// Native functions
//---------------------------------------------------------------------

DECLARE_FUNCTION(Exit, 1)
DECLARE_FUNCTION(Logger_PrintString, 1)

namespace
{
	/// Whether the library has been initialized
	bool __libraryInitialized = false;
	/// Native entries for the core library
	NativeEntry __coreNativeEntries[3];

	/**
	 * Sets up the native entries for the core library.
	 */
	void __setupCoreLibrary()
	{
		if (!__libraryInitialized)
		{
			// There aren't enough entries to warrant the class/function
			// lookup mechanism. Just setup the native entries
			setNativeEntry(&__coreNativeEntries[0], "Exit",               FUNCTION_NAME(Exit),                       1);
			setNativeEntry(&__coreNativeEntries[1], "Logger_PrintString", FUNCTION_NAME(Logger_PrintString),         1);
			// Set the sentinal value
			setNativeEntry(&__coreNativeEntries[2], "", 0, 0);
		}

		__libraryInitialized = true;
	}

	/**
	 * Native resolver for the core library.
	 *
	 * \param name The name of the function to invoke.
	 * \param argumentCount The number of arguments.
	 */
	Dart_NativeFunction __coreLibraryResolver(Dart_Handle name, int argumentCount)
	{
		const char* nativeFunctionName = 0;
		Dart_Handle result = Dart_StringToCString(name, &nativeFunctionName);

		assert(nativeFunctionName);

		std::int32_t hash = fnv1aHash(nativeFunctionName);

		NativeEntry* functionEntry = __coreNativeEntries;

		while (functionEntry->function != 0)
		{
			if (functionEntry->hash == hash)
			{
				assert(functionEntry->argumentCount == argumentCount);
				return functionEntry->function;
			}

			functionEntry++;
		}

		return 0;
	}
} // end anonymous namespace

//---------------------------------------------------------------------

ScriptLibrary* BuiltinLibraries::createCoreLibrary()
{
	// Setup the native entries
	__setupCoreLibrary();

	// The library source is compiled in along with the snapshot.
	// Native entries are present within the library so a resolver needs to be set.
	// IO initializer allows some functions to be called before the library is used.
	return new ScriptLibrary("dart:builtin", 0, __coreLibraryResolver);
}
