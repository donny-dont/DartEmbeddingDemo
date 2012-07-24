/**
 * \file Interpreter.hpp
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

#ifndef DART_EMBED_INTERPRETER_HPP_INCLUDED
#define DART_EMBED_INTERPRETER_HPP_INCLUDED

#include <vector>

//----------------------------------------------------------------------
// Forward declarations
//----------------------------------------------------------------------

typedef struct _Dart_Handle* Dart_Handle;
typedef struct _Dart_Isolate* Dart_Isolate;

namespace DartEmbed
{
	class ScriptLibrary;

	/**
	 * Embedded Dart interpreter.
	 */
	class Interpreter
	{
		public:

			/**
			 * Creates an instance of the Dart interpreter.
			 */
			Interpreter();

			/**
			 * Destroys an instance of the Dart interpreter.
			 */
			~Interpreter();

			/**
			 * Loads a library.
			 *
			 * \param library The library to load.
			 */
			void loadLibrary(ScriptLibrary* library);

			/**
			 * Invokes the main method within the given library.
			 *
			 * \param name The name of the library.
			 */
			void invokeMain(const char* name);

		private:

			/**
			 * Gets the library with the given name.
			 *
			 * \param name The name of the library.
			 * \returns The library with the given name.
			 */
			ScriptLibrary* getLibrary(const char* name) const;

			/// Handle for the library to load into
			Dart_Handle _library;
			/// Main thread of execution for the interpreter
			Dart_Isolate _isolate;
			/// Loaded libraries
			std::vector<ScriptLibrary*> _libraries;

	} ; // class Interpreter

} // end namespace DartEmbed

#endif // end DART_EMBED_INTERPRETER_HPP_INCLUDED
