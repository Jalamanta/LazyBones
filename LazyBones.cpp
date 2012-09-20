#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <dirent.h>
#include <sys/timex.h>
#include <FTGL/FTGLPixmapFont.h>
#include <FTGL/FTGLPolygonFont.h>
#include "GFX_playlist.h"
#include "GFX_media.h"
#include "BMT_texture_manager.h"
#include "BMT_jpeg.h"
#include "LazyBones.h"
#include "LBMT_collection.h"

// 15 1366 / 576 = 2.371527778
// 40 1024 / 192 = 5.333333333

using namespace std;

const string database="mythconverg";
//const string hostname="tk421";
const string hostname="localhost";
const string username="mythtv";
const string password="mythtv";

#define COMIC_FONT "/usr/share/fonts/corefonts/comicbd.ttf"

#define PLAYLIST_UPDOWN_TIME 300

const float GLX_LazyBones::media_left_delta_ = 0.25;
const float GLX_LazyBones::max_media_left_right_time_ = 350.0;
const float GLX_LazyBones::min_media_left_right_time_ = 80.0;

static bool    escape = false;

static clock_t begin_prof_time = 0;
static clock_t end_prof_time = 0;
static float prof_time = 0.0;


static void profile_begin( void)
{
  begin_prof_time = clock();
}

static void profile_end( void)
{
  end_prof_time = clock();

  prof_time = (float)(end_prof_time - begin_prof_time) / (float)CLOCKS_PER_SEC;

  printf("prof_time: %f\n", prof_time);
}

GLX_LazyBones::GLX_LazyBones()
{
  string cache_directory;

  GLX_init();

  cache_directory =  "/home/bmay/.cache/";

  texture_manager_ = new BMT_texture_manager();

  texture_manager_->set_cache_directory( cache_directory);

  media_left_right_time_ = max_media_left_right_time_;

  cd_collection_ = new GFX_playlist( texture_manager_, 0, 193, 1366, 576);
  cd_collection_->set_position( 0.0, -0.010, -0.4);
  cd_collection_->set_scale( 1.0);

  current_playlist_ = new GFX_playlist( texture_manager_, 342, 0, 1024, 192);
  current_playlist_->set_position( 0.0, -0.01, -0.4);
  current_playlist_->set_scale( 1.0);

  selected_playlist_ = cd_collection_;

  current_animation_ = ANIM_NONE;

//  cd_collection_->animate_playlist_up( PLAYLIST_UPDOWN_TIME);
//  current_playlist_->animate_playlist_up( PLAYLIST_UPDOWN_TIME);

}

GLX_LazyBones::~GLX_LazyBones()
{
  ;
}

void GLX_LazyBones::GLX_resize( unsigned int width, unsigned int height)
{
  glViewport( 0, 0, (GLsizei) width, (GLsizei) height);
  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 20.0f);
  glMatrixMode( GL_MODELVIEW);
  glLoadIdentity();
}

void GLX_LazyBones::display_control_buttons( void) 
{
  return;

  glEnable( GL_LIGHTING);
  glDisable( GL_BLEND);

  play_button_.draw();
  stop_button_.draw();
  rewind_button_.draw();
  forward_button_.draw();
}

void GLX_LazyBones::display_now_playing_title( void)
{
  string title;

  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0, width_, height_, 0, -1.0f, 1.0f);
  glMatrixMode( GL_MODELVIEW);
  glLoadIdentity();

  glDisable( GL_DEPTH_TEST);

  glDisable( GL_LIGHTING);
  glColor3f( 0.6, 0.6, 0.6);

  title = "Now Playing:";
  glRasterPos2i( 10, 625); 
  comic_font_small_->Render( title.c_str());

  glColor3f( 0.7, 0.7, 0.7);

  title = "Rage Against The Machine";
  glRasterPos2i( 10, 660); 
  comic_font_small_->Render( title.c_str());

  title = "\"The Battle Of Los Angeles\"";
  glRasterPos2i( 10, 685); 
  comic_font_small_->Render( title.c_str());

  title = "06 Leave Me Out";
  glRasterPos2i( 10, 710); 
  comic_font_small_->Render( title.c_str());

  title = "01:24 / 6:37";
  glRasterPos2i( 10, 735); 
  comic_font_small_->Render( title.c_str());
}

void GLX_LazyBones::set_focus_playlist( void)
{
  GFX_anim_node anim_node_collection;
  GFX_anim_node anim_node_playlist;

  anim_node_playlist.set_origin_screen_position( 342, 0);
  anim_node_playlist.set_origin_screen_size( 1024, 192);

  anim_node_playlist.set_destination_screen_position( 0, 0);
  anim_node_playlist.set_destination_screen_size( 1366, 576);
  anim_node_playlist.set_time( 1000);

  current_playlist_->add_node( anim_node_playlist);
  current_playlist_->set_animate( true);

  anim_node_collection.set_origin_screen_position( 0, 193);
  anim_node_collection.set_origin_screen_size( 1366, 576);

  anim_node_collection.set_destination_screen_position( 0, 576);
  anim_node_collection.set_destination_screen_size( 1366, 192);
  anim_node_collection.set_time( 1000);

  cd_collection_->add_node( anim_node_collection);
  cd_collection_->set_animate( true);
  
  current_animation_ = ANIM_DOWN;
}

