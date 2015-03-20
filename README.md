# Quickstart for the *STM32F4 Discovery* eval board

- Get a shiny [STM32F4 Discovery](http://de.farnell.com/stmicroelectronics/stm32f4discovery/discovery-kit-stm32f407-usb-otg/dp/2009276) board.
- Clone this repo & install [Vagrant](https://www.vagrantup.com/).
- `$ vagrant up && vagrant ssh` to enter the freshly configured Ubuntu 14.04 development VM.
- Download
[stm32cubef4.zip](https://github.com/jonasschneider/stm32-template/releases/download/v0.0.1/stm32cubef4.zip), STM's software bundle, to `pkg/stm32cubef4.zip`, and unzip it. Check shasum:

        vagrant@vagrant-ubuntu-trusty-64:/vagrant$ sha256sum pkg/stm32cubef4.zip
        b5deff0c2da912de9a1d4b2473b66d39e405a6926c085f688ac55212102270da  pkg/stm32cubef4.zip

  (note: This download brought to you by Github Releases, since STM doesn't provide a permalink to any given version of this file.)

- Run `make` to compile `main.bin`, the main file we'll put on the board's flash storage.
- Connect the board to your development machine using the Mini-USB port.
- On the host (outside the Vagrant VM), install [OpenOCD](http://openocd.org/) -- with Homebrew on OS X, it's `brew install openocd`.
- In a second terminal, run `openocd -f openocd.cfg` and leave it running. This will allow the tools inside the VM to interact with the physical board.
- On the host, run `make program` to write the file to flash. (TODO: fix this to run over the OpenOCD server)
- Watch the LEDs!
