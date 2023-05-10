# IRIS framework

This directory includes the Xen patch for the *IRIS* framework. In the following, we described all steps to install the framework and perform record and replay sessions.


## 1. System requirements and software dependencies

The testbed can be installed on a physical or virtual machine having the following hardware configuration:

- Intel CPU with VT-x enabled
- VT-x preemption timer support 
- \> 20 GB HHD

Before installing Xen you first need to install your *Dom0* OS kernel. We chose ``Debian 11``, but any other [DOM0 kernel supported by Xen](https://wiki.xenproject.org/wiki/Dom0_Kernels_for_Xen) could be chosen. We note that in such a case, the dependencies needed may be different from those given below. If you need, you can find a more detailed guide on Debian installation here: [https://www.debian.org/releases/stable/amd64/](https://www.debian.org/releases/stable/amd64/)

### Build dependencies

Once installed and run Debian 11, you need to install the necessary dependencies for Xen:

```
apt-get install build-essential;
apt-get install bcc bin86 gawk bridge-utils iproute2 libcurl4 libcurl4-openssl-dev bzip2 transfig tgif kmod;
apt-get install texinfo texlive-latex-base texlive-latex-recommended texlive-fonts-extra texlive-fonts-recommended libpci-dev mercurial; 
apt-get install make gcc cmake libc6-dev zlib1g-dev python python-dev libncurses5-dev patch libvncserver-dev libsdl-dev libjpeg62-turbo-dev;
apt-get install iasl libbz2-dev e2fslibs-dev git-core uuid-dev ocaml ocaml-findlib ocaml-nox libx11-dev bison flex xz-utils libyajl-dev; 
apt-get install gettext libpixman-1-dev libaio-dev markdown pandoc figlet bridge-utils bison flex;
apt-get install libc6-dev-i386 libnl-3-200 libnl-3-dev liblzma-dev libbz2-dev liblzo2-dev libzstd-dev;
apt-get install ninja-build checkpolicy valgrind;
apt-get install zlib1g-dev python-dev libncurses-dev libssl-dev xorg-dev uuid-dev uuid libyajl-dev;
apt-get install libaio-dev libglib2.0-dev libpixman-1-dev pkg-config iasl;
apt install python3-dev;
```

## 2. Patching Xen

### Obtaining the Xen Project Source Code

To obtain the Xen source code you can clone it from the Git source code repository. The prototype is based on Xen *stable-4.16 version* and must be downloaded in *Dom0*: 

```
git clone git://xenbits.xen.org/xen.git;
cd xen;
git checkout origin/stable-4.16;
```

### Patching Xen with IRIS

Before building Xen, remember to apply the patch provided in this repo by running the following command in the Xen parent folder, after moving the patch into the same parent folder:

```
patch -p0 -i xen_iris.patch
```

## 3. Building Xen from scratch

### Configure

You can apply some custom kernel configurations to control what features are built into Xen Project using menuconfig (in the Xen top folder):

```
# Replace dns nameserver with 8.8.8.8 in resolve.conf 
nano /etc/resolv.conf 

# Add ' export PATH="/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/root/bin" ' for xl commands 
nano /etc/bash.bashrc

# Reboot terminal
exit; 

mv /home/test/xen /root/

cd xen
```

```
make -C xen menuconfig
```

Test coverage support can be turned on by enabling (or disabling) the following options in menuconfig:

```
- common_features/livepatch=n 
- debugging_options/code_coverage=y (disable everything else under debugging menu)
```

To configure Xen Project, simply run the provided configure script (in the Xen top folder):

```
./configure
```


### Python Prefix and Module Layout

On some distros (e.g., Debian and Ubuntu) Xen Project may install the Python parts of the code into the wrong place (See Debian bug #693721). Therefore it is necessary to set ``PYTHON_PREFIX_ARG=--install-layout=deb``:

```
vim .config
PYTHON_PREFIX_ARG=--install-layout=deb
```

### Build and install

To build all Xen components you can use the ``make`` command. You can optionally use ``-j`` option to specify the number of jobs to run simultaneously (num_cpu + 1). 

```
make -j9 xen;
make -j9 tools;
```

Then you can use the following to install Xen:

```
make -j9 install-xen;
make -j9 install-tools;
```

### Post-Installation

After installation some configurations are needed:

- Rebuild dynamic linker cache and update bootloader 

```
echo "/usr/local/lib" > /etc/ld.so.conf.d/xen.conf;
/sbin/ldconfig;
```

- Enable Xen services:

```
update-rc.d xencommons defaults 19 18;
update-rc.d xendomains defaults 21 20;
```

- Update bootloader option:

```
sed -i 's/GRUB_DEFAULT.*/GRUB_DEFAULT=2/' /etc/default/grub;
```

- Assign *Dom0* resources and scheduler:

```
sed -i '/GRUB_DEFAULT.*/a \GRUB_CMDLINE_XEN="dom0_mem=1024M,max:1024M dom0_max_vcpus=1 dom0_vcpus_pin sched=null"' /etc/default/grub;
```

- Alternatively use the following to also enable the serial port (console redirect):

```
sed -i '/GRUB_DEFAULT.*/a \GRUB_CMDLINE_XEN_DEFAULT="dom0_mem=1024M,max:1024M dom0_max_vcpus=1 dom0_vcpus_pin sched=null console=com1 com1=115200,8n1 loglvl=all guest_loglvl=all"' /etc/default/grub;
sed -i '/GRUB_CMDLINE_XEN_DEFAULT.*/a \GRUB_CMDLINE_LINUX_XEN="console=hvc0"' /etc/default/grub;
```

- Finally, update the grub configurations and restart Xen:

```
$ update-grub
$ reboot
```

Make sure to boot the kernel patched with Xen in the grub bootloader.

## 4. *Test VM* configuration (DomU)

In order to run the record and replay sessions, it is essential to configure a guest *test VM* on the hypervisor in the HVM mode supported by Xen. 

### Setup Linux Bridge for guest networking

Assuming a primary network interface named ``ens32`` and ``xenbr0`` as Xen Linux bridge, in *Dom0*, edit the ``/etc/network/interfaces`` file as follows:

```
echo "# The loopback network interface
auto lo 
iface lo inet loopback 

# The primary network interface
auto ens32 
iface ens32 inet manual 

# The xen bridge interface
auto xenbr0 
iface xenbr0 inet dhcp
    bridge_ports ens32" > /etc/network/interfaces
```

Then restart networking:

```
service networking restart
```

### Setup LVM storage for guest

We create a virtual disk where the guest os will be installed:

```
apt-get install lvm2;
pvcreate /dev/sda4;
vgcreate vg0 /dev/sda4;
lvcreate -L 15G -n lv_hvm_guest /dev/vg0;
```

### Creating a HVM guest

Download the ``.iso`` image of the operating system you want to install in the *the VM*. To create the *test VM*, we will use the command ``xl create``. This command uses a configuration file (``hvm_configuration.cfg``) provided in the following: 

```
echo "name = \"hvm_guest\" 
builder= \"hvm\" 
memory = 1024 
vcpus = 1 
cpus= [\"2\"] 
vif = [\"bridge=xenbr0\"] 
disk = [\"phy:/dev/vg0/lv_hvm_guest,xvda,w\",\"file:./debian.iso,hdc:cdrom,r\"] 
vnc = 1 
vnclisten = '0.0.0.0' 
vncdisplay = 1 
boot = \"cd\"" > hvm_configuration.cfg
```

Finally, we create the *test VM*:

```
xl create hvm_configuration.cfg
```

To connect to the virtual console of the *test VM* you can use vnc from *Dom0*:

```
$ apt install tigervnc-viewer
$ vncviewer 0.0.0.0:1
```

## 5. How to use *IRIS*

Run ``make`` to compile ``usr_cli.c`` and obtain the ``IRIS_cli`` executable.

### Recording

Before starting a record session, launch the *test VM*. Further, to start a record session, run:

```
$ ./IRIS_cli v <num_vmexit_to_record> <test_vm_id>
```

The generated *VM seeds* will be stored in the ``seeds`` file, while code coverage data, for each VM exit, will be stored in the ``cov`` directory. Both ``seeds`` file and ``cov`` directory are created in the current directory. 

### Replaying

Unlike the record session, you do not need to start the *dummy VM*. However, you need to know its *domain id*. To get some information, Xen uses an increasing counter for launched VMs. So, after starting a domain with a *domain ID* equal to ``i``, a thumb rule can be the *dummy VM* will get ``i+1`` *domain ID*. 

To start a replay session run:

```
$ ./IRIS_cli m <num_vmexit_to_replay> <dummy_vm_id>
```

In the same working dir for ``IRIS_cli``, the ``seeds`` file should be placed. As for recording, code coverage data, for each replayed VM exit, will be stored in the ``cov`` directory.
