#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include "GLX_base.h"

using namespace std;

/* attributes for a single buffered visual in RGBA format with at least
 * 4 bits per color and a 16 bit depth buffer */
static int attrListSgl[] = {GLX_RGBA,
                             GLX_STENCIL_SIZE, 1,
                            GLX_RED_SIZE, 4,
                            GLX_GREEN_SIZE, 4,
                            GLX_BLUE_SIZE, 4,
                            GLX_DEPTH_SIZE, 16,
                            None};

/* attributes for a double buffered visual in RGBA format with at least
 * 4 bits per color and a 16 bit depth buffer */
static int attrListDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER,
                             GLX_STENCIL_SIZE, 1,
                             GLX_RED_SIZE, 4,
                             GLX_GREEN_SIZE, 4,
                             GLX_BLUE_SIZE, 4,
                             GLX_DEPTH_SIZE, 16,
                             None };

GLX_base::GLX_base()
{
  display_ = 0;
  screen_ = 0;
  window_ = 0;
  fullscreen_ = 0;
  double_buffer_ = false;
  x_ = y_ = 0;
  width_ = height_ = 0;
  depth_ = 0;
  microsecond_timer_ = 0;

  memset( &keys_, 0, 65536);
}

GLX_base::~GLX_base()
{
  XKeyboardControl values;
  values.auto_repeat_mode = 1;
  
  XChangeKeyboardControl( display_, KBAutoRepeatMode, &values);

  if (window_ != 0)
  {
    GLX_destroy_window();
  }
}

void GLX_base::set_display( Display *display)
{
  display_ = display;
}

Display* GLX_base::get_display( void)
{
  return display_;
}

void GLX_base::set_screen( int screen)
{
  screen_ = screen;
}

int GLX_base::get_screen( void)
{
  return screen_;
}

void GLX_base::set_double_buffer( bool double_buffer)
{
  double_buffer_ = double_buffer;
}

bool GLX_base::get_double_buffer( void)
{
  return double_buffer_;
}

void GLX_base::set_fullscreen( bool fullscreen)
{
  fullscreen_ = fullscreen;
}

bool GLX_base::get_fullscreen( void)
{
  return fullscreen_;
}

void GLX_base::set_position( int x, int y)
{
  x_ = x;
  y_ = y;
}

void GLX_base::set_width( unsigned int width)
{
  width_ = width;
}

void GLX_base::set_height( unsigned int height)
{
  height_ = height;
}

void GLX_base::set_depth( unsigned int depth)
{
  depth_ = depth;
}

unsigned int GLX_base::get_depth( void)
{
  return depth_;
}

/* Return True for Escape Key */
bool GLX_base::GLX_check_keys( void)
{
  if (keys_[XK_Escape])
  {
    return true;
  }

  return false;
}

bool GLX_base::GLX_is_key_down( KeySym key)
{
  return keys_[key];
}

void GLX_base::GLX_set_timer( int microseconds)
{
  microsecond_timer_ = microseconds;
}

void GLX_base::GLX_start_timer( void)
{
  struct itimerval value;

//  signal( SIGALRM, GLX_timer_reached); // BM: Does not work in C++!! 

  value.it_interval.tv_sec = 0;
  value.it_interval.tv_usec = microsecond_timer_;
  value.it_value.tv_sec = 0;
  value.it_value.tv_usec = microsecond_timer_;

  setitimer( ITIMER_REAL, &value, 0);
}

void GLX_base::GLX_timer_reached( int signal)
{
  GLX_start_timer();
}

bool GLX_base::GLX_idle( void)
{
  return false;
}


/* function called when our window is resized (should only happen in window mode) */
void GLX_base::GLX_resize( unsigned int width, unsigned int height)
{    
  /* Prevent A Divide By Zero If The Window Is Too Small */
  if (height == 0)
  {
    height = 1;
  }

  glViewport( 0, 0, width_, height_);    /* Reset The Current Viewport And Perspective Transformation */
  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( 45.0f, (GLfloat)width_ / (GLfloat)height_, 0.1f, 100.0f);
  glMatrixMode( GL_MODELVIEW);
}

