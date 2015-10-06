# -*- coding: utf-8 -*-
from kivy.uix.image import Image
from kivy.graphics.texture import Texture


# ToDo: it fails without connected camera
class CameraWindow(Image):
    def __init__(self, **kwargs):
        super(CameraWindow, self).__init__(**kwargs)
        self._camera = None
        self.texture = None
        self._buffer = None

    @property
    def camera(self):
        return self._camera

    @camera.setter
    def camera(self, camera):
        self._camera = camera
        self._camera.bind(on_texture=self._on_update)
        # self._camera.bind(on_load=self._camera_loaded)

    # def _camera_loaded(self, *l):
    #     self.texture = self._camera.texture
    #     self.texture_size = list(self.texture.size)

    def _on_update(self, *l):
        if self.texture is None:
            self.texture = self._create_texture()

        if self._camera.frame is not None:
            frame = self.process_frame()
            if frame is not None:
                try:
                    self._buffer = frame.data.tostring()
                except AttributeError:
                    self._buffer = frame.tostring()
                self._copy_to_gpu()
                self.canvas.ask_update()

    def process_frame(self):
        return self._camera.frame

    def _copy_to_gpu(self):
        if self.texture is None:
            print('CameraWindow: copy_to_gpu() failed, texture is None !')
            return
        self.texture.blit_buffer(self._buffer, colorfmt=self._camera.format)
        self._buffer = None

    def _create_texture(self):
        texture = Texture.create(self._camera.resolution)
        texture.flip_vertical()
        return texture
