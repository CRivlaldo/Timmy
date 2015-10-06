import cv2
import numpy as np


class NimRecogniser(object):

    cap_color_hsv = [100, 200, 220]
    cap_color_threshold = 30

    def __init__(self):
        self._debug_frame = None
        self._camera = None
        self._low_cap_color_range = np.array(NimRecogniser._get_low_cap_range())
        self._high_cap_color_range = np.array(NimRecogniser._get_high_cap_range())

    @property
    def debug_frame(self):
        return self._debug_frame

    @property
    def camera(self):
        return self._camera

    @camera.setter
    def camera(self, camera):
        self._camera = camera
        self._camera.bind(on_texture=self._on_update)

    #  ToDo: it can be done in outer tick event
    def _on_update(self, *l):
        def get_mask_by_range(image_hsv):
            return cv2.inRange(image_hsv, self._low_cap_color_range, self._high_cap_color_range)

        input_frame = self._camera.frame
        filtered_frame = cv2.medianBlur(input_frame, 3)
        hsv_frame = cv2.cvtColor(filtered_frame, cv2.COLOR_BGR2HSV)
        mask_by_range = get_mask_by_range(hsv_frame)
        filtered_mask = cv2.GaussianBlur(mask_by_range, (9, 9), 2, 2)
        self._debug_frame = cv2.cvtColor(filtered_mask, cv2.COLOR_GRAY2BGR)

    @staticmethod
    def _get_low_cap_range():
        return [component - NimRecogniser.cap_color_threshold for component in NimRecogniser.cap_color_hsv]

    @staticmethod
    def _get_high_cap_range():
        return [component + NimRecogniser.cap_color_threshold for component in NimRecogniser.cap_color_hsv]