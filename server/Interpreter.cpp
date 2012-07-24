/**
 * \file Interpreter.cpp
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

#include "Interpreter.hpp"
#include "ScriptLibrary.hpp"
#include "Hash.hpp"
#include "dart_api.h"
#include "InputScriptLibrary.hpp"
using namespace DartEmbed;

namespace
{
	/**
	 * Instance of the Interpreter class.
	 *
	 * Currently only a single instance can be created.
	 */
	Interpreter* __instance = 0;

	/**
	 * Callback for when an isolate is created.
	 */
	bool __isolateCreateCallback(const char* scriptUri, const char* main, void* callbackData, char** error)
	{
		return true;
	}

	/**
	 * Callback for when an isolate is interrupted.
	 */
	bool __isolateInterruptCallback()
	{
		return true;
	}

	/**
	 * Callback for when an isolate is shut down.
	 */
	void __isolateShutdownCallback(void* callbackData)
	{
		
	}

	bool __isDartSchemeUrl(const char* url)
	{
		return strncmp(url, "dart:", 5) == 0;
	}

	bool __isEmbedSchemeUrl(const char* url)
	{
		return strncmp(url, "embed:", 5) == 0;
	}

	Dart_Handle __loadLibrary(Dart_Handle url)
	{
		Dart_Handle library = Dart_LookupLibrary(url);

		if (Dart_IsError(library))
		{
			library = Dart_LoadLibrary(url, Dart_NewString(""));

			if (!Dart_IsError(library))
			{
				return Dart_Error("Booo");
			}
		}

		return library;
	}

	/**
	 * Library handler.
	 */
	Dart_Handle __libraryHandler(Dart_LibraryTag tag, Dart_Handle library, Dart_Handle url)
	{
		if (!Dart_IsLibrary(library))
			return Dart_Error("Not a library");
		if (!Dart_IsString8(url))
			return Dart_Error("URL is not a string");

		const char* urlString = 0;
		Dart_Handle result = Dart_StringToCString(url, &urlString);

		if (tag == kCanonicalizeUrl)
		{
			// See if this is a dart builtin library
			if (__isDartSchemeUrl(urlString))
				return url;
			// See if this is a embed builtin library
			if (__isEmbedSchemeUrl(urlString))
				return url;
		}
		else if (tag == kImportTag)
		{
			if (__isEmbedSchemeUrl(urlString))
			{
				Dart_Handle builtinLibrary = Dart_LookupLibrary(url);

				if (Dart_IsError(builtinLibrary))
				{
					InputScriptLibrary input;
					builtinLibrary = Dart_LoadLibrary(url, Dart_NewString(input.getSource()));

					if (!Dart_IsError(builtinLibrary))
					{
						Dart_NativeEntryResolver resolver = input.getNativeResolver();

						if (resolver)
							Dart_SetNativeResolver(builtinLibrary, resolver);
					}
					else
					{
						return Dart_Error(Dart_GetError(builtinLibrary));
					}
				}

				return builtinLibrary;
			}
		}

		return Dart_Error("Not found");
	}

	bool __checkResult(Dart_Handle result)
	{
		if (!Dart_IsError(result))
			return true;

		return true;
	}
}

//----------------------------------------------------------------------

Interpreter::Interpreter()
{
	__instance = this;

	if (Dart_SetVMFlags(0, 0))
	{
		if (Dart_Initialize(__isolateCreateCallback, __isolateInterruptCallback, __isolateShutdownCallback))
		{
			char* error;
			_isolate = Dart_CreateIsolate("embed", "main", 0, this, &error);

			Dart_EnterScope();
			Dart_SetLibraryTagHandler(__libraryHandler);
		}
	}
}

//----------------------------------------------------------------------

Interpreter::~Interpreter()
{
	Dart_ExitScope();
	Dart_ExitIsolate();
}

//----------------------------------------------------------------------

void Interpreter::invokeMain(const char* library)
{
	const ScriptLibrary* script = getLibrary(library);

	if (script)
	{
		Dart_Handle result = Dart_Invoke(script->getLibrary(), Dart_NewString("main"), 0, 0);

		if (::Dart_IsError(result))
		{
			const char* error = ::Dart_GetError(result);
			printf(error);
		}
	}
}

//----------------------------------------------------------------------

ScriptLibrary* Interpreter::getLibrary(const char* name) const
{
	std::int32_t hash = fnv1aHash(name);
	std::size_t size = _libraries.size();

	for (std::size_t i = 0; i < size; ++i)
	{
		ScriptLibrary* library = _libraries[i];

		if (hash == library->getHashedName())
			return library;
	}

	return 0;
}

//----------------------------------------------------------------------

void Interpreter::loadLibrary(ScriptLibrary* library)
{
	Dart_Handle url = Dart_NewString(library->getName());
	Dart_Handle source = Dart_NewString(library->getSource());
	Dart_Handle lib = Dart_LoadScript(url, source);

	if (!Dart_IsError(lib))
	{
		if (Dart_IsLibrary(lib))
		{
			// Attach the library handle
			library->setLibrary(lib);

			// See if native code is used within the library
			Dart_NativeEntryResolver resolver = library->getNativeResolver();

			if (resolver)
				Dart_SetNativeResolver(lib, resolver);

			// Add the library
			_libraries.push_back(library);

			return;
		}
	}
	
	// An error occurred
	const char* error = Dart_GetError(lib);
	printf("-- Error: %s\n", Dart_GetError(lib));
}
