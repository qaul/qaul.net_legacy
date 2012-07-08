/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "net_qaul_qaul_NativeQaul.h"
#include "../../libqaul/qaullib.h"
#include "../../libqaul/qaullib_threads.h"
#include <android/log.h>

// global variables we need to call java
JNIEnv *global_v_env;
jobject global_v_obj;

void *Qaullib_CheckerThread(void)
{
	int mycounter = 0;
	while (1)
	{
		// check sockets
		Qaullib_TimedSocketReceive();

		// user discovery
		if(mycounter == 10)
		{
			Qaullib_IpcSendCom(1);
		}
		else if (mycounter >= 100)
		{
			Qaullib_TimedDownload();
			mycounter = 0;
		}
		mycounter++;

		// wait until we check again
		usleep(100000);
	}
}


// ---------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_net_qaul_qaul_NativeQaul_libInit
  (JNIEnv *env, jobject obj, jstring resourcePath)
{
	const char *myPath;
	myPath = (*env)->GetStringUTFChars(env, resourcePath, 0);
	Qaullib_Init(myPath);

	// configure qaullib
	Qaullib_SetConfQuit();
}

JNIEXPORT void JNICALL Java_net_qaul_qaul_NativeQaul_libExit
  (JNIEnv *env, jobject obj)
{
	Qaullib_Exit();
}

JNIEXPORT jint JNICALL Java_net_qaul_qaul_NativeQaul_webserverStart
  (JNIEnv *env, jobject obj)
{
	return (jint) Qaullib_WebserverStart();
}

JNIEXPORT void JNICALL Java_net_qaul_qaul_NativeQaul_configStart
  (JNIEnv *env, jobject obj)
{
	Qaullib_ConfigStart();
}

JNIEXPORT jint JNICALL Java_net_qaul_qaul_NativeQaul_captiveStart
  (JNIEnv *env, jobject obj)
{
	return (jint) Qaullib_CaptiveStart();
}

JNIEXPORT jint JNICALL Java_net_qaul_qaul_NativeQaul_ipcConnect
  (JNIEnv *env, jobject obj)
{
	return (jint) Qaullib_IpcConnect();
}

JNIEXPORT jint JNICALL Java_net_qaul_qaul_NativeQaul_ipcClose
  (JNIEnv *env, jobject obj)
{
	return (jint) Qaullib_IpcClose();
}

JNIEXPORT void JNICALL Java_net_qaul_qaul_NativeQaul_ipcSendCom
  (JNIEnv *env, jobject obj, jint commandId)
{
	Qaullib_IpcSendCom(commandId);
}

JNIEXPORT jstring JNICALL Java_net_qaul_qaul_NativeQaul_getIP
  (JNIEnv *env, jobject object)
{
	jstring myJString;
	myJString = (*env)->NewStringUTF(env, Qaullib_GetIP());
	return myJString;
}

JNIEXPORT jint JNICALL Java_net_qaul_qaul_NativeQaul_existsUsername
  (JNIEnv *env, jobject object)
{
	return (jint) Qaullib_ExistsUsername();
}

JNIEXPORT void JNICALL Java_net_qaul_qaul_NativeQaul_configurationFinished
  (JNIEnv *env, jobject obj)
{
	Qaullib_ConfigurationFinished();
	// start timers
	qaullib_pthread_start((qaullib_thread_func_t) Qaullib_CheckerThread, NULL);
}

JNIEXPORT void JNICALL Java_net_qaul_qaul_NativeQaul_filePicked
  (JNIEnv *env, jobject obj, jint check, jstring path)
{
	const char *myPath;
	myPath = (*env)->GetStringUTFChars(env, path, 0);
	__android_log_print(ANDROID_LOG_DEBUG, "qaullib", "file picked %s\n", myPath);
	Qaullib_FilePicked((int)check, myPath);
	(*env)->ReleaseStringUTFChars(env, path, myPath);
}

JNIEXPORT jstring JNICALL Java_net_qaul_qaul_NativeQaul_getAppEventOpenPath
  (JNIEnv *env, jobject obj)
{
	jstring myString;
	myString = (*env)->NewStringUTF(env, Qaullib_GetAppEventOpenPath());
	return myString;
}

JNIEXPORT jint JNICALL Java_net_qaul_qaul_NativeQaul_timedCheckAppEvent
  (JNIEnv *env, jobject obj)
{
	int event = Qaullib_TimedCheckAppEvent();
	return (jint) event;
}

JNIEXPORT void JNICALL Java_net_qaul_qaul_NativeQaul_timedSocketReceive
  (JNIEnv *env, jobject obj)
{
	Qaullib_TimedSocketReceive();
}

JNIEXPORT void JNICALL Java_net_qaul_qaul_NativeQaul_timedDownload
  (JNIEnv *env, jobject obj)
{
	Qaullib_IpcSendCom(1);
	Qaullib_TimedDownload();
}
