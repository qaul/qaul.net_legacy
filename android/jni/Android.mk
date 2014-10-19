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
	../../libqaul/qaullib_webclient.c ../../libqaul/qaullib_threads.c \
	../../libqaul/qaullib_user.c ../../libqaul/qaullib_user_LL.c \
	../../libqaul/qaullib_filesharing.c ../../libqaul/qaullib_file_LL.c \
	../../libqaul/qaullib_exediscovery.c \
	../../libqaul/qaullib_crypto.c \
	../../libqaul/qaullib_udp_communication.c \
	../../libqaul/captive/qaullib_captive.c ../../libqaul/captive/qaullib_captive_dhcp.c ../../libqaul/captive/qaullib_captive_dns.c \
	../../libqaul/mongoose/mongoose.c ../../libqaul/sqlite/sqlite3.c \
	../../libqaul/urlcode/urlcode.c ../../libqaul/polarssl/sha1.c \
	../../libqaul/olsrd/mantissa.c ../../libqaul/olsrd/hashing.c 
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../libqaul
LOCAL_CFLAGS := \
	-I$(LOCAL_PATH)/../../pjproject_android/pjsip/include \
	-I$(LOCAL_PATH)/../../pjproject_android/pjlib/include \
	-I$(LOCAL_PATH)/../../pjproject_android/pjlib-util/include \
	-I$(LOCAL_PATH)/../../pjproject_android/pjmedia/include \
	-I$(LOCAL_PATH)/../../pjproject_android/pjnath/include
LOCAL_STATIC_LIBRARIES := \
    pjsua \
    pjsip-ua \
    pjsip-simple \
	pjsip \
    pjsdp \
	pjmedia-audiodev \
	pjmedia-codec \
	pjmedia \
    pjmedia-videdev \
    pjnath \
	pjlib-util \
	resample \
    milenage \
    srtp \
	gsmcodec \
	speex \
    ilbccodec \
    g7221codec \
    pj 

# add logging for debugging
LOCAL_LDLIBS := \
	-L$(SYSROOT)/usr/lib -llog \
    -lOpenSLES
    
include $(BUILD_SHARED_LIBRARY)

# include externally built libraries
include $(CLEAR_VARS)
LOCAL_MODULE            := pjsip 
LOCAL_SRC_FILES         := ../../pjproject_android/pjsip/lib/libpjsip-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjsip/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pjsip-simple
LOCAL_SRC_FILES         := ../../pjproject_android/pjsip/lib/libpjsip-simple-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjsip/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pjsip-ua 
LOCAL_SRC_FILES         := ../../pjproject_android/pjsip/lib/libpjsip-ua-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjsip/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pjsua 
LOCAL_SRC_FILES         := ../../pjproject_android/pjsip/lib/libpjsua-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjsip/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pjmedia-videodev 
LOCAL_SRC_FILES         := ../../pjproject_android/pjmedia/lib/libpjmedia-videodev-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjmedia/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pjmedia-codec 
LOCAL_SRC_FILES         := ../../pjproject_android/pjmedia/lib/libpjmedia-codec-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjmedia/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pjmedia
LOCAL_SRC_FILES         := ../../pjproject_android/pjmedia/lib/libpjmedia-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjmedia/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pjmedia-audiodev
LOCAL_SRC_FILES         := ../../pjproject_android/pjmedia/lib/libpjmedia-audiodev-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjmedia/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pjlib-util
LOCAL_SRC_FILES         := ../../pjproject_android/pjlib-util/lib/libpjlib-util-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjlib-util/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := speex
LOCAL_SRC_FILES         := ../../pjproject_android/third_party/lib/libspeex-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/third_party/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := ilbccodec
LOCAL_SRC_FILES         := ../../pjproject_android/third_party/lib/libilbccodec-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/third_party/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := resample
LOCAL_SRC_FILES         := ../../pjproject_android/third_party/lib/libresample-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/third_party/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := milenage
LOCAL_SRC_FILES         := ../../pjproject_android/third_party/lib/libmilenage-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/third_party/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := gsmcodec
LOCAL_SRC_FILES         := ../../pjproject_android/third_party/lib/libgsmcodec-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/third_party/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := g7221codec
LOCAL_SRC_FILES         := ../../pjproject_android/third_party/lib/libg7221codec-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/third_party/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := srtp
LOCAL_SRC_FILES         := ../../pjproject_android/third_party/lib/libsrtp-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/third_party/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pjnath
LOCAL_SRC_FILES         := ../../pjproject_android/pjnath/lib/libpjnath-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjnath/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pj
LOCAL_SRC_FILES         := ../../pjproject_android/pjlib/lib/libpj-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjlib/include
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE            := pjsdp
LOCAL_SRC_FILES         := ../../pjproject_android/pjmedia/lib/libpjsdp-arm-unknown-linux-androideabi.a
LOCAL_EXPORT_C_INCLUDES := ../../pjproject_android/pjmedia/include
include $(PREBUILT_STATIC_LIBRARY)

# libnativetask.so
include $(CLEAR_VARS)
LOCAL_MODULE    := libnativetask
LOCAL_SRC_FILES := android_tether_system_NativeTask.c 
include $(BUILD_SHARED_LIBRARY)

