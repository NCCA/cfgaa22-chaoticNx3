#include <QMouseEvent>
#include <QGuiApplication>
#include "NGLScene.h"
#include <ngl/Transformation.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <iostream>
#include <ngl/Texture.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <random>

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("TileGame");
  m_animate=true;
}

NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_project=ngl::perspective(45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}


constexpr auto shaderProgram = "PBR";
void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // now to load the shader and set the values
  // grab an instance of shader manager
  // in the code create some constexpr
  constexpr auto vertexShader  = "PBRVertex";
  constexpr auto fragShader    = "PBRFragment";
  // create the shader program
  ngl::ShaderLib::createShaderProgram( shaderProgram );
  // now we are going to create empty shaders for Frag and Vert
  ngl::ShaderLib::attachShader( vertexShader, ngl::ShaderType::VERTEX );
  ngl::ShaderLib::attachShader( fragShader, ngl::ShaderType::FRAGMENT );
  // attach the source
  ngl::ShaderLib::loadShaderSource( vertexShader, "shaders/PBRVertex.glsl" );
  ngl::ShaderLib::loadShaderSource( fragShader, "shaders/PBRFragment.glsl" );
  // compile the shaders
  ngl::ShaderLib::compileShader( vertexShader );
  ngl::ShaderLib::compileShader( fragShader );
  // add them to the program
  ngl::ShaderLib::attachShaderToProgram( shaderProgram, vertexShader );
  ngl::ShaderLib::attachShaderToProgram( shaderProgram, fragShader );
  // now we have associated that data we can link the shader
  ngl::ShaderLib::linkProgramObject( shaderProgram );
  // and make it active ready to load values
  ngl::ShaderLib::use(shaderProgram );

  ngl::ShaderLib::loadShader(ColourCard, "shaders/CardVertex.glsl",
                                                "shaders/CardFragment.glsl");
  // We now create our view matrix for a static camera
  ngl::Vec3 from(0,0,1);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_view=ngl::lookAt(from,to,up);
  ngl::ShaderLib::setUniform( "camPos", from ); 
  // now a light
  m_lightPos.set( 0.0, 2.0f, 2.0f ,1.0f);
  // setup the default shader material and light porerties
  // these are "uniform" so will retain their values
  ngl::ShaderLib::setUniform("lightPosition",m_lightPos.toVec3());
  ngl::ShaderLib::setUniform("lightColor",400.0f,400.0f,400.0f);
  ngl::ShaderLib::setUniform("exposure",2.2f);
  ngl::ShaderLib::setUniform("albedo",0.950f, 0.71f, 0.29f);

  ngl::ShaderLib::setUniform("metallic",1.02f);
  ngl::ShaderLib::setUniform("roughness",0.38f);
  ngl::ShaderLib::setUniform("ao",0.2f);
  ngl::VAOPrimitives::createTrianglePlane("plane0",2,2.75,80,80,ngl::Vec3(0,1,0));
  ngl::VAOPrimitives::createTrianglePlane("plane1",2,2.75,80,80,ngl::Vec3(0,1,0));
  ngl::VAOPrimitives::createTrianglePlane("plane2",2,2.75,80,80,ngl::Vec3(0,1,0));
  ngl::VAOPrimitives::createTrianglePlane("plane3",2,2.75,80,80,ngl::Vec3(0,1,0));
  ngl::VAOPrimitives::createTrianglePlane("plane4",2,2.75,80,80,ngl::Vec3(0,1,0));
  ngl::VAOPrimitives::createTrianglePlane("plane5",2,2.75,80,80,ngl::Vec3(0,1,0));
  ngl::VAOPrimitives::createTrianglePlane("selector",2.25,3,80,80,ngl::Vec3(0,1,0));
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
  // this timer is going to trigger an event every 40ms which will be processed in the
  //
  m_lightTimer =startTimer(40);

  m_card = std::make_unique<cardlist>(false,false);
  m_card->setrotation(ngl::Vec3(90.0f,0.0f,0.0f));
  m_card->setposition(ngl::Vec3(-5.0f,2.5f,-10.0f));

  m_card1 = std::make_unique<cardlist>(false,false);
  m_card1->setrotation(ngl::Vec3(90.0f,0.0f,0.0f));
  m_card1->setposition(ngl::Vec3(-2.5f,2.5f,-10.0f));

  m_card2 = std::make_unique<cardlist>(false,false);
  m_card2->setrotation(ngl::Vec3(90.0f,0.0f,0.0f));
  m_card2->setposition(ngl::Vec3(0.0f,2.5f,-10.0f));

  m_card3 = std::make_unique<cardlist>(false,false);
  m_card3->setrotation(ngl::Vec3(90.0f,0.0f,0.0f));
  m_card3->setposition(ngl::Vec3(0.0f,-1.0f,-10.0f));

  m_card4 = std::make_unique<cardlist>(false,false);
  m_card4->setrotation(ngl::Vec3(90.0f,0.0f,0.0f));
  m_card4->setposition(ngl::Vec3(-2.5f,-1.0f,-10.0f));

  m_card5 = std::make_unique<cardlist>(false,false);
  m_card5->setrotation(ngl::Vec3(90.0f,0.0f,0.0f));
  m_card5->setposition(ngl::Vec3(-5.0f,-1.0f,-10.0f));

  m_selectionTool = std::make_unique<cardlist>(false,false);
  m_selectionTool->setrotation(ngl::Vec3(90.0f,180.0f,0.0f));
  m_selectionTool->setposition(ngl::Vec3(-4.965f,2.48f,-10.01f));

  ngl::Texture cardtextr ("textures/cata2.png");
  cardtextr.setMultiTexture(0);
  m_cardtexture.push_back(cardtextr.setTextureGL());
  
  cardtextr.loadImage("textures/gasp.jpg");
  cardtextr.setMultiTexture(1);
  m_cardtexture.push_back(cardtextr.setTextureGL());

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(CardOrder.begin(),CardOrder.end(),g);
  for (int i = 0; i < CardOrder.size(); i++) {
        std::cout << CardOrder.at(i) << ' ';
    }

}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  
  if (m_card->getanimationstatus()==true){
    m_card->setrotation(m_card->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
    if (m_card->getanimtransform().getRotation().m_y>=180)
    {
      m_card->setrotation(ngl::Vec3(90,180,0));
      m_card->setanimationstatus(false);
    }
  }
  if (m_card1->getanimationstatus()==true){
    m_card1->setrotation(m_card1->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
    if (m_card1->getanimtransform().getRotation().m_y>=180)
    {
      m_card1->setrotation(ngl::Vec3(90,180,0));
      m_card1->setanimationstatus(false);
    }
  }
  if (m_card2->getanimationstatus()==true){
    m_card2->setrotation(m_card2->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
    if (m_card2->getanimtransform().getRotation().m_y>=180)
    {
      m_card2->setrotation(ngl::Vec3(90,180,0));
      m_card2->setanimationstatus(false);
    }
  }
   if (m_card3->getanimationstatus()==true){
    m_card3->setrotation(m_card3->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
    if (m_card3->getanimtransform().getRotation().m_y>=180)
    {
      m_card3->setrotation(ngl::Vec3(90,180,0));
      m_card3->setanimationstatus(false);
    }
  }
   if (m_card4->getanimationstatus()==true){
    m_card4->setrotation(m_card4->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
    if (m_card4->getanimtransform().getRotation().m_y>=180)
    {
      m_card4->setrotation(ngl::Vec3(90,180,0));
      m_card4->setanimationstatus(false);
    }
  }
   if (m_card5->getanimationstatus()==true){
    m_card5->setrotation(m_card5->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
    if (m_card5->getanimtransform().getRotation().m_y>=180)
    {
      m_card5->setrotation(ngl::Vec3(90,180,0));
      m_card5->setanimationstatus(false);
    }
  }
  update();
}

void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib::setUniform("MVP", m_project*m_view*m_transform.getMatrix());
    // ngl::ShaderLib::use("PBR");
    // struct transform
    // {
    //   ngl::Mat4 MVP;
    //   ngl::Mat4 normalMatrix;
    //   ngl::Mat4 M;
    // };

    //  transform t;
    //  t.M=m_view*m_mouseGlobalTX*m_transform.getMatrix();

    //  t.MVP=m_project*t.M;
    //  t.normalMatrix=t.M;
    //  t.normalMatrix.inverse().transpose();
    //  ngl::ShaderLib::setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);
    //  ngl::ShaderLib::setUniform("lightPosition",(m_mouseGlobalTX*m_lightPos).toVec3());

}

void NGLScene::drawScene()
{
  glEnable(GL_CULL_FACE);
  ngl::ShaderLib::use(ColourCard);
  // clear the screen and depth buffer
  // Rotation based on the mouse position for our global
  // transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_win.spinXFace);
  rotY.rotateY(m_win.spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  //Front Card0

  glPolygonMode(GL_FRONT_FACE,GL_FILL);
  glCullFace(GL_FRONT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[0]);

  m_transform.reset();
  {
    m_transform.setPosition(m_card->getanimtransform().getPosition());
    m_transform.setRotation(m_card->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane0");

  } // and before a pop

  //Back Card0

  glPolygonMode(GL_BACK,GL_FILL);
  glCullFace(GL_BACK);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[1]);

  m_transform.reset();
  {
    m_transform.setPosition(m_card->getanimtransform().getPosition());
    m_transform.setRotation(m_card->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane0");

  } // and before a pop

  //Front Card1

  glPolygonMode(GL_FRONT_FACE,GL_FILL);
  glCullFace(GL_FRONT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[1]);
  m_transform.reset();
  {
    m_transform.setPosition(m_card1->getanimtransform().getPosition());
    m_transform.setRotation(m_card1->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane1");
  } // and before a pop

  //Back Card1

  glPolygonMode(GL_BACK,GL_FILL);
  glCullFace(GL_BACK);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[0]);

  m_transform.reset();
  {
    m_transform.setPosition(m_card1->getanimtransform().getPosition());
    m_transform.setRotation(m_card1->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane1");
  } // and before a pop

  //Front Card2

  m_transform.reset();
  {
    m_transform.setPosition(m_card2->getanimtransform().getPosition());
    m_transform.setRotation(m_card2->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane2");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(m_card3->getanimtransform().getPosition());
    m_transform.setRotation(m_card3->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane3");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(m_card4->getanimtransform().getPosition());
    m_transform.setRotation(m_card4->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane4");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(m_card5->getanimtransform().getPosition());
    m_transform.setRotation(m_card5->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane5");
  } // and before a pop

   m_transform.reset();
  {
    m_transform.setPosition(m_selectionTool->getanimtransform().getPosition());
    m_transform.setRotation(m_selectionTool->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("selector");
  } // and before a pop


}


void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  drawScene();

}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  if (_event->key() == Qt::Key_Up || _event->key() == Qt::Key_W)
  {
    //  for (int i=0; i<=5; i++) mylist.push_back (i+1);
    //  std::cout << "mylist:";
    //  std::for_each (mylist.begin(),
    //                 std::next(mylist.begin(),1),
    //                 [](int x) {std::cout << ' ' << x;});
    //  std::cout << '\n';
    num_ofclicks+=1; 
    if (num_ofclicks==6)
    {
      num_ofclicks=0;
    }

    if (num_ofclicks == 0){
      m_selectionTool->setposition(ngl::Vec3(-4.965f,2.48f,-10.01f));
    }
    if (num_ofclicks == 1){
      m_selectionTool->setposition(ngl::Vec3(-2.466f,2.48f,-10.01f));
    }
    if (num_ofclicks == 2){
      m_selectionTool->setposition(ngl::Vec3(0.0465f,2.48f,-10.01f));
    }
    if (num_ofclicks == 3){
      m_selectionTool->setposition(ngl::Vec3(0.0465f,-0.98f,-10.01f));
    }
    if (num_ofclicks == 4){
      m_selectionTool->setposition(ngl::Vec3(-2.466f,-0.98f,-10.01f));
    }
    if (num_ofclicks == 5){
      m_selectionTool->setposition(ngl::Vec3(-4.965f,-0.98f,-10.01f));
    }
    printf("%d",num_ofclicks);
  }
  if (_event->key() == Qt::Key_Down || _event->key() == Qt::Key_S)
  {
    //  for (int i=0; i<=5; i++) mylist.push_back (i+1);
    //  std::cout << "mylist:";
    //  std::for_each (mylist.begin(),
    //                 std::next(mylist.begin(),1),
    //                 [](int x) {std::cout << ' ' << x;});
    //  std::cout << '\n';
    num_ofclicks-=1; 
    if (num_ofclicks==-1)
    {
      num_ofclicks=5;
    }
    printf("%d",num_ofclicks);

    if (num_ofclicks == 0){
      m_selectionTool->setposition(ngl::Vec3(-4.965f,2.48f,-10.01f));
    }
  }
  if (_event->key() == Qt::Key_Return || _event->key() == Qt::Key_Enter)
  {
    if (num_ofclicks == 0)
    {
      m_card->setanimationstatus(true);
      if (m_attemptOne == true){
        m_attemptTwo = true;
        m_card->setcardstatus(true);
      }
       if (m_attemptOne == false){
        m_attemptOne = true;
        m_card->setcardstatus(true);
      }
    }
    if (num_ofclicks == 1){
      printf("1\n");
      m_card1->setanimationstatus(true);
      if (m_attemptOne == true){
        m_attemptTwo = true;
        m_card1->setcardstatus(true);
      }
      if (m_attemptOne == false){
        m_attemptOne = true;
        m_card1->setcardstatus(true);
      }
    }
    if (num_ofclicks == 2){
      printf("2\n");
      m_card2->setanimationstatus(true);
      if (m_attemptOne == true){
        m_attemptTwo = true;
        m_card2->setcardstatus(true);;
      }
      if (m_attemptOne == false){
        m_attemptOne = true;
        m_card2->setcardstatus(true);
      }
    }
    if (num_ofclicks == 3){
      printf("3\n");
      m_card3->setanimationstatus(true);
      if (m_attemptOne == true){
        m_attemptTwo = true;
        m_card3->setcardstatus(true);
      }
      if (m_attemptOne == false){
        m_attemptOne = true;
        m_card3->setcardstatus(true);
      }
    }
    if (num_ofclicks == 4){
      printf("4\n");
      m_card4->setanimationstatus(true);
      if (m_attemptOne == true){
        m_attemptTwo = true;
        m_card4->setcardstatus(true);
      }
      if (m_attemptOne == false){
        m_attemptOne = true;
        m_card4->setcardstatus(true);
      }
    }
    if (num_ofclicks == 5){
      printf("5\n");
      m_card5->setanimationstatus(true);
      if (m_attemptOne == true){
        m_attemptTwo = true;
        m_card5->setcardstatus(true);
      }
      if (m_attemptOne == false){
        m_attemptOne = true;
        m_card5->setcardstatus(true);
      }
    }
    
  }
  // finally update the GLWindow and re-draw

    update();
} 

