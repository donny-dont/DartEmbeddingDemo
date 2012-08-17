/**
 * \file GamePad.hpp
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

#include <DartEmbed/GamePad.hpp>
#include "PlatformWindows.hpp"
using namespace DartEmbed;

namespace
{
	/// The current state of the controllers
	GamePadState __gamePadState[PlayerIndex::Size];
} // end anonymous namespace

//----------------------------------------------------------------------

GamePadState::GamePadState()
: _connected(false)
, _packetNumber(0)
, _leftTrigger(0.0f)
, _rightTrigger(0.0f)
{ }

//----------------------------------------------------------------------

GamePadState::GamePadState(const GamePadState& copy)
: _connected(copy._connected)
, _packetNumber(copy._packetNumber)
, _leftTrigger(copy._leftTrigger)
, _rightTrigger(copy._rightTrigger)
{ }

//----------------------------------------------------------------------

GamePadState& GamePadState::operator= (const GamePadState& copy)
{
	_connected = copy._connected;
	_packetNumber = copy._packetNumber;

	_leftTrigger  = copy._leftTrigger;
	_rightTrigger = copy._rightTrigger;

	return *this;
}

//----------------------------------------------------------------------

const GamePadState& GamePad::getState(PlayerIndex::Enum player)
{
	return __gamePadState[player];
}

//----------------------------------------------------------------------

void GamePad::setVibration(PlayerIndex::Enum player, const float leftMotor, const float rightMotor)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed  = (std::uint16_t)(leftMotor  * 65535.0f);
	vibration.wRightMotorSpeed = (std::uint16_t)(rightMotor * 65535.0f);

	XInputSetState(player, &vibration);
}

//----------------------------------------------------------------------

void updateGamePads()
{
	DWORD result;

	for (DWORD i = 0; i < PlayerIndex::Size; ++i)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Get the state of the controller
		result = XInputGetState(i, &state);

		// Get the gamepad
		GamePadState& gamePad = __gamePadState[i];

		if (result == ERROR_SUCCESS)
		{
			gamePad.setConnected(true);
			std::int32_t packetNumber = state.dwPacketNumber;

			// See if the packet number has changed
			if (gamePad.getPacketNumber() != packetNumber)
			{
				gamePad.setPacketNumber(packetNumber);

				// Set the trigger
				gamePad.setLeftTrigger((float)state.Gamepad.bLeftTrigger / 255.0f);
				gamePad.setRightTrigger((float)state.Gamepad.bRightTrigger / 255.0f);
			}
		}
		else
		{
			// Zero out the values
			gamePad.setConnected(false);
			gamePad.setPacketNumber(0);

			// Set the trigger
			gamePad.setLeftTrigger(0.0f);
			gamePad.setRightTrigger(0.0f);
		}
	}
}
