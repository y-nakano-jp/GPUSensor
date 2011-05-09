#ifndef __GPU_THRMOMETER__
#define __GPU_THRMOMETER__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <NVCtrl/NVCtrl.h>
#include <NVCtrl/NVCtrlLib.h>
#include <string.h>

#define BUFFER_SIZE 256

enum AttributeType{
	GPU_TEMPERATURE = 0, 
	CORE_THRESHOLD, 
	DEFAULT_CORE_THRESHOLD, 
	MAX_CORE_THRESHOLD,
	AMBIENT_TEMPERATURE, 
	INTAKE_TEMPERATURE, 
	EXHAUST_TEMPERATURE, 
	BOARD_TEMPERATURE,
	FAN_SPEED,
	RAM_USED,
	RAM_SIZE,
	GPU_USED
};

static const char* AttributeStr(enum AttributeType atr){
	switch(atr){
		case GPU_TEMPERATURE:
			return "GPU temperature";
		case CORE_THRESHOLD:
			return "Core threshold";
		case DEFAULT_CORE_THRESHOLD:
			return "default Core threshold";
		case MAX_CORE_THRESHOLD:
			return "max core threshold";
		case AMBIENT_TEMPERATURE:
			return "ambient temperature";
		case INTAKE_TEMPERATURE:
			return "intake temperature";
		case EXHAUST_TEMPERATURE:
			return "exhaust temperature";
		case BOARD_TEMPERATURE:
			return "board temperature";
		case FAN_SPEED:
			return "fan speed(%)";
		case RAM_USED:
			return "ram used(%)";
		case RAM_SIZE:
			return "ram size(byte)";
		case GPU_USED:
			return "GPU used(%)";
		default:
			return NULL;
	}
}

int GetAttributeNVCtrl(enum AttributeType atr,int* ans){
	int ret = EIO;
	Display *display = NULL; 
	display = XOpenDisplay(NULL);
	int iAttr = 0;
	int iBuf = 0;
	char** tokens = NULL;
	int num = 0;
	int i = 0;
	if(!ans){
		return ret;
	}
	if(!display){
		//ret = errno;
		goto bail;
	}
	switch (atr){
		case RAM_SIZE:
			iAttr = NV_CTRL_VIDEO_RAM;
			break;
		case GPU_TEMPERATURE:
			iAttr = NV_CTRL_GPU_CORE_TEMPERATURE; 
			break;
		case CORE_THRESHOLD:
			iAttr = NV_CTRL_GPU_CORE_THRESHOLD;
			break;
		case DEFAULT_CORE_THRESHOLD:
			iAttr = NV_CTRL_GPU_DEFAULT_CORE_THRESHOLD;
			break;
		case MAX_CORE_THRESHOLD:
			iAttr = NV_CTRL_GPU_MAX_CORE_THRESHOLD;
			break;
		case AMBIENT_TEMPERATURE:
			iAttr = NV_CTRL_AMBIENT_TEMPERATURE;
			break;
		default:
			iAttr = 0;
			break;
	}
	if(iAttr){
		XNVCTRLQueryAttribute(display, 0, 0, iAttr, &iBuf);
		*ans = iBuf;
		ret = 0;
	}
	if(!XNVCTRLQueryTargetCount(display, NV_CTRL_TARGET_TYPE_VCSC,&num)){
		goto bail;
	}
	if(num <= 0){
		goto bail;
	}

	if(atr == FAN_SPEED){
		iAttr =  NV_CTRL_STRING_VCSC_FAN_STATUS;
		if(XNVCTRLQueryStringAttribute(display,0,0,iAttr,tokens)){
			if(tokens){
				*ans = atoi(tokens[1]);
				ret = 0;
			}
		}
	}
	switch(atr){
		case INTAKE_TEMPERATURE:
			iAttr = NV_CTRL_STRING_VCSC_TEMPERATURES;
			if(XNVCTRLQueryTargetStringAttribute(display,NV_CTRL_TARGET_TYPE_VCSC,0,0,iAttr,tokens)){
				if(tokens){
					*ans = atoi(tokens[0]);
					ret = 0;
				}
			}
			break;
		case EXHAUST_TEMPERATURE:
			iAttr = NV_CTRL_STRING_VCSC_TEMPERATURES;
			if(XNVCTRLQueryStringAttribute(display,0,0,iAttr,tokens)){
				if(tokens){
					*ans = atoi(tokens[1]);
					ret = 0;
				}
			}
			break;
		case BOARD_TEMPERATURE:
			iAttr = NV_CTRL_STRING_VCSC_TEMPERATURES;
			if(XNVCTRLQueryStringAttribute(display,0,0,iAttr,tokens)){
				if(tokens){
					*ans = atoi(tokens[2]);
					ret = 0;
				}
			}
			break;
		default:
			break;
	}
bail:
	if(display){
		XCloseDisplay(display);
	}
	if(tokens){
		for(i = 0; i < 3; i++){
			free(tokens[i]);
		}
	}	
	return ret;
}


