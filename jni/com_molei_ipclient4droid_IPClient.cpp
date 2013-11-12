#include "com_molei_ipclient4droid_IPClient.h"
#include "ipClientPack.h"

// jstring To char*
char* jstringTostring(JNIEnv* env, jstring jstr) {
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}

JNIEXPORT jbyteArray JNICALL Java_com_molei_ipclient4droid_IPClient_Send05(
		JNIEnv *env, jobject thiz) {
	ipClientPack pack;
	char *ptr = (char*) &pack;
	Send_0x05(&pack);
	int len = sizeof(char[500]);
	ptr = ptr + sizeof(char*);
	jbyteArray ret = env->NewByteArray(len);
	env->SetByteArrayRegion(ret, 0, len, (jbyte*) ptr);
	return ret;

}

JNIEXPORT jbyteArray JNICALL Java_com_molei_ipclient4droid_IPClient_Recv06(
		JNIEnv *env, jobject thiz, jbyteArray data) {
	ipClientPack *pack;
	pack = (ipClientPack*) env->GetByteArrayElements(data, 0);
	char* info;
	info = Recv_0x06(pack);
	int len = sizeof(info);
	jbyteArray ret = env->NewByteArray(len);
	env->SetByteArrayRegion(ret, 0, len, (jbyte*) info);
	return ret;

}
JNIEXPORT jbyteArray JNICALL Java_com_molei_ipclient4droid_IPClient_Send1F(
		JNIEnv *env, jobject thiz, jstring username) {

	char password[32];

	/////convert jstring to char*
	char* p = jstringTostring(env, username);

	/////////////////////
	ipClientPack pack(p, password);

	char *ptr = (char*) &pack;
	Send_0x1F(&pack);
	int len = sizeof(char[300]);
	ptr = ptr + sizeof(char*);
	jbyteArray ret = env->NewByteArray(len);
	env->SetByteArrayRegion(ret, 0, len, (jbyte*) ptr);
	return ret;
}

JNIEXPORT jbyteArray JNICALL Java_com_molei_ipclient4droid_IPClient_Send21(
		JNIEnv *env, jobject thiz, jstring username, jstring password,
		jbyteArray key) {
	char* ptr_username = jstringTostring(env, username);
	char* ptr_password = jstringTostring(env, password);
	unsigned short *k;
	k = (unsigned short*) env->GetByteArrayElements(key, NULL);

	ipClientPack pack(ptr_username, ptr_password, k);
	char *ptr = (char*) &pack;
	Send_0x21(&pack);
	int len = sizeof(char[300]);
	ptr = ptr + sizeof(char*);
	jbyteArray ret = env->NewByteArray(len);
	env->SetByteArrayRegion(ret, 0, len, (jbyte*) ptr);
	return ret;

}
JNIEXPORT jbyteArray JNICALL Java_com_molei_ipclient4droid_IPClient_Send_11e
  (JNIEnv *env, jobject thiz, jstring username, jbyteArray key){
	char* ptr_username = jstringTostring(env, username);
	char password[32];
	unsigned short *k;
	k = (unsigned short*) env->GetByteArrayElements(key, NULL);
	ipClientPack pack(ptr_username,password,k);
	char *ptr = (char*) &pack;
	Send_0x1e(&pack);
	int len = sizeof(char[500]);
	ptr = ptr + sizeof(char*);
	jbyteArray ret = env->NewByteArray(len);
	env->SetByteArrayRegion(ret, 0, len, (jbyte*) ptr);
	return ret;
}

