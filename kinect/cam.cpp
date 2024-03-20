#include <iostream>
#include <libfreenect.hpp>
#include <opencv2/opencv.hpp>

class MyFreenectDevice : public Freenect::FreenectDevice {
public:
    MyFreenectDevice(freenect_context* _ctx, int _index) : Freenect::FreenectDevice(_ctx, _index) {}

    void VideoCallback(void* _rgb, uint32_t timestamp) override {
        std::cout << "Received RGB frame" << std::endl;
        cv::Mat rgb(480, 640, CV_8UC3, _rgb);
        cv::imshow("RGB", rgb);
        cv::waitKey(1);
    }

    void DepthCallback(void* _depth, uint32_t timestamp) override {
        std::cout << "Received depth frame" << std::endl;

        // Normalize depth values to 0-255 range

        cv::Mat depth_image(480, 640, CV_16UC1, _depth);
        cv::Mat color_depth;
        depth_image.convertTo(color_depth, CV_8U, 255.0 / 2048.0);
        cv::applyColorMap(color_depth, color_depth, cv::COLORMAP_JET);

        // Display the normalized depth map
        cv::imshow("Depth Map", color_depth);
        cv::waitKey(1);
    }
};

int main() {
    // Initialize the Kinect device
    Freenect::Freenect freenect;
    MyFreenectDevice* device = &freenect.createDevice<MyFreenectDevice>(0);

    // Start Kinect loop
    device->startVideo();
    device->startDepth();

    while (true) {
        // Do other processing here if needed
    }

    // Stop Kinect loop
    device->stopVideo();
    device->stopDepth();

    return 0;
}
