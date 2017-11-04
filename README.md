# little-os
This is to discover the awesomeness (tediousness) of developing your own OS

![Grub 2 Bootloader](https://i.imgur.com/nh1E1Z2.png)
![The boot message and tty0](https://i.imgur.com/t15qxCl.png)
https://imgur.com/a/lG0HS

## Why Little OS?
Well, nothing is too little compared to the priceless discovery of how Operating System works really, like you have to code for it to work, even before you can even type something into the console. 

I'll use it as a teaching tool for my friends, please feel free if you want to do so

## Development Environment
It hurts when you have to format your lovely Mac with Windows or any other operating system just for the sake of developing a little os. *Use Vagrant* instead, you can develop your os from everywhere you are, life will be easier.

## Vagrant
It's everyone's favourite development environment. Head to the [Vagrant download page!](https://www.vagrantup.com/downloads.html) to download it now. We'll use Ubuntu as our development environment.

### Setting up your Vagrant box
Create a new directory anywhere that you like. cd to the directory and type
```bash
vagrant init
```

Edit the **Vagrantfile** and change this line
```bash
  # Every Vagrant development environment requires a box. You can search for
  # boxes at https://vagrantcloud.com/search.
  config.vm.box = "XXX"
```

into

```bash
  # Every Vagrant development environment requires a box. You can search for
  # boxes at https://vagrantcloud.com/search.
  config.vm.box = "bento/ubuntu-16.04"
```

save the file and then type

```bash
$ vagrant up
```

You'll see the installation of the dev environment, or lets call it box from now on. Later, when the job's done, you can SSH into the box you've just created like this

```bash
$ vagrant ssh
```

### A little bit tuning for the Vagrant box
When you have SSH'ed into the box, you'll be able to install packages from Ubuntu just like you do with a real installed Ubuntu. And you can also read to your host machine or vice versa. Just need a little tuning on ~/.bashrc

```bash
$ nano ~/.bashrc
```

Scroll to the bottom and add

```bash
sudo mount -t vboxsf vagrant environment/
```

This will be useful if you want to transfer the file into the box or into your host machine. We'll always do this, so you'll want to ensure the mountpoint is working correctly:

```bash
$ sudo mount -t vboxsf vagrant environment/
```

Check the capacity/size

```bash
$ df -h
```

output:

```bash
vagrant@vagrant:~/environment$ df -h
Filesystem                    Size  Used Avail Use% Mounted on
udev                          477M     0  477M   0% /dev
tmpfs                         100M   14M   87M  14% /run
/dev/mapper/vagrant--vg-root   38G  1.3G   35G   4% /
tmpfs                         497M     0  497M   0% /sys/fs/cgroup
tmpfs                         100M     0  100M   0% /run/user/1000
vagrant                       234G  223G   12G  96% /home/vagrant/environment <<< THIS IS IT
vagrant@vagrant:~/environment$ 
```

cd to /home/vagrant/environment and do your works here

### Get barebones-toolchain to get GCC cross compiler
The cross compiler contains everything we need to compile/assemble our kernel, modules and bootloaders. In the /home/vagrant/environment directory:
```bash
$ git clone https://github.com/rm-hull/barebones-toolchain.git
$ cd barebones-toolchain
$ . ./setenv.sh
```

Check if it works
```
$ i686-elf-gcc --version
i686-elf-gcc (GCC) 6.2.0
Copyright (C) 2016 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ arm-none-eabi-gcc --version
arm-none-eabi-gcc (GCC) 6.2.0
Copyright (C) 2016 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ mips-gcc --version
mips-gcc (GCC) 6.2.0
Copyright (C) 2016 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## Getting little-os to Work
Now we will build Little OS into build/boot.img, a bootable disk image file powered by Grub2

### Clone little-os to the environment directory
You can either clone or just download the zip file and extract it here.

In the /home/vagrant/environment directory:
```bash
git clone https://github.com/farizluqman/little-os.git
cd little-os
sudo make
```

When you see this message ```Everything is done now with success! ``` it means the build is successful. If you have any issue building little-os then let me know [by creating an issue!](https://github.com/farizluqman/little-os/issues)

## Boot the disk image
I used Qemu to start the disk image, you can do so by issuing this command on your host machine, in the /home/vagrant/environment directory: 

!! Don't run it on your vagrant box

```bash
$ qemu-system-i386 build/disk.img
```

API documentations will be on the [wiki page later!](https://github.com/farizluqman/little-os/wiki)
