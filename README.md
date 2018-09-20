# Disk Sleep bug test kernel module
This is a kernel module that is a driver for a character device that will block when reading, until a write() happens.

# Building
Edit the Makefile to point to your kernel sources and run make.

# Running
Find the chardev.ko file. In a second terminal run `dmesg -w`
Run `insmod chardev.ko`, in the dmesg terminal find the major device number that was allocated (on my pc: 239)
Run `mknod chr c $MAJOR_NUMBER 0`
you can then `cat chr` which will block until you `echo 1 > chr`


# Preliminary results

Docker itself seems to hang when processes in D state are running inside.
to test : `docker run --name chartest -it --rm --device $PWD/chr:/root/chr fedora:28 bash`
```
# cat /root/chr &
[1] (16)
# 
```

And then `docker kill chartest`.
this will hang until you `echo 1 > chr`.