/* SIE CONFIDENTIAL
PlayStation(R)4 Programmer Tool Runtime Library Release 07.008.001
* Copyright (C) 2016 Sony Interactive Entertainment Inc.
*/

#ifndef __FLATSHADER_H__
#define __FLATSHADER_H__

#include "toolkit/shader_common/shader_base.h"

unistruct FlatShaderConstants
{
	Matrix4Unaligned u_MVP;
	Vector4Unaligned u_Colour;
};

#endif
