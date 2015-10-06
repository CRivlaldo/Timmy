import os.path


class TrackManager(object):

    music_folder_name = 'Music'
    track_extension = '.wav'

    def __init__(self):
        self._data_folder_path = ''
        self.tracks = []
        self.items = []
        self.args_converter = TrackManager._get_args_converter()

    def init(self, data_folder):
        self._data_folder_path = data_folder
        self.tracks = list(self._load_tracks())  # ToDo: appends
        self.items = self._get_items()  # ToDo: appends

    def destroy(self):
        pass

    def get_music_folder_path(self):
        return os.path.join(self._data_folder_path, TrackManager.music_folder_name)

    def _load_tracks(self):
        for path in os.listdir(self.get_music_folder_path()):
            if path.endswith(TrackManager.track_extension):
                yield os.path.basename(path)

    @staticmethod
    def _get_args_converter():
        return lambda row_index, rec: {'text': rec['text'], 'size_hint_y': None, 'height': 25}

    def _get_items(self):
        if len(self.tracks) == 0:
            return []
        return [{'text': track, 'is_selected': i == 0} for i, track in enumerate(self.tracks)]

track_manager = TrackManager()