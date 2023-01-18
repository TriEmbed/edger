# Bootable thumbdrive information
This is a detailed HOW-TO for bootable Linux thumbdrives preinstalled with Edger and Espressif software. The first section is about using the thumbdrive and a later section details creation of thumbdrives.

## Using a bootable thumbdrive

Some general information:
1. The drive boots to an Ubuntu 20.04 desktop session: there is no need to "log in". 
2. The system user name is "edger" and the password is "none".
3. The host computer's hard drive is only accessible with advanced commands executed as super user, so it is effectively isolated from the system.

How to use the thumbdrive:
1. The host PC must be configured to boot off alternate media. Specifically, if "secure boot" is enabled the steps below will not work properly.
2. As the host PC starts up use the keyboard character (frequently enter, F2 or DEL) to break into the "BIOS".
3. Select the boot menu (frequently F12).
4. Use the down arrow to "Samsung Flash Drive" and hit enter.
5. There is currently a problem that is causing "disk fixup" to be run on the thumbdrive at every boot. This makes startup exceedingly slow. The screen will remain blank for multiple minutes because of this.
6. When fixup is completed a status line on the top of the screen appears.
7. The startup process proceeds until there is a desktop screen with a picture of a cat.
8. Various circumstances may require "command line interpreter (CLI)" commands to be entered. This is done in a "terminal window" that is gotton by putting the mouse pointer in the main (middle) part of the screen (but not on any application window within that screen), pressing the right mouse button and selecting "open in terminal" and then pressing the left mouse button. This can be done repeatedly to create multiple terminal screen.
9. Before Edger can be used the system has to be informed of the user's WIFI parameters. These must be the same SSID/password that the host PC is using. The WIFI parameters only have to be updated once or until they are changed for the host PC.
10.It is necessary to have an Edger development board plugged into the host PC during the WIFI parameter change because the Ant subsystem build process involves querying the specific ESP32 hardware involved.
11. To change the WIFI parameters double left click on the "Change Wifi" icon on the desktop screen. A terminal session will appear and interact with the user to get the new parameters. After you've OK'd the new parameters the Ant subsystem will be rebuilt in the background. This can take multiple minutes. When it is completed ...
12. To start Edger first make sure an Edger development board is connected to the host PC via USB.
13. Next double left click "Start Aardvark" to start the Aardvark subsystem. A terminal window will appear and the startup status will be shown on the screen. The Aardvark startup is complete when ""
14. Finally double left click on "Start Browser" icon to start the Firefox browser at URL localhost:8080. The Edger web browser screen will appear. 


The following steps are used to create a Linux+Edger thumbdrive. If you already have intermediate materials simply start with the steps following as instructed below.

These steps create a Linux install thumbdrive. This is a safety measure to make it less likely to accidentally damage the PC's disk drives.

1. Fetch a copy of ubuntu-20.04.5-desktop-amd64.iso
2. Insert thumbdrive to hold the master Ubuntu + Edger files
3. df
4. Note the device path for the /media/$USER/dir entry that is typically at the bottom of the screen.  This is the target thumb drive. It should be something like /dev/sdb (i.e. if the host system has one disk drive /dev/sda). 
BE VERY CAREFUL TO USE THIS PATH IN THE NEXT STEP. YOU MIGHT DESTROY YOUR SYSTEM IF YOU GET THIS WRONG.
5. Close the file manager window for this thumbdrive.
6. Unmount the thumbdrive with "sudo umount /dev/sdX1" where X is the drive letter from the command above.
7. sudo dd if=ubuntu-20.04.-desktop-amd64.iso of=/dev/sd{device letter from df output} 1M conv=fsync
for instance: sudo dd if=ubuntu-20.04.-desktop-amd64.iso of=/dev/sdb 1M conv=fsync (BUT THIS IS AN EXAMPLE: 
USE THE DRIVE LETTER FROM THE ABOVE df COMMAND OUTPUT).
8. Remove this thumb drive and mark it "LINUX 22.04 INSTALL". No need to unmount. You will never write to this, it is strictly for creating the UBUNTU MASTER system thumb drives in the future. 

If you have a LINUX 22.04 INSTALL thumbdrive start here:

