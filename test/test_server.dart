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

String _player1 =
"""
{
    "index": 0,
    "connected": true,
    "leftThumbstick": {
        "x": 0.5,
        "y": 0.1
    },
    "rightThumbstick": {
        "x": 0.2,
        "y": 0.2
    },
    "leftTrigger": 0.25,
    "rightTrigger": 0.33,
    "buttons": 1
}
""";

String _player2 =
"""
{
    "index": 1,
    "connected": true,
    "leftThumbstick": {
        "x": 0.5,
        "y": 0.1
    },
    "rightThumbstick": {
        "x": 0.2,
        "y": 0.2
    },
    "leftTrigger": 0.5,
    "rightTrigger": 0.75,
    "buttons": 1
}
""";

String _player3 =
"""
{
    "index": 2,
    "connected": true,
    "leftThumbstick": {
        "x": 0.5,
        "y": 0.1
    },
    "rightThumbstick": {
        "x": 0.2,
        "y": 0.2
    },
    "leftTrigger": 0.20,
    "rightTrigger": 0.95,
    "buttons": 6
}
""";

String _player4 =
"""
{
    "index": 3,
    "connected": true,
    "leftThumbstick": {
        "x": 0.5,
        "y": 0.1
    },
    "rightThumbstick": {
        "x": 0.2,
        "y": 0.2
    },
    "leftTrigger": 0.8,
    "rightTrigger": 0.4,
    "buttons": 1
}
""";

List<String> gamePadStates;

int getPlayerIndex(String index)
{
  if (index == "player2")
    return 1;
  else if (index == "player3")
    return 2;
  else if (index == "player4")
    return 3;
  
  // Default to 1
  return 0;
}

void handleConnection(WebSocketConnection connection)
{
  print('New connection');

  connection.onMessage = (message) {
    int index = getPlayerIndex(message);
    
    connection.send(gamePadStates[index]);
  };
  
  connection.onClosed = (int status, String reason) {
    print('Closed with $status for $reason');
  };
        
  connection.onError = (e) {
    print('Error was $e');
  };
}

void main()
{
  gamePadStates = new List<String>();
  gamePadStates.add(_player1);
  gamePadStates.add(_player2);
  gamePadStates.add(_player3);
  gamePadStates.add(_player4);
  
  HttpServer server = new HttpServer();
  WebSocketHandler wsHandler = new WebSocketHandler();
  server.addRequestHandler((req) => req.path == '/ws', wsHandler.onRequest);
  
  wsHandler.onOpen = handleConnection;
  
  print('Starting server');
  server.listen('127.0.0.1', 8000);
}
