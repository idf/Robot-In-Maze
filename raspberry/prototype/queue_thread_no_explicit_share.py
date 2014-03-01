from prototype.queue_thread import *
from utils.decorators import Override

__author__ = 'Danyang'

# unfinished draft
class WritingThread2(threading.Thread):
    def __init__(self, queue):
        super(WritingThread2, self).__init__()
        self.queue_wrapper = queue

    @Override(threading.Thread)
    def run(self):
        for i in range(10, 20):
            self.queue_wrapper.enqueue(i)
            time.sleep(0.1)


def main():
    queue = QueueWrapper()
    rt = ReadingThread(queue)



## still need to share something