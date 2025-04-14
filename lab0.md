# Lab0

This lab is intended for you to get access to and get familiar with your Raspberry Pi. This lab needs no submission and does not have a DDL, but if you want to get a smoother experience on later labs, this lab is highly recommended. If you encounter any issue during this lab, feel free to discuss it in detail with your classmates and TAs in the course group.

## About RPi and your RPi

Raspberry Pi (RPi) is a computer, just like any of your PCs or laptops. It has almost every component of your PC, like CPU, GPU, RAM, storage and network, yet its specification is different from your PC because its intended use cases are not the same as those of your PC, which is again a trade-off (a recurring topic throughout the entire course). For example, you may use your PC (Equipped with powerful GPU) to play 3D video games but you definitely do not want to do so on an RPi; in the meanwhile, you definitely do not want to spend thousands of CNY on a simple router or a NAS or a dormitory temperature monitor. Because of its cheap (though not true recently) yet powerful enough characteristics, RPi has received wide recognition as an affordable toy, a usable development board, or even a reliable production unit among hobbyists, students, developers and more.

This course, Introduction to Computer System, is not only about important theoretical ideas in computer systems, but we also want to give you some training on practical skills to be used in the future. More specifically, because of the popularity of RPi, it may be helpful to know how to use it for you on your later projects/research.

The RPi you will use is an RPi 4B with 4GB Memory and 32GB storage. For further details, you may refer to the [official website](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/).

We have pre-installed a customized Raspbian image on your SD card. Raspbian is a GNU/Linux operating system with some customization to RPi. TA has also customized the image on network setups like Ethernet and Wifi.

## 1. Assemble

### Bill of materials

|Component|Quantity|
|----------|--------|
|RPi4 Baseboard|1|
|Case|1|
|Fan (with 4 nuts and bolts)|1|
|Heat sink|1|
|Power supply|1|
|SD Card (32G)|1|
|Ethernet Cable (1m)|1|

You should keep the packaging material of all of them (especially SD card) as after this semester you should hand them back to us.

### Procedures

You may refer to Week 3 TA Session.

