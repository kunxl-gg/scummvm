
#include "engine/tinygl/zgl.h"

#define VERTEX_ARRAY	0x0001
#define COLOR_ARRAY		0x0002
#define NORMAL_ARRAY	0x0004
#define TEXCOORD_ARRAY	0x0008

void glopArrayElement(GLContext *c, TGLParam *param) {
	int i;
	int states = c->client_states;
	int idx = param[1].i;

	if (states & COLOR_ARRAY) {
		TGLParam p[5];
		int size = c->color_array_size; 
		i = idx * (size + c->color_array_stride);
		p[1].f = c->color_array[i];
		p[2].f = c->color_array[i + 1];
		p[3].f = c->color_array[i + 2];
		p[4].f = size > 3 ? c->color_array[i + 3] : 1.0f;
		glopColor(c, p);  
	}
	if (states & NORMAL_ARRAY) {
		i = idx * (3 + c->normal_array_stride);
		c->current_normal.X = c->normal_array[i];
		c->current_normal.Y = c->normal_array[i + 1];
		c->current_normal.Z = c->normal_array[i + 2];
		c->current_normal.Z = 0.0f;
	}
	if (states & TEXCOORD_ARRAY) {
		int size = c->texcoord_array_size;
		i = idx * (size + c->texcoord_array_stride);
		c->current_tex_coord.X = c->texcoord_array[i];
		c->current_tex_coord.Y = c->texcoord_array[i + 1];
		c->current_tex_coord.Z = size > 2 ? c->texcoord_array[i + 2] : 0.0f;
		c->current_tex_coord.W = size > 3 ? c->texcoord_array[i + 3] : 1.0f;
	}
	if (states & VERTEX_ARRAY) {
		TGLParam p[5];
		int size = c->vertex_array_size;
		i = idx * (size + c->vertex_array_stride);
		p[1].f = c->vertex_array[i];
		p[2].f = c->vertex_array[i + 1];
		p[3].f = size > 2 ? c->vertex_array[i+2] : 0.0f;
		p[4].f = size > 3 ? c->vertex_array[i+3] : 1.0f;
		glopVertex(c, p);
	}
}

void glArrayElement(TGLint i) {
	TGLParam p[2];
	p[0].op = OP_ArrayElement;
	p[1].i = i;
	gl_add_op(p);
}

void glopEnableClientState(GLContext *c, TGLParam *p) {
	c->client_states |= p[1].i;
}

void glEnableClientState(TGLenum array) {
	TGLParam p[2];
	p[0].op = OP_EnableClientState;

	switch(array) {
	case TGL_VERTEX_ARRAY:
		p[1].i = VERTEX_ARRAY;
		break;  
	case TGL_NORMAL_ARRAY:
		p[1].i = NORMAL_ARRAY;
		break;
	case TGL_COLOR_ARRAY:
		p[1].i = COLOR_ARRAY;
		break;
	case TGL_TEXTURE_COORD_ARRAY:
		p[1].i = TEXCOORD_ARRAY;
		break;
	default:
		assert(0);
		break;
	}
	gl_add_op(p);
}

void glopDisableClientState(GLContext *c, TGLParam *p) {
	c->client_states &= p[1].i;
}

void glDisableClientState(TGLenum array) {
	TGLParam p[2];
	p[0].op = OP_DisableClientState;
    
	switch(array) {
	case TGL_VERTEX_ARRAY:
		p[1].i = ~VERTEX_ARRAY;
		break;  
	case TGL_NORMAL_ARRAY:
		p[1].i = ~NORMAL_ARRAY;
		break;
	case TGL_COLOR_ARRAY:
		p[1].i = ~COLOR_ARRAY;
		break;
	case TGL_TEXTURE_COORD_ARRAY:
		p[1].i = ~TEXCOORD_ARRAY;
		break;
	default:
		assert(0);
		break;
	}
	gl_add_op(p);
}

void glopVertexPointer(GLContext *c, TGLParam *p) {
	c->vertex_array_size = p[1].i;
	c->vertex_array_stride = p[2].i;
	c->vertex_array = (float *)p[3].p;
}

void  glVertexPointer(TGLint size, TGLenum type, TGLsizei stride, const TGLvoid *pointer) {
	TGLParam p[4];
	assert(type == TGL_FLOAT);
	p[0].op = OP_VertexPointer;
	p[1].i = size;
	p[2].i = stride;
	p[3].p = (void *)pointer;
	gl_add_op(p);
}

void glopColorPointer(GLContext *c, TGLParam *p) {
	c->color_array_size = p[1].i;
	c->color_array_stride = p[2].i;
	c->color_array = (float *)p[3].p;  
}

void  glColorPointer(TGLint size, TGLenum type, TGLsizei stride, const TGLvoid *pointer) {
	TGLParam p[4];
	assert(type == TGL_FLOAT);
	p[0].op = OP_ColorPointer;
	p[1].i = size;
	p[2].i = stride;
	p[3].p = (void *)pointer;
	gl_add_op(p);
}

void glopNormalPointer(GLContext *c, TGLParam *p) {
	c->normal_array_stride = p[1].i;
	c->normal_array = (float *)p[2].p;
}

void glNormalPointer(TGLenum type, TGLsizei stride, const TGLvoid *pointer) {
	TGLParam p[3];
	assert(type == TGL_FLOAT);
	p[0].op = OP_NormalPointer;
	p[1].i = stride;
	p[2].p = (void *)pointer;
}

void glopTexCoordPointer(GLContext *c, TGLParam *p) {
	c->texcoord_array_size = p[1].i;
	c->texcoord_array_stride = p[2].i;
	c->texcoord_array = (float *)p[3].p;
}

void glTexCoordPointer(TGLint size, TGLenum type, TGLsizei stride, const TGLvoid *pointer) {
	TGLParam p[4];
	assert(type == TGL_FLOAT);
	p[0].op = OP_TexCoordPointer;
	p[1].i = size;
	p[2].i = stride;
	p[3].p = (void *)pointer;
}
