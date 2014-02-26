import utility
import threading
import time

threading.Thread(target=utility.startTimer).start()

time.sleep(2)
print utility.getTime()
time.sleep(1)
print utility.getTime()
utility.startTimer()
time.sleep(1)
print utility.getTime()
time.sleep(1)
print utility.getTime()