int GetAttributeSMI(enum AttributeType atr,int* ans){
	int ret = EIO;
        char token[32];
        FILE *fp = NULL;
	int ibuf = 0;
	if(!ans){
		return ret;
	}
	fp = popen("nvidia-smi -a", "r");
	if( fp == NULL){
                ret = errno;
                perror("nvidia-smi");
                goto bail;
	}
	char buffer[BUFFER_SIZE];
	while(fgets(buffer,BUFFER_SIZE,fp)){
		
		if(atr == GPU_TEMPERATURE){
			if(sscanf(buffer,"\tTemperature\t\t: %d C\n",&ibuf) > 0){
				*ans = ibuf;
				ret = 0;
				break;
			}
		}

		if(atr == FAN_SPEED){
			if(sscanf(buffer,"\tFan Speed\t\t: %d%%\n",&ibuf) > 0){
				*ans = ibuf;
				ret = 0;
				break;
			}
		}
		if(atr == RAM_USED){
			if(sscanf(buffer,"\t    Memory\t\t: %d%%\n",&ibuf) > 0){
				*ans = ibuf;
				ret  = 0;
				break;
			}
		}

		if(atr == GPU_USED){
			if(sscanf(buffer,"\t    GPU\t\t\t: %d%%\n",&ibuf) > 0){
				*ans = ibuf;
				ret  = 0;
				break;
			}
		}
	}
	
bail:
	if(fp){
		fclose(fp);
	}
	return ret;
}


int GetAttribute(enum AttributeType atr, int* ans){
	int ret = EIO;
	if(!ans){
		return ret;
	}
	ret = GetAttributeNVCtrl(atr,ans);
	if(ret != 0){
		ret = GetAttributeSMI(atr,ans);
		return ret;
	}
	return ret;
}


static const char* AttributeSymbol(enum AttributeType atr){
	switch(atr){
		case GPU_TEMPERATURE:
			return "GPU_TEMPERATURE";
		case CORE_THRESHOLD:
			return "CORE_THRESHOLD";
		case DEFAULT_CORE_THRESHOLD:
			return "DEFAULT_CORE_THRESHOLD";
		case MAX_CORE_THRESHOLD:
			return "MAX_CORE_THRESHOLD";
		case AMBIENT_TEMPERATURE:
			return "AMBIENT_TEMPERATURE";
		case INTAKE_TEMPERATURE:
			return "INTAKE_TEMPERATURE";
		case EXHAUST_TEMPERATURE:
			return "EXHAUST_TEMPERATURE";
		case BOARD_TEMPERATURE:
			return "BOARD_TEMPERATURE";
		case FAN_SPEED:
			return "FAN_SPEED";
		case RAM_USED:
			return "RAM_USED";
		case RAM_SIZE:
			return "RAM_SIZE";
		case GPU_USED:
			return "GPU_USED";
		default:
			return "";
	}
}
#endif
