## Test Data

- `models` contain the URDF model of the robot (iCubGenova04) used for conducting the walking experiment.
- `walking` folder contains the time-synchronized proprioceptive sensors dataset (post-processed from a real-time acquisition) along with a file containing the recorded image file names for the data collected during a front-and-back walking of the robot in a rigid terrain environment of Aruco Markers. The ground truth trajectory of the base link pose is obtained form the Vicon motion capture system. The contact wrench measurements are obtained from a whole-body dynamics estimation algorithm (See [robotology/whole-body-estimators/wholeBodyDynamics](https://github.com/robotology/whole-body-estimators/blob/master/devices/wholeBodyDynamics/README.md)). The images dataset are captured from a Intel Realsense D435 mounted rigidly on the base link of the robot. An XSens MTI IMU rigidly attached to the base link of the robot is used for IMU measurements. Please note that, the images dataset is not being uploaded at the moment due to its heavy size. In case of need for the images dataset, please contact the maintainer of this repository.
- `KinDynVIO::TestData` library contains `EstimatorTestUtils.h` file which contains the necessary configuration parameters relevant to the `walking` experiment.