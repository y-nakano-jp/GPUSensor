require "mkmf"
$libs = "-lstdc++ -lXext -lX11 -lXNVCtrl"
$LDFLAGS = "-L/usr/lib"


have_headers = true

%w{
X11/Xlib.h
X11/Xutil.h
NVCtrl/NVCtrl.h
NVCtrl/NVCtrlLib.h
}.each{|h|
  have_headers = have_header(h)
  break unless have_headers
}

if have_headers
  create_makefile("GPUSensor")
end

