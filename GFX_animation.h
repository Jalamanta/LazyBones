#ifndef __GFX_ANIMATION_H
#define __GFX_ANIMATION_H

#include <vector>
#include "GFX_types.h"

using namespace std;

class GFX_anim_node
{
  private:

  T_OrgDest  origin_;
  T_OrgDest  destination_;
  int        millisecs_;
  int        i_millisec_;
  GLuint     texture_;
  bool       pause_;
  bool       stop_;
  bool       keep_;
  bool       relative_position_;
  bool       relative_rotation_;
  bool       animate_position_;
  bool       animate_rotation_;
  bool       animate_size_;
  bool       animate_screen_position_;
  bool       animate_screen_size_;

  void init( void);

  public:

  GFX_anim_node();
  ~GFX_anim_node();

  void set_origin_position( GLfloat x, GLfloat y, GLfloat z);
  void set_origin_position( T_Position position);
  T_Position get_origin_position( void);

  void set_destination_position( GLfloat x, GLfloat y, GLfloat z);
  void set_destination_position( T_Position position);
  T_Position get_destination_position( void);

  void set_origin_rotation( GLfloat x, GLfloat y, GLfloat z);
  void set_origin_rotation( T_Rotation rotation);
  T_Rotation get_origin_rotation( void);

  void set_destination_rotation( GLfloat x, GLfloat y, GLfloat z);
  void set_destination_rotation( T_Rotation rotation);
  T_Rotation get_destination_rotation( void);

  void set_relative_position( bool value);
  bool get_relative_position( void);
  void set_relative_rotation( bool value);
  bool get_relative_rotation( void);

  void set_origin_size( GLuint width, GLuint height);
  void set_origin_size( T_Size size);
  T_Size get_origin_size( void);

  void set_destination_size( GLuint width, GLuint height);
  void set_destination_size( T_Size size);
  T_Size get_destination_size( void);

  void set_origin_screen_size( GLuint width, GLuint height);
  void set_origin_screen_size( T_ScreenSize screen_size);
  T_ScreenSize get_origin_screen_size( void);

  void set_destination_screen_size( GLuint width, GLuint height);
  void set_destination_screen_size( T_ScreenSize screen_size);
  T_ScreenSize get_destination_screen_size( void);

  void set_origin_screen_position( GLuint x, GLuint y);
  void set_origin_screen_position( T_ScreenPosition screen_position);
  T_ScreenPosition get_origin_screen_position( void);

  void set_destination_screen_position( GLuint x, GLuint y);
  void set_destination_screen_position( T_ScreenPosition screen_position);
  T_ScreenPosition get_destination_screen_position( void);

  bool get_animate_position( void);
  bool get_animate_rotation( void);
  bool get_animate_size( void);
  bool get_animate_screen_position( void);
  bool get_animate_screen_size( void);

  void set_texture( GLuint texture);
  GLuint get_texture( void);

  void set_time( int millisecs);
  int get_time( void);

  void advance_time( int millisecs);
  float get_percentage( void);

  void set_pause( void);
  bool get_pause( void);

  void set_stop( void);
  bool get_stop( void);

  void set_keep( void);
  bool get_keep( void);

  void clear( void);

};

class GFX_object;

class GFX_animation
{
  private:

  vector<GFX_anim_node>            animation_;
  vector<GFX_anim_node>::iterator  anim_iter_;
  GFX_anim_node                    current_node_;
  bool                             current_node_initialized_;

  void init( void);

  public:

  GFX_animation();
  ~GFX_animation();

  bool clear( void);
  bool add_node( GFX_anim_node node);
  bool init_current_node( void);
  bool get_next_node( void);
  bool set_relative_data( GFX_object& object);

  bool animate( GFX_object &object, int millisecs);

};

#endif
