/**
 * \file Isolate.hpp
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

#ifndef DART_EMBED_ISOLATE_HPP_INCLUDED
#define DART_EMBED_ISOLATE_HPP_INCLUDED

#include <DartEmbed/VirtualMachine.hpp>

namespace DartEmbed
{
	class Isolate
	{
		//---------------------------------------------------------------------
		// Creation/Destruction
		//---------------------------------------------------------------------

		private:

			/**
			 * Creates an instance of the Isolate class.
			 */
			Isolate(
				Dart_Isolate isolate,
				Dart_Handle library,
				Isolate* parent
			);

		public:

			/**
			 * Destroys the current Isolate.
			 */
			~Isolate();

		//---------------------------------------------------------------------
		// Properties
		//---------------------------------------------------------------------

		public:

			/**
			 * Retrieves the parent of the isolate.
			 *
			 * The parent points to the isolate that started this
			 * isolate instance.
			 */
			inline Isolate* getParent() const
			{
				return _parent;
			}

		public:

			/**
			 * Invokes a function within the isolate.
			 *
			 * \param name The name of the function to invoke.
			 */
			void invokeFunction(const char* name);

			/**
			 * Creates an isolate from a script.
			 *
			 * \param path The path to the script.
			 */
			static Isolate* loadScript(const char* path);

		//---------------------------------------------------------------------
		// Static methods
		//
		// Used to spawn isolates within the VM.
		//---------------------------------------------------------------------

		private:

			/**
			 * Creates an isolate from the given script.
			 *
			 * \param scriptUri The URI to the script.
			 * \param main The entry point to the isolate.
			 * \param resolveScript Whether the script needs resoltion to a uri.
			 * \param callbackData Pointer to the parent isolate.
			 * \param error A pointer to an error message.
			 */
			static Isolate* createIsolate(
				const char* scriptUri,
				const char* main,
				bool resolveScript,
				void* data,
				char** error);

			/**
			 * Callback for when an isolate is created.
			 *
			 * This is only called when an isolate is created within a script. It
			 * will not be called when creating an isolate manually.
			 *
			 * \param scriptUri The URI to the script.
			 * \param main The entry point to the isolate.
			 * \param callbackData Pointer to the parent isolate.
			 * \param error A pointer to an error message.
			 */
			static bool isolateCreateCallback(
				const char* scriptUri,
				const char* main,
				void* callbackData,
				char** error);

			/**
			 * Callback for when an isolate is shut down.
			 *
			 * This is only called when an isolate that was created within a script
			 * was shutdown.
			 *
			 * \param callbackData A pointer to the isolate data.
			 */
			static void isolateShutdownCallback(void* callbackData);

		//---------------------------------------------------------------------
		// Member variables
		//---------------------------------------------------------------------

		private:

			friend class VirtualMachine;

			/// The isolate handle
			Dart_Isolate _isolate;
			/// The library held in the isolate
			Dart_Handle _library;
			/// The parent of the isolate
			Isolate* _parent;
	} ;
} // end namespace Dart

#endif // end DART_EMBED_ISOLATE_HPP_INCLUDED
