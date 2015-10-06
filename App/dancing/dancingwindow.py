# -*- coding: utf-8 -*-
import os
from time import time
from kivy.core.audio import SoundLoader
from kivy.clock import Clock
from kivy.uix.screenmanager import Screen
from kivy.uix.listview import ListItemButton
from kivy.adapters.listadapter import ListAdapter
from dancing import Dance, SoundData, Bands
from trackmanager import track_manager


class DancingWindow(Screen):

    # ToDo: move playing stuff into the Playback Window or into the new class

    def __init__(self, **kwargs):

        super(DancingWindow, self).__init__(**kwargs)

        self.settings_group = None
        self.playback_group = None
        self.tracks_group = None

        self.current_track_name = None
        self.track_sound = None
        self.sound_data = None
        self.dance = None

        self.bands = Bands()

        self.playing_start = None

    def on_show(self):
        self.settings_group = self.ids['settings_group']
        self.settings_group.on_low_changed = self.on_servo_settings_low_changed
        self.settings_group.on_high_changed = self.on_servo_settings_high_changed
        self.settings_group.on_scale_changed = self.on_servo_settings_scale_changed
        self.settings_group.on_enabled_changed = self.on_servo_settings_enabled_changed
        self.settings_group.reload(self.bands)

        self.playback_group = self.ids['playback_group']
        self.tracks_group = self.ids['tracks_group']

        self.ids['track_list'].adapter.bind(on_selection_change=lambda value: self.on_selected_track_changed(value))

        if len(track_manager.tracks) > 0:
            self.init_track_data(track_manager.tracks[0])

        self._update_playback_window()

        Clock.schedule_interval(self.update_seek_bar, 0.05)

    def on_hide(self):
        Clock.unschedule(self.update_seek_bar)

    def on_play_button_press(self, *largs):
        if self.dance.start():
            self.ids['button_play'].disabled = True
            self.ids['button_stop'].disabled = False
            self.settings_group.disabled = True
            self.tracks_group.disabled = True

    def on_stop_button_press(self, *largs):
        self.dance.stop()

    def on_stop_dancing(self):
        self.stop_track()
        self.ids['button_play'].disabled = False
        self.ids['button_stop'].disabled = True
        self.settings_group.disabled = False
        self.tracks_group.disabled = False

    def on_exit_button_press(self, *largs):
        if not self.dance.is_stopped:
            self.dance.stop()

        if self.current_track_name:
            self.bands.save()

        self.parent.back_to_main_menu()

    def on_servo_settings_low_changed(self, servo_id, text):
        try:
            self.bands[servo_id].low = float(text)
            self.sound_data.update_signal(servo_id, self.bands[servo_id])
            self._update_playback_window()
        except:
            self.settings_group.settings_rows[servo_id].low.text = str(self.bands[servo_id].low)

    def on_servo_settings_high_changed(self, servo_id, text):
        try:
            self.bands[servo_id].high = float(text)
            self.sound_data.update_signal(servo_id, self.bands[servo_id])
            self._update_playback_window()
        except:
            self.settings_group.settings_rows[servo_id].high.text = str(self.bands[servo_id].high)

    def on_servo_settings_scale_changed(self, servo_id, text):
        try:
            self.bands[servo_id].scale = float(text)
            self.sound_data.update_signal(servo_id, self.bands[servo_id])
            self._update_playback_window()
        except:
            self.settings_group.settings_rows[servo_id].scale.text = str(self.bands[servo_id].scale)

    def on_servo_settings_enabled_changed(self, servo_id, value):
        self.bands[servo_id].enabled = value
        self.sound_data.update_signal(servo_id, self.bands[servo_id])
        self._update_playback_window()

    # ToDo: slow !!!
    def init_track_data(self, track_name):
        self.current_track_name = track_name
        self.bands = Bands(track_name, track_manager.get_music_folder_path(), self.bands)
        self.track_sound = SoundLoader.load(self.get_current_track_path())
        self.sound_data = SoundData(self.get_current_track_path(), self.bands)
        self.dance = Dance(self.sound_data, self.play_track, self.on_stop_dancing)
        self.settings_group.reload(self.bands)

    def on_selected_track_changed(self, list_adapter):
        if list_adapter.selection and len(list_adapter.selection) > 0:
            new_track = list_adapter.selection[0].text
            if new_track == self.current_track_name:
                return
            self.bands.save()
            self.init_track_data(new_track)
            self._update_playback_window()

    def get_current_track_path(self):
        if self.current_track_name:
            return os.path.join(track_manager.get_music_folder_path(), self.current_track_name)
        return None

    def play_track(self):
        if self.track_sound:
            self.track_sound.play()
            self.playing_start = time()

    def stop_track(self):
        if self.track_sound:
            self.track_sound.stop()

    def update_seek_bar(self, *args):
        seek_bar = self.playback_group.get_seek_bar()
        if not self.track_sound or self.track_sound.state == 'stop':
            seek_bar.value = 0
        else:
            elapsed_time = time() - self.playing_start
            if elapsed_time > self.sound_data.sample_length:
                self.track_sound.stop()
                seek_bar.value = 0
            else:
                seek_bar.value = 100 * elapsed_time / self.sound_data.sample_length

    def _update_playback_window(self):
        self.playback_group.update_graph(self.sound_data)