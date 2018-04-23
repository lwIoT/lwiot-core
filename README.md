# lwIoT

lwIoT is a light weight, 21 century, internet of things library. To more precise,
it is a configurable *collection* of libraries. The feature list of lwIoT includes:

* Network layer abstraction (socket wrapper classes)
* MQTT client implementation
* Drivers for popular sensors

lwIoT aims to make embedded software development easy while using the full power of
your embedded hardware platofrm.

## Dependencies

lwIoT does have dependencies. The following is to be provided by external libraries:

* Socket layer implementation
* Standard C libarary (POSIX)
* DNS client

## Hardware

Most of lwIoT is built on top of a HAL, which makes lwIoT highly portable. Some parts
are heavily platform specific (think WiFi layer, for example). All unportable code is
annotated as such in the documentation.

## Contributing

All contributions to lwIoT are welcome. There are however, some guidelines to prevent
lwIoT from turning into a big unmaintainable mess. Further more, ***every*** developer
of lwIoT is expected to follow the Contributors Covenant Code of Conduct.

