#ifndef __GFX_OBJECT_H
#define __GFX_OBJECT_H

#include <iostream>
#include <GL/gl.h>
#include <GL/gl.h>
#include "GFX_types.h"
#include "GFX_animation.h"

class GFX_object
{
  private:

  protected:

  GFX_animation    animation_;
  T_Position       position_;
  T_Rotation       rotation_;
  T_ScreenPosition screen_position_;  // BM: Either position on screen or position OF screen.
  T_ScreenSize     screen_size_;;     // BM: Either size on screen or size OF screen.
  GLfloat          width_;
  GLfloat          height_;
  bool             animate_;
  GLuint           texture_;
  GLuint           mask_;

  void init( void);

  public:

  GFX_object();

  GFX_object( GLuint texture);
  GFX_object( GLuint texture, GLuint mask);
  GFX_object( const GFX_object& rhs);

  virtual ~GFX_object();

  GFX_object& GFX_object::operator=( const GFX_object& rhs);

  void set_position( GLfloat x, GLfloat y, GLfloat z);
  void set_position( T_Position position);
  T_Position get_position( void);

  void set_screen_position( GLuint x, GLuint y);
  void set_screen_position( T_ScreenPosition position);
  T_ScreenPosition get_screen_position( void);
  GLuint get_screen_x( void);
  GLuint get_screen_y( void);

  void set_screen_size( GLuint x, GLuint y);
  void set_screen_size( T_ScreenSize size);
  T_ScreenSize get_screen_size( void);
  GLuint get_screen_width( void);
  GLuint get_screen_height( void);

  void set_rotation( GLfloat x, GLfloat y, GLfloat z);
  void set_rotation( T_Rotation rotation);
  T_Rotation get_rotation( void);

  void set_size( GLfloat width, GLfloat height);
  void set_size( T_Size size);
  void set_width( GLfloat width);
  void set_height( GLfloat height);
  GLfloat get_width( void);
  GLfloat get_height( void);
  T_Size get_size(void);

  void set_animate( bool animate);
  bool get_animate( void);
  virtual bool animate( int millisecs);

  bool add_node( GFX_anim_node node);
  bool clear_animation( void);

  bool set_animation( GFX_animation animation);

  void set_texture( GLuint texture);
  GLuint get_texture( void);

  void set_mask( GLuint mask);
  GLuint get_mask( void);

  virtual void draw( void);
};

#endif
