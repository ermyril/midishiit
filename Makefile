ARDUINO_VERSION = 10801
#ARDUINO_DIR   = /Applications/Arduino.app/Contents/Java
#ARDMK_DIR     = /usr/local
#AVR_TOOLS_DIR = /usr
BOARD_TAG    = micro
#BOARD_SUB = 8MHzatmega32U4
MONITOR_PORT = /dev/cu.usbmodemMIDI1
ARDUINO_PORT = /dev/cu.usbmodem14101
DEVICE_PORT = /dev/cu.usbmodem14101
AVRDUDE_OPTS = -v -P /dev/cu.usbmodem14101

include /usr/local/opt/arduino-mk/Arduino.mk

