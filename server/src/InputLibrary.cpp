/**
 * \file InputLibrary.cpp
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

#include "EmbedLibraries.hpp"
#include <DartEmbed/GamePad.hpp>
#include <DartEmbed/VirtualMachine.hpp>
#include "Arguments.hpp"
#include "ScriptLibrary.hpp"
#include "NativeResolution.hpp"
using namespace DartEmbed;

namespace
{
	//---------------------------------------------------------------------
	// Source code
	//---------------------------------------------------------------------

	const char* __sourceCode =
		"#library('embed:input');\n"
		"#import('dart:nativewrappers');\n"
		"\n"
		"class GamePadState extends NativeFieldWrapperClass1\n"
		"{\n"
		"  GamePadState() { _initialize(); }\n"
		"  void _initialize() native 'GamePadState_New';\n"
		"  bool get isConnected() native 'GamePadState_IsConnected';\n"
		"  double get leftThumbstickX() native 'GamePadState_GetLeftThumbstickX';\n"
		"  double get leftThumbstickY() native 'GamePadState_GetLeftThumbstickY';\n"
		"  double get rightThumbstickX() native 'GamePadState_GetRightThumbstickX';\n"
		"  double get rightThumbstickY() native 'GamePadState_GetRightThumbstickY';\n"
		"  double get leftTrigger() native 'GamePadState_GetLeftTrigger';\n"
		"  double get rightTrigger() native 'GamePadState_GetRightTrigger';\n"
		"  int get buttons() native 'GamePadState_GetButtons';\n"
		"}\n"
		"\n"
		"class GamePad\n"
		"{\n"
		"  static void getState(int index, GamePadState state) native 'GamePad_GetState';\n"
		"  static void setVibration(int index, double leftMotor, double rightMotor) native 'GamePad_SetVibration';\n"
		"}\n";

	//---------------------------------------------------------------------
	// Native functions
	//---------------------------------------------------------------------

	inline PlayerIndex::Enum __getPlayerIndex(Dart_NativeArguments args, int index)
	{
		std::int32_t player;
		getValue(args, 0, &player);

		if ((player < 0) || (player >= PlayerIndex::Size))
			return PlayerIndex::One;
		else
			return static_cast<PlayerIndex::Enum>(player);
	}

	void GamePad_GetState(Dart_NativeArguments args)
	{
		PlayerIndex::Enum index = __getPlayerIndex(args, 0);

		GamePadState* state;
		getNativeField(args, 1, &state);

		*state = GamePad::getState(index);
	}

	void GamePad_SetVibration(Dart_NativeArguments args)
	{
		PlayerIndex::Enum index = __getPlayerIndex(args, 0);

		float leftMotor;
		getValue(args, 1, &leftMotor);

		float rightMotor;
		getValue(args, 2, &rightMotor);

		GamePad::setVibration(index, leftMotor, rightMotor);
	}

	void GamePadState_Delete(Dart_Handle handle, void* data)
	{
		GamePadState* state = static_cast<GamePadState*>(data);
		delete state;
	}

	void GamePadState_New(Dart_NativeArguments args)
	{
		Dart_Handle instance = Dart_GetNativeArgument(args, 0);

		GamePadState* state = new GamePadState();
		Dart_SetNativeInstanceField(instance, 0, reinterpret_cast<intptr_t>(state));

		Dart_NewWeakPersistentHandle(instance, state, GamePadState_Delete);
	}

	void GamePadState_IsConnected(Dart_NativeArguments args)
	{
		GamePadState* state = 0;
		getNativeField(args, 0, &state);

		Dart_SetReturnValue(args, Dart_NewBoolean(state->isConnected()));
	}

	void GamePadState_GetLeftThumbstickX(Dart_NativeArguments args)
	{
		GamePadState* state = 0;
		getNativeField(args, 0, &state);

		Dart_SetReturnValue(args, Dart_NewDouble(state->getLeftThumbstickX()));
	}

	void GamePadState_GetLeftThumbstickY(Dart_NativeArguments args)
	{
		GamePadState* state = 0;
		getNativeField(args, 0, &state);

		Dart_SetReturnValue(args, Dart_NewDouble(state->getLeftThumbstickY()));
	}

	void GamePadState_GetRightThumbstickX(Dart_NativeArguments args)
	{
		GamePadState* state = 0;
		getNativeField(args, 0, &state);

		Dart_SetReturnValue(args, Dart_NewDouble(state->getRightThumbstickX()));
	}

	void GamePadState_GetRightThumbstickY(Dart_NativeArguments args)
	{
		GamePadState* state = 0;
		getNativeField(args, 0, &state);

		Dart_SetReturnValue(args, Dart_NewDouble(state->getRightThumbstickY()));
	}

	void GamePadState_GetLeftTrigger(Dart_NativeArguments args)
	{
		GamePadState* state = 0;
		getNativeField(args, 0, &state);

		Dart_SetReturnValue(args, Dart_NewDouble(state->getLeftTrigger()));
	}

	void GamePadState_GetRightTrigger(Dart_NativeArguments args)
	{
		GamePadState* state = 0;
		getNativeField(args, 0, &state);

		Dart_SetReturnValue(args, Dart_NewDouble(state->getRightTrigger()));
	}

	void GamePadState_GetButtons(Dart_NativeArguments args)
	{
		GamePadState* state = 0;
		getNativeField(args, 0, &state);

		Dart_SetReturnValue(args, Dart_NewInteger(state->getButtons()));
	}

	//---------------------------------------------------------------------
	// Virtual machine entries
	//---------------------------------------------------------------------

	/// Whether the library has been initialized
	bool __libraryInitialized = false;
	/// Class entries for the input library
	NativeClassEntry __libraryEntries[2];

	/// Native entries for the GamePad class
	NativeEntry __gamePadNativeEntries[3];
	/// Native entries for the GamePadState class
	NativeEntry __gamePadStateNativeEntries[10];

	/**
	 * Setup hooks to the GamePad class entries.
	 */
	void __setupGamePadEntries()
	{
		setNativeEntry(&__gamePadNativeEntries[0], "GetState",     GamePad_GetState,     2);
		setNativeEntry(&__gamePadNativeEntries[1], "SetVibration", GamePad_SetVibration, 3);
		// Set the sentinal value
		setNativeEntry(&__gamePadNativeEntries[2], "", 0, 0);
	}

	/**
	 * Setup hooks to the GamePadState class entries.
	 */
	void __setupGamePadStateEntries()
	{
		setNativeEntry(&__gamePadStateNativeEntries[0], "New",                 GamePadState_New,                 1);
		setNativeEntry(&__gamePadStateNativeEntries[1], "IsConnected",         GamePadState_IsConnected,         1);
		setNativeEntry(&__gamePadStateNativeEntries[2], "GetLeftThumbstickX",  GamePadState_GetLeftThumbstickX,  1);
		setNativeEntry(&__gamePadStateNativeEntries[3], "GetLeftThumbstickY",  GamePadState_GetLeftThumbstickY,  1);
		setNativeEntry(&__gamePadStateNativeEntries[4], "GetRightThumbstickX", GamePadState_GetRightThumbstickX, 1);
		setNativeEntry(&__gamePadStateNativeEntries[5], "GetRightThumbstickY", GamePadState_GetRightThumbstickY, 1);
		setNativeEntry(&__gamePadStateNativeEntries[6], "GetLeftTrigger",      GamePadState_GetLeftTrigger,      1);
		setNativeEntry(&__gamePadStateNativeEntries[7], "GetRightTrigger",     GamePadState_GetRightTrigger,     1);
		setNativeEntry(&__gamePadStateNativeEntries[8], "GetButtons",          GamePadState_GetButtons,          1);
		// Set the sentinal value
		setNativeEntry(&__gamePadStateNativeEntries[9], "", 0, 0);
	}

	/**
	 * Setup the class entries.
	 */
	void __setupClassEntries()
	{
		setNativeClassEntry(&__libraryEntries[0], "GamePad",      __gamePadNativeEntries);
		setNativeClassEntry(&__libraryEntries[1], "GamePadState", __gamePadStateNativeEntries);
		// Set the sentinal value
		setNativeClassEntry(&__libraryEntries[2], "", 0);
	}

	/**
	 * Sets up the native entries for the input library.
	 */
	void __setupInputLibrary()
	{
		if (!__libraryInitialized)
		{
			__setupClassEntries();

			__setupGamePadEntries();
			__setupGamePadStateEntries();
		}

		__libraryInitialized = true;
	}

	/**
	 * Native resolver for the dart:io library.
	 */
	//CREATE_NATIVE_RESOLVER(__inputLibraryResolver, __libraryEntries)
	Dart_NativeFunction __inputLibraryResolver(Dart_Handle name, int argumentCount)
	{
		const char* nativeFunctionName = 0;
		Dart_Handle result = Dart_StringToCString(name, &nativeFunctionName);

		assert(nativeFunctionName);

		printf("Resolving: %s\n", nativeFunctionName);

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
} // end anonymous namespace

//---------------------------------------------------------------------

void EmbedLibraries::createInputLibrary()
{
	// Setup the native entries
	__setupInputLibrary();

	VirtualMachine::loadScriptLibrary("embed:input", __sourceCode, __inputLibraryResolver);
}
