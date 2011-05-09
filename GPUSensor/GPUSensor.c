#include <ruby.h>
#include "../gpu_sensor/gpu_sensor.h"



enum AttributeType
symbol_to_type(VALUE s){
#define SYM_TYPE(v) if(ID2SYM(rb_intern(AttributeSymbol( v ))) == s) return v;
	SYM_TYPE( GPU_TEMPERATURE );
	SYM_TYPE( CORE_THRESHOLD );
	SYM_TYPE( DEFAULT_CORE_THRESHOLD ); 
	SYM_TYPE( MAX_CORE_THRESHOLD );
	SYM_TYPE( AMBIENT_TEMPERATURE );
	SYM_TYPE( INTAKE_TEMPERATURE );
	SYM_TYPE( EXHAUST_TEMPERATURE ); 
	SYM_TYPE( BOARD_TEMPERATURE );
	SYM_TYPE( FAN_SPEED );
	SYM_TYPE( RAM_USED );
	SYM_TYPE( RAM_SIZE );
	SYM_TYPE( GPU_USED );
}
static VALUE
type_to_symbol(enum AttributeType atr)
{
	if(atr)return ID2SYM(rb_intern(AttributeSymbol(atr)));
	return Qnil;	
}

static VALUE
rb_gpusensor_getattribute(VALUE obj, VALUE arg){
	int ret = EIO;
	int buf;
	//Check_type(arg,T_SYMBOL); ??
	ret = GetAttribute(symbol_to_type(arg),&buf);
	if(ret == 0){
		ret = buf;
	}else{
		ret = -1;
	}	
	return INT2NUM(ret);
}

static VALUE
rb_gpusensor_attribute(VALUE obj, VALUE arg){
	return rb_str_new2(AttributeStr(symbol_to_type(arg)));
}

void
Init_GPUSensor(){
	VALUE mGPUSensor = rb_define_module("GPUSensor");
	rb_define_module_function(mGPUSensor, "getAttribute",rb_gpusensor_getattribute,1);
	rb_define_module_function(mGPUSensor, "attribute",rb_gpusensor_attribute,1);
}
