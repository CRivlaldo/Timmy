# -*- coding: utf-8 -*-
from xml.dom.minidom import *
from xml.etree.ElementTree import *

servo_count = 6
servos = []
default_min_rotation = 0
default_max_rotation = 120
default_speed = 133
timeout = 1000


class Servo:
    def __init__(self, min_rotation, max_rotation, speed):
        self.min_rotation = min_rotation
        self.max_rotation = max_rotation
        self.speed = speed


def read(file_name):
    global servos
    servos = [(None, None) for i in range(servo_count)] # ToDo: * operator here?
    try:
        tree = xml.etree.ElementTree.parse(file_name)
        root = tree.getroot()
        servos_node = root.find('servos')
        for servo_node in servos_node:
            id = int(servo_node.get('id'))
            min_rotation = int(servo_node.find('min_rotation').text)
            max_rotation = int(servo_node.find('max_rotation').text)
            speed = float(servo_node.find('speed').text)
            servos[id] = Servo(min_rotation, max_rotation, speed)
    except Exception as e:
        print 'Could not read file: {}'.format(e)
        return False
    return True


def save(file_name):
    root = Element('settings')

    servo_settings = SubElement(root, 'servos')
    for i, servo in enumerate(servos):
        servo_element = SubElement(servo_settings, 'servo')
        servo_element.set('id', str(i))

        min_rotation_element = SubElement(servo_element, 'min_rotation')
        min_rotation_element.text = str(servo.min_rotation)

        max_rotation_element = SubElement(servo_element, 'max_rotation')
        max_rotation_element.text = str(servo.max_rotation)

        speed_element = SubElement(servo_element, 'speed')
        speed_element.text = str(servo.speed)

    tree = ElementTree(root)

    try:
        tree.write(file_name)
    except Exception as e:
        print 'Could not save file: {}'.format(e)
        return False

    return True


def create():
    for i in range(servo_count):
        servos.append(Servo(default_min_rotation, default_max_rotation, default_speed))


def get_servo(index):
    return servos[index]

# def set_servo_limit(index, min_rotation, max_rotation):
#     (servos[index].min_rotation, servos[index].max_rotation) = (min_rotation, max_rotation)