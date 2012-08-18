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
#source('game_pad.dart');

String gamePadStateMessage(int index, GamePadState gamePad)
{
  return
    """
{
    "index": ${index},
  "connected": ${gamePad.isConnected},
  "leftThumbstick": {
      "x": ${gamePad.leftThumbstickX},
      "y": ${gamePad.leftThumbstickY}
  },
  "rightThumbstick": {
      "x": ${gamePad.rightThumbstickX},
      "y": ${gamePad.rightThumbstickY}
  },
  "leftTrigger": ${gamePad.leftTrigger},
  "rightTrigger": ${gamePad.rightTrigger},
  "buttons": ${gamePad.buttons}
}      
      """;
}

void _handleConnection(WebSocketConnection connection)
{
  print('New connection');
  bool connected = true;
  GamePadState gamePad = new GamePadState();
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
      GamePad.setVibration(index, leftMotor, rightMotor);
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
    {
      GamePad.getState(playerIndex, gamePad);
      connection.send(gamePadStateMessage(playerIndex, gamePad));
    }
    else
    {
      e.cancel();
    }
  });
}

void _startServer(String host, int port)
{
  // Create the websockets server
  HttpServer server = new HttpServer();
  WebSocketHandler wsHandler = new WebSocketHandler();
  server.addRequestHandler((req) => req.path == '/ws', wsHandler.onRequest);

  wsHandler.onOpen = _handleConnection;

  print('Starting server ${host}:${port}');
  server.listen(host, port);
}

void main()
{
  // Create the gamepads
  GamePad.onInitialize();

  // Load configuration
  Path path = new Path('config.json');
  File file = new File.fromPath(path);
  Future<String> configuration = file.readAsText(Encoding.ASCII);

  configuration.onComplete((result) {
    // Set defaults
    String host = '127.0.0.1';
    int port = 8000;

    if (result.hasValue)
    {
      Map config = JSON.parse(result.value);
      host = config['host'];
      port = config['port'];
    }
    else
    {
      print('config.json not found using defaults');
    }

    _startServer(host, port);
  });
}
