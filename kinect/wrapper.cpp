#include <pybind11/pybind11.h>
#include <libfreenect.h>
#include <opencv2/opencv.hpp>
#include <mutex>

namespace py = pybind11;

class Kinect {
public:
    Kinect() {
        if (freenect_init(&f_ctx, NULL) < 0) {
            throw std::runtime_error("Error initializing libfreenect");
        }
    }

    ~Kinect() {
        freenect_shutdown(f_ctx);
    }

    void start() {
        if (freenect_open_device(f_ctx, &f_dev, 0) < 0) {
            throw std::runtime_error("Error opening Kinect device");
        }

        // Set a flag to indicate that the depth callback was successfully set
        depth_callback_set = true;

        freenect_set_user(f_dev, this);

        if (freenect_start_depth(f_dev) < 0) {
            throw std::runtime_error("Error starting depth stream");
        }
    }

    void stop() {
        freenect_stop_depth(f_dev);
        freenect_close_device(f_dev);
    }

    cv::Mat get_depth_frame() {
        std::lock_guard<std::mutex> lock(depth_mutex);
        return depth_data.clone();
    }

private:
    freenect_context* f_ctx;
    freenect_device* f_dev;
    cv::Mat depth_data;
    bool depth_callback_set = false;
    std::mutex depth_mutex;

    static void depth_callback(freenect_device* dev, void* depth, uint32_t timestamp) {
        Kinect* kinect = static_cast<Kinect*>(freenect_get_user(dev));
        if (kinect && kinect->depth_callback_set) {
            std::lock_guard<std::mutex> lock(kinect->depth_mutex);
            kinect->depth_data = cv::Mat(480, 640, CV_16UC1, depth).clone();
        }
    }
};

PYBIND11_MODULE(Kinect, m) {
    py::class_<Kinect>(m, "Kinect")
        .def(py::init<>())
        .def("start", &Kinect::start)
        .def("stop", &Kinect::stop)
        .def("get_depth_frame", &Kinect::get_depth_frame);
}
