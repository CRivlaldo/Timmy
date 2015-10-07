# -*- coding: utf-8 -*-
from kivy.clock import Clock
from kivy.uix.screenmanager import Screen
from kivy.app import App
from nimrecogniser import NimRecogniser


class NimGameWindow(Screen):

    def __init__(self, **kwargs):
        super(NimGameWindow, self).__init__(**kwargs)
        self._recogniser = NimRecogniser()
        self._recogniser.camera = App.get_running_app().camera
        self._nim_game_board = None

    def on_show(self):
        recognition_debug_window = self.ids['recognition_debug_window']
        recognition_debug_window.recogniser = self._recogniser
        self._nim_game_board = self.ids['nim_game_board']
        Clock.schedule_interval(self._do_tick, 1.)

    def on_hide(self):
        Clock.unschedule(self._do_tick)

    def on_start_button_press(self, *largs):
        pass

    def on_stop_button_press(self, *largs):
        pass

    def on_exit_button_press(self, *largs):
        self.parent.back_to_main_menu()

    def _do_tick(self, *largs):
        self._nim_game_board.set_rows(self._recogniser.caps_lines)