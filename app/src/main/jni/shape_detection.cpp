#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "shape_detection.h"
#include <string>
#include <vector>

using namespace std;
using namespace cv;

bool acceptLinePair(Vec2f line1, Vec2f line2, float minTheta) {
    float theta1 = line1[1], theta2 = line2[1];

    if (theta1 < minTheta) {
        theta1 += CV_PI; // dealing with 0 and 180 ambiguities...
    }

    if (theta2 < minTheta) {
        theta2 += CV_PI; // dealing with 0 and 180 ambiguities...
    }

    return abs(theta1 - theta2) > minTheta;
}

vector <Point2f> lineToPointPair(Vec2f line) {
    vector <Point2f> points;

    float r = line[0], t = line[1];
    double cos_t = cos(t), sin_t = sin(t);
    double x0 = r * cos_t, y0 = r * sin_t;
    double alpha = 1000;

    points.push_back(Point2f(x0 + alpha * (-sin_t), y0 + alpha * cos_t));
    points.push_back(Point2f(x0 - alpha * (-sin_t), y0 - alpha * cos_t));

    return points;
}

Point2f computeIntersect(Vec2f line1, Vec2f line2) {
    vector <Point2f> p1 = lineToPointPair(line1);
    vector <Point2f> p2 = lineToPointPair(line2);

    float denom =
            (p1[0].x - p1[1].x) * (p2[0].y - p2[1].y) - (p1[0].y - p1[1].y) * (p2[0].x - p2[1].x);
    Point2f intersect(((p1[0].x * p1[1].y - p1[0].y * p1[1].x) * (p2[0].x - p2[1].x) -
                       (p1[0].x - p1[1].x) * (p2[0].x * p2[1].y - p2[0].y * p2[1].x)) / denom,
                      ((p1[0].x * p1[1].y - p1[0].y * p1[1].x) * (p2[0].y - p2[1].y) -
                       (p1[0].y - p1[1].y) * (p2[0].x * p2[1].y - p2[0].y * p2[1].x)) / denom);

    return intersect;
}


JNIEXPORT jstring JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_nativeHello
        (JNIEnv *jenv, jclass) {

    return jenv->NewStringUTF("Fenerbahce.");
}

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_blackAndWhite
        (JNIEnv *, jclass, jlong matAddress) {

    Mat *rgb = (Mat *) matAddress;
    cvtColor(*rgb, *rgb, CV_RGB2GRAY);
    return true;

}

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_detectRectangle
        (JNIEnv *, jclass, jlong matAddress) {

    Mat *rgb = (Mat *) matAddress;


    Mat occludedSquare8u;
    cvtColor(*rgb, occludedSquare8u, CV_BGR2GRAY);

    Mat thresh;
    threshold(occludedSquare8u, thresh, 100.0, 255.0, THRESH_BINARY);

    GaussianBlur(thresh, thresh, Size(7, 7), 2.0, 2.0);


    Mat edges;
    Canny(thresh, edges, 66.0, 133.0, 3);


    vector <Vec2f> lines;
    HoughLines(edges, lines, 1, CV_PI / 180, 150, 0, 0);


    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];

        if (theta > CV_PI / 180 * 172 || theta < CV_PI / 180 * 8) {
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a * rho, y0 = b * rho;
            pt1.x = cvRound(x0 + 1000 * (-b));
            pt1.y = cvRound(y0 + 1000 * (a));
            pt2.x = cvRound(x0 - 1000 * (-b));
            pt2.y = cvRound(y0 - 1000 * (a));
            line(*rgb, pt1, pt2, Scalar(0, 10, 100), 2, 0);
        }
        else if (theta > CV_PI / 180 * 82 && theta < CV_PI / 180 * 98) {
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a * rho, y0 = b * rho;
            pt1.x = cvRound(x0 + 2000 * (-b));
            pt1.y = cvRound(y0 + 1000 * (a));
            pt2.x = cvRound(x0 - 2000 * (-b));
            pt2.y = cvRound(y0 - 1000 * (a));
            line(*rgb, pt1, pt2, Scalar(0, 10, 100), 2, 0);
        }
    }


/*
    vector <Vec2f> selectedLines;

    vector <Point2f> intersections;
    for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = 0; j < lines.size(); j++) {
            Vec2f line1 = lines[i];
            Vec2f line2 = lines[j];
            if (acceptLinePair(line1, line2, CV_PI / 32)) {
                Vec2f l1 = lines[i];
                selectedLines.push_back(l1);
                //selectedLines.push_back(line2);
                //Point2f intersection = computeIntersect(line1, line2);
                //intersections.push_back(intersection);
            }
        }

    }




/*
    if (intersections.size() > 0) {
        vector<Point2f>::iterator i;
        for (i = intersections.begin(); i != intersections.end(); ++i) {
            circle(*rgb, *i, 1, Scalar(0, 255, 0), 3);
        }
    }

    */

    return true;

}


JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_detectCircle
        (JNIEnv *, jclass, jlong matAddress) {

    Mat *rgb = (Mat *) matAddress;
    Mat gray;

    cvtColor(*rgb, gray, CV_BGR2GRAY);

    GaussianBlur(gray, gray, Size(9, 9), 2, 2);
    vector <Vec3f> circles;

    HoughCircles(gray, circles, CV_HOUGH_GRADIENT,
                 2, gray.rows / 4, 200, 100);

    for (size_t i = 0; i < circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // draw the circle center
        circle(*rgb, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        // draw the circle outline
        circle(*rgb, center, radius, Scalar(0, 0, 255), 3, 8, 0);
    }


}

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_fillShape
        (JNIEnv *, jclass, jlong matAddress) {

    Mat *rgb = (Mat *) matAddress;


    Mat occludedSquare8u;
    cvtColor(*rgb, occludedSquare8u, CV_BGR2GRAY);

    Mat thresh;
    threshold(occludedSquare8u, thresh, 100.0, 255.0, THRESH_BINARY);

    GaussianBlur(thresh, thresh, Size(7, 7), 2.0, 2.0);


    Mat edges;
    Canny(thresh, edges, 66.0, 133.0, 3);



    vector <vector<Point> > contours;
    vector <Vec4i> hierarchy;

    Mat src_copy = edges.clone();

    findContours(src_copy, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    int idx = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        Scalar color( rand()&255, rand()&255, rand()&255 );
        drawContours( *rgb, contours, idx, color, CV_FILLED, 8, hierarchy );
    }


    /// Calculate the distances to the contour
    Mat raw_dist(rgb->size(), CV_32FC1);

    for (int j = 0; j < rgb->rows; j++) {
        for (int i = 0; i < rgb->cols; i++) {
            raw_dist.at<float>(j, i) = pointPolygonTest(contours[0], Point2f(i, j), true);
        }
    }

    double minVal;
    double maxVal;
    minMaxLoc(raw_dist, &minVal, &maxVal, 0, 0, Mat());
    minVal = abs(minVal);
    maxVal = abs(maxVal);

    /// Depicting the  distances graphically
    Mat drawing = Mat::zeros(rgb->size(), CV_8UC3);

    for (int j = 0; j < rgb->rows; j++) {
        for (int i = 0; i < rgb->cols; i++) {
            if (raw_dist.at<float>(j, i) < 0) {
                drawing.at<Vec3b>(j, i)[0] = 255 - (int) abs(raw_dist.at<float>(j, i)) * 255 /
                                                   minVal;
            }
            else if (raw_dist.at<float>(j, i) > 0) {
                drawing.at<Vec3b>(j, i)[2] = 255 - (int) raw_dist.at<float>(j, i) * 255 / maxVal;
            }
            else {
                drawing.at<Vec3b>(j, i)[0] = 255;
                drawing.at<Vec3b>(j, i)[1] = 255;
                drawing.at<Vec3b>(j, i)[2] = 255;
            }
        }
    }


    *rgb = drawing;

    return true;

}

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_getThreshold
        (JNIEnv *, jclass, jlong matAddress) {

    Mat *rgb = (Mat *) matAddress;

    Mat gray;
    cvtColor(*rgb, gray, CV_BGR2GRAY);

    Mat thresh;
    threshold(gray, thresh, 100.0, 255.0, THRESH_BINARY);

    *rgb = thresh;

}

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_getGaussian
        (JNIEnv *, jclass, jlong matAddress) {

    Mat *input = (Mat *) matAddress;
    Mat g;
    GaussianBlur(*input, g, Size(7, 7), 2.0, 2.0);
    *input = g;

}

JNIEXPORT jboolean JNICALL Java_bahcesehir_edu_shapedetection_ShapeDetection_getCanny
        (JNIEnv *, jclass, jlong matAddress) {

    Mat *input = (Mat *) matAddress;
    Mat edges;
    Canny(*input, edges, 66.0, 133.0, 3);
    *input = edges;
}

void GetVerticalHorizontalLines(jlong matAddress){

    Mat *rgb = (Mat *) matAddress;

    Mat occludedSquare8u;
    cvtColor(*rgb, occludedSquare8u, CV_BGR2GRAY);

    Mat thresh;
    threshold(occludedSquare8u, thresh, 100.0, 255.0, THRESH_BINARY);

    GaussianBlur(thresh, thresh, Size(7, 7), 2.0, 2.0);


    Mat edges;
    Canny(thresh, edges, 66.0, 133.0, 3);

}


