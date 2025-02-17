include (${CMAKE_CURRENT_LIST_DIR}/uavcan_board_identity)

add_definitions(-DCONSTRAINED_FLASH_NO_HELP="https://docs.px4.io/master/en/modules/modules_main.html")

px4_add_board(
	PLATFORM nuttx
	TOOLCHAIN arm-none-eabi
	ARCHITECTURE cortex-m4
	CONSTRAINED_MEMORY
	EXTERNAL_METADATA
	ROMFSROOT cannode
	UAVCAN_INTERFACES 1
	DRIVERS
		barometer/bmp388
		bootloaders
		gps
		imu/invensense/icm42688p
		magnetometer/bosch/bmm150
		safety_button
		tone_alarm
		uavcannode
	MODULES
		#ekf2
		#load_mon
		#sensors
	SYSTEMCMDS
		#i2cdetect
		#led_control
		param
		#perf
		reboot
		top
		topic_listener
		tune_control
		uorb
		#ver
		work_queue
)
