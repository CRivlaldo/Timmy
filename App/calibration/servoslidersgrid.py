# -*- coding: utf-8 -*-
import settings
from servoslider import ServoSlider
from timmy import instance as timmy_device


def fill_sliders_grid(sliders_grid, on_slider_value_changed=None):
    def get_slider(servo_index):
        servo_settings = settings.get_servo(servo_index)
        return ServoSlider(min=servo_settings.min_rotation, max=servo_settings.max_rotation,
                           value=timmy_device.get_servo_rotation(servo_index),
                           on_value_changed=lambda value: on_slider_value_changed(servo_index, value))

    sliders_grid.clear_widgets()

    for i in range(settings.servo_count):
        sliders_grid.add_widget(get_slider(i))

    sliders_grid.do_layout()
