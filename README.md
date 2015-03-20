# Quickstart for the *STM32F4 Discovery* eval board

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
- Watch the LEDs!