void GLX_LazyBones::set_focus_collection( void)
{
  GFX_anim_node anim_node_collection;
  GFX_anim_node anim_node_playlist;

  anim_node_playlist.set_origin_screen_position( 0, 0);
  anim_node_playlist.set_origin_screen_size( 1366, 576);

  anim_node_playlist.set_destination_screen_position( 342, 0);
  anim_node_playlist.set_destination_screen_size( 1024, 192);
  anim_node_playlist.set_time( 1000);

  current_playlist_->add_node( anim_node_playlist);
  current_playlist_->set_animate( true);

  anim_node_collection.set_origin_screen_position( 0, 576);
  anim_node_collection.set_origin_screen_size( 1366, 192);

  anim_node_collection.set_destination_screen_position( 0, 193);
  anim_node_collection.set_destination_screen_size( 1366, 576);
  anim_node_collection.set_time( 1000);

  cd_collection_->add_node( anim_node_collection);
  cd_collection_->set_animate( true);

  current_animation_ = ANIM_DOWN;
}

bool GLX_LazyBones::GLX_draw_scene( void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glDisable( GL_LIGHTING);

  cd_collection_->draw();
  current_playlist_->draw( false);
  
  glViewport( 0, 0, width_, height_);
  display_now_playing_title();

  if (double_buffer_)
  {
    glXSwapBuffers( display_, window_);
  }
  
  return True;
}

bool GLX_LazyBones::GLX_check_keys( void)
{
  if (GLX_is_key_down( XK_Escape))
  {
    if (selected_playlist_->get_animate() == false)
    {
      selected_playlist_->animate_playlist_down( PLAYLIST_UPDOWN_TIME);
      escape = true;
    }
  }

  if (GLX_is_key_down( XK_Down))
  {
    if (selected_playlist_->get_animate() == false)
    {
      if (selected_playlist_ == cd_collection_)
      {
        selected_playlist_ = current_playlist_;
        set_focus_playlist();
      }
//      selected_playlist_->animate_playlist_down( PLAYLIST_UPDOWN_TIME);
    }
  }
  if (GLX_is_key_down( XK_Up))
  {
    if (selected_playlist_->get_animate() == false)
    {
      if (selected_playlist_ == current_playlist_)
      {
        selected_playlist_ = cd_collection_;
        set_focus_collection();
      }
//      selected_playlist_->animate_playlist_up( PLAYLIST_UPDOWN_TIME);
    }
  }

  if (GLX_is_key_down( XK_Left))
  {
    if (selected_playlist_->get_animate() == false)
    {
      selected_playlist_->animate_all_media_right( (int)media_left_right_time_);
    }
    media_left_right_time_ -= media_left_delta_;
    if (media_left_right_time_ < min_media_left_right_time_)
    {
      media_left_right_time_ = min_media_left_right_time_;
    }
  }
  else
  {
    if (GLX_is_key_down( XK_Right) == false)
    {
      media_left_right_time_ += media_left_delta_;
      if (media_left_right_time_ > max_media_left_right_time_)
      {
        media_left_right_time_ = max_media_left_right_time_;
      }
    }
  }

  if (GLX_is_key_down( XK_Left) == false &&
      GLX_is_key_down( XK_Right) == false)
  {
    media_left_right_time_ = max_media_left_right_time_;
  }
      

  if (GLX_is_key_down( XK_Right))
  {
    if (selected_playlist_->get_animate() == false)
    {
      selected_playlist_->animate_all_media_left( (int)media_left_right_time_);
    }
    media_left_right_time_ -= media_left_delta_;
    if (media_left_right_time_ < min_media_left_right_time_)
    {
      media_left_right_time_ = min_media_left_right_time_;
    }
  }
  else
  {
    if (GLX_is_key_down( XK_Left) == false)
    {
      media_left_right_time_ += media_left_delta_;
      if (media_left_right_time_ > max_media_left_right_time_)
      {
        media_left_right_time_ = max_media_left_right_time_;
      }
    }
  }

  if (GLX_is_key_down( XK_g))
  {
    if (selected_playlist_->get_animate() == false)
    {
      selected_playlist_->spin_selected_media();
    }
  }
  
  if (GLX_is_key_down( XK_Return))
  {
    cout << "cd_collection_: "; cd_collection_->stats();
    cout << "current_playlist_:: "; current_playlist_->stats();

    BMT_jpeg *screenshot = new BMT_jpeg();
    screenshot->screen_to_image( width_, height_, 3);
    screenshot->save( "screenshot.jpg", 95);
  }

  if (GLX_is_key_down( XK_x))
  {
    play_button_.set_animate( true);
  }

  if (GLX_is_key_down( XK_z))
  {
    rewind_button_.set_animate( true);
  }

  if (GLX_is_key_down( XK_v))
  {
    stop_button_.set_animate( true);
  }

  if (GLX_is_key_down( XK_b))
  {
    forward_button_.set_animate( true);
  }

  if (GLX_is_key_down( XK_Page_Up))
  {
    cout << alpha_ << endl;
    alpha_ += 0.05;
    if (alpha_ > 1.0)
    {
      alpha_ = 1.0;
    }
  }

  if (GLX_is_key_down( XK_Page_Down))
  {
    cout << alpha_ << endl;
    alpha_ -= 0.05;
    if (alpha_ < 0.0)
    {
      alpha_ = 0.0;
    }
  }

  return False;
}

