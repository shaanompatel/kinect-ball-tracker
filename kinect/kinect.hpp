#ifndef KINECT
#define KINECT

#include <libfreenect.hpp>
#include <opencv2/opencv.hpp>
#include "myMutex.hpp"

class Kinect : public Freenect::FreenectDevice {
public:
    Kinect(freenect_context* _ctx, int _index);
    void VideoCallback(void* _rgb, uint32_t timestamp) override;
    void DepthCallback(void* _depth, uint32_t timestamp) override;
    bool getVideo(cv::Mat& output);
    bool getDepth(cv::Mat& output);

private:
    std::vector<uint8_t> m_buffer_depth;
    std::vector<uint8_t> m_buffer_rgb;
    std::vector<uint16_t> m_gamma;
    cv::Mat depthMat;
    cv::Mat rgbMat;
    cv::Mat ownMat;
    myMutex m_rgb_mutex;
    myMutex m_depth_mutex;
    bool m_new_rgb_frame;
    bool m_new_depth_frame;
};

#endif // MY_FREENECT_DEV