LOCAL_PATH := $(call my-dir)

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
	 ../../libqaul/qaullib.c ../../libqaul/qaullib_user.c ../../libqaul/qaullib_ipc.c ../../libqaul/qaullib_webserver.c \
     ../../libqaul/qaullib_webclient.c ../../libqaul/qaullib_filesharing.c ../../libqaul/qaullib_threads.c \
     ../../libqaul/qaullib_user_LL.c \
     ../../libqaul/captive/qaullib_captive.c ../../libqaul/captive/qaullib_captive_dhcp.c ../../libqaul/captive/qaullib_captive_dns.c \
     ../../libqaul/mongoose/mongoose.c ../../libqaul/sqlite/sqlite3.c \
     ../../libqaul/urlcode/urlcode.c ../../libqaul/bstrlib/bstrlib.c ../../libqaul/polarssl/sha1.c \
     ../../libqaul/olsrd/mantissa.c ../../libqaul/olsrd/hashing.c 
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../libqaul
# add logging for debugging
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
include $(BUILD_SHARED_LIBRARY)

# libnativetask.so
include $(CLEAR_VARS)
LOCAL_MODULE    := libnativetask
LOCAL_SRC_FILES := android_tether_system_NativeTask.c 
include $(BUILD_SHARED_LIBRARY)
