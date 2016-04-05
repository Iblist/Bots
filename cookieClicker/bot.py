'''
I hereby christen thee,

Chippy!

Clicker of Cookies.
'''

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
purchase = False
item = [4]
item = 0, 0, 0, 0

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
        global purchase
        global item
        while state == True:
            m.click(213,388,1)
            if upgrade:
                m.click(1080,249,1) #Upgrade Position
                upgrade = False
            if purchase:
                m.click(item[0]+126, item[1]+31, 1)
                purchase = False

            time.sleep(.02) #A speed of .05 seems stable

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

    def check_upgrade(self, fileName, xStart, yStart, xEnd, yEnd):
        im=ImageGrab.grab(bbox=(xStart, yStart, xEnd, yEnd))
        im.save(fileName, "png")
        

    def check_if_upgradable(self, fileName):
        global upgrade
        im = Image.open(fileName)
        stat = ImageStat.Stat(im)
        r,g,b = stat.mean
        if math.sqrt(0.241*(r**2) + 0.691*(g**2) + 0.068*(b**2)) > 66: #Lowest brightness of clickable = 67
            upgrade = True
        else:
            upgrade = False

    def check_if_buyable(self, fileName, coordinates):
        global item
        global purchase
        im = Image.open(fileName)
        stat = ImageStat.Stat(im)
        r,g,b = stat.mean
        if math.sqrt(0.241*(r**2) + 0.691*(g**2) + 0.068*(b**2)) > 125:
            #print "Buy buy buy"
            item = coordinates
            purchase = True
        

handler = EventHandler()
camera = EyeBalls()
coordinateList = [[0 for x in range(4)] for x in range(6)]

coordinateList[0][0] = 1052
coordinateList[1][0] = 1052
coordinateList[2][0] = 1052
coordinateList[3][0] = 1052
coordinateList[4][0] = 1052
coordinateList[5][0] = 1052
coordinateList[0][1] = 327
coordinateList[1][1] = 391
coordinateList[2][1] = 455
coordinateList[3][1] = 519
coordinateList[4][1] = 583
coordinateList[5][1] = 647
coordinateList[0][2] = 1315
coordinateList[1][2] = 1315
coordinateList[2][2] = 1315
coordinateList[3][2] = 1315
coordinateList[4][2] = 1315
coordinateList[5][2] = 1315
coordinateList[0][3] = 385
coordinateList[1][3] = 449
coordinateList[2][3] = 518
coordinateList[3][3] = 582
coordinateList[4][3] = 646
coordinateList[5][3] = 710

'''Multi threading wee!'''
t1 = threading.Thread(target=handler.run)
t1.start()
t2 = threading.Thread(target=handler.clickLoop)
t2.start()

'''Loop for the eyeball'''
while state == True:
    for i in range (0, 6):
        time.sleep(.05)
        camera.check_upgrade("screenGrab.png", 1053, 220, 1110, 277)
        camera.check_if_upgradable("screenGrab.png")
        camera.check_upgrade("item.png", coordinateList[i][0], coordinateList[i][1], coordinateList[i][2], coordinateList[i][3])
        camera.check_if_buyable("item.png", coordinateList[i])

    

#m.click(214,310,1)
