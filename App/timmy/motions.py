# -*- coding: utf-8 -*-

from time import sleep
import settings


class Motion:
    def __init__(self, device, next_motion=None):
        self.device = device
        self.continuation = next_motion

    def do(self):
        self.end()

    def end(self):
        if self.continuation:
            self.continuation.do()

    def continue_with(self, next_motion):
        self.continuation = next_motion
        return next_motion


class Iddle(Motion):
    def __init__(self, device, duration=1.0, next_motion=None):
        Motion.__init__(self, device, next_motion)
        self.duration = duration

    def do(self):
        sleep(self.duration)
        self.end()


class Rotation(Motion):
    def __init__(self, device, servo_id, angle, next_motion=None):
        Motion.__init__(self, device, next_motion)
        self.servo_id = servo_id
        self.angle = angle

    def do(self):
        old_angle = self.device.get_servo_rotation(self.servo_id)
        speed = settings.get_servo(self.servo_id).speed
        duration = abs(old_angle - self.angle) / speed
        self.device.set_servo_rotation(self.servo_id, self.angle)
        sleep(duration)
        self.end()