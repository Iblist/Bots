from pymouse import PyMouse
from pykeyboard import PyKeyboardEvent
import time
import threading

state = True

class eventHandler(PyKeyboardEvent):
    def __init__(self):
        PyKeyboardEvent.__init__(self)

    def tap(self, keycode, character, press):
        if press == False:
            character = 'q'

        if character == 'q':
            if press:
                print "State changed"
                global state
                state = False
                self.stop()
        else:
            print "Enter q to quit"

    def clickLoop(self):
        m = PyMouse()
        while state == True:
            m.click(213,388,1)
            #m.click(1080,249,1) Upgrade Position
            time.sleep(0.001)

handler = eventHandler()

t1 = threading.Thread(target=handler.run)
t1.start()
t2 = threading.Thread(target=handler.clickLoop)
t2.start()

#m.click(214,310,1)
