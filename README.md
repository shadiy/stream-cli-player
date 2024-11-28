# stream-cli-player
Windows only

A webview embedded twitch stream player cli

Uses:
[webview](https://github.com/webview/webview)
[cpp-httplib](https://github.com/yhirose/cpp-httplib)
[subprocess.h](https://github.com/sheredom/subprocess.h)

Usage
```
stream <channel name>
```

Usage (Attached to console)
```
player <channel name>
```

Building
```
git clone https://github.com/shadiy/stream-cli-player.git
cd stream-cli-player
mkdir build
cd build
cmake ..
cmake --build . --config Release
cd Release
stream <channel name>
```
