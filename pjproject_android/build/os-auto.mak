# build/os-auto.mak.  Generated from os-auto.mak.in by configure.

export OS_CFLAGS   := $(CC_DEF)PJ_AUTOCONF=1  -I/home/sojus/src/android/android-ndk-r8/platforms/android-8/arch-arm/usr/include -DPJ_IS_BIG_ENDIAN=0 -DPJ_IS_LITTLE_ENDIAN=1

export OS_CXXFLAGS := $(CC_DEF)PJ_AUTOCONF=1  -I/home/sojus/src/android/android-ndk-r8/platforms/android-8/arch-arm/usr/include  -shared --sysroot=/home/sojus/src/android/android-ndk-r8/platforms/android-8/arch-arm

export OS_LDFLAGS  :=  -nostdlib -L/home/sojus/src/android/android-ndk-r8/platforms/android-8/arch-arm/usr/lib/ -lm  -lc -lgcc -lOpenSLES -llog -lgcc

export OS_SOURCES  := 


