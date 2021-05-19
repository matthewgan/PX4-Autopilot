## Beagle Systems PX4 Development Quick Start Guide
We will use this guide to train new developers joining our team, and keep tracking customized features of the firmware.

### Requirements
* Support Beagle S/M/One flying with customized firmware in the near future.
* Optimize VTOL airframe in PX4.
* New features for Beagle One:
	- [ ] Adding Flaps + Elevator to Servo function for Beagle One.
	- [ ] Adding Flaps angle setting.
	- [ ] Landing gear turn and brake functions.
	- [ ] Adjustable Tilt servo end position.
	- [ ] Vector thrust control support.
	- [ ] Safety switch should not cover all IOs.
	- [ ] VTOL should not be limited by Q state.
	- [ ] Short take off and landing modes.
	- [ ] VTOL moving forward and backward should use tilt servo rather than normal copter.
	- [ ] New flight modes for testing airframe.
	- [ ] RTL should return as the way it goes if possible(Smart RTL).

### PX4 Architecture Overview
1. We are using Hex Cube Black(Beagle S) and Orange(Beagle M and later) as our main Flight Controller. So we can focus on only two type of controllers from board and build configuration settings.
	```
	# for Cube Black
	make px4_fmu-v3_default

	# for Cube Orange
	make cubepilot_cubeorange
	```

1. The cmake file for Cube Black is located in ```./boards/px4/fmu-v3/default.cmake```, the compiled bin file will also naming as ```px4_fmu-v3_default```.
	All the .cpp and .c files are included in the cmake, please __add customize task or sensor driver here in order to compile them into the firmware__.

1. ```msg``` folder is short for ```uORB manager```, all the data structures(.msg) can be found in this folder. Customized data structures should also be added here if needed.

1. ```NuttX``` is the OS of PX4, similar to uCos. __Not recommended to change__. OS version should match PX4 version.

1. ```ROMFS``` contains the start up procedure and configurations(init.d).
	* rcS: First boot up script, mount SD, start uORB, config system level parameters.
	* rc.sensors: Start sensor drivers.
	* rc.mc_defaults/fw_defaults/vtol_defaults: config PWM related parameters.
	* rc.mc_apps/fw_apps/vtol_apps: Start high level applications, e.g. attitude_estimate, attitude_control, position_estimate, position_control, etc.

1. ```src``` folder:
	* drivers: hardware drivers. __:fire: Don't modify, always make new driver if needed!__
	* examples: Make sure go through this folder before adding customize codes.
	* lib: Standard useful libraries. __:fire:Don't modify, always make a new one if needed!__
	* __modules: algorithms and high level applications.__ (Find the right folder for your new codes.)

1. Makefile: shell script to call the compiler and link.