/* general OpenGL initialization function */
bool GLX_base::GLX_init( void)
{
  glShadeModel( GL_SMOOTH);
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth( 1.0f);
  glEnable( GL_DEPTH_TEST);
  glDepthFunc( GL_LEQUAL);
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  /* we use resizeGLScene once to set up our initial perspective */
  GLX_resize( width_, height_);

  /* Reset the rotation angles of our objects */
  glFlush();

  return true;
}

/* Here goes our drawing code */
bool GLX_base::GLX_draw_scene( void)
{
  if (double_buffer_)
  {
    glXSwapBuffers( display_, window_);
  }
  return true;
}

/* function to release/destroy our resources and restoring the old desktop */
GLvoid GLX_base::GLX_destroy_window(GLvoid)
{
  if (context_)
  {
    if (!glXMakeCurrent( display_, None, NULL))
    {
      cout << "Could not release drawing context." << endl;
    }

    glXDestroyContext( display_, context_);
    context_ = NULL;
  }

  /* switch back to original desktop resolution if we were in fs */
  if (fullscreen_)
  {
    XF86VidModeSwitchToMode( display_, screen_, &desk_mode_);
    XF86VidModeSetViewPort( display_, screen_, 0, 0);
  }
  XCloseDisplay( display_);
}

/* this function creates our window and sets it up properly */
bool GLX_base::GLX_create_window( char* title, 
                                  int width, int height,
                                  bool fullscreenflag)
{
  XVisualInfo *vi;
  Colormap cmap;
  int dpyWidth, dpyHeight;
  int i;
  int glxMajorVersion, glxMinorVersion;
  int vidModeMajorVersion, vidModeMinorVersion;
  XF86VidModeModeInfo **modes;
  int modeNum;
  int bestMode;
  Atom wmDelete;
  Window winDummy;
  unsigned int borderDummy;

  fullscreen_ = fullscreenflag;

  /* set best mode to current */
  bestMode = 0;
  /* get a connection */

  display_ = XOpenDisplay( 0);
  screen_ = DefaultScreen( display_);

  XF86VidModeQueryVersion( display_, &vidModeMajorVersion, &vidModeMinorVersion);
  cout << "XF86VidModeExtension-Version: " <<  vidModeMajorVersion << " " << vidModeMinorVersion << endl;

  XF86VidModeGetAllModeLines( display_, screen_, &modeNum, &modes);

  XKeyboardControl values;
  values.auto_repeat_mode = 0;
  
  XChangeKeyboardControl( display_, KBAutoRepeatMode, &values);

  /* save desktop-resolution before switching modes */
  desk_mode_ = *modes[0];

  /* look for mode with requested resolution */
  for (i = 0; i < modeNum; i++)
  {
    if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height))
    {
      bestMode = i;
    }
  }

  /* get an appropriate visual */
  vi = glXChooseVisual( display_, screen_, attrListDbl);
  if (vi == NULL)
  {
    vi = glXChooseVisual( display_, screen_, attrListSgl);
    double_buffer_ = false;
    cout << "Only Singlebuffered Visual." << endl;
  }
  else
  {
    double_buffer_ = true;
    cout << "Got Doublebuffered Visual." << endl;
  }

  glXQueryVersion( display_, &glxMajorVersion, &glxMinorVersion);
  cout << "glX-Version " << glxMajorVersion << " " << glxMinorVersion << endl;

  /* create a GLX context */
  context_ = glXCreateContext( display_, vi, 0, GL_TRUE);

  /* create a color map */
  cmap = XCreateColormap( display_, RootWindow(display_, vi->screen), vi->visual, AllocNone);
  attributes_.colormap = cmap;
  attributes_.border_pixel = 0;
  
  if (fullscreen_)
  {
    XF86VidModeSwitchToMode( display_, screen_, modes[bestMode]);
    XF86VidModeSetViewPort( display_, screen_, 0, 0);

    dpyWidth = modes[bestMode]->hdisplay;
    dpyHeight = modes[bestMode]->vdisplay;

    cout << "Resolution " << dpyWidth << " " << dpyHeight << endl;
    XFree( modes);

    /* create a fullscreen window */
    attributes_.override_redirect = true;
    attributes_.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;

    window_ = XCreateWindow( display_, RootWindow(display_, vi->screen),
                             0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
                             CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
                             &attributes_);

    XWarpPointer( display_, None, window_, 0, 0, 0, 0, 0, 0);
    XMapRaised( display_, window_);
    XGrabKeyboard( display_, window_, true, GrabModeAsync, GrabModeAsync, CurrentTime);
    XGrabPointer( display_, window_, true, ButtonPressMask,
                  GrabModeAsync, GrabModeAsync, window_, None, CurrentTime);
  }
  else
  {
    /* create a window in window mode*/
    attributes_.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | StructureNotifyMask;
    window_ = XCreateWindow( display_, RootWindow(display_, vi->screen),
                             0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
                             CWBorderPixel | CWColormap | CWEventMask, &attributes_);

    /* only set window title and handle wm_delete_events if in windowed mode */
    wmDelete = XInternAtom( display_, "WM_DELETE_WINDOW", true);
    XSetWMProtocols( display_, window_, &wmDelete, 1);
    XSetStandardProperties( display_, window_, title,
                            title, None, NULL, 0, NULL);
    XMapRaised( display_, window_);
  }

  /* connect the glx-context to the window */
  glXMakeCurrent( display_, window_, context_);
  XGetGeometry( display_, window_, &winDummy, &x_, &y_,
                &width_, &height_, &borderDummy, &depth_);

  cout << "Depth: " << depth_ << endl;

  if (glXIsDirect(display_, context_))
  {
    cout << "Congrats, you have Direct Rendering!" << endl;
  }
  else
  {
    cout << "Sorry, no Direct Rendering possible!" << endl;
  }

  GLX_init();

  return true;
}

