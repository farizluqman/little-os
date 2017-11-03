# /bin/bash
BUILD_DIR="build"
MOUNT_DIR="/mnt/out0"
BOOTLOADER_DIR="bootloader"

DISK_NAME="disk.img"
DISK_IMG_SIZE="100MB"

echo "$(tput setaf 3)Creating a 100MB disk image... $(tput sgr0)"
# create a 100 MB disk image
dd if=/dev/zero of=${BUILD_DIR}/${DISK_NAME} seek=${DISK_IMG_SIZE} count=1k bs=1

echo "$(tput setaf 3)Setting up loop device... $(tput sgr0)"

# setup loopback device and reserve it for this process
outlo=`sudo losetup -f --show build/disk.img`

echo "$(tput setaf 3)We are using '${outlo}' as our loop device now $(tput sgr0)"

# create MSDOS table for the disk
sudo parted ${outlo} mklabel msdos

# add a new primary partition starting from 32k to the end
sudo parted ${outlo} mkpart primary ext2 32k 100% -a minimal

# set the partition as active/bootable
sudo parted ${outlo} set 1 boot on

# create a new block device (/dev/loopXp1)
sudo partx -a ${outlo}

# format the partition with ext2
sudo mke2fs ${outlo}p1

# create a temporary mount point
sudo mkdir -p ${MOUNT_DIR}

# mount the block device with the new filesystem to the mount point we created
sudo mount -t ext2 ${outlo}p1 ${MOUNT_DIR}

# create a new directory for grub2 files
sudo mkdir -p ${MOUNT_DIR}/boot/grub

# copy grub configurations
sudo cp ${BOOTLOADER_DIR}/grub.cfg ${MOUNT_DIR}/boot/grub/

echo "$(tput setaf 3)Installing the boot loader... $(tput sgr0)"

# install the grub bootloader
sudo grub-install --boot-directory=${MOUNT_DIR}/boot/ --modules="ext2 part_msdos" ${outlo}

echo "$(tput setaf 3)Copying kernel to the disk $(tput sgr0)"

cp build/kernel ${MOUNT_DIR}/boot/kernel-701

echo "$(tput setaf 3)Sleeping for 2 seconds... $(tput sgr0)"

sleep 2

echo "$(tput setaf 3)Unmount and remove the mount point... $(tput sgr0)"
# remove this if you need to modify something
sudo umount ${MOUNT_DIR}
sudo partx -d ${outlo}p1
sudo losetup -d ${outlo}
rm -rf ${MOUNT_DIR}

echo "$(tput setaf 3)Everything is done now with success! $(tput sgr0)"

return 0
