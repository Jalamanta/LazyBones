#ifndef __GLX_LAZY_BONES_H
#define __GLX_LAZY_BONES_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include "GLX_base.h"
#include <FTGL/FTGLPolygonFont.h>
#include "LBDC_database.h"
#include "LBDC_database_factory.h"

class GLX_LazyBones: public GLX_base
{
  private:

  vector <GFX_object> gfx_objects_;
  BMT_texture_manager *texture_manager_;
  GFX_object          play_button_;
  GFX_object          pause_button_;
  GFX_object          stop_button_;
  GFX_object          rewind_button_;
  GFX_object          forward_button_;
  GFX_object          green_led_;
  E_Animation         current_animation_;

  float               media_left_right_time_;
  static const float  media_left_delta_;
  static const float  max_media_left_right_time_;
  static const float  min_media_left_right_time_;

  GLfloat light_ambient[4];
  GLfloat light_diffuse[4];
  GLfloat light_position[4];
  int     window;

  float   alpha_;

  unsigned char keys[256];

  LBDC_Database_Factory db_factory_;
  LBDC_database         *database_;

  FTGLPixmapFont *comic_font_small_;
  FTGLPolygonFont *comic_font_medium_;
  FTGLPolygonFont *comic_font_large_;

  void display_now_playing_title( void);
  void display_control_buttons( void);

  void set_light_ambient( GLfloat a, GLfloat b, GLfloat c, GLfloat d);
  void set_light_diffuse( GLfloat a, GLfloat b, GLfloat c, GLfloat d);
  void set_light_position( GLfloat a, GLfloat b, GLfloat c, GLfloat d);

  void set_focus_playlist( void);
  void set_focus_collection( void);

  virtual bool GLX_init( void);
  virtual void GLX_resize( unsigned int width, unsigned int height);
  virtual bool GLX_draw_scene( void);
  virtual bool GLX_check_keys( void);
  virtual bool GLX_idle( void);

 public:
  GFX_playlist *cd_collection_;
  GFX_playlist *current_playlist_;
  GFX_playlist *selected_playlist_;

  GLX_LazyBones();
  virtual ~GLX_LazyBones();

  void load_database( void);
};


#endif
