from kivy.uix.boxlayout import BoxLayout
from kivy.graphics import Color, Ellipse
from kivy.uix.widget import Widget


class NimGameBoard(BoxLayout):
    def __init__(self, **kwargs):
        super(NimGameBoard, self).__init__(**kwargs)
        self._current_move = None
        self._rows = []
        self._pole = None

    def set_rows(self, rows):
        self._rows = rows
        self._update()

    def _update(self):
        if self._rows is None:
            return
        if self._pole is None:
            self._pole = self.ids['pole']
        self._pole.update()
        # for y, row in enumerate(self._rows):
        #     for x, has_cap in enumerate(row):
        #         if has_cap:
        #             with self._pole.canvas:
        #                 Color(0, 0, 1)
        #                 Ellipse(pos=(initial_x + d * (x + 1) * 1, initial_y - d * (y + 1) * 1), size=(d, d))

    @property
    def rows(self):
        return self._rows


class NimGamePole(Widget):
    def update(self):
        self.canvas.clear()
        d = 30.
        size_x = self.size[0]
        size_y = self.size[1]
        initial_x = self.pos[0] + d
        initial_y = self.pos[1] + size_y - d
        # with self.canvas:
        #     Color(0, 0, 1)
        #     Ellipse(pos=(initial_x, initial_y), size=(d, d))

        for y, row in enumerate(self.parent.rows):
            for x, has_cap in enumerate(row):
                if has_cap:
                    with self.canvas:
                        Color(0, 0, 1)
                        Ellipse(pos=(initial_x + d * (x + 1) * 1.25, initial_y - d * (y + 1) * 1.25), size=(d, d))
        self.canvas.ask_update()