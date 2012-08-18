/**
 * \file NativeResolution.hpp
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

#ifndef DART_EMBED_NATIVE_RESOLUTION_HPP_INCLUDED
#define DART_EMBED_NATIVE_RESOLUTION_HPP_INCLUDED

#include <cstdint>
#include "dart_api.h"

namespace DartEmbed
{
	/**
	 * Maps the name of a function to the function pointer.
	 */
	struct NativeEntry
	{
		/// Hash containing the native entry name
		std::int32_t hash;
		/// Function pointer
		Dart_NativeFunction function;
	#ifdef _DEBUG
		/// Name of the native entry
		const char* name;
		/// Number of arguments
		std::int32_t argumentCount;
	#endif
	} ; // end struct NativeEntry

	/**
	 * Maps the name of a class to an array of associated functions.
	 */
	struct NativeClassEntry
	{
		/// Hash containing the native entry name
		std::int32_t hash;
		/// Pointer to an array of NativeEntries
		NativeEntry* entries;
	#ifdef _DEBUG
		/// Name of the class
		const char* name;
	#endif
	} ; // end struct NativeClassEntry

	/**
	 * Hash structure for a native call.
	 */
	struct NativeCallHash
	{
		/// Hash for the class name
		std::int32_t classHash;
		/// Hash for the function name
		std::int32_t functionHash;
	} ; // end struct NativeCallHash


	/**
	 * Implementation of the FNV1A hashing algorithm.
	 *
	 * \param str The string to hash.
	 * \returns The computed hash.
	 */
	inline std::int32_t fnv1aHash(const char* str)
	{
		std::int32_t hash = 2166136261u;

		while (*str != '\0')
		{
			hash ^= *str++;
			hash *= 16777619u;
		}

		return hash;
	}

	/**
	 * Hashes the function call.
	 *
	 * Assumes that the string is of the format className_functionName.
	 *
	 * \param str The string to hash.
	 * \param hash Structure containing the hash.
	 */
	inline void fnv1aHashFunctionCall(const char* str, NativeCallHash* hash)
	{
		std::int32_t classHash = 2166136261u;

		while (*str != '_')
		{
			classHash ^= *str++;
			classHash *= 16777619u;
		}

		// Advance past '_'
		str++;

		hash->classHash    = classHash;
		hash->functionHash = fnv1aHash(str);
	}

	/**
	 * Sets the fields of a NativeClassEntry.
	 *
	 * \param entry The NativeClassEntry to populate.
	 * \param name The name of the class.
	 * \param entries An array holding the class' native functions.
	 */
	inline void setNativeClassEntry(NativeClassEntry* entry, const char* name, NativeEntry* entries)
	{
		entry->hash = fnv1aHash(name);
		entry->entries = entries;

	#ifdef _DEBUG
		entry->name = name;
	#endif
	}

	/**
	 * Sets the fields of a NativeEntry.
	 *
	 * \param entry The NativeEntry to populate.
	 * \param name The name of the function.
	 * \param function The function pointer to associate to the name.
	 * \param argumentCount The number of arguments the function takes.
	 */
	inline void setNativeEntry(NativeEntry* entry, const char* name, Dart_NativeFunction function, std::int32_t argumentCount)
	{
		entry->hash = fnv1aHash(name);
		entry->function = function;

	#ifdef _DEBUG
		entry->name = name;
		entry->argumentCount = argumentCount;
	#endif
	}

	/**
	 * Macro to create a native resolver.
	 *
	 * Once constexpr is available this should change to a template. It
	 * could also be used as a default if a native resolver is passed a void*
	 * which would contain the NativeClassEntry array.
	 *
	 * \param functionName Function name to use for the resolver.
	 * \param nativeEntries Pointer to a NativeClassEntries array.
	 */
	#define CREATE_NATIVE_RESOLVER(functionName, libraryEntries) \
	Dart_NativeFunction functionName(Dart_Handle name, int argumentCount)      \
	{                                                                          \
		const char* nativeFunctionName = 0;                                    \
		Dart_Handle result = Dart_StringToCString(name, &nativeFunctionName);  \
		                                                                       \
		assert(nativeFunctionName);                                            \
		                                                                       \
		NativeCallHash hash;                                                   \
		fnv1aHashFunctionCall(nativeFunctionName, &hash);                      \
		                                                                       \
		NativeClassEntry* classEntry = libraryEntries;                         \
		                                                                       \
		while (classEntry->entries != 0)                                       \
		{                                                                      \
			NativeEntry* functionEntry = classEntry->entries;                  \
			                                                                   \
			if (classEntry->hash == hash.classHash)                            \
			{                                                                  \
				while (functionEntry->function != 0)                           \
				{                                                              \
					if (functionEntry->hash == hash.functionHash)              \
					{                                                          \
						assert(functionEntry->argumentCount == argumentCount); \
						return functionEntry->function;                        \
					}                                                          \
					                                                           \
					functionEntry++;                                           \
				}                                                              \
				                                                               \
				return 0;                                                      \
			}                                                                  \
			                                                                   \
			classEntry++;                                                      \
		}                                                                      \
		                                                                       \
		return 0;                                                              \
	}
} // end namespace DartEmbed

#endif // end DART_EMBED_NATIVE_RESOLUTION_HPP_INCLUDED
