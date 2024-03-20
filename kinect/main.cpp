#include "kalman.hpp"
#include "kinect.hpp"
#include "detector.hpp"
#include <iostream>
#include <conio.h>

/*
int main() {
    const double fps = 30;
    const double dt = 1 / fps;
    
    Matrix6d P = Matrix6d::Identity() * 1000.0 * 1000.0; // uncertainty

    Matrix6d Q = (sigmaM * sigmaM) * Matrix6d::Identity();
    Matrix3d R = (sigmaZ * sigmaZ) * Matrix3d::Identity();

    Eigen::Matrix<double, 6, 3> B;
    B << (dt * dt) / 2, 0, 0,
        0, (dt * dt) / 2, 0,
        0, 0, (dt * dt) / 2,
        dt, 0, 0,
        0, dt, 0,
        0, 0, dt;

    Eigen::Matrix<double, 6, 6> F;
    F << 1, 0, 0, dt, 0, 0,
        0, 1, 0, 0, dt, 0,
        0, 0, 1, 0, 0, dt,
        0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 1;

    Eigen::Matrix<double, 3, 6> H;
    H << 1, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0;

    Vector6d mu;
    mu << 0, 0, 0, 0, 0, 0;

    Vector3d a;
    a << 0, 900, 0;

    Vector3d z;
    z << 0, 900, 0;

    Vector6d new_mu;
    Matrix6d new_P;
    Vector3d zp;
    std::tie(new_mu, new_P, zp) = kalman(mu, P, F, Q, B, a, z, H, R);
    std::cout << "new_mu:\n" << new_mu << std::endl;
    std::cout << "new_P:\n" << new_P << std::endl;
    std::cout << "zp:\n" << zp << std::endl;

    return 0;
}
*/

int main() {
	cv::Mat depthMat(cv::Size(640, 480), CV_16UC1);
	cv::Mat depthf(cv::Size(640, 480), CV_8UC1);
	cv::Mat rgbMat(cv::Size(640, 480), CV_8UC3, cv::Scalar(0));
	cv::Mat ownMat(cv::Size(640, 480), CV_8UC3, cv::Scalar(0));

	Freenect::Freenect freenect;
	Kinect& device = freenect.createDevice<Kinect>(0);

	cv::namedWindow("rgb", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("depth", cv::WINDOW_AUTOSIZE);
	device.startVideo();
	device.startDepth();
	while (true) {
		device.getVideo(rgbMat);
		device.getDepth(depthMat);
		cv::imshow("rgb", rgbMat);
        get_coords(rgbMat, depthMat);
		depthMat.convertTo(depthf, CV_8UC1, 255.0 / 2048.0);
		cv::imshow("depth", depthf);
		char k = cv::waitKey(5);
		if (k == 27) {
			cv::destroyWindow("rgb");
			cv::destroyWindow("depth");
			break;
		}
	}


	device.stopVideo();
	device.stopDepth();
    
    plot_trajectory();
    while (_getch() != 27) {}

	return 0;
}