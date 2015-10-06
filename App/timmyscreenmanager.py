# -*- coding: utf-8 -*-
from kivy.uix.screenmanager import ScreenManager


class TimmyScreenManager(ScreenManager):

    def __init__(self, **kwargs):
        super(TimmyScreenManager, self).__init__(**kwargs)

    def back_to_main_menu(self):
        self.current = 'Main Screen'
