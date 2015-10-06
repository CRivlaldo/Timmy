# -*- coding: utf-8 -*-
from kivy.garden.graph import Graph, MeshLinePlot
from . import bands_colors
from . import source_sound_signal_color


class BandsGraph(Graph):

    max_plot_width = 2000

    def __init__(self, **kwargs):
        super(BandsGraph, self).__init__(**kwargs)

        self._sound_data = None
        self._max_points = None
        self._step = None
        self._sample_length = None

    @property
    def sound_data(self):
        return self._sound_data

    @sound_data.setter
    def sound_data(self, sound_data):
        self._sound_data = sound_data
        if self._sound_data is None:
            self._max_points = None
            self._step = None
            self._sample_length = None
        else:
            self._max_points = min(self._sound_data.length - 1, BandsGraph.max_plot_width)
            self._step = (self._sound_data.length - 1) / self._max_points
            self.xmax = self._sound_data.sample_length

    def create_plot(self):
        if self._sound_data is None:
            return

        for n, signal in enumerate(self._sound_data.signals):
            if signal is not None:
                self.add_plot(self._get_plot(signal, bands_colors[n]))

        self.add_plot(self._get_plot(self._sound_data.original_signal, source_sound_signal_color))

    def _get_plot(self, signal, color):
        plot = MeshLinePlot(color=[color.r, color.g, color.b, color.a])
        plot.points = [(t * self.xmax / self._max_points, signal[t * self._step]) for t in range(self._max_points + 1)]
        return plot
