#ifndef __GFX_PLAYLIST_H
#define __GFX_PLAYLIST_H

#include <iostream>
#include <vector>
#include <FTGL/FTGLPixmapFont.h>
#include <FTGL/FTGLPolygonFont.h>
#include "GFX_object.h"
#include "GFX_media.h"
#include "GFX_anchor.h"
#include "BMT_texture_manager.h"
#include "LBMT_media.h"
#include "LBMT_media_bag.h"
#include "LBMT_iterator.h"


class GFX_playlist : public GFX_object
{
  private:

  std::vector<GFX_anchor>  anchors_; // Visible GFX_anchor objects (10ish)
  LBMT_MediaBag            *playlist_;
  LBMT_Iterator            playlist_left_it_;
  LBMT_Iterator            playlist_right_it_;
  LBMT_Iterator            selected_media_iterator_;

  BMT_texture_manager *texture_manager_;

  E_Animation  current_animation_;

  unsigned int    selected_media_;
  float           aspect_;
  float           selected_media_z_offset_;
  float           media_y_angle_;
  float           media_x_spacing_;
  float           media_x_offset_;
  unsigned int    media_nbr_left_;
  unsigned int    media_nbr_right_;
  unsigned int    number_anchors_;
  float           original_x_pos_;
  float           original_y_pos_;
  float           original_z_pos_;
  float           scale_;
  float           screen_x_, screen_y_;

  FTGLPixmapFont *comic_font_small_;
  FTGLPolygonFont *comic_font_large_;

  GFX_playlist();

  void init( void);
  void init_anchors( void);
  void reset_positions( void);
  void move_textures_left( void);
  void move_textures_right( void);
  void draw_floor( void);
  void draw_selected_media_title( void);

  public:

  GFX_playlist( BMT_texture_manager *texture_manager,
                GLuint screen_x, GLuint screen_y,
                GLuint width, GLuint height);

  ~GFX_playlist();

  void add_album( LBMT_Album *album);
  void remove_album( LBMT_Album& album);

  void add_song( LBMT_Song& song);
  void remove_song( LBMT_Song& song);

  void add_playlist( LBMT_MediaBag *playlist);

  void add_media( LBMT_Media *media);

  void set_position( GLfloat x, GLfloat y, GLfloat z);

  void set_selected_media( int media_id);
  GFX_media& get_selected_media( void);

  void resize( void);

  void set_scale( float value);
  float get_scale( void);

  void set_aspect( float value);
  float get_aspect( void);

  void animate_all_media_left( int millisecs);
  void animate_all_media_right( int millisecs);
  void animate_playlist_up( int millisecs);
  void animate_playlist_down( int millisecs);
  void spin_selected_media( void);

  virtual bool animate( int millisecs);
  bool get_animate( void);

  void draw_media( bool draw_selected_title);
  void draw_selected_media( void);
  void draw( bool draw_selected);
  virtual void draw( void);

  void stats( void);

};

#endif
