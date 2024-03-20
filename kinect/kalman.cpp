#include <Eigen/Dense>
#include <iostream>
#include "kalman.hpp"

std::tuple<VectorXd, MatrixXd, VectorXd> kalman(
    const VectorXd& mu, const MatrixXd& P, const MatrixXd& F, const MatrixXd& Q,
    const MatrixXd& B, const VectorXd& u, const VectorXd& z, const MatrixXd& H,
    const MatrixXd& R) {

    VectorXd mup = F * mu + B * u;
    MatrixXd pp = F * P * F.transpose() + Q;
    VectorXd zp = H * mup;

    if (z.size() == 0) {
        return std::make_tuple(mup, pp, zp);
    }

    VectorXd epsilon = z - zp;
    MatrixXd K = pp * H.transpose() * (H * pp * H.transpose() + R).inverse();
    VectorXd new_mu = mup + K * epsilon;
    MatrixXd new_P = (MatrixXd::Identity(P.rows(), P.cols()) - K * H) * pp;

    return std::make_tuple(new_mu, new_P, zp);
}