1. Insert a second thumb drive that will be "UBUNTU MASTER". Close the file manager window for it.
2. df
3. Note the drive letter again by seeing the line having /media/$USER/c9* or 429* in it and getting the letter after /dev/sd in the left most column. 
4. umount /dev/sdX1 where X is the above drive letter
5. Destroy the existing partition table. NOTICE YOU DO NOT ADD A "1" to the output file path:
6. sudo dd if=/dev/zero of=/dev/sdX bs=1M count=20 conv=fsync
7. Now create a new partition table:
8. sudo fdisk /dev/sdx
9. p 
10. This should display info about the thumbdrive with an automagically created partition table. NOTE that these steps are done because completely incorrect information has been found in the PNY thumb drives that does not cause problem with Windows but causes havoc with Linux. Next use this command to create a new partition in the table:
11. n
12. return until you run out of prompts. This creates a new partition. If you are prompted to select primary or extended partition select primary. Next use this command to set the partition type:
13. t
14. Type "83" as the partition type and then hit enter. Enter the following to get info about the partition table.
15. p
16. This shows a new linux partition and its size should be close to the total size of the thumbdrive. Enter the following and 'enter' to write out the new partition table. This thumbdrive is now ready for use.
17. w
19. This thumb drive will contain a fully installed, fully updated/patched Linux system to be used as a readonly thumb drive that gets duplicated to create the EDGER MASTER drive. The EDGER MASTER drive will also end up as a readonly drive to sit on the shelf. When Edger updates are available they will be applied to EDGER MASTER and any new thumb drive copies (simply called EDGER. can be made. 
20. Remove this thumb drive and label it UBUNTU MASTER.

The following steps are used to bring up the UBUNTU INSTALL thumbdrive and use it to install Linux on the UBUNTU MASTER thumb drive.

1. Use "sudo shutdown -h" to shut the linux system down.
2. Insert UBUNTU INSTALL
3. Power on the PC and get it into the bios, then into the boot order selector and select the "Samsung Thumb Drive" to boot from.
4. IMMEDIATELY INSERT THE UBUNTU MASTER THUMBDRIVE while the thumbdrive UBUNTU INSTALL is 
starting up. This is necessary to avoid ambiguity in certain cases where the PC is unable to differentiate between two thumbdrives with identical labels. 
5. It takes a few minutes for OS INSTALL to put up the Ubuntu "try out or install" screen. Left click on "INSTALL".
6. AGAIN, YOU MUST BE SUPER CAREFUL TO SELECT A TARGET DRIVE FOR BOTH THE OS INSTALL AND THE BOOTING. IF YOU SELECT
one of your PC drives you will be forever sorry.
7. Click on the "load third party" box and continue
8. Click "do something custom" at the bottom and continue
9) NOW VERY CAREFULLY notice the path of the drive that is UBUNTU MASTER. The system will not show the drive doing the install but will show your PC drive(s). The UBUNTU MASTER is the 64GB drive with just a small fraction of the drive in use (the superblocks and partition table).
10. double click on the UBUNTU MASTER drive path that ends in 1. Go through the menus and tell it to be set up as an
ext4 filesystem with path "/" and then get back to the containing screen. 
11. Click the "format" box for the UBUNTU MASTER drive partition ending in "1"
12. With the boot dropdown menu at the bottom select the UBUNTU MASTER drive without a "1"
13. DOUBLE CHECK what you have done
14. Continue
15. As the install runs in the background select a time zone and then define the system and user. The system name and user name should both be "edger" and the password should be "none". Select "without login" check box.
15. The install takes a VERY long time. Multiple hours.
16. When done, tell it to restart. It will prompt you to pull out the install media. Remove the UBUNTU INISTALL thumbdrives and hit enter. As the system is restarting break into the BIOS, select the boot menu (typically F12) and arrow down to the thumb drive.
17. It takes a LONG TIME to start up. There seems to be a flaw with thumbdrives such that their filesystems appear
dirty and so the file checker has to run. This is a slow process across dozens of gigabytes of thumb drive.
Always shut your thumbdrive-driven PC down with the "power off/logout" dropdown menu in the upper right corner of
the desktop to hopefully avoid a long fsck. But it seems that fsck always runs. As of Jan 18 there is a theory about why this is happening.
18. The fsck will finish with a status line on the top of the screen, the the OS initial startup screens will appear.
19. Click next until you get to the "do you want to share info" and disable that or not: it just slows things down and
the system will already be slow. You'll finally get to screen with a selection of apps to run. Click next away from
that and get the cat face desktop screen. 
20. IMPORTANT. You will see a dialog saying you can upgrade Ubuntu. DON'T DO IT. Click the leftmost button then
the left again to acknowledge you are REJECTING the upgrade. Upgrading to 22 would put you into unknown 
territory as well as taking many more hours to get to the possible chaos.
21. Click "remind me later" for the software upgrade dialog that comes next.
22. Right click in the middle of the desktop to get a dropdown menu and left click "open in terminal"
23. The sudo password is "none". Enter these commands:
24. sudo apt-get update
25. sudo apt-get upgrade
26. This flavor of upgrade just pulls in all the fixes for the Ubuntu 20.04 you have, it doesn't upgrade to 22. This will take a long time.
27. You now have UBUNTU MASTER. Click on the icon in the extreme upper right and select "shut down". 
28. Now you must copy the full disk image of UBUNTU MASTER to your PC.
29. Insert it into your PC, close the file manager window for it and unmount it:
30. df 
31. Use the file system path that corresponds to the mount in /media/$USER
32  sudo umount /dev/sdX
33. Make a copy of the drive on your PC:
34. sudo dd if=/dev/sdX of=ubuntu-master-22.04-YYYYMMDD.img bs=16M conv=fsync
35. This takes several minutes.
36. When the copy finishes (with a line about MB/s) you can pull out UBUNTU MASTER and put it on your shelf.
37. You're now ready to make an "EDGER MASTER" thumbdrive