1. Put the baseboard inside the red part of the case
2. Stick the heat sink on the CPU (the one with Metal cap and BROADCOM on it)
3. Install the fan on the white part of the case. You should note that only on one side of the fan can you put nuts on it.
4. Connect the dupunt line to the GPIO pins. Note that the red line is 5V (Connect to Pin 4) and the black line is GND (Connect to Pin 6). You should refer to the GPIO section (5.1.1) on [RPI4B datasheet](https://datasheets.raspberrypi.com/rpi4/raspberry-pi-4-datasheet.pdf). You should align them according to the J8 silkscreen on the PCB and the J8 in the datasheet figure.
5. Close the case
6. Insert the SD card via a port on the red part of the case 

## 2. Network access

```
Ethernet: Static IP 10.11.13.1/24. RPi also DHCP on this interface.
WLAN: Static IP 10.11.14.1/24. SSID: sysintro-<random_number>, passphrase: sysintro
```

Note that the passphrase is quite weak. Remember to change the password afterwards (in advanced topics)

1. Use the Ethernet cable to connect your RPi with the port in your dormitory.
2. Connect your PC and your teammate's PC to the Wifi named sysintro-1 (vary with your group number). Now you are in the same WLAN. You can SSH into your RPi now.
3. Access <http://login.tsinghua.edu.cn> or 3.3.3.3 on your PC to authenticate your RPi4. Now your PC and your RPi have Internet access.

## 3. SSH and IDE

```
username: pi
passwd1: SysIntro2024!
passwd2: 123456
host: 10.11.14.1
port: 22
```

There are many providers of SSH client, e.g.

1. VS Code Remote (demoed in TA Session)
2. MobaXterm
3. Git-for-windows
4. OpenSSH that comes with the system (e.g. in MacOS)
5. Linux users (since Linux users are power users, we won't detail the guide for them here, they are listed here just for completeness)

You may refer to the manual of them. If you encounter issues, you can reach out to your classmates or search online, and also feel free to ask the TAs. Generally speaking, if you are using GUI (e.g. VS Code Remote / MobaXterm), you can just fill the parameters above and all done. If you are using the command line, the SSH command is in the form of

```bash
$ ssh pi@10.11.14.1
```

Note that `$` is just a shell prompt and you should not enter it in the command line.

After pressing ENTER, you should enter the passwd above. Note that the command line won't echo `*****` like a typical password input field.

Now you have logged into your RPi4! You may see a prompt of this form

```bash
pi@sysintro-1:~ $
```

Then remember to change your password! You can type `passwd` to change your passwd

```bash
$ passwd
Old passwd:
New passwd:
Verify New passwd:
Updated successfully!
```

Note that this again won't echo `****` as you input your passwd.

## 4. Advanced topics

We won't give a detailed explanation in this section as there are so many details and mechanisms behind these commands. You can just look up the wiki (e.g. Arch Wiki) and man page (e.g. `man man` and `man useradd`) instead of TA explaining here, but we will give example command snippets and brief explanations above each of them.

### Change Wifi password

Sharing the same password with all your classmates is dangerous! TA do not have the power to generate and distribute a strong password one by one for you. Now it is your responsibility to guarantee the safety of your precious code!

After login, you can

```bash
# change to root
pi@sysintro-1:~ $ sudo -i
# inspect the contents of /etc/hostapd/hostapd.conf
#   Side Note: Cat abuse here, though
root@sysintro-1:~ $ cat /etc/hostapd/hostapd.conf
# change password from `sysintro` to `YoUr!New!Pa55Wd` in /etc/hostapd/hostapd.conf
#   namely `s`ubstitute `passphrase=sysintro` to `passphrase=YoUr!New!Pa55Wd` in file /etc/hostapd/hostapd.conf
#   you may change `YoUr!New!Pa55Wd` to your new passphrase
#   Side Note: TA use sed here because it is more stable for you to use
#   if we recommend using vim, you may wont be able to exit it.
root@sysintro-1:~ $ sed -i 's/passphrase=sysintro/passphrase=YoUr!New!Pa55Wd/' /etc/hostapd/hostapd.conf
# restart related software to make the config effective
#   You may lose connection this time, you should re-connect to your Wifi.
#   You should wait for a while for the software to get started
#   If you do not see the Wifi of your RPi, you can try to restart your RPi
#   Side Note: systemctl is a cli interface to systemd,
#   which is PID1 and controls all the daemons (background services)
#   hostapd is the deamon responsible for Wifi and AP
root@sysintro-1:~ $ systemctl restart hostapd
```

### Add new users and your teammate

You may get tired of the username `pi` as it does not show your uniqueness. You may want to use your name abbr like `zhenghr` or your nickname like `zenithal`.

Also, there are privacy issues regarding you and your teammate if you two share the home. It is a common practice to add two new users for both of you instead of sharing the `pi` account.

```bash
# change to root
#   of course you need root privilege to add new users
pi@sysintro-1:~ $ sudo -i
# add a user named `zenithal`
#   of course you should change it to your name
#   -m creates /home/zenithal for zenithal
root@sysintro-1:~ $ useradd -m zenithal
# set the passwd for zenithal
root@sysintro-1:~ $ passwd zenithal
# promote zenithal to be a sudoer
#   sudoers should sudo with caution, especially not to sudo after getting drunk
root@sysintro-1:~ $ usermod -aG sudo zenithal
# add a user named `zrt`
#   of course you should change it to your teammate's name
root@sysintro-1:~ $ useradd -m zrt
root@sysintro-1:~ $ passwd zrt
root@sysintro-1:~ $ usermod -aG sudo zrt
```

Now you and your teammate can just SSH using your own username

```bash
$ ssh zenithal@10.11.14.1
```

Since each of you has a different home, you can not access the code of your teammate directly now, then how to collaborate? The easiest one is falling back to the `pi` account and you two both develop inside it. But this may result in conflict! You two may change one file at the same time and then there are inconsistencies!

You should use Git, which we will talk about in later classes and labs.

### SSH Key and delete your password

Still worry about your password not being strong enough? Why not go passwordless! Public key cryptography creates such a miracle that if you hold the private key and you have registered your public key in the server, you can authenticate with your private key.

First, you need to generate a keypair locally

```bash
# Generate a private key and a public key
#   For convenience, you may leave the passphrase empty (Just enter ENTER to skip it)
$ ssh-keygen -t ed25519
```

Then you can register your public key to your RPi

```bash
$ ssh-copy-id zenithal@10.11.14.1
```

Now you can try `ssh zenithal@10.11.14.1` and you should go smoothly without passwd.

Deleting password is fairly easy, namely `passwd -d zenithal` as `root` then `zenithal` has no password now. However, there are extra steps before this, because when you sudo, you have to enter password, we can turn off this feature by the following command.

```bash
# Change the line `%sudo   ALL=(ALL:ALL) ALL` to be `%sudo   ALL=(ALL:ALL) NOPASSWD: ALL`
#   users in sudo group now do not need to enter passwd
#   Dangerous zone! Edit with caution and enough acquaintance
root@sysintro-1:~ # EDITOR=nano visudo
```

Then you can safely delete your password.

### SSH access via the Internet for your teammate

You two can collaborate on the same RPi if you are physically close because you can connect to the same Wifi. But what if you are separated from each other? Due to the characteristics of Tsinghua Campus Network, your teammate can just SSH into your RPi via the Internet!

```bash
# find the IP address
$ ip address
```

You may find an ip address in the form of `59.66.xx.yy`, and you can give it to your teammate, then your teammate can connect to it if he/she is also in Tsinghua Campus Network

```bash
$ ssh zrt@59.66.xx.yy
```

### Exploit spare spaces in SD card

This is a very dangerous operation, proceed with caution! You may revisit this section after you have learned more in later courses. If you then know the difference between partition and file system, you may proceed.

For TA's convenience, we have used a 4GB partition as rootfs because we can flash the image faster (flashing 4GB takes 10 minutes but 32GB takes 80 minutes!).

However, now your available spaces are only around 500MB because of those systems files and executables. In the meanwhile, there is nearly 28GB of spare space in the SD card. You can create a new partition to occupy the spare space, which is trivial (relatively).

Another more tricky way is to online grow your rootfs. First, you need to alter your partition table (e.g. fdisk) to enlarge the partition of your rootfs, then you need to resize your file system (e.g. resize2fs). You may refer to StackOverflow (SOF) for detailed instructions using keywords like online growing ext4 rootfs.
