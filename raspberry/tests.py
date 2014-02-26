from unittest import TestCase
from utils.decorators import Override

__author__ = 'Danyang'


class PasswordTest(TestCase):
    @Override(TestCase)
    def setUp(self):
        pass

    def test_serial(self):
        #TODO
        self.assertIsNotNone(None)
