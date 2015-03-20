# -*- mode: ruby -*-
# vi: set ft=ruby :

$script = <<SCRIPT
set -eux

sudo apt-get update
sudo apt-get -y install build-essential gcc-arm-none-eabi gdb-arm-none-eabi
SCRIPT

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/trusty64"
  config.vm.provision "shell", inline: $script

  config.vm.network :private_network, ip: "10.10.1.5" # host: 10.10.1.1
end
