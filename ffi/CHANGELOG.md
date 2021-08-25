## 0.1.4

- Fix OnOpen callback

## 0.1.3

- Update `ReceivePort` listener to match new NativePort callbacks.

## 0.1.2

- Improved memory cleanup inside FFI.

## 0.1.1

- Removed `Player::onVideo` callbacks for win32.

## 0.1.0

- Added new native methods for handling with memory leaks.

## 0.0.9

- Fixed `Devices.all` & `Media.parse` causing crash on Windows. 

## 0.0.8

- Fixed build warnings on Linux.

## 0.0.7

- Fixed `Media.asset` for Windows.

## 0.0.6

- Added back `Media.asset` for Flutter.

## 0.0.5

- Sending player ID along frame `uint8_t*` through native port.

## 0.0.4

- Fixed incorrect `Devices.all` list.

## 0.0.3

- Fixed video frame callbacks.

## 0.0.2

- Fixed segmentation fault open calling `Player::open` with `Playlist` as argument.
- Fixed build warnings.

## 0.0.1

- Initial version of dart_vlc_ffi.
