# -*- coding: utf-8 -*-
from array import array
from time import sleep
from random import randint
import time
import settings


class TestTimmyDevice:
    def __init__(self):
        self.rotations = [60] * settings.servo_count

    def shutdown(self):
        pass

    def is_active(self):
        return True

    def set_servo_rotation(self, servo_id, rotation):
        if servo_id < 0 or servo_id > settings.servo_count - 1:
            print 'Wrong servo index: {}'.format(servo_id)
            return

        servo_settings = settings.get_servo(servo_id)

        if rotation < servo_settings.min_rotation:
            rotation = servo_settings.min_rotation

        if rotation > servo_settings.max_rotation:
            rotation = servo_settings.max_rotation

        self.rotations[servo_id] = rotation

        return True

    def get_servo_rotation(self, servo_id):
        return self.rotations[servo_id]