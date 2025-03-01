# GameSpy Replacement for Command & Conquer: Generals - Zero Hour

This project is a replacement for the GameSpy API used in Command & Conquer: Generals - Zero Hour. It provides a modular WinSock networking system that can connect to various backend services like Revora's C&C:Online or OpenSpy.

## Features

- **Modular Backend System:** Easily switch between different backend services.
- **Full Multiplayer Support:** Authentication, game listing, hosting, joining, and NAT traversal.
- **Drop-in Replacement:** Works with the original game without any modifications to the game files.
- **Logging System:** Comprehensive logging for easy debugging.

## Installation

1. Copy the `gamespy.dll` file to your Command & Conquer: Generals - Zero Hour installation directory.
2. (Optional) Edit the `GSReplacement.ini` file to configure the backend service.

## Configuration

The `GSReplacement.ini` file allows you to configure the backend service:

```ini
# GameSpy Replacement Configuration
backend = Revora
loginServer = gpcm.gamespy.com
loginPort = 29900
chatServer = peerchat.gamespy.com
chatPort = 6667
masterServer = ccgenzh.ms6.gamespy.com
masterPort = 28900
natNegServer = natneg1.gamespy.com
natNegPort = 27901
```

## Supported Backends

### Revora (C&C:Online)

Revora's C&C:Online is the default backend service. It requires a C&C:Online account, which you can create at [https://cnc-online.net](https://cnc-online.net).

### OpenSpy

OpenSpy is an alternative backend service that does not require registration. It may not support all features available in Revora.

## Building from Source

### Prerequisites

- Visual Studio 2015 or later
- Windows SDK
- WinSock2 library

### Build Steps

1. Open the solution file in Visual Studio.
2. Build the solution.
3. The output files will be in the `bin` directory.

## Technical Details

This implementation replaces the GameSpy API by intercepting calls to the GameSpy functions and redirecting them to our own implementation. It uses a combination of DLL replacement and function hooking.

- **DLL Replacement:** The `gamespy.dll` file is replaced with our implementation.
- **Function Hooking:** For functions that are statically linked, we use function hooking to redirect the calls.

The system is designed to be modular, with a clean interface between the game and the backend service. This makes it easy to add support for new backend services in the future.

## Troubleshooting

If you encounter issues, check the `GSReplacement.log` file for detailed error messages. Common issues include:

- **Connection Issues:** Make sure your firewall allows the game to access the internet.
- **Login Issues:** Verify your account credentials with the backend service.
- **NAT Issues:** If you're having trouble connecting to games, you may need to configure port forwarding on your router.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgements

- The Revora team for maintaining the C&C:Online service.
- The OpenSpy team for their alternative GameSpy replacement.
- The Command & Conquer community for keeping the game alive. 