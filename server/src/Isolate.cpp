/**
 * \file Isolate.cpp
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

#include <DartEmbed/Isolate.hpp>
#include <DartEmbed/VirtualMachine.hpp>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOKERNEL
#define NOUSER
#define NOSERVICE
#define NOSOUND
#define NOMCX

#include <windows.h>

#include "dart_api.h"
#include "isolate_data.h"
#include "NativeResolution.hpp"
#include "ScriptLibrary.hpp"
#include "BuiltinLibraries.hpp"
using namespace DartEmbed;

/// The snapshot data
extern const uint8_t* snapshot_buffer;

namespace
{
	//----------------------------------------------------------------------
	// Virtual machine instances
	//----------------------------------------------------------------------

	/// The current directory
	char* __currentDirectory;
	/// Whether the virtual machine has been initialized
	bool __initialized = false;
	/// The isolates currently running in the virtual machine
	std::vector<Isolate*> __runningIsolates;

	//----------------------------------------------------------------------
	// ScriptLibrary instances
	//----------------------------------------------------------------------

	/// The dart:core library
	ScriptLibrary* __coreLibrary;
	/// The dart:io library
	ScriptLibrary* __ioLibrary;
	/// The dart:json library
	ScriptLibrary* __jsonLibrary;
	/// The dart:uri library
	ScriptLibrary* __uriLibrary;
	/// The dart:crypto library
	ScriptLibrary* __cryptoLibrary;
	/// The dart:utf library
	ScriptLibrary* __utfLibrary;

	/// Any additional libraries specified
	std::vector<ScriptLibrary*> __libraries;

	//----------------------------------------------------------------------
	// URI/URL functions
	//----------------------------------------------------------------------

	/**
	 * Resolves the script's URI.
	 *
	 * Invokes _resolveScriptUri from within the core library to determine the
	 * full URI to the file.
	 *
	 * \note Could be moved to aux lib. Should be overloadable assuming that
	 *       _resolveScriptUri doesn't have to be invoked. This would allow
	 *       for embedders who already have their own URI implementation and
	 *       file system accessor to be used.
	 *
	 * \param scriptUri The path to the script.
	 * \param coreLibrary Handle to the core library.
	 * \returns The full URI to the script.
	 */
	Dart_Handle __resolveScriptUri(const char* scriptUri, Dart_Handle coreLibrary)
	{
		const std::int32_t numberOfArgs = 3;

		Dart_Handle args[numberOfArgs];
		args[0] = Dart_NewString(__currentDirectory);
		args[1] = Dart_NewString(scriptUri);
		args[2] = Dart_True(); // Windows host is always true

		return Dart_Invoke(coreLibrary, Dart_NewString("_resolveScriptUri"), numberOfArgs, args);
	}

	/**
	 * Determines the file path from the given URI.
	 *
	 * Invokes _filePathFromUri from within the core library to determine the
	 * path to the file.
	 *
	 * \note Could be moved to aux lib. Should be overloadable assuming that
	 *       _filePathFromUri doesn't have to be invoked. This would allow
	 *       for embedders who already have their own URI implementation and
	 *       file system accessor to be used.
	 *
	 * \param scriptUri URI to the script.
	 * \param coreLibrary Handle to the core library.
	 * \returns The file path to the script.
	 */
	Dart_Handle __filePathFromUri(Dart_Handle scriptUri, Dart_Handle coreLibrary)
	{
		const std::int32_t numberOfArgs = 2;
		Dart_Handle args[numberOfArgs];
		args[0] = scriptUri;
		args[1] = Dart_True(); // Windows host is always true

		return Dart_Invoke(coreLibrary, Dart_NewString("_filePathFromUri"), numberOfArgs, args);
	}

	/**
	 * Detemines if the URL signifies a Dart library.
	 *
	 * \param url The url to query.
	 * \returns true if the URL is a Dart libray; false otherwise.
	 */
	bool __isDartSchemeUrl(const char* url)
	{
		return strncmp(url, "dart:", 5) == 0;
	}

	//----------------------------------------------------------------------
	// File loading
	//----------------------------------------------------------------------

	/**
	 * Reads the source from the given URI.
	 *
	 * \note Could be moved to the aux lib. Should be overloadable so file handling
	 *       can be handled by the embedder.
	 *
	 * \param scriptUri URI for the script.
	 * \param coreLibrary Handle to the core library.
	 */
	Dart_Handle __readSource(Dart_Handle scriptUri, Dart_Handle coreLibrary)
	{
		Dart_Handle scriptPath = __filePathFromUri(scriptUri, coreLibrary);

		const char* scriptPathString;
		Dart_StringToCString(scriptPath, &scriptPathString);

		FILE* file = fopen(scriptPathString, "r");

		if (file)
		{
			fseek(file, 0, SEEK_END);
			long length = ftell(file);
			fseek(file, 0, SEEK_SET);

			char* buffer = new char[length + 1];
			size_t read = fread(buffer, 1, length, file);
			fclose(file);
			buffer[read] = '\0';

			Dart_Handle source = Dart_NewString(buffer);

			delete[] buffer;

			return source;
		}

		return Dart_Error("Unable to read file");
	}

	/**
	 * Loads a script from the given URI.
	 *
	 * \param scriptUri URI pointing to the script's location.
	 * \param resolve Whether the script's location needs to be resolved.
	 * \param coreLibrary The dart:core library.
	 */
	Dart_Handle __loadScript(const char* scriptUri, bool resolve, Dart_Handle coreLibrary)
	{
		Dart_Handle resolvedScriptUri;

		if (resolve)
		{
			resolvedScriptUri = __resolveScriptUri(scriptUri, coreLibrary);

			if (Dart_IsError(resolvedScriptUri))
			{
				return resolvedScriptUri;
			}
		}
		else
		{
			resolvedScriptUri = Dart_NewString(scriptUri);
		}

		Dart_Handle source = __readSource(resolvedScriptUri, coreLibrary);

		if (Dart_IsError(source))
		{
			return source;
		}

		return Dart_LoadScript(resolvedScriptUri, source);
	}

	//----------------------------------------------------------------------
	// Library tag handler
	//----------------------------------------------------------------------

	/**
	 * Handles loading libraries and scripts into the isolate.
	 *
	 * \param tag The type of tag received.
	 * \param library The library to load into.
	 * \param url The URL for the library.
	 */
	Dart_Handle __libraryTagHandler(Dart_LibraryTag tag, Dart_Handle library, Dart_Handle url)
	{
		const char* urlString = 0;
		Dart_Handle result = Dart_StringToCString(url, &urlString);

		if (Dart_IsError(result))
			return result;
		bool isDartScheme = __isDartSchemeUrl(urlString);

		if (tag == kCanonicalizeUrl)
		{
			if (isDartScheme)
				return url;

			// Check other libraries that were added
			std::int32_t hash = fnv1aHash(urlString);
			std::size_t count = __libraries.size();

			for (std::size_t i = 0; i < count; ++i)
			{
				ScriptLibrary* library = __libraries[i];

				if (hash == library->getHashedName())
					return url;
			}
		}
		else
		{
			// Check other libraries that were added
			std::int32_t hash = fnv1aHash(urlString);
			std::size_t count = __libraries.size();

			for (std::size_t i = 0; i < count; ++i)
			{
				ScriptLibrary* library = __libraries[i];

				if (hash == library->getHashedName())
					return library->load();
			}
		}

		return Dart_Error("Do not know how to load '%s'", urlString);
	}

} // end anonymous namespace