If you have an UBUNTU MASTER thumbdrive disk image file ubuntu-master-22.04-YYYYMMDD.img image file start here:

1. Insert a thumbdrive in the fastest USB socket you've got on your PC: ideally version 3.1 or 3.0. It will take a VERY LONG TIME with a USB 2 socket.
2. Close the file manager window
3. df
4. Note the drive path along side the /media/$USER pathname as /dev/sdX1 and unmount this:
5. sudo umount /dev/sdX1
6. Now copy the image file to the new thumbdrive. 
7. sudo time dd if=ubuntu-master-22.04-YYYYMMDD.img of=/dev/sdb bs=16M conv=fsync
8. Remove and label the thumbdrive "EDGER MASTER" 
9. Put "EDGER MASTER" back in, boot it and use the usual "open in terminal" for a shell session.
10. Install the prerequisite Linux packages (this takes several minutes):
11. sudo apt-get install git wget flex bison gperf python3 python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0 python3-pip curl
14. Make directories:
15. cd
16. mkdir workspace workspace/esp32 esp
14. Copy the v4.4 Espressif esp-idf repo:
15. cd $HOM$/esp
16. scp -r yourusernameonyourpc@yourpc:esporwherever/esp-idf-v4.4 esp-idf
17. cd esp-idf
18. Install the esp-idf. This takes a long time. 
19. . ./install.sh
20. Set up the idf.py tool to do a test build. This takes a long time.
20. . ./export.sh
21. cd $HOME/workspace/esp32
22. Copy in the Edger repository
23. scp -r yourusernameonyhourpc@yourpc:repoorwherever/edger edger
24. cd edger/ant
25. Add user edger to "dialout" group.
26. sudo addgroup edger dialout
27. Log out and log back in to apply group change
28. Install nvm:
29. curl https://raw.githubusercontent.com/creationix/nvm/master/install.sh 2>/dev/null | bash >/tmp/log 2>&1
30. Check for errors:
31. cat /tmp/log
32. Install npm:
33. export NVM_DIR=$HOME/.nvm
34. . $NVM_DIR/nvm.sh
35. nvm install 14
36. install pnpm. This takes several minutes.
37. npm add -g @pnpm/exe
38. cd $HOME/workspace/esp32/node/aardvark
39. pnpm install
40. Create a local bin directory for the desktop icon commands:
41. mkdir $HOME/bin
42. Add this to search rules by editing $HOME/.bashrc and adding the line: export PATH="$PATH:$HOME/bin"
43. Add the desktop and $HOME/bin files: 
44. cd $HOME
45. scp -r yourusernameonyhourpc@yourpc:repoorwherever/edger/tools/thumbdrive/home/bin .
46. scp -r yourusernameonyhourpc@yourpc:repoorwherever/edger/tools/thumbdrive/home/Desktop .
47. Double click the "change wifi" icon and set the SSID/password values to match the host environment and rebuild ant. The Edger dev board must be plugged into the USB for this.
48. Double click the "start aardvark" icon to start the Aardvark server
49. Double click the "start browser" icon to start the Edger system
50. Check for correct operation.
51. Shut the system down and reboot the host PC
52. Copy EDGER MASTER to edger-master-YYYYMMDD.img
53. sudo dd if=/dev/sdX of=edger-master-YYMMDD.img bs=16M conv=fsync

If you have an EDGER MASTER thumbdrive image file start here:

1. To make duplicates of EDGER MASTER insert as many blank thumbdrives into a fast USB hub as will be duplicated at one time.
2. Use df to find the drive letters and unmount all the thumbdrives
3. For each thumbdrive:
4. sudo dd if=edger-master-YYMMDD.img of=/dev/sdX bs=16M conv=fsync &
5. The trailing "&" causes the dd to execute in the background.
6. After the last dd command use this command to wait until all the copies are done:
7. wait
8. Because the thumbdrive write speed is so slow several drives can be duplicated at the same time in the elapsed time a single copy takes. With a relatively fast PC the copies onto Samsung thumbdrives should take about a half hour.
