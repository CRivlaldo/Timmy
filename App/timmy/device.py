# -*- coding: utf-8 -*-
import usb.core
import usb.util
import usb._interop
import usb.backend.libusb0
from array import array
from time import sleep
from random import randint
import time
import settings

# instance = None

# def init():
#     if instance:
#         return

#     instance = TimmyDevice()

# def shutdown():
#     if not instance:
#         return

#     instance.shutdown()

class TimmyDevice:

    servo_pulse = 0x30
    write_end_point_address = 0x02
    # timeout = 1000
    # servo_count = 6
    # min_rotation = 0
    # max_rotation = 120

    def __init__(self):
        backend = usb.backend.libusb0.get_backend(find_library=lambda x: r"D:\vladimir\MyProjects\Timmy\Driver\x86\libusb0_x86.dll") ### !!! parameter
        
        self.usb_dev = usb.core.find(idVendor=0x0483, idProduct=0xFFF0, backend=backend)

        if self.usb_dev:
            self.usb_dev.set_configuration()

        self.rotations = [60 for i in range(settings.servo_count)] #!!! set on real servo

    def shutdown(self):
        pass

    def is_active(self):
        return self.usb_dev != None

    def set_servo_rotation(self, servo_id, rotation):

        if servo_id < 0 or servo_id > settings.servo_count - 1:
            print 'Wrong servo index: {}'.format(servo_id)
            return

        servo_settings = settings.get_servo(servo_id)

        if rotation < servo_settings.min_rotation:
            rotation = servo_settings.min_rotation

        if rotation > servo_settings.max_rotation:
            rotation = servo_settings.max_rotation

        buffer = array('B', [0, 0, 0x01, TimmyDevice.servo_pulse, servo_id, rotation % 256, rotation / 256, 0, 0, 0, 0])
        result = self.usb_dev.write(TimmyDevice.write_end_point_address, buffer, settings.timeout)

        if result:
            self.rotations[servo_id] = rotation

        return result

    def get_servo_rotation(self, servo_id):
        return self.rotations[servo_id]