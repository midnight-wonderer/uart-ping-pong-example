# UART Ping-Pong example

## What is it?

It is a demonstration of the Haystack Codec.
Adding [ping](https://en.wiktionary.org/wiki/pinging#English) functionality to a UART device.
Read more about the codec at [https://slime.systems/content/usb-liked-uart/](https://slime.systems/content/usb-liked-uart/).

## The hardware

The project is targetting STM8S devices, but the codec module itself is hardware-independent.  
The codec should work on any serial interface that receives bytes, and on any device that can be programmed with standard C.

## Build dependencies

* [Git](https://git-scm.com/) version >= 2.0
* [Small Device C Compiler](http://sdcc.sourceforge.net/) version >= 3.9.0
* [GNU Make](https://www.gnu.org/software/make/)

## Build instructions

Simply clone this repository with submodules and `make`
```
$ git clone --recurse-submodules https://github.com/midnight-wonderer/uart-ping-pong-example.git
$ cd uart-ping-pong-example
$ make
```

## License

STM8S UART1 Transceiver is released under the [BSD 3-Clause License](LICENSE.md). :tada:
