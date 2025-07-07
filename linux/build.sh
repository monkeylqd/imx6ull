#!/bin/bash

if [ "$_" != "$0" ];	then
	_exit="return"
else
	_exit="exit"
fi

export PATH=/home/lqd/imx6ull/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/:${PATH}
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

if [ $# -gt 0 ]; then
    # make distclean
    # rm -rf out
    make O=out ebyte_imx6ull_defconfig || ${_exit}
    # make O=out imx_v7_defconfig || ${_exit}

    # make O=out menuconfig || ${_exit}
    # make O=out savedefconfig || ${_exit}
fi

# build all kernel
make LOCALVERSION= O=out -j $(($(nproc)-1)) || ${_exit}
scripts/clang-tools/gen_compile_commands.py -d out/

# build modules and tar as modules.tar.bz2
make O=out modules_install INSTALL_MOD_PATH=. -j $(($(nproc)-1)) > /dev/null || ${_exit}
cd out/lib/ || ${_exit}
rm modules/*/build modules/*/source
tar -caf ../modules.tar.bz2 modules
cd - || ${_exit}
exit
# cp output file
cp out/arch/arm/boot/zImage /srv/tftp/
cp out/arch/arm/boot/dts/ebyte-imx6ull-*.dtb /srv/tftp/

cp out/arch/arm/boot/zImage /mnt/hgfs/share/tools/imx6ULx/uuu/files/
cp out/arch/arm/boot/dts/ebyte-imx6ull-*.dtb /mnt/hgfs/share/tools/imx6ULx/uuu/files/

cp out/arch/arm/boot/zImage ../image/files/
cp out/arch/arm/boot/dts/ebyte-imx6ull-*.dtb ../image/files/
cp out/modules.tar.bz2 ../image/rootfs/

# dtc -I dtb -O dts -o out-ebyte.dts out/arch/arm/boot/dts/ebyte-imx6ull-emmc.dtb
