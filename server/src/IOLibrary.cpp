/**
 * \file IOLibrary.cpp
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

#include "BuiltinLibraries.hpp"
#include "ScriptLibrary.hpp"
#include "NativeResolution.hpp"
using namespace DartEmbed;

//---------------------------------------------------------------------
// Directory functions
//---------------------------------------------------------------------

DECLARE_FUNCTION(Directory_Exists, 1)
DECLARE_FUNCTION(Directory_Create, 1)
DECLARE_FUNCTION(Directory_Current, 0)
DECLARE_FUNCTION(Directory_CreateTemp, 1)
DECLARE_FUNCTION(Directory_Delete, 2)
DECLARE_FUNCTION(Directory_Rename, 2)
DECLARE_FUNCTION(Directory_NewServicePort, 0)

//---------------------------------------------------------------------
// EventHandler functions
//---------------------------------------------------------------------

DECLARE_FUNCTION(EventHandler_Start, 1)                                                     \
DECLARE_FUNCTION(EventHandler_SendData, 4)

//---------------------------------------------------------------------
// File functions
//---------------------------------------------------------------------

DECLARE_FUNCTION(File_Open, 2)
DECLARE_FUNCTION(File_Exists, 1)
DECLARE_FUNCTION(File_Close, 1)
DECLARE_FUNCTION(File_ReadByte, 1)
DECLARE_FUNCTION(File_WriteByte, 2)
DECLARE_FUNCTION(File_WriteString, 2)
DECLARE_FUNCTION(File_ReadList, 4)
DECLARE_FUNCTION(File_WriteList, 4)
DECLARE_FUNCTION(File_Position, 1)
DECLARE_FUNCTION(File_SetPosition, 2)
DECLARE_FUNCTION(File_Truncate, 2)
DECLARE_FUNCTION(File_Length, 1)
DECLARE_FUNCTION(File_LengthFromName, 1)
DECLARE_FUNCTION(File_LastModified, 1)
DECLARE_FUNCTION(File_Flush, 1)
DECLARE_FUNCTION(File_Create, 1)
DECLARE_FUNCTION(File_Delete, 1)
DECLARE_FUNCTION(File_Directory, 1)
DECLARE_FUNCTION(File_FullPath, 1)
DECLARE_FUNCTION(File_OpenStdio, 1)
DECLARE_FUNCTION(File_GetStdioHandleType, 1)
DECLARE_FUNCTION(File_NewServicePort, 0)

//---------------------------------------------------------------------
// Platform functions
//---------------------------------------------------------------------

DECLARE_FUNCTION(Platform_NumberOfProcessors, 0)
DECLARE_FUNCTION(Platform_OperatingSystem, 0)
DECLARE_FUNCTION(Platform_PathSeparator, 0)
DECLARE_FUNCTION(Platform_LocalHostname, 0)
DECLARE_FUNCTION(Platform_Environment, 0)

//---------------------------------------------------------------------
// Process functions
//---------------------------------------------------------------------

DECLARE_FUNCTION(Process_Start, 10)
DECLARE_FUNCTION(Process_Kill, 3)

//---------------------------------------------------------------------
// ServerSocket functions
//---------------------------------------------------------------------

DECLARE_FUNCTION(ServerSocket_CreateBindListen, 4)
DECLARE_FUNCTION(ServerSocket_Accept, 2)

//---------------------------------------------------------------------
// Socket functions
//---------------------------------------------------------------------

DECLARE_FUNCTION(Socket_CreateConnect, 3)
DECLARE_FUNCTION(Socket_Available, 1)
DECLARE_FUNCTION(Socket_ReadList, 4)
DECLARE_FUNCTION(Socket_WriteList, 4)
DECLARE_FUNCTION(Socket_GetPort, 1)
DECLARE_FUNCTION(Socket_GetRemotePeer, 1)
DECLARE_FUNCTION(Socket_GetError, 1)
DECLARE_FUNCTION(Socket_GetStdioHandle, 2)
DECLARE_FUNCTION(Socket_NewServicePort, 0)

namespace
{
	/// Whether the library has been initialized
	bool __libraryInitialized = false;
	/// Class entries for the IO library
	NativeClassEntry __libraryEntries[8];

	/// Native entries for the Directory class
	NativeEntry __directoryNativeEntries[8];
	/// Native entries for the EventHandlerClass
	NativeEntry __eventHandlerNativeEntries[3];
	/// Native entries for the File class
	NativeEntry __fileNativeEntries[23];
	/// Native entries for the Platform class
	NativeEntry __platformNativeEntries[6];
	/// Native entries for the Process class
	NativeEntry __processNativeEntries[3];
	/// Native entries for the ServerSocket class
	NativeEntry __serverSocketNativeEntries[3];
	/// Native entries for the Socket class
	NativeEntry __socketNativeEntries[10];

	/**
	 * Setup hooks to the Directory class entries.
	 */
	void __setupDirectoryEntries()
	{
		setNativeEntry(&__directoryNativeEntries[0], "Exists",         FUNCTION_NAME(Directory_Exists),         1);
		setNativeEntry(&__directoryNativeEntries[1], "Create",         FUNCTION_NAME(Directory_Create),         1);
		setNativeEntry(&__directoryNativeEntries[2], "Current",        FUNCTION_NAME(Directory_Current),        0);
		setNativeEntry(&__directoryNativeEntries[3], "CreateTemp",     FUNCTION_NAME(Directory_CreateTemp),     1);
		setNativeEntry(&__directoryNativeEntries[4], "Delete",         FUNCTION_NAME(Directory_Delete),         2);
		setNativeEntry(&__directoryNativeEntries[5], "Rename",         FUNCTION_NAME(Directory_Rename),         2);
		setNativeEntry(&__directoryNativeEntries[6], "NewServicePort", FUNCTION_NAME(Directory_NewServicePort), 0);
		// Set the sentinal value
		setNativeEntry(&__directoryNativeEntries[7], "", 0, 0);
	}

	/**
	 * Setup hooks to the EventHandler class entries.
	 */
	 void __setupEventHandlerEntries()
	 {
		setNativeEntry(&__eventHandlerNativeEntries[0], "Start",    FUNCTION_NAME(EventHandler_Start), 1);
		setNativeEntry(&__eventHandlerNativeEntries[1], "SendData", FUNCTION_NAME(EventHandler_SendData), 4);
		// Set the sentinal value
		setNativeEntry(&__eventHandlerNativeEntries[2], "", 0, 0);
	}

	/**
	 * Setup hooks to the File class entries.
	 */
	void __setupFileEntries()
	{
		setNativeEntry(&__fileNativeEntries[ 0], "Open",               FUNCTION_NAME(File_Open),               2);
		setNativeEntry(&__fileNativeEntries[ 1], "Exists",             FUNCTION_NAME(File_Exists),             1);
		setNativeEntry(&__fileNativeEntries[ 2], "Close",              FUNCTION_NAME(File_Close),              1);
		setNativeEntry(&__fileNativeEntries[ 3], "ReadByte",           FUNCTION_NAME(File_ReadByte),           1);
		setNativeEntry(&__fileNativeEntries[ 4], "WriteByte",          FUNCTION_NAME(File_WriteByte),          2);
		setNativeEntry(&__fileNativeEntries[ 5], "WriteString",        FUNCTION_NAME(File_WriteString),        2);
		setNativeEntry(&__fileNativeEntries[ 6], "ReadList",           FUNCTION_NAME(File_ReadList),           4);
		setNativeEntry(&__fileNativeEntries[ 7], "WriteList",          FUNCTION_NAME(File_WriteList),          4);
		setNativeEntry(&__fileNativeEntries[ 8], "Position",           FUNCTION_NAME(File_Position),           1);
		setNativeEntry(&__fileNativeEntries[ 9], "SetPosition",        FUNCTION_NAME(File_SetPosition),        2);
		setNativeEntry(&__fileNativeEntries[10], "Truncate",           FUNCTION_NAME(File_Truncate),           2);
		setNativeEntry(&__fileNativeEntries[11], "Length",             FUNCTION_NAME(File_Length),             1);
		setNativeEntry(&__fileNativeEntries[12], "LengthFromName",     FUNCTION_NAME(File_LengthFromName),     1);
		setNativeEntry(&__fileNativeEntries[13], "LastModified",       FUNCTION_NAME(File_LastModified),       1);
		setNativeEntry(&__fileNativeEntries[14], "Flush",              FUNCTION_NAME(File_Flush),              1);
		setNativeEntry(&__fileNativeEntries[15], "Create",             FUNCTION_NAME(File_Create),             1);
		setNativeEntry(&__fileNativeEntries[16], "Delete",             FUNCTION_NAME(File_Delete),             1);
		setNativeEntry(&__fileNativeEntries[17], "Directory",          FUNCTION_NAME(File_Directory),          1);
		setNativeEntry(&__fileNativeEntries[18], "FullPath",           FUNCTION_NAME(File_FullPath),           1);
		setNativeEntry(&__fileNativeEntries[19], "OpenStdio",          FUNCTION_NAME(File_OpenStdio),          1);
		setNativeEntry(&__fileNativeEntries[20], "GetStdioHandleType", FUNCTION_NAME(File_GetStdioHandleType), 1);
		setNativeEntry(&__fileNativeEntries[21], "NewServicePort",     FUNCTION_NAME(File_NewServicePort),     0);
		// Set the sentinal value
		setNativeEntry(&__fileNativeEntries[22], "", 0, 0);
	}

	/**
	 * Setup hooks to the Platform class entries.
	 */
	void __setupPlatformEntries()
	{
		setNativeEntry(&__platformNativeEntries[0], "NumberOfProcessors", FUNCTION_NAME(Platform_NumberOfProcessors), 0);
		setNativeEntry(&__platformNativeEntries[1], "OperatingSystem",    FUNCTION_NAME(Platform_OperatingSystem), 0);
		setNativeEntry(&__platformNativeEntries[2], "PathSeparator",      FUNCTION_NAME(Platform_PathSeparator), 0);
		setNativeEntry(&__platformNativeEntries[3], "LocalHostname",      FUNCTION_NAME(Platform_LocalHostname), 0);
		setNativeEntry(&__platformNativeEntries[4], "Environment",        FUNCTION_NAME(Platform_Environment), 0);
		// Set the sentinal value
		setNativeEntry(&__platformNativeEntries[5], "", 0, 0);
	}

	/**
	 * Setup hooks to the Process class entries.
	 */
	void __setupProcessEntries()
	{
		setNativeEntry(&__processNativeEntries[0], "Start", FUNCTION_NAME(Process_Start), 10);
		setNativeEntry(&__processNativeEntries[1], "Kill",  FUNCTION_NAME(Process_Kill),   3);
		// Set the sentinal value
		setNativeEntry(&__processNativeEntries[2], "", 0, 0);
	}

	/**
	 * Setup hooks to the ServerSocket class entries.
	 */
	void __setupServerSocketEntries()
	{
		setNativeEntry(&__serverSocketNativeEntries[0], "CreateBindListen", FUNCTION_NAME(ServerSocket_CreateBindListen), 4);
		setNativeEntry(&__serverSocketNativeEntries[1], "Accept",           FUNCTION_NAME(ServerSocket_Accept),           2);
		// Set the sentinal value
		setNativeEntry(&__serverSocketNativeEntries[2], "", 0, 0);
	}

	/**
	 * Setup hooks to the Socket class entries.
	 */
	void __setupSocketEntries()
	{
		setNativeEntry(&__socketNativeEntries[0], "CreateConnect",  FUNCTION_NAME(Socket_CreateConnect),  3);
		setNativeEntry(&__socketNativeEntries[1], "Available",      FUNCTION_NAME(Socket_Available),      1);
		setNativeEntry(&__socketNativeEntries[2], "ReadList",       FUNCTION_NAME(Socket_ReadList),       4);
		setNativeEntry(&__socketNativeEntries[3], "WriteList",      FUNCTION_NAME(Socket_WriteList),      4);
		setNativeEntry(&__socketNativeEntries[4], "GetPort",        FUNCTION_NAME(Socket_GetPort),        1);
		setNativeEntry(&__socketNativeEntries[5], "GetRemotePeer",  FUNCTION_NAME(Socket_GetRemotePeer),  1);
		setNativeEntry(&__socketNativeEntries[6], "GetError",       FUNCTION_NAME(Socket_GetError),       1);
		setNativeEntry(&__socketNativeEntries[7], "GetStdioHandle", FUNCTION_NAME(Socket_GetStdioHandle), 2);
		setNativeEntry(&__socketNativeEntries[8], "NewServicePort", FUNCTION_NAME(Socket_NewServicePort), 0);
		// Set the sentinal value
		setNativeEntry(&__socketNativeEntries[9], "", 0, 0);
	}

	/**
	 * Setup the class entries.
	 */
	void __setupClassEntries()
	{
		setNativeClassEntry(&__libraryEntries[0], "Directory",    __directoryNativeEntries);
		setNativeClassEntry(&__libraryEntries[1], "EventHandler", __eventHandlerNativeEntries);
		setNativeClassEntry(&__libraryEntries[2], "File",         __fileNativeEntries);
		setNativeClassEntry(&__libraryEntries[3], "Platform",     __platformNativeEntries);
		setNativeClassEntry(&__libraryEntries[4], "Process",      __processNativeEntries);
		setNativeClassEntry(&__libraryEntries[5], "ServerSocket", __serverSocketNativeEntries);
		setNativeClassEntry(&__libraryEntries[6], "Socket",       __socketNativeEntries);
		// Set the sentinal value
		setNativeClassEntry(&__libraryEntries[7], "", 0);
	}

	/**
	 * Sets up the native entries for the IO library.
	 */
	void __setupIOLibrary()
	{
		if (!__libraryInitialized)
		{
			__setupClassEntries();

			__setupDirectoryEntries();
			__setupEventHandlerEntries();
			__setupFileEntries();
			__setupPlatformEntries();
			__setupProcessEntries();
			__setupServerSocketEntries();
			__setupSocketEntries();
		}

		__libraryInitialized = true;
	}

	/**
	 * Native resolver for the dart:io library.
	 */
	//CREATE_NATIVE_RESOLVER(__ioLibraryResolver, __libraryEntries)
	Dart_NativeFunction __ioLibraryResolver(Dart_Handle name, int argumentCount)
	{
		const char* nativeFunctionName = 0;
		Dart_Handle result = Dart_StringToCString(name, &nativeFunctionName);

		assert(nativeFunctionName);

		NativeCallHash hash;
		fnv1aHashFunctionCall(nativeFunctionName, &hash);

		NativeClassEntry* classEntry = __libraryEntries;

		while (classEntry->entries != 0)
		{
			NativeEntry* functionEntry = classEntry->entries;

			if (classEntry->hash == hash.classHash)
			{
				while (functionEntry->function != 0)
				{
					if (functionEntry->hash == hash.functionHash)
					{
						assert(functionEntry->argumentCount == argumentCount);
						return functionEntry->function;
					}

					functionEntry++;
				}

				return 0;
			}

			classEntry++;
		}

		return 0;
	}

	/**
	 * Initialize the dart:io library.
	 *
	 * \param library The IO library to initialize.
	 */
	void __ioLibraryInitializer(Dart_Handle library)
	{
		Dart_Handle timerClosure = Dart_Invoke(library, Dart_NewString("_getTimerFactoryClosure"), 0, 0);
		Dart_Handle isolateLibrary = Dart_LookupLibrary(Dart_NewString("dart:isolate"));

		Dart_Handle args[1];
		args[0] = timerClosure;
		Dart_Handle result = Dart_Invoke(isolateLibrary, Dart_NewString("_setTimerFactoryClosure"), 1, args);
	}
} // end anonymous namespace

//---------------------------------------------------------------------

ScriptLibrary* BuiltinLibraries::createIOLibrary()
{
	// Setup the native entries
	__setupIOLibrary();

	// The library source is compiled in along with the snapshot.
	// Native entries are present within the library so a resolver needs to be set.
	// IO initializer allows some functions to be called before the library is used.
	return new ScriptLibrary("dart:io", 0, __ioLibraryResolver, __ioLibraryInitializer);
}