//----------------------------------------------------------------------
// Isolate methods
//----------------------------------------------------------------------

Isolate::Isolate(Dart_Isolate isolate, Dart_Handle library, Isolate* parent)
	: _isolate(isolate)
	, _library(library)
	, _parent(parent)
{
	__runningIsolates.push_back(this);
}

//----------------------------------------------------------------------

Isolate::~Isolate()
{
	//Dart_ExitScope();

	//Dart_EnterIsolate(_isolate);
	//Dart_ShutdownIsolate();
}

//----------------------------------------------------------------------

void Isolate::invokeFunction(const char* name)
{
	Dart_EnterScope();
	Dart_Handle result = Dart_Invoke(_library, Dart_NewString(name), 0, NULL);

	// Run the main loop
	// This should probably be somewhere else.
	// Wondering how requestAnimationFrame handles this situation.
	Dart_RunLoop();

	Dart_ExitScope();
}

//----------------------------------------------------------------------

Isolate* Isolate::loadScript(const char* path)
{
	char* error = 0;
	return createIsolate(path, "main", true, new IsolateData(), &error);
}

//----------------------------------------------------------------------

Isolate* Isolate::createIsolate(const char* scriptUri, const char* main, bool resolveScript, void* data, char** error)
{
	Dart_Isolate isolate = Dart_CreateIsolate(scriptUri, main, snapshot_buffer, data, error);

	if (isolate)
	{
		Dart_EnterScope();

		Dart_Handle result;
		result = Dart_SetLibraryTagHandler(__libraryTagHandler);

		if (Dart_IsError(result))
		{
			*error = strdup(Dart_GetError(result));
			Dart_ExitScope();
			Dart_ShutdownIsolate();
			return 0;
		}

		// Should an import map be created?

		// Setup URI library
		Dart_Handle uriLibrary = __uriLibrary->load();

		if (Dart_IsError(uriLibrary))
		{
			*error = strdup(Dart_GetError(result));
			Dart_ExitScope();
			Dart_ShutdownIsolate();
			return 0;
		}

		// Setup core builtin library
		Dart_Handle coreLibrary = __coreLibrary->load();

		if (Dart_IsError(coreLibrary))
		{
			*error = strdup(Dart_GetError(result));
			Dart_ExitScope();
			Dart_ShutdownIsolate();
			return 0;
		}

		// Setup IO library
		Dart_Handle ioLibrary = __ioLibrary->load();

		if (Dart_IsError(ioLibrary))
		{
			*error = strdup(Dart_GetError(result));
			Dart_ExitScope();
			Dart_ShutdownIsolate();
			return 0;
		}

		// Load the script into the isolate
		Dart_Handle library = __loadScript(scriptUri, resolveScript, coreLibrary);

		if (Dart_IsError(library))
		{
			*error = strdup(Dart_GetError(library));
			Dart_ExitScope();
			Dart_ShutdownIsolate();
			return 0;
		}

		// Implicitly import the core library
		// It isn't clear why this seems to be the only library
		// that requires this import call
		result = Dart_LibraryImportLibrary(library, coreLibrary);

		if (Dart_IsError(result))
		{
			*error = strdup(Dart_GetError(library));
			Dart_ExitScope();
			Dart_ShutdownIsolate();
			return 0;
		}

		//Dart_ExitScope();

		return new Isolate(isolate, library, static_cast<Isolate*>(data));
	}

	return 0;
}

