#ifndef __GLX_BASE_H
#define __GLX_BASE_H

#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

class GLX_base
{
 protected:

  Display              *display_;
  int                  screen_;
  Window               window_;
  GLXContext           context_;
  XSetWindowAttributes attributes_;
  bool                 fullscreen_;
  bool                 double_buffer_;
  XF86VidModeModeInfo  desk_mode_;
  int                  x_, y_;
  unsigned int         width_,  height_;
  unsigned int         depth_;
  int                  microsecond_timer_;
  bool                 keys_[65536];


  void GLX_destroy_window( void);

 public:

  GLX_base();
  virtual ~GLX_base();

  void set_display( Display *display);
  Display *get_display( void);

  void set_screen( int screen);
  int get_screen( void);

  void set_double_buffer( bool double_buffer);
  bool get_double_buffer( void);

  void set_fullscreen( bool fullscreen);
  bool get_fullscreen( void);

  void set_position( int x, int y);

  void set_width( unsigned int width);
  unsigned int get_width( void);

  void set_height( unsigned int height);
  unsigned int get_height( void);

  void set_depth( unsigned int depth);
  unsigned int get_depth( void);

  bool GLX_is_key_down( KeySym key);

  void GLX_set_timer( int microseconds);
  void GLX_start_timer( void);

  virtual void GLX_timer_reached( int signal);

  virtual bool GLX_init( void);
  virtual void GLX_resize( unsigned int width, unsigned int height);
  virtual bool GLX_check_keys( void);
  virtual bool GLX_idle( void);
  virtual bool GLX_draw_scene( void);
  bool GLX_create_window( char* title,
                          int width, int height,
                          bool fullscreenflag);

  virtual void GLX_after_creation_init( void);

  virtual void GLX_mainloop( void);


};

#endif
