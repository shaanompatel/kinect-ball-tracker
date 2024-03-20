#ifndef DETECTOR
#define DETECTOR

#include <opencv2/opencv.hpp>

const cv::Scalar lowerBlue = cv::Scalar(28, 98, 63);
const cv::Scalar upperBlue = cv::Scalar(65, 255, 255);

std::tuple<double, double, double> get_coords(cv::Mat rgbFrame, cv::Mat depthFrame);
double get_depth(cv::Mat depthMap, cv::Rect roi);
void plot_trajectory();

#endif // DETECTOR