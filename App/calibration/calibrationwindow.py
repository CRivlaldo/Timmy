# -*- coding: utf-8 -*-
from random import randint

from kivy.clock import Clock
from kivy.uix.screenmanager import Screen
from kivy.core.audio import SoundLoader
import pygame

import settings
from servoslidersgrid import fill_sliders_grid
from timmy.motions import Rotation
from timmy import instance as timmy_device


class CalibrationWindow(Screen):

    # ToDo: remove old joystick stuff

    joystick_hat_speed = 2
    joystick_axis_speed = 2
    joystick_button_speed = 2

    def __init__(self, **kwargs):

        def init_joystick():
            pygame.joystick.init()

            self.joystick_count = pygame.joystick.get_count()

            if self.joystick_count:
                self.joystick = pygame.joystick.Joystick(0)
                self.joystick.init()

                # print 'joystick name = %s' % (self.joystick.get_name())

                # axes = self.joystick.get_numaxes()
                # print "Number of axes: {}".format(axes)
                
                # for i in range( axes ):
                #     axis = self.joystick.get_axis( i )
                #     print "Axis {} value: {:>6.3f}".format(i, axis)
                    
                # buttons = self.joystick.get_numbuttons()
                # print "Number of buttons: {}".format(buttons)

                # for i in range( buttons ):
                #     button = self.joystick.get_button( i )
                #     print "Button {:>2} value: {}".format(i,button)
                    
                # hats = self.joystick.get_numhats()
                # print "Number of hats: {}".format(hats)

                # for i in range( hats ):
                #     hat = self.joystick.get_hat( i )
                #     print "Hat {} value: {}".format(i, str(hat)) 
            else:
                self.joystick = None

        super(CalibrationWindow, self).__init__(**kwargs)

        init_joystick()

    def on_show(self):
        fill_sliders_grid(self.ids['sliders_grid'], self.on_slider_value_changed)
        self.ids['device_info'].text = "Timmy device %s active" % ("is" if timmy_device.is_active() else "isn't",)
        Clock.schedule_interval(self.update_input, 0.05)

    def on_hide(self):
        Clock.unschedule(self.update_input)

    def update_input(self, *largs):

        def update_joystick():

            def update_slider(slider_index, delta):
                slider = self.ids['sliders_grid'].children[slider_index]
                slider.value = slider.value + delta

            def update_slider_by_joystick_axis(axis_index, slider_index):
                axis_value = self.joystick.get_axis(axis_index)
                if abs(axis_value) > .5:
                    update_slider(slider_index, axis_value * CalibrationWindow.joystick_axis_speed)

            def update_slider_by_joystick_hat(hat_index, axis_index, slider_index):
                # (hat_value,) = self.joystick.get_hat(hat_index)[button_index:button_index+1]
                hat_value = self.joystick.get_hat(hat_index)[axis_index] * CalibrationWindow.joystick_hat_speed
                if abs(hat_value):
                    update_slider(slider_index, hat_value)

            def update_slider_by_joystick_button(button_index, slider_index, delta):
                button_value = self.joystick.get_button(button_index)
                if button_value:
                    update_slider(slider_index, delta)

            if not self.joystick:
                return

            update_slider_by_joystick_axis(0, slider_index=2)
            update_slider_by_joystick_axis(2, slider_index=3)

            update_slider_by_joystick_hat(0, 1, slider_index=1)
            
            update_slider_by_joystick_button(4, slider_index=0, delta=-CalibrationWindow.joystick_button_speed)
            update_slider_by_joystick_button(5, slider_index=0, delta=CalibrationWindow.joystick_button_speed)

            update_slider_by_joystick_button(6, slider_index=4, delta=-CalibrationWindow.joystick_button_speed)
            update_slider_by_joystick_button(7, slider_index=4, delta=CalibrationWindow.joystick_button_speed)
            # update_slider_by_joystick_hat(0, 0, slider_index=4)

            update_slider_by_joystick_button(3, slider_index=5, delta=-CalibrationWindow.joystick_button_speed)
            update_slider_by_joystick_button(1, slider_index=5, delta=CalibrationWindow.joystick_button_speed)

            # buttons = self.joystick.get_numbuttons()
            # for i in range( buttons ):
            #     button = self.joystick.get_button( i )
            #     if button:
            #         print "Button {} is pressed".format(i)

            # for event in pygame.event.get():
            #     pass
            #     # # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
            #     # if event.type == pygame.JOYBUTTONDOWN:
            #     #     print "joystick button pressed."
            #     # if event.type == pygame.JOYBUTTONUP:
            #     #     print "joystick button released."
            #     # if event.type == pygame.JOYAXISMOTION:
            #     #     print "joystick axis motion."
            #     # if event.type == pygame.JOYBALLMOTION:
            #     #     print "joystick ball motion."
            #     # if event.type == pygame.JOYHATMOTION:
            #     #     print "joystick hat motion."

        update_joystick()
    
    def on_slider_value_changed(self, slider_index, value):
        if not timmy_device.is_active():
            return
        timmy_device.set_servo_rotation(slider_index, int(value))

    def on_ok_button_press(self, *largs):
        for i in range(settings.servo_count):
            slider = self.ids['sliders_grid'].children[i]
            servo_settings = settings.get_servo(i)
            servo_settings.min_rotation = int(slider.min)
            servo_settings.max_rotation = int(slider.max)

        self.parent.back_to_main_menu()

    def on_exit_button_press(self, *largs):
        self.parent.back_to_main_menu()

    def on_timmy_button_press(self, *largs):
        if not timmy_device.is_active():
            return

        def load_voice_sound(index):
            try:
                path = r'D:\vladimir\MyProjects\Timmy\App\Data\Sounds\Timmy_{:02d}.wav'.format(index)  # !!! path
                return SoundLoader.load(path)
            except e as Exception:
                print 'Could not load sound: {}'.format(e)
            
            return None

        def timmy(servo_id, sound):

            current_rotation = 60  # !!! use real data
            first_rotation = current_rotation - 20
            last_rotation = current_rotation + 20

            # self.set_servo_rotation(servo_id, first_rotation)

            sound.play()

            # speed = 60 / 0.15 # 0.15s -> 60 degree
            # speed /= 3

            # waiting_time = abs(first_rotation - current_rotation) / speed # 30 / (60 / 0.15) = 0.5 * 0.15 = 0.075

            # sleep(waiting_time)

            # self.set_servo_rotation(servo_id, last_rotation)

            # waiting_time = abs(first_rotation - last_rotation) / speed

            # sleep(waiting_time)
            # self.set_servo_rotation(servo_id, current_rotation)

            def get_rotation(degrees, next_motion=None):
                return Rotation(timmy_device, servo_id, degrees, next_motion)

            # action = get_rotation(first_rotation, get_rotation(last_rotation, get_rotation(current_rotation)))
            action = get_rotation(first_rotation).\
                continue_with(get_rotation(last_rotation)).\
                continue_with(get_rotation(current_rotation))
            action.do()
            
        variant = randint(0, 3)

        sound = load_voice_sound(variant + 1)
        if not sound:
            return       

        timmy(variant, sound)