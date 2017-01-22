# RFM69GW change log

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

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
