/**
 * @file GraphManager.h
 * @authors Prashanth Ramadoss
 * @copyright 2021 Istituto Italiano di Tecnologia (IIT). This software may be modified and
 * distributed under the terms of the GNU Lesser General Public License v2.1 or any later version.
 */

#ifndef KINDYNVIO_ESTIMATORS_GRAPH_MANAGER_H
#define KINDYNVIO_ESTIMATORS_GRAPH_MANAGER_H

#include <BipedalLocomotion/FloatingBaseEstimators/FloatingBaseEstimatorParams.h>

#include <gtsam_unstable/nonlinear/IncrementalFixedLagSmoother.h>
#include <gtsam/nonlinear/ISAM2.h>
#include <gtsam/nonlinear/NonlinearFactorGraph.h>
#include <gtsam/navigation/CombinedImuFactor.h>
#include <gtsam/navigation/ImuBias.h>
#include <gtsam/inference/Symbol.h>
#include <gtsam/nonlinear/Values.h>
#include <gtsam/geometry/Pose3.h>
#include <gtsam/inference/Key.h>

#include <KinDynVIO/Estimators/IO.h>

#include <memory>
namespace KinDynVIO
{
namespace Estimators
{
using BaseStateDev = BipedalLocomotion::Estimators::FloatingBaseEstimators::StateStdDev;
using IMUBias = gtsam::imuBias::ConstantBias;
class GraphManager
{
public:
    GraphManager();
    ~GraphManager();
    GraphManager(const GraphManager& other) = delete;
    GraphManager& operator=(const GraphManager& other) = delete;
    GraphManager(GraphManager &&) = default;
    GraphManager& operator=(GraphManager &&) = default;

    void setCameraIntrinsics(const std::vector<double>& K);
    bool addBaseStatePriorAtCurrentKey(const double& timeStamp,
                                       const gtsam::Pose3& basePose,
                                       const gtsam::Vector3& baseVel,
                                       const BaseStateDev& priorNoise,
                                       const IMUBias& bias = IMUBias());

    void spawnNewState(const double& timeStamp);
    void setInitialGuessForCurrentStates(const gtsam::Pose3& pose,
                                         const gtsam::Vector3& vel,
                                         IMUBias imuBias = IMUBias());

    void processPreintegratedIMUMeasurements(const gtsam::PreintegratedCombinedMeasurements& preintIMU);
    void processArucoKeyFrames(const ArucoKeyFrame& arucoKF, double pixelNoise = 1.0);
    void processAbsolutePoseMeasurement(const gtsam::Pose3& absPose,
                                        double sigmaPos = 0.001,
                                        double sigmaRot = 0.0174533);
    void processOdometryMeasurement(const gtsam::Pose3& betweenPose,
                                    const double& sigmaPos,
                                    const double& sigmaRot);
    //void processZeroVelocityMeasurements(const double& zeroVelNoise);
    //void processNoMotionMeasurements(const double& noMotionNoise);

    bool optimize();
    void resetManager();

    gtsam::Pose3 getEstimatedBasePose() const;
    IMUBias getEstimatedIMUBias() const;
    gtsam::Vector3 getEstimatedBaseLinearVelocity() const;

    const gtsam::IncrementalFixedLagSmoother& smoother() const;
    const gtsam::NonlinearFactorGraph& graph() const;

private:
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
};

} // namespace Estimators
} // namespace KinDynVIO

#endif