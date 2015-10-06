# -*- coding: utf-8 -*-
from math import sin
import os
import numpy as np
import wave
from threading import Thread
from time import time, sleep
from scipy.signal import butter, lfilter
import settings
import pickle
from timmy import instance as timmy_device


class Band:
    def __init__(self, servo_id, enabled=False, low=0, high=0, scale=1):
        self.servo_id = servo_id
        self.enabled = enabled
        self.low = low
        self.high = high
        self.scale = scale


class Bands:
    def __init__(self, track_name='', tracks_directory='', old_bands=None):
        self.path_to_stored_data = os.path.join(tracks_directory, track_name+'.bands')
        if os.path.isfile(self.path_to_stored_data):
            with open(self.path_to_stored_data, 'rb') as stored_data:
                self.data = pickle.load(stored_data)
        else:
            if old_bands:
                self.data = old_bands.data
            else:
                self.data = [Band(servo_id) for servo_id in range(settings.servo_count)]

    def __getitem__(self, index):
        return self.data[index]

    def save(self):
        with open(self.path_to_stored_data, 'wb') as stored_data:
            pickle.dump(self.data, stored_data)


class SoundData:

    # ToDo: test high freq (>20000 raise exception)

    # comment

    def __init__(self, path, bands):
        spf = wave.open(path, 'r')

        self.rate = spf.getframerate()
        frames = spf.readframes(-1)

        self.signal = np.fromstring(frames, 'Int16')

        if spf.getnchannels() == 2:
            print "{0} isn't mono wav-file".format(path)

        self.length = len(self.signal)
        self.sample_length = float(self.length) / self.rate
        self.original_signal = self.signal
        self.signals = [self.get_signal(band) if band.enabled else None for band in bands]

    def get_signal(self, band):
        def butter_bandpass(low_cutoff, high_cuttoff, fs, order=5):
            nyq = 0.5 * fs
            low = low_cutoff / nyq
            high = high_cuttoff / nyq
            return butter(order, [low, high], btype='bandpass', analog=False)

        b, a = butter_bandpass(band.low, band.high, self.rate, order=3)
        return lfilter(b, a, self.signal)

    def update_signal(self, servo_id, band):
        print('update signal', servo_id)  # ToDo: delete
        if band.enabled:
            self.signals[servo_id] = self.get_signal(band)
        else:
            self.signals[servo_id] = None

    # def savitzky_golay(y, window_size, order, deriv=0, rate=1):
    #
    #     from math import factorial
    #
    #     try:
    #         window_size = np.abs(np.int(window_size))
    #         order = np.abs(np.int(order))
    #     except ValueError, msg:
    #         raise ValueError("window_size and order have to be of type int")
    #     if window_size % 2 != 1 or window_size < 1:
    #         raise TypeError("window_size size must be a positive odd number")
    #     if window_size < order + 2:
    #         raise TypeError("window_size is too small for the polynomials order")
    #     order_range = range(order+1)
    #     half_window = (window_size -1) // 2
    #     # precompute coefficients
    #     b = np.mat([[k**i for i in order_range] for k in range(-half_window, half_window+1)])
    #     m = np.linalg.pinv(b).A[deriv] * rate**deriv * factorial(deriv)
    #     # pad the signal at the extremes with
    #     # values taken from the signal itself
    #     firstvals = y[0] - np.abs(y[1:half_window+1][::-1] - y[0])
    #     lastvals = y[-1] + np.abs(y[-half_window-1:-1][::-1] - y[-1])
    #     y = np.concatenate((firstvals, y, lastvals))
    #     return np.convolve( m[::-1], y, mode='valid')
    #
    # def butter_lowpass(self, cutoff, fs, order=5):
    #     nyq = 0.5 * fs
    #     normal_cutoff = cutoff / nyq
    #     b, a = butter(order, normal_cutoff, btype='low', analog=False)
    #     return b, a
    #
    # def butter_lowpass_filter(self, data, cutoff, fs, order=5):
    #     b, a = self.butter_lowpass(cutoff, fs, order=order)
    #     y = lfilter(b, a, data)
    #     return y
    #
    # def butter_bandpass(self, low_cutoff, high_cuttof, fs, order=5):
    #     nyq = 0.5 * fs
    #     low = low_cutoff / nyq
    #     high = high_cuttof / nyq
    #     b, a = butter(order, [low, high], btype='bandpass', analog=False)
    #     return b, a
    #
    # def butter_bandpass_filter(self, data, low_cutoff, high_cuttof, fs, order=5):
    #     b, a = self.butter_bandpass(low_cutoff, high_cuttof, fs, order=order)
    #     y = lfilter(b, a, data)
    #     return y


class Dance:

    def __init__(self, sound_data, on_device_ready=None, on_dance_stopped=None):
        self.sound_data = sound_data
        self.on_device_ready = on_device_ready
        self.on_dance_stopped = on_dance_stopped
        self.is_stopped = True

    def start(self):

        def go_into_dance_pose():
            for i in range(settings.servo_count):
                servo_settings = settings.get_servo(i)
                default_init_rotation = (servo_settings.max_rotation + servo_settings.min_rotation) / 2
                if i == 1:
                    timmy_device.set_servo_rotation(i, 14)  # magic const
                else:
                    timmy_device.set_servo_rotation(i, default_init_rotation)

            sleep(1)

        def dance():
            def dance_processing():
                while not self.is_stopped:
                    elapsed_time = time() - start_time
                    # if elapsed_time > length:
                    #     break

                    # if elapsed_time > beats[current_beat]:
                    #     current_beat += 1
                    #     if current_beat >= len(beats):
                    #         break
                    #     state = 'min'
                    #     self.app.timmy_device.set_servo_rotation(servo_id, min_rotation)
                    # elif current_beat > 0 and elapsed_time > (beats[current_beat - 1] + beats[current_beat]) / 2 and state == 'min':
                    #     state = 'max'
                    #     self.app.timmy_device.set_servo_rotation(servo_id, max_rotation)

                    if elapsed_time >= total_duration:
                        break

                    t = int(elapsed_time / total_duration * length)
                    s = self.sound_data.signal[t] * scale
                    rotation = int((max_rotation + min_rotation) / 2 + (max_rotation - min_rotation) / 2 * s / 32768)
                    timmy_device.set_servo_rotation(servo_id, rotation)

            if not timmy_device.is_active():
                return False

            servo_id = 2 # !!! gui
            scale = 4 # ??? calc
            servo_settings = settings.get_servo(servo_id)
            min_rotation = servo_settings.min_rotation
            max_rotation = servo_settings.max_rotation
            timmy_device.set_servo_rotation(servo_id, max_rotation)
            state = 'max'
            current_beat = 0
            start_time = time()

            length = len(self.sound_data.signal) #!!!: already calc
            total_duration = float(length) / self.sound_data.rate

            dance_thread = Thread(target=dance_processing)
            dance_thread.start()

            #ToDO call stop()

        if not self.is_stopped:
            return False

        if not timmy_device.is_active():
                return False

        self.is_stopped = False

        go_into_dance_pose()

        if self.on_device_ready:
            self.on_device_ready()

        dance()

        return True

    def stop(self):
        if not self.is_stopped:
            self.is_stopped = True
            if self.on_dance_stopped:
                self.on_dance_stopped()
