#include "HelloWorld.h"

JNIEXPORT void JNICALL
Java_HelloWorld_myprint(JNIEnv *env, jobject obj, jint c)
{
	printf("C++ print res = %d !\n",c);
	int a[5] = {1,2,3,4,5};
	for (auto obj : a)
	{
		printf("%d \n",obj + c);
	}
}

JNIEXPORT jint JNICALL 
Java_HelloWorld_add(JNIEnv *env, jobject obj, jint a, jint b)
{
	auto res = a + b;
	return res;
}
