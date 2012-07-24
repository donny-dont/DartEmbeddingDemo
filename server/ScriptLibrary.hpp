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

#include "Hash.hpp"

//----------------------------------------------------------------------
// Forward declarations
//----------------------------------------------------------------------

typedef struct _Dart_Handle* Dart_Handle;
typedef struct _Dart_NativeArguments* Dart_NativeArguments;
typedef void (*Dart_NativeFunction)(Dart_NativeArguments);
typedef Dart_NativeFunction (*Dart_NativeEntryResolver)(Dart_Handle, int);

namespace DartEmbed
{
	class Interpreter;

	/**
	 * Contains a library to load into the interpreter.
	 */
	class ScriptLibrary
	{
		protected:

			/**
			 * Creates an instance of the ScriptLibrary class.
			 */
			ScriptLibrary();

		public:

			static ScriptLibrary* fromSource(const char* name, const char* source);

			/**
			 * Queries whether the library is loaded.
			 *
			 * \returns Whether the library is loaded.
			 */
			inline bool isLoaded() const
			{
				return _library == 0;
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
			 * Gets the hashed name of the library.
			 *
			 * \returns The hashed name of the library.
			 */
			inline std::int32_t getHashedName() const
			{
				return _hashedName;
			}

			/**
			 * Gets the source code for the library.
			 *
			 * \returns The source code for the library.
			 */
			inline const char* getSource() const
			{
				return _source;
			}

			/**
			 * Gets the handle to the native resolver.
			 *
			 * If this value is null then the library is pure Dart
			 * code.
			 *
			 * \returns The handle to the native resolver.
			 */
			inline Dart_NativeEntryResolver getNativeResolver() const
			{
				return _resolver;
			}

		protected:

			/**
			 * Sets the name of the library.
			 *
			 * \param name The name of the library.
			 */
			inline void setName(const char* name)
			{
				_name = name;
				_hashedName = fnv1aHash(_name);
			}

			/**
			 * Sets the source code for the library.
			 *
			 * \param source The source code for the library.
			 */
			inline void setSource(const char* source)
			{
				_source = source;
			}

			/**
			 * Sets the native resolver for the library.
			 *
			 * \param resolver The native resolver for the library.
			 */
			inline void setNativeResolver(Dart_NativeEntryResolver resolver)
			{
				_resolver = resolver;
			}

		private:

			inline Dart_Handle getLibrary() const
			{
				return _library;
			}

			inline void setLibrary(Dart_Handle library)
			{
				_library = library;
			}

		private:

			friend class Interpreter;

			/// Handle to the library
			Dart_Handle _library;
			/// The name of the library
			const char* _name;
			/// The hashed name of the library
			std::int32_t _hashedName;
			/// The source code for the library
			const char* _source;
			/// The native resolver for the library
			Dart_NativeEntryResolver _resolver;

	} ; // end class ScriptLibrary

} // end namespace DartEmbed

#endif // end DART_EMBED_SCRIPT_LIBRARY_HPP_INCLUDED
