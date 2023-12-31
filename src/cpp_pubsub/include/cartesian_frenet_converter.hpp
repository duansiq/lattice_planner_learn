#ifndef _CARTESIAN_FRENET_
#define _CARTESIAN_FRENET_
#include <array>
#include <vector>
#include <math.h>
using namespace std;
class Cartesian_Frenet_Converter
{
private:
public:
    Cartesian_Frenet_Converter(){};
    ~Cartesian_Frenet_Converter(){};

    /**
     * Convert a vehicle state in Cartesian frame to Frenet frame.
     * Decouple a 2d movement to two independent 1d movement w.r.t. reference
     * line.
     * The lateral movement is a function of longitudinal accumulated distance s
     * to achieve better satisfaction of nonholonomic constraints.
     */
    static void cartesian_to_frenet(const double rs, const double rx,
                                    const double ry, const double rtheta,
                                    const double rkappa, const double rdkappa,
                                    const double x, const double y,
                                    const double v, const double a,
                                    const double theta, const double kappa,
                                    std::array<double, 3> *const ptr_s_condition,
                                    std::array<double, 3> *const ptr_d_condition);

    static void cartesian_to_frenet(const double rs, const double rx,
                                    const double ry, const double rtheta,
                                    const double x, const double y,
                                    double *ptr_s, double *ptr_d);

    /**
     * Convert a vehicle state in Frenet frame to Cartesian frame.
     * Combine two independent 1d movement w.r.t. reference line to a 2d movement.
     */
    static void frenet_to_cartesian(const double rs, const double rx,
                                    const double ry, const double rtheta,
                                    const double rkappa, const double rdkappa,
                                    const std::array<double, 3> &s_condition,
                                    const std::array<double, 3> &d_condition,
                                    double *const ptr_x, double *const ptr_y,
                                    double *const ptr_theta,
                                    double *const ptr_kappa, double *const ptr_v,
                                    double *const ptr_a);

    // given sl point extract x, y, theta, kappa
    static double CalculateTheta(const double rtheta, const double rkappa,
                                 const double l, const double dl);

    static double CalculateKappa(const double rkappa, const double rdkappa,
                                 const double l, const double dl,
                                 const double ddl);

    static vector<double> CalculateCartesianPoint(const double rtheta,
                                                  const vector<double> &rpoint,
                                                  const double l);
    /**
     * @brief: given sl, theta, and road's theta, kappa, extract derivative l,
     *second order derivative l:
     */
    static double CalculateLateralDerivative(const double theta_ref,
                                             const double theta, const double l,
                                             const double kappa_ref);

    // given sl, theta, and road's theta, kappa, extract second order derivative
    static double CalculateSecondOrderLateralDerivative(
        const double theta_ref, const double theta, const double kappa_ref,
        const double kappa, const double dkappa_ref, const double l);
};

#endif