//
#include <jni.h>
//

#ifndef _Included_edu_bahcesehir_shapedetection_ShapeDetection
#define _Included_edu_bahcesehir_shapedetection_ShapeDetections
#ifdef __cplusplus
extern "C"{
#endif


void GetVerticalHorizontalLines(jlong matAddress);

JNIEXPORT jstring JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_nativeHello
        (JNIEnv *, jclass);

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_blackAndWhite
        (JNIEnv *, jclass, jlong matAddress);

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_detectRectangle
        (JNIEnv *, jclass, jlong matAddress);

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_detectCircle
        (JNIEnv *, jclass, jlong matAddress);

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_fillShape
        (JNIEnv *, jclass, jlong matAddress);

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_getThreshold
        (JNIEnv *, jclass, jlong matAddress);

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_getGaussian
        (JNIEnv *, jclass, jlong matAddress);

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_getCanny
        (JNIEnv *, jclass, jlong matAddress);


#ifdef __cplusplus
}
#endif
#endif
