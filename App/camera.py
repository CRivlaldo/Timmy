# -*- coding: utf-8 -*-
from kivy.clock import Clock
from kivy.event import EventDispatcher

import cv2 as cv

# ToDo: start, stop
# ToDo: properties for frame, resolution and format


class Camera(EventDispatcher):

    __events__ = ('on_texture', )

    def __init__(self, **kwargs):  
        super(Camera, self).__init__()      
        # kwargs.setdefault('resolution', (640, 480))# magic constants !!!!

        self.resolution = kwargs.get('resolution', (640, 480))
        self._index = 0
        self.format = 'bgr'

        self._device = cv.VideoCapture(self._index)
        self._device.set(3, self.resolution[0])  # magic constants !!!!
        self._device.set(4, self.resolution[1])  # magic constants !!!!

        self.frame = None
        ret, self.frame = self._device.read()
        self.resolution = (self._device.get(3), self._device.get(4)) # magic constants !!!!

        self.fps = self._device.get(5)  # magic constants !!!!
        if self.fps <= 0:
            self.fps = 1 / 30.

        Clock.schedule_interval(self._update_camera, self.fps)

    def destroy(self):
        if self._device:
            self._device.release()

    def _update_camera(self, *largs):
        ret, self.frame = self._device.read()
        if ret:
            self.dispatch('on_texture')
        else:
            self.frame = None

    def on_texture(self):
        pass
