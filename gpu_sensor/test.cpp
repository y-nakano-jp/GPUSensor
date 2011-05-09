#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <NVCtrl/NVCtrl.h>
#include <NVCtrl/NVCtrlLib.h>
#include "gpu_sensor.h"
int main(){
	int buf = 0;
	for(int i = 0; i < 12; i++ ){
		if(!GetAttribute((AttributeType)i,&buf))
			printf("%s %d\n",AttributeStr((AttributeType)i),buf);
	}
	return 0;
}
