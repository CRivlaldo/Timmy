# -*- coding: utf-8 -*-
from camerawindow import CameraWindow


class RecognitionDebugWindow(CameraWindow):

    def __init__(self, **kwargs):
        super(RecognitionDebugWindow, self).__init__(**kwargs)
        self._recogniser = None

    @property
    def recogniser(self):
        return self._recogniser

    @recogniser.setter
    def recogniser(self, value):
        self._recogniser = value

    def process_frame(self):
        return self._recogniser.debug_frame if self._recogniser is not None else None
