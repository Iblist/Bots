'''This bot needs a name, I'm going to go take suggestions'''

from pymouse import PyMouse
from pykeyboard import PyKeyboardEvent
import time
import threading
import Image
import ImageStat
import math
import pyscreenshot as ImageGrab

state = True        #Should the bot continue running.
upgrade = False     #Is an upgrade availuable?

'''This class is kind of a clusterfuck. First off, it creates an event listener
   waiting for keyboard input of the "q" key. When pressed, state is set to
   false
   The clicking is also done here. It's pretty simple, the bot just keeps
   clicking until the state is false. Will also click upgrades.
'''
class EventHandler(PyKeyboardEvent):

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
        global upgrade
        while state == True:
            m.click(213,388,1)
            if upgrade:
                m.click(1080,249,1) #Upgrade Position
                upgrade = False
            time.sleep(.05) #A speed of .05 seems stable

'''
Does the actual seeing for the bot.
check_upgrade takes a screenshot of the first box in the upgrade
slot.
check_if_byable checks the brightness of the icon. If it is above
the threshold, the upgrade variable is set to true and the bot
will begin clicking the upgrade. When the upgrade is clicked,
upgrade is set to false.
'''
class EyeBalls():
    def __init__(self):
        print "Initiated"

    def check_upgrade(self, fileName):
        im=ImageGrab.grab(bbox=(1053, 220, 1110, 277))
        im.save(fileName, "png")
        

    def check_if_buyable(self, fileName):
        global upgrade
        im = Image.open(fileName)
        stat = ImageStat.Stat(im)
        r,g,b = stat.mean
        if math.sqrt(0.241*(r**2) + 0.691*(g**2) + 0.068*(b**2)) > 70:
            upgrade = True
        else:
            upgrade = False


        
        
        

handler = EventHandler()
camera = EyeBalls()

'''Multi threading wee!'''
t1 = threading.Thread(target=handler.run)
t1.start()
t2 = threading.Thread(target=handler.clickLoop)
t2.start()

'''Loop for the eyeball'''
while state == True:
    time.sleep(1)
    camera.check_upgrade("screenGrab.png")
    camera.check_if_buyable("screenGrab.png")

    

#m.click(214,310,1)
