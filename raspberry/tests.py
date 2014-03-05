from unittest import TestCase
from serial_comminication import *
from utils.decorators import Override

__author__ = 'Danyang'


class MessageTest(TestCase):
    @Override(TestCase)
    def setUp(self):
        self.commander = SerialCommander(production=False)

    def test_machine_code(self):
        expected = "0001000"
        output = self.commander._convert_to_machine_code(0, 10)
        self.assertEqual(expected, output)

    def test_parse(self):
        dic = self.commander._parse_function_status({"function": 0, "status": 200})

        self.assertEqual(dic.get(0, None), 200)

        dic = self.commander._parse_sensor_readings(
            {"sensors": [{"sensor": 0, "value": 100}, {"sensor": 1, "value": 30}, {"sensor": 2, "value": 30}]})

        self.assertEqual(dic.get(0, None), 100)
        self.assertEqual(dic.get(1, None), 30)
        self.assertEqual(dic.get(2, None), 30)

