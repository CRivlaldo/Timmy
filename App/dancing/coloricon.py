# -*- coding: utf-8 -*-
from kivy.graphics import *
from kivy.uix.widget import Widget


class ColorIcon(Widget):

    icon_shift = .6

    def __init__(self, color=Color(0, 0, 0)):
        super(ColorIcon, self).__init__()
        self.color = color
        with self.canvas:
            Color(color.r, color.g, color.b)
            self.background = Rectangle(pos=self.get_icon_pos(), size=self.get_icon_size())

        self.bind(pos=self.update_background)
        self.bind(size=self.update_background)

    def update_background(self, *args):
        self.background.pos = self.get_icon_pos()
        self.background.size = self.get_icon_size()

    def get_icon_pos(self):
        shift = (1 - ColorIcon.icon_shift) * .5
        return self.pos[0] + shift * self.size[0], self.pos[1] + shift * self.size[1]

    def get_icon_size(self):
        shift = ColorIcon.icon_shift
        return self.size[0] * shift, self.size[1] * shift