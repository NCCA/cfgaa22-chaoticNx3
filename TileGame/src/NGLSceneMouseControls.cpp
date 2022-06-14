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
    int winX = (0);
    int winY = (0);
    winX = _event->x();//QCursor::pos().x();
    winY = _event->y(); //QCursor::pos().y();
    //QPoint pos0 = QCursor::pos(screen);
    //printf("pos0: %d, %d \n", pos0.x(), pos0.y());s
    printf("coords %d, %d\n", winX, winY);
  //   if ((winX <= 500) && (winY <=900)){
  //     printf("0\n");
  //     m_card->setanimationstatus(true);
  //     if (m_attemptOne == true){
  //       m_attemptTwo = true;
  //       m_card->setcardstatus(true);
  //     }
  //      if (m_attemptOne == false){
  //       m_attemptOne = true;
  //       m_card->setcardstatus(true);
  //     }
  //    }
  //   if ((winX >= 500) && (winY <=900)){
  //     printf("1\n");
  //     m_card1->setanimationstatus(true);
  //     if (m_attemptOne == true){
  //       m_attemptTwo = true;
  //       m_card1->setcardstatus(true);
  //      }
  //      if (m_attemptOne == false){
  //       m_attemptOne = true;
  //       m_card1->setcardstatus(true);
  //      }
  //    }
  //   if ((winX >= 500) && (winY <=900)){
  //     printf("2\n");
  //     m_card2->setanimationstatus(true);
  //     if (m_attemptOne == true){
  //       m_attemptTwo = true;
  //       m_card2->setcardstatus(true);;
  //     }
  //      if (m_attemptOne == false){
  //       m_attemptOne = true;
  //       m_card2->setcardstatus(true);
  //      }
  //    }
  //    if ((winX >= 500) && (winY <=900)){
  //     printf("3\n");
  //     m_card3->setanimationstatus(true);
  //     if (m_attemptOne == true){
  //       m_attemptTwo = true;
  //       m_card3->setcardstatus(true);
  //     }
  //     if (m_attemptOne == false){
  //       m_attemptOne = true;
  //       m_card3->setcardstatus(true);
  //     }
  //    }
  //   if ((winX >= 500) && (winY <=900)){
  //     printf("4\n");
  //     m_card4->setanimationstatus(true);
  //     if (m_attemptOne == true){
  //       m_attemptTwo = true;
  //       m_card4->setcardstatus(true);
  //      }
  //      if (m_attemptOne == false){
  //       m_attemptOne = true;
  //       m_card4->setcardstatus(true);
  //     }
  //    }
  //    if ((winX >= 500) && (winY <=900)){
  //     printf("5\n");
  //     m_card5->setanimationstatus(true);
  //     if (m_attemptOne == true){
  //       m_attemptTwo = true;
  //       m_card5->setcardstatus(true);
  //      }
  //      if (m_attemptOne == false){
  //       m_attemptOne = true;
  //       m_card5->setcardstatus(true);
  //     }
  //    }
  // }
  // // right mouse translate mode
  // else if ( _event->button() == Qt::RightButton )
  // {
  //   printf("lol");
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
    if ((m_attemptOne == true)&&(m_attemptTwo == true)){
        printf("are these the same card? \n");
        if ((m_card->getcardstatus() == true)&&(m_card1->getcardstatus() == true == true)){
          m_isPairedOne = true;
          printf("yes");
          m_attemptOne=false;
          m_attemptTwo=false;
        }
        if ((m_card2->getcardstatus() == true)&&(m_card3->getcardstatus() == true == true)){
          m_isPairedTwo = true;
          printf("yes");
          m_attemptOne=false;
          m_attemptTwo=false;
        }
        if ((m_card4->getcardstatus() == true)&&(m_card5->getcardstatus() == true == true)){
          m_isPairedThree = true;
          printf("yes");
          m_attemptOne=false;
          m_attemptTwo=false;
        }
        m_attemptOne=false;
        m_attemptTwo=false;
      }
  }
  // right mouse translate mode
  if ( _event->button() == Qt::RightButton )
  {
    printf("loldone");
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
