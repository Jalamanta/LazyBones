#ifndef __GFX_TYPES_H
#define __GFX_TYPES_H

#include<GL/gl.h>

typedef struct
{
  GLfloat x;
  GLfloat y;
  GLfloat z;
} T_Position;

typedef struct
{
  GLuint x;
  GLuint y;
} T_ScreenPosition;

typedef struct
{
  GLuint width;
  GLuint height;
} T_ScreenSize;

typedef struct
{
  GLfloat x;
  GLfloat y;
  GLfloat z;
} T_Rotation;

typedef struct
{
  GLfloat width;
  GLfloat height;
} T_Size;

typedef struct
{
  T_Position       position;
  T_Rotation       rotation;
  T_Size           size;
  T_ScreenPosition screen_position;
  T_ScreenSize     screen_size;
} T_OrgDest;

typedef enum
{
  ANIM_NONE = 0,
  ANIM_LEFT,
  ANIM_RIGHT,
  ANIM_UP,
  ANIM_DOWN
} E_Animation;

#endif