bool GLX_LazyBones::GLX_idle( void)
{
  bool animate = false;
  bool was_animating = false;
  bool result = false;
  int millisecs_elapsed = 10;
  static struct ntptimeval previous_time = {0};
  struct ntptimeval current_time;

  ntp_gettime( &current_time);

  if (previous_time.time.tv_sec != 0)
  {
    millisecs_elapsed = ((current_time.time.tv_sec - previous_time.time.tv_sec) * 1000) +
                        ((current_time.time.tv_usec - previous_time.time.tv_usec) / 1000);
  }

//  cout << millisecs_elapsed << endl;

  previous_time = current_time;

  was_animating = cd_collection_->get_animate() | current_playlist_->get_animate();
  animate = cd_collection_->animate( millisecs_elapsed);
  animate &= current_playlist_->animate( millisecs_elapsed);

  if (was_animating && animate == false)
  {
  }

  if (animate)
  {
    cd_collection_->resize();
    current_playlist_->resize();
  }

  if (animate == false && escape)
  {
    result = true;
  }

  usleep( 1000);

  return result;
}

void GLX_LazyBones::set_light_ambient( GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
  light_ambient[0] = a;
  light_ambient[1] = b;
  light_ambient[2] = c;
  light_ambient[3] = d;
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
}

void GLX_LazyBones::set_light_diffuse( GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
  light_diffuse[0] = a;
  light_diffuse[1] = b;
  light_diffuse[2] = c;
  light_diffuse[3] = d;
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
}

void GLX_LazyBones::set_light_position( GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
  light_position[0] = a;
  light_position[1] = b;
  light_position[2] = c;
  light_position[3] = d;
  glLightfv(GL_LIGHT1, GL_POSITION,light_position);
}

bool GLX_LazyBones::GLX_init( void)
{
  glClearColor( 0.0, 0.0, 0.0, 0.0);
  glClearDepth( 1.0);
//  glDepthFunc( GL_LESS);
  glEnable( GL_DEPTH_TEST);
  glShadeModel( GL_SMOOTH);

  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width_/(GLfloat)height_, 0.1f, 20.0f);
  glMatrixMode( GL_MODELVIEW);
  glLoadIdentity();

  // set up light number 1.
  set_light_ambient( 0.9f, 0.9f, 0.9f, 1.0f);
  set_light_diffuse( 1.0f, 1.0f, 1.0f, 1.0f);
  set_light_position( 0.0f, -0.5f, 2.0f, 0.0f);

  glEnable(GL_LIGHT1);                            
  glEnable( GL_LIGHTING);

  comic_font_small_ = new FTGLPixmapFont( COMIC_FONT);
  comic_font_small_->FaceSize( 18);
  comic_font_small_->UseDisplayList( true);

  comic_font_large_ = new FTGLPolygonFont( COMIC_FONT);
  comic_font_large_->FaceSize( 34);
  comic_font_large_->UseDisplayList( true);

  alpha_ = 0.8;

  return True;
}

void GLX_LazyBones::load_database( void)
{
  string coverart;

  cout << "load_database" << endl;

  database_ = db_factory_.createDatabaseConnection( "SQL", hostname, database, username, password, true);

  LBMT_Collection *collection = database_->get_collection_by_name( "All Music");

  cout << "load_database" << endl;

  if (collection != 0)
  {
    LBMT_Collection::media_iterator media_it;
    for (media_it = collection->get_iterator_begin(); media_it != collection->get_iterator_end(); media_it++)
    {
      coverart = (*media_it)->get_coverart();
      if (coverart != "")
      {
        cd_collection_->add_media( dynamic_cast<LBMT_Album*>(*media_it));
        current_playlist_->add_media( dynamic_cast<LBMT_Album*>(*media_it));
      }
    }
  }

#if 0

  LBDC_database::album_iterator album_it;

  for (album_it = database_->get_album_iterator_begin(); album_it != database_->get_album_iterator_end(); album_it++)
  {
    cout << **album_it << endl;
  }

  LBDC_database::playlist_iterator playlist_it;
  for (playlist_it = database_->get_playlist_iterator_begin(); playlist_it != database_->get_playlist_iterator_end(); playlist_it++)
  {
    cout << **playlist_it << endl;
  }
#endif
}

int main( int argc, char **argv)
{
  GLX_LazyBones app;

  app.set_position( 50, 50);

  app.GLX_create_window( "LazyBones", 1366, 768, false);

  app.load_database();

  app.GLX_mainloop();
}
