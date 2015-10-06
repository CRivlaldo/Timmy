# -*- coding: utf-8 -*-
from kivy.uix.boxlayout import BoxLayout
from bandsgraph import BandsGraph


class PlaybackWindow(BoxLayout):

    def update_graph(self, sound_data):
        # ToDo: too slow; make faster
        seek_bar = self.get_seek_bar()
        self.clear_widgets()

        graph = BandsGraph()
        graph.sound_data = sound_data
        graph.create_plot()

        self.add_widget(graph)
        self.add_widget(seek_bar)
        self.do_layout()

    def get_seek_bar(self):
        return self.ids['seek_bar']