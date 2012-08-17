/**
 * \file VirtualMachine.hpp
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

#ifndef DART_EMBED_VIRTUAL_MACHINE_HPP_INCLUDED
#define DART_EMBED_VIRTUAL_MACHINE_HPP_INCLUDED

#include <cstdio>
#include <cstdint>

//---------------------------------------------------------------------
// Forward declarations for Dart types
//---------------------------------------------------------------------

/// Reference to a Dart isolate
typedef struct _Dart_Isolate* Dart_Isolate;
/// Reference to a Dart object
typedef struct _Dart_Handle* Dart_Handle;
/// Native arguments for Dart
typedef struct _Dart_NativeArguments* Dart_NativeArguments;
/// Glue for communication between Dart and C/C++
typedef void (*Dart_NativeFunction)(Dart_NativeArguments arguments);
/// Resolves a name from a string into a Dart_NativeFunction
typedef Dart_NativeFunction (*Dart_NativeEntryResolver)(Dart_Handle name, int num_of_arguments);
/// Initialization routines for a library
typedef void (*Dart_LibraryInitializer)(Dart_Handle library);

namespace DartEmbed
{
	/**
	 * Embedded Dart virtual machine.
	 */
	class VirtualMachine
	{
		public:
			/**
			 * Attempts to initialize the virtual machine.
			 *
			 * \returns true if the virtual machine was initialized; false otherwise.
			 */
			static bool initialize();

			/**
			 * Terminates the virtual machine.
			 *
			 * Halts any currently running isolates.
			 */
			static void terminate();

			/**
			 * Loads a script library into the virtual machine.
			 *
			 * \param name The name of the library.
			 * \param source The source code for the library.
			 * \param nativeResolver The native resolver for the library.
			 * \param initializer Function to call that will provide any initialization for the library.
			 */
			static void loadScriptLibrary(
				const char* name,
				const char* source,
				Dart_NativeEntryResolver nativeResolver = 0,
				Dart_LibraryInitializer initializer = 0
			);

			/**
			 * Queries the number of isolates currently running.
			 *
			 * \returns The number of isolates currently running.
			 */
			static std::size_t getNumberOfIsolates();

		private:

			VirtualMachine() { }

	} ; // end namespace VirtualMachine

} // end namespace DartEmbed

#endif // end DART_EMBED_VIRTUAL_MACHINE_HPP_INCLUDED
