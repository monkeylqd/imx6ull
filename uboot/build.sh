#!/bin/bash

if [ "$_" != "$0" ];	then
	_exit="return"
else
	_exit="exit"
fi

export PATH=${HOME}/.local/share/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/:${PATH}
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

function init_config() {
    # make distclean
    rm -rf out
    if which bear; then
        gen_compile_commands="bear --"
    fi

    make O=out "$1" || ${_exit}

    # make O=out menuconfig || ${_exit}
    # make O=out savedefconfig || ${_exit}
}

if [ $# -gt 0 ]; then
    if [ "$1" = "n" ]; then
        init_config ebyte_imx6ull_nand_defconfig    # DDR 256M
    elif [ "$1" = "e" ]; then
        init_config ebyte_imx6ull_emmc_defconfig    # DDR 512M
    else
        echo "Usage: $0 [n|e]"
        echo "n: build for NAND boot"
        echo "e: build for EMMC boot"
        echo "no arg: just build, not configed"
        ${_exit}
    fi
fi

${gen_compile_commands} make O=out -j $(($(nproc)-1)) || ${_exit}

if grep -q "CONFIG_NAND_BOOT=y" out/.config; then
    output_file="u-boot-nand.imx"
else
    output_file="u-boot-emmc.imx"
fi

if grep -q "CONFIG_SD_BOOT=y" out/.config; then
    output_file="SD_${output_file}"
fi

cp out/u-boot-dtb.imx ./${output_file}

# 如果optput_file是以SD_开头，则执行拷贝文件到image/files目录
if [[ ${output_file:0:3} == "SD_" ]]; then
    mv ./${output_file} ../image/files/
else
    mv ./${output_file} /mnt/hgfs/share/tools/imx6ULx/uuu/files
fi
