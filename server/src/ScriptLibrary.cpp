/**
 * \file ScriptLibrary.cpp
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

#include "ScriptLibrary.hpp"
#include "dart_api.h"
#include "NativeResolution.hpp"
using namespace DartEmbed;

//----------------------------------------------------------------------

ScriptLibrary::ScriptLibrary(
	const char* name,
	const char* source,
	Dart_NativeEntryResolver nativeResolver,
	Dart_LibraryInitializer initializer)
	: _name(name)
	, _hashedName(fnv1aHash(name))
	, _nativeResolver(nativeResolver)
	, _initializer(initializer)
{ }

//----------------------------------------------------------------------

Dart_Handle ScriptLibrary::load()
{
	// Lookup the library
	Dart_Handle url = Dart_NewString(_name);
	Dart_Handle library = Dart_LookupLibrary(url);

	// See if the library needs to be loaded
	// If its contained in the snapshot it's already present
	if (Dart_IsError(library))
	{
		library = Dart_LoadLibrary(url, Dart_NewString(_source));
	}

	// See if the library requires setup
	if (!Dart_IsError(library))
	{
		// Setup a native resolver
		if (_nativeResolver != 0)
			Dart_SetNativeResolver(library, _nativeResolver);

		// Call the initialization routine
		if (_initializer != 0)
			_initializer(library);
	}

	return library;
}
