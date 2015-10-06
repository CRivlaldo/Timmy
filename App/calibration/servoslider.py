# -*- coding: utf-8 -*-
from kivy.uix.label import Label
from kivy.uix.slider import Slider
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
from kivy.uix.widget import Widget
from kivy.lang import Builder


class ServoSlider(BoxLayout):

    kv = '''
BoxLayout:
    orientation: 'vertical'
    Label:
        id: value_label
        text: '%(value_label)s'
    BoxLayout:
        id: bottom_group
        orientation: 'horizontal'
        TextInput:
            id: min_input
            text: '%(min_label)s'
            input_filter: int
            size_hint: 0.3, 0.65
            on_text_validate: self.parent.parent.parent.update_min_value(args[0].text)
        Slider:
            id: slider
            value: %(value)d
            min: %(min)d
            max: %(max)d
            on_value: self.parent.parent.parent._on_value_changed(args[1])
        TextInput:
            id: max_input
            text: '%(max_label)s'
            input_filter: int
            size_hint: 0.3, 0.65
            on_text_validate: self.parent.parent.parent.update_max_value(args[0].text)
    '''

    def __init__(self, min=0, max=100, value=50, value_format='{:.0f}', on_value_changed=None):
        def get_element(id):
            return layout.ids[id]

        super(ServoSlider, self).__init__()

        self.on_value_changed_event = on_value_changed
        self.value_format = value_format

        layout = self._load_layout(min, max, value)

        self.add_widget(layout)

        self.value_label = get_element('value_label')
        self.min_input = get_element('min_input')
        self.slider = get_element('slider')
        self.max_input = get_element('max_input')

    def _load_layout(self, min, max, value):
        return Builder.load_string(ServoSlider.kv % {'value': value, 'min': min, 'max': max,
                                                     'value_label': self.get_value_str(value),
                                                     'min_label': self.get_value_str(min),
                                                     'max_label': self.get_value_str(max)})

    @property
    def value(self):
        return self.slider.value

    @value.setter
    def value(self, new_value):        
        def clamp(value):
            return max(self.slider.min, min(self.slider.max, value))

        new_value = clamp(new_value)

        if self.slider.value != new_value:
            self.slider.value = new_value

    @property
    def min(self):
        return self.slider.min

    @min.setter
    def min(self, value):
        if self.slider.min != value:
            self.slider.min = value
            self.value = self.slider.value
            
        self.update_text(self.min_input, value)

    @property
    def max(self):
        return self.slider.max

    @max.setter
    def max(self, value):
        if self.slider.max != value:
            self.slider.max = value
            self.value = self.slider.value
            
        self.update_text(self.max_input, value)

    def get_value_str(self, value):
        return self.value_format.format(value)

    def _on_value_changed(self, value):
        self.value_label.text = self.get_value_str(self.slider.value)

        if self.on_value_changed_event:
            self.on_value_changed_event(value)

    def update_min_value(self, value):
        try:
            new_value = int(value)
            if new_value >= self.max:
                raise ValueError('Too big min value')
            self.min = new_value
        except:
            self.min = self.slider.min

    def update_max_value(self, value):
        try:
            new_value = int(value)
            if new_value <= self.min:
                raise ValueError('Too small max value')
            self.max = new_value
        except:
            self.max = self.slider.max

    def set_limit(self, min_value, max_value):
        self.slider.min = min_value
        self.update_text(self.min_input, min_value)

        self.slider.max = max_value
        self.update_text(self.max_input, max_value)

        self.value = self.slider.value

    def update_text(self, element, value):
        value_str = self.get_value_str(value)
        if value_str != element.text:
            element.text = self.get_value_str(value)