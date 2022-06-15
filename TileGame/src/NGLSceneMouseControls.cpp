#include "NGLScene.h"
#include <QMouseEvent>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent( QMouseEvent* _event )
{
  // that method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if ( _event->button() == Qt::LeftButton )
  {
    
  }
   update();
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent( QMouseEvent* _event )
{
  // that event is called when the mouse button is released
  // we then set Rotate to false
  if ( _event->button() == Qt::LeftButton )
  {
   
  }
  // right mouse translate mode
  if ( _event->button() == Qt::RightButton )
  {
  
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent( QWheelEvent* _event )
{

  // check the diff of the wheel position (0 means no change)
  if ( _event->angleDelta().y() > 0 )
  {
    m_modelPos.m_z += ZOOM;
  }
  else if ( _event->angleDelta().y() < 0 )
  {
    m_modelPos.m_z -= ZOOM;
  }
  update();
}
