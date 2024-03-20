#ifndef KALMAN
#define KALMAN

#include <Eigen/Dense>

typedef Eigen::MatrixXd MatrixXd;
typedef Eigen::VectorXd VectorXd;
typedef Eigen::Matrix<double, 6, 1> Vector6d;
typedef Eigen::Matrix<double, 6, 6> Matrix6d;
typedef Eigen::Matrix<double, 3, 1> Vector3d;
typedef Eigen::Matrix<double, 3, 3> Matrix3d;

const double noise = 3;
const double sigmaM = 0.0001;  // model noise
const double sigmaZ = 3 * noise;  // should be equal to the average noise of the process


std::tuple<VectorXd, MatrixXd, VectorXd> kalman(
    const VectorXd& mu, const MatrixXd& P, const MatrixXd& F, const MatrixXd& Q,
    const MatrixXd& B, const VectorXd& u, const VectorXd& z, const MatrixXd& H,
    const MatrixXd& R);

#endif // KALMAN
