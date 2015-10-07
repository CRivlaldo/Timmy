import cv2
import numpy as np


# ToDo - It's possible to do cool color filtering by range after some calibration.
# ToDo - User can show a cap and recogniser have to get a real color from it.
class NimRecogniser(object):

    # cap_color_hsv = [100, 200, 220]
    _cap_color_low_range_hsv = np.array([[[70, 140, 110]]])
    _cap_color_high_range_hsv = np.array([[[130, 255, 255]]])
    # cap_color_threshold = 30

    _caps_line_threshold_pixels = 10

    def __init__(self):
        self._debug_frame = None
        self._camera = None
        # self._low_cap_color_range = np.array(NimRecogniser._get_low_cap_range())
        # self._high_cap_color_range = np.array(NimRecogniser._get_high_cap_range())
        self._caps_centers = None
        self._caps_lines = None

        # ToDo: delete
        blue = np.uint8([[[120, 77, 0]]])
        print(cv2.cvtColor(blue, cv2.COLOR_BGR2HSV))
        blue = np.uint8([[[252, 200, 97]]])
        print(cv2.cvtColor(blue, cv2.COLOR_BGR2HSV))

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

    @property
    def caps_lines(self):
        return self._caps_lines

    #  ToDo: it can be done in outer tick event
    def _on_update(self, *l):
        input_frame = self._camera.frame
        filtered_frame = cv2.medianBlur(input_frame, 3)
        hsv_frame = cv2.cvtColor(filtered_frame, cv2.COLOR_BGR2HSV)
        mask_by_range = cv2.inRange(hsv_frame, NimRecogniser._cap_color_low_range_hsv,
                                    NimRecogniser._cap_color_high_range_hsv)
        # filtered_mask = cv2.GaussianBlur(mask_by_range, (9, 9), 2, 2)
        # self._debug_frame = cv2.cvtColor(mask_by_range, cv2.COLOR_GRAY2BGR)
        kernel = np.ones((3, 3), np.uint8)
        opened_mask = cv2.morphologyEx(mask_by_range, cv2.MORPH_OPEN, kernel, iterations=2)
        dilated_mask = cv2.dilate(mask_by_range, kernel, iterations=3)
        distance_transform = cv2.distanceTransform(opened_mask, cv2.DIST_L2, 5)
        ret, threshold = cv2.threshold(opened_mask, opened_mask.max() * .7, 255, 0)
        ret, markers, stats, self._caps_centers = cv2.connectedComponentsWithStats(threshold)

        self._debug_frame = cv2.cvtColor(threshold, cv2.COLOR_GRAY2BGR)
        self._caps_lines = self._get_caps_lines()

    # ToDo: take view projection into account (the y-axis is not the Y-axis)
    # ToDo: take gaps into account (x-axis)
    # ToDo: check lines, not all states are ok
    def _get_caps_lines(self):

        def get_y_marks():
            return [center[1] for center in self._caps_centers]

        def scan_line(y):
            found_caps = []
            for cap in caps:
                if abs(cap[1][1] - y) < NimRecogniser._caps_line_threshold_pixels:
                    found_caps.append(cap)
            line = []
            for cap in found_caps:
                caps.remove(cap)
                line.append(cap)
            if len(line) > 0:
                lines.append(line)

        if self._caps_centers is None:
            return None
        y_marks = get_y_marks()
        lines = []
        caps = []
        for i, pos in enumerate(self._caps_centers[1:]):  # ToDo: strange workaround
            caps.append((i, pos))
        y_marks.reverse()
        for y_mark in y_marks:
            scan_line(y_mark)
        rows = []
        print('*********')  # ToDo: delete
        for line in lines:
            print(line)  # ToDo: delete
            row = [True for cap in line]  # ToDo: try True * len(line) ?
            rows.append(row)
        print('*********')  # ToDo: delete
        return rows


    # @staticmethod
    # def _get_low_cap_range():
    #     return [component - NimRecogniser.cap_color_threshold for component in NimRecogniser.cap_color_hsv]
    #
    # @staticmethod
    # def _get_high_cap_range():
    #     return [component + NimRecogniser.cap_color_threshold for component in NimRecogniser.cap_color_hsv]