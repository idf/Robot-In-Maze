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
            self.responses_outgoing.put([False, SENSOR, json.dumps({
                "sensors": [{"sensor": 0, "value": 100}, {"sensor": 1, "value": 30}, {"sensor": 2, "value": 30},
                            {"sensor": 10, "value": 30}, {"sensor": 11, "value": 30}]})])
        else:
            self.responses_outgoing.put([True, FUNCTION, json.dumps({"function": function, "status": parameter})])

    @Override(SerialCommander)
    def response_pop(self):
        """


        :return: [ack, type_data, data] : [bool, int, dic]
        """
        return super(SerialCommanderStub, self).response_pop()

