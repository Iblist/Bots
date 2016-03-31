import gtk.gdk
import time

time.sleep(5)
w = gtk.gdk.get_default_root_window()
sz = w.get_size()
print "The size of the window is %d x %d" % sz
pb = gtk.gdk.Pixbuf(gtk.gdk.COLORSPACE_RGB,False,8,sz[0],sz[1])
pb = pb.get_from_drawable(w,w.get_colormap(),1053,220,1110,277,1110-1053,277-220)

if (pb != None):
    pb.save("screenshot3.png","png")
    print "Screenshot saved to screenshot3.png."
else:
    print "Unable to get the screenshot."
