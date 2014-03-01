from Queue import Queue
import threading
import time
from utils.decorators import Deprecated, Override

__author__ = 'Danyang'

class QueueWrapper(object):
    def __init__(self):
        self.queue = Queue()
        for i in range(10):
            self.enqueue(i)

    def enqueue(self, item):
        self.queue.put(item)

    def dequeue(self):
        return self.queue.get()

    def is_empty(self):
        return self.queue.empty()

class ReadingThread(threading.Thread):
    def __init__(self, queue):
        super(ReadingThread, self).__init__()
        self.queue_wrapper = queue
        self.setDaemon(True)

    @Override(threading.Thread)
    def run(self):
        while True:
            if not self.queue_wrapper.is_empty():
                print self.queue_wrapper.dequeue()

    def put(self, item):
        self.queue_wrapper.enqueue(item)

class WritingThread(threading.Thread):
    def __init__(self, queue):
        super(WritingThread, self).__init__()
        self.queue_wrapper = queue

    @Override(threading.Thread)
    def run(self):
        for i in range(10, 20):
            self.queue_wrapper.enqueue(i)
            time.sleep(0.1)



def main():

    queue = QueueWrapper()
    rt = ReadingThread(queue)
    wt = WritingThread(queue)

    rt.start()
    wt.start()

    wt.join()
    # rt terminates since it is daemon thread


if __name__=="__main__":
    main()
