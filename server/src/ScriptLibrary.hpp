/**
 * \file ScriptLibrary.hpp
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

#ifndef DART_EMBED_SCRIPT_LIBRARY_HPP_INCLUDED
#define DART_EMBED_SCRIPT_LIBRARY_HPP_INCLUDED

#include <DartEmbed/VirtualMachine.hpp>

namespace DartEmbed
{
	//---------------------------------------------------------------------
	// Forward declarations for Dart types
	//---------------------------------------------------------------------

	class Isolate;

	/**
	 *
	 */
	class ScriptLibrary
	{
		public:

			/**
			 * Creates an instance of the ScriptLibrary class.
			 *
			 * \param name The name of the library.
			 * \param source The source code for the library.
			 * \param nativeResolver The native resolver for the library.
			 * \param initializer Function to call that will provide any initialization for the library.
			 */
			ScriptLibrary(
				const char* name,
				const char* source,
				Dart_NativeEntryResolver nativeResolver = 0,
				Dart_LibraryInitializer initializer = 0
			);

		//---------------------------------------------------------------------
		// Properties
		//---------------------------------------------------------------------

		public:

			/**
			 * Gets the hashed name of the library.
			 *
			 * \returns The hashed name of the library.
			 */
			std::int32_t getHashedName() const
			{
				return _hashedName;
			}

			/**
			 * Gets the name of the library.
			 *
			 * \returns The name of the library.
			 */
			inline const char* getName() const
			{
				return _name;
			}

			/**
			 * Whether the library has native functions.
			 *
			 * \returns true if the library has native functions; false otherwise.
			 */
			inline bool hasNatives() const
			{
				return _nativeResolver != 0;
			}

		//---------------------------------------------------------------------
		// Class methods
		//---------------------------------------------------------------------

		public:

			/**
			 * Loads the library into the current isolate.
			 */
			Dart_Handle load();

		//---------------------------------------------------------------------
		// Member variables
		//---------------------------------------------------------------------

		private:

			friend class Isolate;

			/// Hashed name of the library
			std::int32_t _hashedName;
			/// The name of the library
			const char* _name;
			/// The source code for the library
			const char* _source;
			/// Native resolver for the library
			Dart_NativeEntryResolver _nativeResolver;
			/// Function call that provides any initialization for the library
			Dart_LibraryInitializer _initializer;
	} ; // end class ScriptLibrary
} // end namespace DartEmbed

#endif // end DART_EMBED_SCRIPT_LIBRARY_HPP_INCLUDED
