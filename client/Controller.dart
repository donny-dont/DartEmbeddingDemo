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

#import('dart:html');
#import('dart:json');
#source('game_pad.dart');
#source('game_pad_canvas.dart');

GamePadCanvas _canvas;

Element _selected;
bool _connected = false;
InputElement _url;
InputElement _connectButton;

void connectClicked(e)
{
  if (_connected)
  {
    // Disconnect
    GamePad.disconnectFromServer();
    
    _connectButton.value = "Connect";
  }
  else
  {
    // Connect
    GamePad.connectToServer(_url.value);
    
    _connectButton.value = "Disconnect";
  }

  _url.disabled = !_url.disabled;
  _connected = !_connected;
}

void setupTab(String id, int index)
{
  Element tab = document.query(id);
  
  void tabClicked(e)
  {
    _selected.classes.remove('selected');
    tab.classes.add('selected');
    
    _selected = tab;
    _canvas.playerIndex = index;
  }
  
  tab.on.click.add(tabClicked);
}

/**
 * Updates the canvas.
 */
bool update(int time)
{
  _canvas.draw();
  
  window.requestAnimationFrame(update);
}

void main()
{
  // Setup the gamepad class
  GamePad.onInitialize();
  // Setup the gamepad canvas
  _canvas = new GamePadCanvas('#gamepad');
  
  // Setup the tabbed UI
  _selected = document.query('#player1');
  setupTab('#player1', 0);
  setupTab('#player2', 1);
  setupTab('#player3', 2);
  setupTab('#player4', 3);
  
  // Setup the connection button
  _connectButton = document.query('#connect');
  _connectButton.on.click.add(connectClicked);
  _url = document.query('#url');
  
  // Start the canvas animation
  window.requestAnimationFrame(update);
}
