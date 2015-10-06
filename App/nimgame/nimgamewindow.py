from kivy.uix.screenmanager import Screen
from kivy.app import App
from nimrecogniser import NimRecogniser


class NimGameWindow(Screen):

    def __init__(self, **kwargs):
        super(NimGameWindow, self).__init__(**kwargs)
        self._recogniser = NimRecogniser()
        self._recogniser.camera = App.get_running_app().camera

    def on_show(self):
        recognition_debug_window = self.ids['recognition_debug_window']
        recognition_debug_window.recogniser = self._recogniser

    def on_hide(self):
        pass

    def on_start_button_press(self, *largs):
        pass

    def on_stop_button_press(self, *largs):
        pass

    def on_exit_button_press(self, *largs):
        self.parent.back_to_main_menu()