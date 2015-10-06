# -*- coding: utf-8 -*-
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput
from kivy.uix.checkbox import CheckBox
from coloricon import ColorIcon


class ServoSettingsRow(object):

    def __init__(self, servo_id, color):
        self.servo_id = servo_id
        self.color = color

        self.servo_id_label = Label(text=str(servo_id))
        self.low = TextInput(text='0', input_filter='float', multiline=False)
        self.high = TextInput(text='0', input_filter='float', multiline=False)
        self.scale = TextInput(text='1', input_filter='float', multiline=False)
        self.enabled_checkbox = CheckBox()
        self.color_icon = ColorIcon(color)

    def reload(self, bands):
        band = bands[self.servo_id]
        self.low.text = str(band.low)
        self.high.text = str(band.high)
        self.scale.text = str(band.scale)
        self.enabled_checkbox.active = band.enabled