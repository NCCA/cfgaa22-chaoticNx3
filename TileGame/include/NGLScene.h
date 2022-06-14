#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <ngl/Mat4.h>
#include "cardlist.h"
#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include "WindowParams.h"
#include <memory>
// this must be included after NGL includes else we get a clash with gl libs
#include <QOpenGLWindow>
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we resize the window
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h) override;


private:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the windows params such as mouse and rotations etc
    //----------------------------------------------------------------------------------------------------------------------
    WinParams m_win;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the global mouse transforms
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_mouseGlobalTX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our Camera
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief transformation stack for the gl transformations etc
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Transformation m_transform;
    ngl::Transformation m_card0anim;
    ngl::Transformation m_card1anim;
    ngl::Transformation m_card2anim;
    ngl::Transformation m_card3anim;
    ngl::Transformation m_card4anim;
    ngl::Transformation m_card5anim;
    std::vector<GLuint>m_cardtexture;
    std::vector<int>CardOrder = {0,0,1,1,2,2};
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a simple light use to illuminate the screen
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec4 m_lightPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief timer id for the animation timer
    //----------------------------------------------------------------------------------------------------------------------
    int m_lightTimer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if were animating the light
    //----------------------------------------------------------------------------------------------------------------------
    bool m_animate;
    //----------------------------------------------------------------------------------------------------------------------
    /// the angle of the light updated when the timer triggers to make it rotate
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real m_lightAngle;
    
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to load transform matrices to the shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event ) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event) override;
    /// @brief windows parameters for mouse control etc.
    //----------------------------------------------------------------------------------------------------------------------
    void updateLight();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief draw our scene passing in the shader to use
    /// @param[in] _shader the name of the shader to use when drawing
    //----------------------------------------------------------------------------------------------------------------------
    void drawScene();
    void timerEvent(QTimerEvent *_event) override;
    //animation status checkers
    //attempt checkers
    bool m_attemptOne = false;
    bool m_attemptTwo = false;
    //pair checkers
    bool m_isPairedOne = false;
    bool m_isPairedTwo = false;
    bool m_isPairedThree = false;
    //classforcardanimstuff
    std::unique_ptr<cardlist>m_card;
    std::unique_ptr<cardlist>m_card1;
    std::unique_ptr<cardlist>m_card2;
    std::unique_ptr<cardlist>m_card3;
    std::unique_ptr<cardlist>m_card4;
    std::unique_ptr<cardlist>m_card5;
    std::unique_ptr<cardlist>m_selectionTool;
    
    int num_ofclicks = 0;

    const char*ColourCard = "ColourCard";
    
};



#endif
