/*
This file is automatically generated
DO NOT MODIFY BY HAND
Ft_Esd_Layout_Stretch
C Source
*/

#include "Ft_Esd_Layout_Stretch.h"

#include "Esd_Context.h"
ESD_CORE_EXPORT void Esd_Noop(void *context);


static Ft_Esd_WidgetSlots s_Ft_Esd_Layout_Stretch__Slots = {
	(void(*)(void *))Ft_Esd_Widget_Initialize,
	(void(*)(void *))Ft_Esd_Widget_Start,
	(void(*)(void *))Ft_Esd_Widget_Enable,
	(void(*)(void *))Ft_Esd_Widget_Update,
	(void(*)(void *))Ft_Esd_Widget_Render,
	(void(*)(void *))Ft_Esd_Widget_Idle,
	(void(*)(void *))Ft_Esd_Widget_Disable,
	(void(*)(void *))Ft_Esd_Widget_End,
};


void Ft_Esd_Layout_Stretch__Initializer(Ft_Esd_Layout_Stretch *context)
{
	Ft_Esd_Widget__Initializer((Ft_Esd_Widget *)context);
	context->Widget.ClassId = Ft_Esd_Layout_Stretch_CLASSID;
	context->Widget.Slots = &s_Ft_Esd_Layout_Stretch__Slots;
	context->Widget.LocalX = 0;
	context->Widget.LocalY = 0;
	context->Widget.LocalWidth = 50;
	context->Widget.LocalHeight = 50;
	context->Weight = 1;
}


/* end of file */
