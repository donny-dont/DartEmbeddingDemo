/**
 * \file BuiltinLibraries.hpp
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

#ifndef DART_EMBED_BUILTIN_LIBRARIES_HPP_INCLUDED
#define DART_EMBED_BUILTIN_LIBRARIES_HPP_INCLUDED

//---------------------------------------------------------------------
// Defines taken from builtin.h
//
// Used to reference the native entries within the VM.
//---------------------------------------------------------------------

#define FUNCTION_NAME(name) Builtin_##name
#define DECLARE_FUNCTION(name, count)  \
	extern void FUNCTION_NAME(name)(Dart_NativeArguments args);

namespace DartEmbed
{
	//---------------------------------------------------------------------
	// Forward declarations
	//---------------------------------------------------------------------

	class ScriptLibrary;

	/**
	 * Creates the libraries bundled with Dart.
	 */
	namespace BuiltinLibraries
	{

		/**
		 * Creates the dart:builtin library.
		 *
		 * \returns The dart:builtin library.
		 */
		ScriptLibrary* createCoreLibrary();

		/**
		 * Creates the dart:io library.
		 *
		 * \returns The dart:io library.
		 */
		ScriptLibrary* createIOLibrary();

		/**
		 * Creates the dart:json library.
		 *
		 * \returns The dart:json library.
		 */
		ScriptLibrary* createJsonLibrary();

		/**
		 * Creates the dart:uri library.
		 *
		 * \returns The dart:uri library.
		 */
		ScriptLibrary* createUriLibrary();

		/**
		 * Creates the dart:crypto library.
		 *
		 * \returns The dart:crypto library.
		 */
		ScriptLibrary* createCryptoLibrary();

		/**
		 * Creates the dart:utf library.
		 *
		 * \returns The dart:utf library.
		 */
		ScriptLibrary* createUtfLibrary();

	} // end namespace BuitlinLibraries

} // end namespace DartEmbed

#endif // end DART_EMBED_BUILTIN_LIBRARIES_HPP_INCLUDED
