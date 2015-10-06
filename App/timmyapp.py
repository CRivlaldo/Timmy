# -*- coding: utf-8 -*-
import os
import os.path

import kivy
kivy.require('1.8.0')
 
from kivy.app import App
from kivy.core.window import Window
import pygame

from camera import Camera
import settings
import timmy
from dancing.trackmanager import track_manager


class TimmyApp(App):

    settings_file_name = 'Settings.xml'  # ToDo: move to user folder
    data_folder_name = 'Data'

    def __init__(self):

        super(TimmyApp, self).__init__()

        self.working_directory = os.path.dirname(os.path.realpath(__file__))

        self._load_settings()

        self.camera = self._get_camera()

        self._keyboard = Window.request_keyboard(self._keyboard_closed, self)  # need it ????
        self._keyboard.bind(on_key_down=self._on_keyboard_down)

        track_manager.init(self.get_path_to_data())

    def on_start(self):
        pass

    def on_stop(self):
        track_manager.destroy()
        self.camera.destroy()
        timmy.instance.shutdown()
        settings.save(TimmyApp.settings_file_name)

    def get_path_to_data(self):
        return os.path.join(self.working_directory, TimmyApp.data_folder_name)

    def _get_camera(self):
        return Camera(resolution=(320, 240))  # ToDo: use settings

    def _load_settings(self):
        settings_path = os.path.join(self.working_directory, TimmyApp.settings_file_name)
        if os.path.exists(settings_path):
            settings.read(settings_path)
        else:
            settings.create()

    def _keyboard_closed(self):  # ToDo: remove
        self._keyboard.unbind(on_key_down=self._on_keyboard_down)
        self._keyboard = None

    def _on_keyboard_down(self, keyboard, keycode, text, modifiers):  # ToDo: remove
        # if keycode[1] == 'escape':
        #     self.player1.center_y += 10
        return True
                         
if __name__ == '__main__':
        pygame.init()
        TimmyApp().run()
        pygame.quit()