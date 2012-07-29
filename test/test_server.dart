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

#import('dart:io');
#import('dart:json');
#import('dart:isolate');

List<String> gamePadStates;

class GamePad
{
  bool connected;
  double leftThumbstickX;
  double leftThumbstickY;
  double rightThumbstickX;
  double rightThumbstickY;
  double leftTrigger;
  double rightTrigger;
  int buttons;
  
  GamePad()
    : connected = false
    , leftThumbstickX = 0.0
    , leftThumbstickY = 0.0
    , rightThumbstickX = 0.0
    , rightThumbstickY = 0.0
    , leftTrigger = 0.0
    , rightTrigger = 0.0
    , buttons = 0;
  
  String toJSON(int index)
  {
    return
      """
{
    "index": $index,
    "connected": $connected,
    "leftThumbstick": {
        "x": $leftThumbstickX,
        "y": $leftThumbstickY
    },
    "rightThumbstick": {
        "x": $rightThumbstickX,
        "y": $rightThumbstickY
    },
    "leftTrigger": $leftTrigger,
    "rightTrigger": $rightTrigger,
    "buttons": $buttons
}      
      """;
  }
}

List<GamePad> _gamePads;

void handleConnection(WebSocketConnection connection)
{
  print('New connection');
  bool connected = true;
  int playerIndex = 0;

  connection.onMessage = (message) {
    // Parse the message
    Map parsed = JSON.parse(message);
    
    // Get the standard values
    String type = parsed['type'];
    int index = parsed['index'];
    
    if (type == 'index')
    {
      // Start sending this game pad data
      playerIndex = index;
      
      print('Request $index');
    }
    else if (type == 'vibration')
    {
      double leftMotor = parsed['leftMotor'];
      double rightMotor = parsed['rightMotor'];
      
      print('Vibration $index: left $leftMotor right $rightMotor');
    }
  };
  
  connection.onClosed = (int status, String reason) {
    print('Closed with $status for $reason');
    connected = false;
  };
        
  connection.onError = (e) {
    print('Error was $e');
    connected = false;
  };

  // Emulate a 60fps application
  Timer timer = new Timer.repeating(16, (e) {
    if (connected)
      connection.send(_gamePads[playerIndex].toJSON(playerIndex));
    else
      e.cancel();
  });
}

void main()
{
  _gamePads = new List<GamePad>();
  
  GamePad gamePad;

  // Create player 1
  gamePad = new GamePad();
  gamePad.connected = true;
  gamePad.leftThumbstickX = 0.5;
  gamePad.leftThumbstickY = 0.1;
  gamePad.rightThumbstickX = -0.2;
  gamePad.rightThumbstickY = -0.6;
  gamePad.leftTrigger = 0.33;
  gamePad.rightTrigger = 0.25;
  gamePad.buttons = 12288;
  
  _gamePads.add(gamePad);
  
  // Create player 2
  gamePad = new GamePad();
  gamePad.connected = true;
  gamePad.leftThumbstickX = 0.5;
  gamePad.leftThumbstickY = -0.3;
  gamePad.rightThumbstickX = -0.8;
  gamePad.rightThumbstickY = -0.6;
  gamePad.leftTrigger = 0.5;
  gamePad.rightTrigger = 0.75;
  gamePad.buttons = 13;
  
  _gamePads.add(gamePad);

  // Create player 3
  gamePad = new GamePad();
  gamePad.connected = true;
  gamePad.leftThumbstickX = -0.5;
  gamePad.leftThumbstickY = -0.3;
  gamePad.rightThumbstickX = 0.8;
  gamePad.rightThumbstickY = -0.2;
  gamePad.leftTrigger = 0.2;
  gamePad.rightTrigger = 0.95;
  gamePad.buttons = 63;
  
  _gamePads.add(gamePad);
  
  // Create player 4
  gamePad = new GamePad();
  gamePad.connected = true;
  gamePad.leftThumbstickX = 0.33;
  gamePad.leftThumbstickY = -0.40;
  gamePad.rightThumbstickX = -0.1;
  gamePad.rightThumbstickY = -0.25;
  gamePad.leftTrigger = 0.8;
  gamePad.rightTrigger = 0.4;
  gamePad.buttons = 15;
  
  _gamePads.add(gamePad);
  
  // Create the websockets server
  HttpServer server = new HttpServer();
  WebSocketHandler wsHandler = new WebSocketHandler();
  server.addRequestHandler((req) => req.path == '/ws', wsHandler.onRequest);
  
  wsHandler.onOpen = handleConnection;
  
  print('Starting server');
  server.listen('127.0.0.1', 8000);
}