//----------------------------------------------------------------------

bool Isolate::isolateCreateCallback(const char* scriptUri, const char* main, void* callbackData, char** error)
{
	Isolate* isolate = createIsolate(scriptUri, main, true, new IsolateData(), error);

	// See if the isolate was created successfully
	return isolate != 0;
}

//----------------------------------------------------------------------

void Isolate::isolateShutdownCallback(void* callbackData)
{
	
}

//----------------------------------------------------------------------
// Virtual machine methods
//----------------------------------------------------------------------

bool VirtualMachine::initialize()
{
	if (!__initialized)
	{
		if (Dart_SetVMFlags(0, 0))
		{
			if (Dart_Initialize(Isolate::isolateCreateCallback, 0, Isolate::isolateShutdownCallback))
			{
				// Setup the core libraries
				__coreLibrary   = BuiltinLibraries::createCoreLibrary();
				__ioLibrary     = BuiltinLibraries::createIOLibrary();
				__jsonLibrary   = BuiltinLibraries::createJsonLibrary();
				__uriLibrary    = BuiltinLibraries::createUriLibrary();
				__cryptoLibrary = BuiltinLibraries::createCryptoLibrary();
				__utfLibrary    = BuiltinLibraries::createUtfLibrary();

				// Get the current directory
				std::int32_t length = GetCurrentDirectory(0, 0);
				__currentDirectory = new char[length];
				GetCurrentDirectory(length + 1, __currentDirectory);

				__initialized = true;
			}
		}
	}

	return __initialized;
}

//----------------------------------------------------------------------

void VirtualMachine::terminate()
{
	if (__initialized)
	{
		// Shutdown the libraries
		delete __coreLibrary;
		delete __ioLibrary;
		delete __jsonLibrary;
		delete __uriLibrary;
		delete __cryptoLibrary;
		delete __utfLibrary;

		std::size_t count = __libraries.size();

		for (std::size_t i = 0; i < count; ++i)
			delete __libraries[i];

		__libraries.clear();
	}

	__initialized = false;
}

//----------------------------------------------------------------------

void VirtualMachine::loadScriptLibrary(
	const char* name,
	const char* source,
	Dart_NativeEntryResolver nativeResolver,
	Dart_LibraryInitializer initializer)
{
	ScriptLibrary* library = new ScriptLibrary(name, source, nativeResolver, initializer);

	__libraries.push_back(library);
}

//----------------------------------------------------------------------

std::size_t VirtualMachine::getNumberOfIsolates()
{
	return __runningIsolates.size();
}
