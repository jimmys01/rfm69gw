# RFM69GW change log

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [1.0.1] 2018-01-11
### Changed
- Updated JustWifi library to 1.1.6
- Heartbeat every 60 seconds
- Show friendly uptime in Web UI

## [1.0.0] 2017-10-31
### Changed
- Migrating code base from ESPurna project
- Changes in the web UI
- WS and MQTT fixes and addons

## [0.9.4] 2017-02-14
### Changed
- Issue #3. Using forken Time.h library to prevent build error on Windows hosts

## [0.9.3] 2017-02-06
### Added
- EEPROM management commands from terminal interface

### Changed
- Defer reconnection when issued from web interface to allow the response to get out
- Improved MQTT and NTP connections
- Multiple small changes from ESPurna firmware

## [0.9.1] 2017-01-22
### Added
- Added messages tab in web interface with sorting and filtering

### Fixed
- Issue #2. Set max packet length in RFMManager::send to RF69_MAX_DATA_LEN
- Issue #2. Allow user to define packet separator, defaults to :
- Allow multi-packet websocket frames

## [0.9.0] 2016-12-01
### Added
- Added unique admin password (OTA, WIFI and WEB interface)
- Using ESPAsyncWebServer and AsyncMqttClient
- Added reset command to console

### Changed
- Updated gulp builder script
- Moved git submodule references to PlatformIO dependencies
- Overriden RFM69::select() method with custom DIV_CLOCK
- Replaced AJAX requests completely by WebSockets

## [0.3.1] 2016-11-17
- Imported changes from ESPurna project

## [0.3.1] 2016-10-03
- Initial working version
