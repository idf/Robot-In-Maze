from serial_comminication import *
from utils.decorators import Override

__author__ = 'Danyang'


class SerialCommanderStub(SerialCommander):
    @Override(SerialCommander)
    def __init__(self):
        super(SerialCommanderStub, self).__init__(production=False)
    @Override(SerialCommander)
    def command_put(self, function, parameter):
        if function==10:
            self.responses_outgoing.put([False, SENSOR, {0:30, 1:30, 2:30, 10:30, 11:30}])
        else:
            self.responses_outgoing.put([True, FUNCTION, {function: 200}])

    @Override(SerialCommander)
    def response_pop(self):
        """


        :return: [ack, type_data, data] : [bool, int, dic]
        """
        return super(SerialCommanderStub, self).response_pop()

