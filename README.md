# Quickstart for the *STM32F4 Discovery* eval board

## See below for the TX/RX!

- Get a shiny [STM32F4 Discovery](http://de.farnell.com/stmicroelectronics/stm32f4discovery/discovery-kit-stm32f407-usb-otg/dp/2009276) board.
- Clone this repo & install [Vagrant](https://www.vagrantup.com/).
- `$ vagrant up && vagrant ssh` to enter the freshly configured Ubuntu VM.
- Download [stm32cubef4.zip](http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stm32cubef4.zip), STM's software bundle, to `pkg/stm32cubef4.zip`, and unzip it. The bad news is that they don't version this file... I'm using this one, you can [email me](mailto:mail@jonasschneider.com) if you run into trouble and can't find the same version:

        vagrant@vagrant-ubuntu-trusty-64:/vagrant$ sha256sum pkg/stm32cubef4.zip
        b5deff0c2da912de9a1d4b2473b66d39e405a6926c085f688ac55212102270da  pkg/stm32cubef4.zip

- Run `make` to compile `main.bin`, the main file we'll put on the board's flash storage.
- Connect the board to your development machine using the Mini-USB port.
- On the host (outside the Vagrant VM), install [OpenOCD](http://openocd.org/) -- in Homebrew, it's `brew install openocd`.
- In a second terminal, run `openocd -f openocd.cfg` and leave it running. This will allow the tools inside the VM to interact with the physical board.
- On the host, run `make program` to write the file to flash. (TODO: fix this to run over the OpenOCD server)

## Setting up the TX+RX circuit
- Get a super-cheap RF transceiver combo like the [FS1000A](http://www.ananiahelectronics.com/FS100A.htm) and its receiving cousin.
- Program an Arduino Mega using the files in `rx/`. (Careful, there are name clashes between the stuff in here and in that directory. I'm sorry.)
- Connect the TX module to VCC, GND and the data pin to the STM32's `GD13`.
- Connect the RX module to VCC & GND on the Arduino, and the data pin to the Arduino's `3` pin.
- Program the STM32 as described above.
- Open the Arduino serial console! You should see a bunch of `-`s and `X`s -- they stand for a received 0/1, respectively. After a while, you should see the received message like this:

    ----X---X--X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X-X---XXX--XX-X
    GOT START
    X-XX--X-X--Xlength:9 -XX-X-XX---X-X-XX-X--XX--X-XX--X-X-X-X-XX--X-X-X-X-XX---X-XXXX--X--XXX--X-XX----X-XX-XXX---XXX--
    Got 6:H e l l o !

  Yay!

More details on the hardware and wiring are available from many Arduino fans on the web, see for example [here](http://www.glacialwanderer.com/hobbyrobotics/?p=291), [here](http://forum.arduino.cc/index.php?topic=129749.15) and [here](http://vanceance.blogspot.co.at/2014/01/xy-mk-5v-arduino.html).

Also, I can only recommand reading the excellent code in `VirtualWire.c` (which I adapted a tiny bit from the more or less pristine `rx/VirtualWire.cpp`). You don't need much hardware/telecomms knowledge to understand the code (I'm just a software guy, too).
It implements an interrupt-driven asynchronous data link layer protocol using a phase-locked loop and CRC error detection for robustness against bit errors due to the shitty RF hardware.

This means that it's even better for us than just a serial link, since framing is provided for us already. All that's left is to form actual packets (e.g. using Google's [Protocol Buffers](https://github.com/google/protobuf)) and send them instead of the text.
