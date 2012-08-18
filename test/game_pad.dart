/**
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

class GamePadState
{
  bool isConnected;
  double leftThumbstickX;
  double leftThumbstickY;
  double rightThumbstickX;
  double rightThumbstickY;
  double leftTrigger;
  double rightTrigger;
  int buttons;

  GamePadState()
    : isConnected = false
    , leftThumbstickX = 0.0
    , leftThumbstickY = 0.0
    , rightThumbstickX = 0.0
    , rightThumbstickY = 0.0
    , leftTrigger = 0.0
    , rightTrigger = 0.0
    , buttons = 0;

  void cloneTo(GamePadState copy)
  {
    copy.isConnected = isConnected;
    copy.leftThumbstickX = leftThumbstickX;
    copy.leftThumbstickY = leftThumbstickY;
    copy.rightThumbstickX = rightThumbstickX;
    copy.rightThumbstickY = rightThumbstickY;
    copy.leftTrigger = leftTrigger;
    copy.rightTrigger = rightTrigger;
    copy.buttons = buttons;
  }
}

class GamePad
{
  /// List of game pads
  static List<GamePadState> _gamePads;

  static void onInitialize()
  {
    _gamePads = new List<GamePadState>();

    GamePadState gamePad;

    // Create player 1
    gamePad = new GamePadState();
    gamePad.isConnected = true;
    gamePad.leftThumbstickX = 0.5;
    gamePad.leftThumbstickY = 0.1;
    gamePad.rightThumbstickX = -0.2;
    gamePad.rightThumbstickY = -0.6;
    gamePad.leftTrigger = 0.33;
    gamePad.rightTrigger = 0.25;
    gamePad.buttons = 12288;

    _gamePads.add(gamePad);

    // Create player 2
    gamePad = new GamePadState();
    gamePad.isConnected = true;
    gamePad.leftThumbstickX = 0.5;
    gamePad.leftThumbstickY = -0.3;
    gamePad.rightThumbstickX = -0.8;
    gamePad.rightThumbstickY = -0.6;
    gamePad.leftTrigger = 0.5;
    gamePad.rightTrigger = 0.75;
    gamePad.buttons = 13;

    _gamePads.add(gamePad);

    // Create player 3
    gamePad = new GamePadState();
    gamePad.isConnected = true;
    gamePad.leftThumbstickX = -0.5;
    gamePad.leftThumbstickY = -0.3;
    gamePad.rightThumbstickX = 0.8;
    gamePad.rightThumbstickY = -0.2;
    gamePad.leftTrigger = 0.2;
    gamePad.rightTrigger = 0.95;
    gamePad.buttons = 63;

    _gamePads.add(gamePad);

    // Create player 4
    gamePad = new GamePadState();
    gamePad.isConnected = true;
    gamePad.leftThumbstickX = 0.33;
    gamePad.leftThumbstickY = -0.40;
    gamePad.rightThumbstickX = -0.1;
    gamePad.rightThumbstickY = -0.25;
    gamePad.leftTrigger = 0.8;
    gamePad.rightTrigger = 0.4;
    gamePad.buttons = 15;

    _gamePads.add(gamePad);
  }

  static void getState(int index, GamePadState state)
  {
    _gamePads[index].cloneTo(state);
  }

  static void setVibration(int index, double leftMotor, double rightMotor)
  {

  }
}