void GLX_base::GLX_after_creation_init( void)
{
  ;
}

void GLX_base::GLX_mainloop( void)
{
  XEvent event;
  bool done;
  int  key_idx = 0;

  done = false;
  
  /* wait for events*/
  while (!done)
  {
    /* handle the events in the queue */
    while (XPending( display_) > 0)
    {
      XNextEvent( display_, &event);
      switch (event.type)
      {
        case Expose:
          if (event.xexpose.count != 0)
            break;
          GLX_draw_scene();
          break;

        case ConfigureNotify:
          /* call resizeGLScene only if our window-size changed */
          if ((event.xconfigure.width != (int)width_) ||
              (event.xconfigure.height != (int)height_))
          {
            width_ = event.xconfigure.width;
            height_ = event.xconfigure.height;
            printf("Resize event\n");
            GLX_resize( event.xconfigure.width, event.xconfigure.height);
          }
          break;

          /* exit in case of a mouse button press */
        case ButtonPress:
          break;

        case KeyPress:
          key_idx = XLookupKeysym( &event.xkey, 0);

          if (key_idx == XK_F1)
          {
            GLX_destroy_window();
            fullscreen_ = !fullscreen_;
            GLX_create_window("NeHe's Solid Objects Tutorial", 640, 480, fullscreen_);
          }
          else
          {
            keys_[key_idx] = true;
          }
          break;

        case KeyRelease:
          keys_[XLookupKeysym(&event.xkey,0)] = false;
          break;

        case ClientMessage:
          if (*XGetAtomName( display_, event.xclient.message_type) == *"WM_PROTOCOLS")
          {
            printf("Exiting sanely...\n");
            done = true;
          }
          break;

        default:
          break;
      }
    }
    done = GLX_idle();
    done != GLX_check_keys();
    GLX_draw_scene();
  }
}
