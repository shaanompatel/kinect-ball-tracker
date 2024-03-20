#include "detector.hpp"
#include <gnuplot-iostream.h>

cv::Ptr<cv::BackgroundSubtractor> bg_subtractor = cv::createBackgroundSubtractorMOG2();
std::vector<cv::Point> trajectory;

std::tuple<double, double, double> get_coords(cv::Mat rgbFrame, cv::Mat depthFrame) {
	cv::Mat fg_mask;
	bg_subtractor->apply(rgbFrame, fg_mask);

    // Draw rectangles around the contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(fg_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Filter out small contours
    std::vector<cv::Rect> rects;
    for (const auto& contour : contours) {
        cv::Rect bounding_rect = cv::boundingRect(contour);
        if (cv::contourArea(contour) > 1000) { // Adjust the area threshold as needed
            rects.push_back(bounding_rect);
        }
    }

    // Merge overlapping bounding boxes
    std::vector<cv::Rect> merged_rects;
    for (const auto& rect : rects) {
        bool merged = false;
        for (auto& merged_rect : merged_rects) {
            if ((rect & merged_rect).area() > 0) { // If the rectangles overlap
                merged_rect |= rect; // Merge the rectangles
                merged = true;
                break;
            }
        }
        if (!merged) {
            merged_rects.push_back(rect);
        }
    }

    // Draw rectangles around the merged bounding boxes
    cv::Mat frame_with_rectangles = rgbFrame.clone(); // Create a copy of the frame
    if (merged_rects.size() > 0) {
        cv::rectangle(frame_with_rectangles, merged_rects.at(0), cv::Scalar(0, 255, 0), 2);
        cv::putText(frame_with_rectangles, std::to_string(get_depth(depthFrame, merged_rects.at(0))), cv::Point(50, 200), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        cv::Point center_of_rect = (merged_rects.at(0).br() + merged_rects.at(0).tl()) * 0.5;
        trajectory.push_back(center_of_rect);
        circle(frame_with_rectangles, center_of_rect, 3, cv::Scalar(0, 0, 255));
    }

    cv::imshow("Frame with Rectangles", frame_with_rectangles);
   

	return std::make_tuple<double, double, double>(0, 0, 0);
}

double get_depth(cv::Mat depthMap, cv::Rect roi) {
    cv::Mat roiDepth = depthMap(roi);
    double avgDepth = cv::mean(roiDepth)[0];
    return avgDepth;
}

void plot_trajectory() {
    Gnuplot gp;
    std::vector<double> x, y;
    for (const cv::Point& point : trajectory) {
        x.push_back(point.x);
        y.push_back(point.y);
    }
    gp << "plot '-' with lines\n";
    gp.send1d(std::make_tuple(x, y));
}

