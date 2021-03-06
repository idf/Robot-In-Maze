from serial_comminication import *
from utils.decorators import Override

__author__ = 'Danyang'


class SerialAPIStub(SerialAPI):
    @Override(SerialAPI)
    def __init__(self):
        super(SerialAPIStub, self).__init__(production=False)
    @Override(SerialAPI)
    def command_put(self, function, parameter):
        if function==10:
            self.responses_outgoing.put([False, SENSOR, json.dumps({
                "sensors": [{"sensor": 0, "value": 20}, {"sensor": 1, "value": 20}, {"sensor": 2, "value": 40},
                            {"sensor": 10, "value": 10}, {"sensor": 11, "value": 30}, {"sensor": 12, "value": 30}]})])
            self.responses_outgoing.put([True, FUNCTION, json.dumps({"function": function, "status": 200})])
        else:
            self.responses_outgoing.put([True, FUNCTION, json.dumps({"function": function, "status": 200})])

    @Override(SerialAPI)
    def response_pop(self):
        """
        :return: [ack, type_data, data] : [bool, int, json_str]
        """
        return super(SerialAPIStub, self).response_pop()

