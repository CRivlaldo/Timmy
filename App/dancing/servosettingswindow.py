# -*- coding: utf-8 -*-
from kivy.uix.label import Label
from kivy.uix.gridlayout import GridLayout
import settings
from servosettingsrow import ServoSettingsRow
from . import bands_colors


class ServoSettingsWindow(GridLayout):

    captions = ['Servo Id', 'Low, db', 'High, db', 'Scale', 'Enabled', 'Color']

    def __init__(self, **kwargs):
        super(ServoSettingsWindow, self).__init__(**kwargs)

        self.add_settings_header()

        self.settings_rows = []
        for i in range(settings.servo_count):
            row = ServoSettingsRow(i, bands_colors[i])
            self._bind_row_events(row)
            self._add_row(row)

        self._on_low_changed_event = None
        self._on_high_changed_event = None
        self._on_scale_changed_event = None
        self._on_enabled_changed_event = None

    @property
    def on_low_changed(self):
        return self._on_low_changed_event

    @on_low_changed.setter
    def on_low_changed(self, event):
        self._on_low_changed_event = event

    @property
    def on_high_changed(self):
        return self._on_high_changed_event

    @on_high_changed.setter
    def on_high_changed(self, event):
        self._on_high_changed_event = event

    @property
    def on_scale_changed(self):
        return self._on_scale_changed_event

    @on_scale_changed.setter
    def on_scale_changed(self, event):
        self._on_scale_changed_event = event

    @property
    def on_enabled_changed(self):
        return self._on_enabled_changed_event

    @on_enabled_changed.setter
    def on_enabled_changed(self, event):
        self._on_enabled_changed_event = event

    def add_settings_header(self):
        for caption in ServoSettingsWindow.captions:
            self.add_widget(Label(text=caption))

    def reload(self, bands):
        for settings_row in self.settings_rows:
            settings_row.reload(bands)

    def _on_servo_settings_low_changed(self, servo_id, text):
        if self._on_low_changed_event is not None:
            self._on_low_changed_event(servo_id, text)

    def _on_servo_settings_high_changed(self, servo_id, text):
        if self._on_high_changed_event is not None:
            self._on_high_changed_event(servo_id, text)

    def _on_servo_settings_scale_changed(self, servo_id, text):
        if self._on_scale_changed_event is not None:
            self._on_scale_changed_event(servo_id, text)

    def _on_servo_settings_enabled_changed(self, servo_id, value):
        if self._on_enabled_changed_event is not None:
            self._on_enabled_changed_event(servo_id, value)

    def _add_row(self, row):
        self.settings_rows.append(row)
        self.add_widget(row.servo_id_label)
        self.add_widget(row.low)
        self.add_widget(row.high)
        self.add_widget(row.scale)
        self.add_widget(row.enabled_checkbox)
        self.add_widget(row.color_icon)

    def _bind_row_events(self, row):
        servo_id = row.servo_id
        row.low.bind(on_text_validate=lambda instance: self._on_servo_settings_low_changed(servo_id, instance.text))
        row.high.bind(on_text_validate=lambda instance: self._on_servo_settings_high_changed(servo_id, instance.text))
        row.scale.bind(on_text_validate=lambda instance: self._on_servo_settings_low_changed(servo_id, instance.text))
        row.enabled_checkbox.bind(active=lambda c, value: self._on_servo_settings_enabled_changed(servo_id, value))