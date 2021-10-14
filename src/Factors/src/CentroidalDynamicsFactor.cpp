/**
 * @file CentroidalDynamicsFactor.cpp
 * @authors Prashanth Ramadoss
 * @copyright 2021 Istituto Italiano di Tecnologia (IIT). This software may be modified and
 * distributed under the terms of the GNU Lesser General Public License v2.1 or any later version.
 */

#include <BipedalLocomotion/Math/Constants.h>
#include <BipedalLocomotion/TextLogging/Logger.h>
#include <KinDynVIO/Factors/CentroidalDynamicsFactor.h>

using namespace KinDynVIO::Factors;

CentroidalDynamicsFactor::CentroidalDynamicsFactor(gtsam::Key pose_i,
                                                   gtsam::Key cdot_i,
                                                   gtsam::Key c_i,
                                                   gtsam::Key ha_i,
                                                   gtsam::Key pose_j,
                                                   gtsam::Key cdot_j,
                                                   gtsam::Key c_j,
                                                   gtsam::Key ha_j,
                                                   gtsam::Key bias_i,
                                                   gtsam::Key bias_j,
                                                   const PreintegratedCDMCumulativeBias& pim) :
    Base(gtsam::noiseModel::Gaussian::Covariance(pim.preintMeasCov()),
         pose_i, cdot_i, c_i, ha_i,
         pose_j, cdot_j, c_j, ha_j,
         bias_i, bias_j), m_PIM(pim)
{
}

gtsam::NonlinearFactor::shared_ptr CentroidalDynamicsFactor::clone() const
{
    return boost::static_pointer_cast<gtsam::NonlinearFactor>(
        gtsam::NonlinearFactor::shared_ptr(new This(*this)));
}

void CentroidalDynamicsFactor::print(const std::string& s,
                                     const gtsam::KeyFormatter& keyFormatter) const
{
    std::cout << (s.empty() ? s : s + "\n") << "CentroidalDynamicsFactor("
              << keyFormatter(this->key1()) << "," << keyFormatter(this->key2()) << ","
              << keyFormatter(this->key3()) << "," << keyFormatter(this->key4()) << ","
              << keyFormatter(this->key5()) << "," << keyFormatter(this->key6()) << ","
              << keyFormatter(this->key7()) << "," << keyFormatter(this->key8()) << ","
              << keyFormatter(this->key9()) << "," << keyFormatter(this->key10())
              << ")\n";
    m_PIM.print("  preintegrated measurements:");
    this->noiseModel_->print("  noise model: ");
}

bool CentroidalDynamicsFactor::equals(const gtsam::NonlinearFactor& other, double tol) const
{
    const This* e = dynamic_cast<const This*>(&other);
    return e != nullptr && Base::equals(*e, tol) && m_PIM.equals(e->m_PIM, tol);
}

gtsam::Vector
CentroidalDynamicsFactor::evaluateError(const gtsam::Pose3& H_i,
                                        const gtsam::Vector3& cdot_i,
                                        const gtsam::Vector3& c_i,
                                        const gtsam::Vector3& ha_i,
                                        const gtsam::Pose3& H_j,
                                        const gtsam::Vector3& cdot_j,
                                        const gtsam::Vector3& c_j,
                                        const gtsam::Vector3& ha_j,
                                        const Bias& bias_i,
                                        const Bias& bias_j,
                                        boost::optional<gtsam::Matrix&> H1,
                                        boost::optional<gtsam::Matrix&> H2,
                                        boost::optional<gtsam::Matrix&> H3,
                                        boost::optional<gtsam::Matrix&> H4,
                                        boost::optional<gtsam::Matrix&> H5,
                                        boost::optional<gtsam::Matrix&> H6,
                                        boost::optional<gtsam::Matrix&> H7,
                                        boost::optional<gtsam::Matrix&> H8,
                                        boost::optional<gtsam::Matrix&> H9,
                                        boost::optional<gtsam::Matrix&> H10) const
{
    gtsam::Vector r = m_PIM.computeErrorAndJacobians(H_i, cdot_i, c_i, ha_i,
                                                     H_j, cdot_j, c_j, ha_j,
                                                     bias_i, bias_j,
                                                     H1, H2, H3, H4, H5,
                                                     H6, H7, H8, H9, H10);

    return r;
}