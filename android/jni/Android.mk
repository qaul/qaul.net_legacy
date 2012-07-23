LOCAL_PATH := $(call my-dir)
JNI_PATH := $(LOCAL_PATH)

# wificonfig
# test from MeshApp, wlan_slovenia
# -> libwpa_client.so & libcutils.so pulled from phone and put into NDK
include $(CLEAR_VARS)
LOCAL_SRC_FILES := wificonfig/log.c \
				wificonfig/wifi_config.c 
LOCAL_STATIC_LIBRARIES := libedify
LOCAL_SHARED_LIBRARIES := libhardware_legacy
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib/ -lwpa_client -lcutils			  
LOCAL_MODULE := wificonfig
include $(BUILD_EXECUTABLE)

# libedify.a
include $(CLEAR_VARS)
LOCAL_SRC_FILES := edify/lex.yy.c \
	edify/parser.c \
	edify/expr.c
LOCAL_MODULE := libedify
include $(BUILD_STATIC_LIBRARY)

# hardware_legacy.so
include $(CLEAR_VARS)
LOCAL_SRC_FILES := hardware_legacy/hardware_legacy_stub.c 
LOCAL_MODULE    := libhardware_legacy
include $(BUILD_SHARED_LIBRARY)

# tether
include $(CLEAR_VARS)
LOCAL_SRC_FILES := tether/install.c \
				tether/tether.c 
LOCAL_STATIC_LIBRARIES := libedify
LOCAL_SHARED_LIBRARIES := libhardware_legacy
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib/ -l cutils			  
#LOCAL_LDLIBS += -L$(NDK_ARCH)/usr/lib/ -l cutils			  
LOCAL_MODULE := tether
include $(BUILD_EXECUTABLE)

# libqaul.so
include $(CLEAR_VARS)
LOCAL_MODULE := libqaul
LOCAL_SRC_FILES := net_qaul_qaul_NativeQaul.c \
	../../libqaul/qaullib.c ../../libqaul/qaullib_ipc.c ../../libqaul/qaullib_webserver.c ../../libqaul/qaullib_voip.c \
	../../libqaul/qaullib_webclient.c ../../libqaul/qaullib_filesharing.c ../../libqaul/qaullib_threads.c \
	../../libqaul/qaullib_user.c ../../libqaul/qaullib_user_LL.c \
	../../libqaul/captive/qaullib_captive.c ../../libqaul/captive/qaullib_captive_dhcp.c ../../libqaul/captive/qaullib_captive_dns.c \
	../../libqaul/mongoose/mongoose.c ../../libqaul/sqlite/sqlite3.c \
	../../libqaul/urlcode/urlcode.c ../../libqaul/bstrlib/bstrlib.c ../../libqaul/polarssl/sha1.c \
	../../libqaul/olsrd/mantissa.c ../../libqaul/olsrd/hashing.c 
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../libqaul
LOCAL_CFLAGS := \
	-I$(LOCAL_PATH)/../../pjproject_android/pjsip/include \
	-I$(LOCAL_PATH)/../../pjproject_android/pjlib/include \
	-I$(LOCAL_PATH)/../../pjproject_android/pjlib-util/include \
	-I$(LOCAL_PATH)/../../pjproject_android/pjmedia/include \
	-I$(LOCAL_PATH)/../../pjproject_android/pjnath/include

LOCAL_STATIC_LIBRARIES := \
	pjsip \
	pjlib \
	pjlib-util \
	pjmedia \
	pjnath \
	resample \
	speex \
	gsm 

#LOCAL_LDLIBS += \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libpjsip.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libpjlib.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libpjlib-util.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libpjmedia.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libpjnath.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libresample.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libspeex.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libsrtp.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libzrtp4pj.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libgsm.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libpj_g729_codec.a \
#	-l/home/sojus/src/meshnet/qaul.net/android/obj/local/armeabi/libpj_amr_stagefright_codec.a

# add logging for debugging
LOCAL_LDLIBS := \
	-L$(SYSROOT)/usr/lib -llog 
include $(BUILD_SHARED_LIBRARY)

# build sip libraries
# Include submodules
include $(JNI_PATH)/pjsip/android_toolchain/Android.mk
include $(JNI_PATH)/silk/android_toolchain/Android.mk


# libnativetask.so
include $(CLEAR_VARS)
LOCAL_MODULE    := libnativetask
LOCAL_SRC_FILES := android_tether_system_NativeTask.c 
include $(BUILD_SHARED_LIBRARY)
