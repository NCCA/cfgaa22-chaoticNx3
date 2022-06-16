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
  clocktimer.start();

  m_card = std::make_unique<cardlist>(false,false);
  m_card->setrotation(ngl::Vec3(90.0f,0.0f,180.0f));
  m_card->setposition(ngl::Vec3(-5.0f,2.5f,-10.0f));

  //adds pointer to list
  cards.emplace_back(std::move(m_card));

  m_card = std::make_unique<cardlist>(false,false);
  m_card->setrotation(ngl::Vec3(90.0f,0.0f,180.0f));
  m_card->setposition(ngl::Vec3(-2.5f,2.5f,-10.0f));

  cards.emplace_back(std::move(m_card));

  m_card = std::make_unique<cardlist>(false,false);
  m_card->setrotation(ngl::Vec3(90.0f,0.0f,180.0f));
  m_card->setposition(ngl::Vec3(0.0f,2.5f,-10.0f));

  cards.emplace_back(std::move(m_card));

  m_card = std::make_unique<cardlist>(false,false);
  m_card->setrotation(ngl::Vec3(90.0f,0.0f,180.0f));
  m_card->setposition(ngl::Vec3(0.0f,-1.0f,-10.0f));

  cards.emplace_back(std::move(m_card));

  m_card = std::make_unique<cardlist>(false,false);
  m_card->setrotation(ngl::Vec3(90.0f,0.0f,180.0f));
  m_card->setposition(ngl::Vec3(-2.5f,-1.0f,-10.0f));

  cards.emplace_back(std::move(m_card));

  m_card = std::make_unique<cardlist>(false,false);
  m_card->setrotation(ngl::Vec3(90.0f,0.0f,180.0f));
  m_card->setposition(ngl::Vec3(-5.0f,-1.0f,-10.0f));

  cards.emplace_back(std::move(m_card));

  m_selectionTool = std::make_unique<cardlist>(false,false);
  m_selectionTool->setrotation(ngl::Vec3(90.0f,180.0f,0.0f));
  m_selectionTool->setposition(ngl::Vec3(-4.965f,2.48f,-10.01f));

  ngl::Texture cardtextr ("textures/Card1.png");
  cardtextr.setMultiTexture(0);
  m_cardtexture.push_back(cardtextr.setTextureGL());
  
  cardtextr.loadImage("textures/card2.png");
  cardtextr.setMultiTexture(1);
  m_cardtexture.push_back(cardtextr.setTextureGL());

  cardtextr.loadImage("textures/Card3.png");
  cardtextr.setMultiTexture(2);
  m_cardtexture.push_back(cardtextr.setTextureGL());

  cardtextr.loadImage("textures/cardback.png");
  cardtextr.setMultiTexture(3);
  m_cardtexture.push_back(cardtextr.setTextureGL());

  cardtextr.loadImage("textures/selector.png");
  cardtextr.setMultiTexture(4);
  m_cardtexture.push_back(cardtextr.setTextureGL());


  
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(CardOrder.begin(),CardOrder.end(),g);
  // for (int i = 0; i < CardOrder.size(); i++) {
  //       std::cout << CardOrder.at(i) << ' ';
  //   }
  
  

}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  
  // in game card flips

  for (int i = 0; i<cards.size(); i++)
  {
    if (cards[i]->getanimationstatus()==true)
    {
      cards[i]->setrotation(cards[i]->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
      if (cards[i]->getanimtransform().getRotation().m_y>=180)
      {
        cards[i]->setrotation(ngl::Vec3(90,180,180));
        cards[i]->setanimationstatus(false);
        cards[i]->setcardstatus(true); 
      }
    }
  }
  
  for (int i = 0; i<cards.size(); i++)
  {
    if (cards[i]->getcardstatus()==true && cards[i]->getflipitback()==true)
    {
      cards[i]->setrotation(cards[i]->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
      if (cards[i]->getanimtransform().getRotation().m_y>=0)
      {
        cards[i]->setrotation(ngl::Vec3(90,0,180));
        cards[i]->setflipitback(false);
        cards[i]->setcardstatus(false); 
        
      }
    }
  }

  //restart function

  bool allflipped = true;
  //flips facedown if not already facedown
  for (int i = 0; i<cards.size(); i++)
  {
    if (cards[i]->getcardstatus()==true && restartgamestart == true)
    {
      cards[i]->setrotation(cards[i]->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
      if (cards[i]->getanimtransform().getRotation().m_y>=0)
      {
        cards[i]->setrotation(ngl::Vec3(90,0,180));
        cards[i]->setflipitback(false);
        cards[i]->setcardstatus(false); 
        
      }
      else
      {
        allflipped = false;
      }
    }
  }

  //checks everything is face down
  if (allflipped == true)
  {
    restartgamestart = false;
  }
  
  if (shuffledone == true && restartgamestart == false)
  {
    for (int i = 0; i<cards.size(); i++)
    {
      cards[i]->setrotation(cards[i]->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
      if (cards[i]->getanimtransform().getRotation().m_y>=180)
      {
        cards[i]->setrotation(ngl::Vec3(90,180,180));
        cards[i]->setcardstatus(true); 
        shuffledone = false;
      }
    }
  }

  if (clocktimer.elapsed() >= startTime && clocktimer.elapsed() <= endTime)
  {
    istimerunning = true;
    std::cout<<clocktimer.elapsed()<<" \n";
  }
  else
  {
    istimerunning = false;
  }

  if (istimerunning == false && resetreveal == true)
  {
    for (int i = 0; i<cards.size(); i++)
    {
      if (cards[i]->getcardstatus()==true)
      {
        cards[i]->setrotation(cards[i]->getanimtransform().getRotation()+ngl::Vec3(0,40,0));
        if (cards[i]->getanimtransform().getRotation().m_y>=0)
        {
          cards[i]->setrotation(ngl::Vec3(90,0,180));
          cards[i]->setcardstatus(false); 
          resetreveal = false;
        }
      }
      
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
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[CardOrder[0]]);

  m_transform.reset();
  {
    m_transform.setPosition(cards[0]->getanimtransform().getPosition());
    m_transform.setRotation(cards[0]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane0");

  } // and before a pop

  //Back Card0

  glPolygonMode(GL_BACK,GL_FILL);
  glCullFace(GL_BACK);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[3]);

  m_transform.reset();
  {
    m_transform.setPosition(cards[0]->getanimtransform().getPosition());
    m_transform.setRotation(cards[0]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane0");

  } // and before a pop

  //Front Card1

  glPolygonMode(GL_FRONT_FACE,GL_FILL);
  glCullFace(GL_FRONT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[CardOrder[1]]);
  m_transform.reset();
  {
    m_transform.setPosition(cards[1]->getanimtransform().getPosition());
    m_transform.setRotation(cards[1]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane1");
  } // and before a pop

  //Back Card1

  glPolygonMode(GL_BACK,GL_FILL);
  glCullFace(GL_BACK);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[3]);
  m_transform.reset();
  {
    m_transform.setPosition(cards[1]->getanimtransform().getPosition());
    m_transform.setRotation(cards[1]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane1");
  } // and before a pop

  //Front Card2

  glPolygonMode(GL_FRONT_FACE,GL_FILL);
  glCullFace(GL_FRONT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[CardOrder[2]]);
  m_transform.reset();
  {
    m_transform.setPosition(cards[2]->getanimtransform().getPosition());
    m_transform.setRotation(cards[2]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane2");
  } // and before a pop

  //Back Card2

  glPolygonMode(GL_BACK,GL_FILL);
  glCullFace(GL_BACK);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[3]);
  m_transform.reset();
  {
    m_transform.setPosition(cards[2]->getanimtransform().getPosition());
    m_transform.setRotation(cards[2]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane2");
  } // and before a pop

  //Front Card3

  glPolygonMode(GL_FRONT_FACE,GL_FILL);
  glCullFace(GL_FRONT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[CardOrder[3]]);
  m_transform.reset();
  {
    m_transform.setPosition(cards[3]->getanimtransform().getPosition());
    m_transform.setRotation(cards[3]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane3");
  } // and before a pop

  //Back Card3

  glPolygonMode(GL_BACK,GL_FILL);
  glCullFace(GL_BACK);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[3]);
  m_transform.reset();
  {
    m_transform.setPosition(cards[3]->getanimtransform().getPosition());
    m_transform.setRotation(cards[3]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane3");
  } // and before a pop

  //Front Card4

  glPolygonMode(GL_FRONT_FACE,GL_FILL);
  glCullFace(GL_FRONT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[CardOrder[4]]);
  m_transform.reset();
  {
    m_transform.setPosition(cards[4]->getanimtransform().getPosition());
    m_transform.setRotation(cards[4]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane4");
  } // and before a pop

  //Back Card4

  glPolygonMode(GL_BACK,GL_FILL);
  glCullFace(GL_BACK);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[3]);
  m_transform.reset();
  {
    m_transform.setPosition(cards[4]->getanimtransform().getPosition());
    m_transform.setRotation(cards[4]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane4");
  } // and before a pop

  

  //Front Card5

  glPolygonMode(GL_FRONT_FACE,GL_FILL);
  glCullFace(GL_FRONT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[CardOrder[5]]);
  m_transform.reset();
  {
    m_transform.setPosition(cards[5]->getanimtransform().getPosition());
    m_transform.setRotation(cards[5]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane5");
  } // and before a pop

  //Back Card5

  glPolygonMode(GL_BACK,GL_FILL);
  glCullFace(GL_BACK);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[3]);
  m_transform.reset();
  {
    m_transform.setPosition(cards[5]->getanimtransform().getPosition());
    m_transform.setRotation(cards[5]->getanimtransform().getRotation());
    loadMatricesToShader();
    ngl::VAOPrimitives::draw("plane5");
  } // and before a pop

  //Front Card6

  glPolygonMode(GL_FRONT_FACE,GL_FILL);
  glCullFace(GL_FRONT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_cardtexture[4]);
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


  if (_event->key() == Qt::Key_R || _event->key() == Qt::Key_Tab)
  {
    gamestart = true;
    restartgamestart = true;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(CardOrder.begin(),CardOrder.end(),g);
    shuffledone = true;
    resetreveal = true;
    startTime = clocktimer.elapsed();
    endTime = clocktimer.elapsed() + 500;
    m_attemptOne = false;
    m_attemptTwo = false;
    m_isPairedOne = false;
    m_isPairedTwo = false;
    m_isPairedThree = false;

    printf("restarting!");
  }
  if (gamestart == true && _event->key() == Qt::Key_Up || _event->key() == Qt::Key_Right || _event->key() == Qt::Key_W || _event->key() == Qt::Key_D)
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
  if (gamestart == true && _event->key() == Qt::Key_Down || _event->key() == Qt::Key_Left || _event->key() == Qt::Key_S || _event->key() == Qt::Key_A)
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
  }
  if (gamestart == true && _event->key() == Qt::Key_Return || _event->key() == Qt::Key_Enter)
  {

    if (cards[num_ofclicks]->getcardstatus() == false)
    {
      cards[num_ofclicks]->setanimationstatus(true);
      if (m_attemptOne == true){
        m_attemptTwo = true;
        if (CardOrder[num_ofclicks] == 0){
          cards[num_ofclicks]->setTextureStatus(0);
        }
        if (CardOrder[num_ofclicks] == 1){
          cards[num_ofclicks]->setTextureStatus(1);
         }
        if (CardOrder[num_ofclicks] == 2){
          cards[num_ofclicks]->setTextureStatus(2);
        }
        choiceTwotxt = cards[num_ofclicks]->getTextureStatus();
        card2num = num_ofclicks;  
      }
       if (m_attemptOne == false){
        m_attemptOne = true;                                  
        if (CardOrder[num_ofclicks] == 0){
          cards[num_ofclicks]->setTextureStatus(0);
        }
        if (CardOrder[num_ofclicks] == 1){
           cards[num_ofclicks]->setTextureStatus(1);
         }
        if (CardOrder[num_ofclicks] == 2){
           cards[num_ofclicks]->setTextureStatus(2);
        }
        choiceOnetxt = cards[num_ofclicks]->getTextureStatus();
        card1num = num_ofclicks;
      }
    }

    if ((m_attemptOne == true)&&(m_attemptTwo == true)){
        printf("are these the same card? \n");
        if ((choiceOnetxt == 0)&&(choiceTwotxt == 0)) 
        {
          m_isPairedOne = true;
          printf("yes you got a pair!\n");
          printf("first card num is: card %d\n",card1num);
          printf("seccond card num is: card %d\n",card2num);
          m_attemptOne=false;
          m_attemptTwo=false;
        }
        if ((choiceOnetxt == 1)&&(choiceTwotxt == 1)) 
        {
          m_isPairedTwo = true;
          printf("yes you got a pair!\n");
          printf("first card num is: card %d\n",card1num);
          printf("seccond card num is: card %d\n",card2num);
          m_attemptOne=false;
          m_attemptTwo=false;
        }
        if ((choiceOnetxt == 2)&&(choiceTwotxt == 2)) 
        {
          m_isPairedThree = true;
          printf("yes you got a pair!\n");
          printf("first card num is: card %d\n",card1num);
          printf("seccond card num is: card %d\n",card2num);
          m_attemptOne=false;
          m_attemptTwo=false;
        }
    if ((m_attemptOne == true) && (m_attemptTwo == true) && (m_isPairedOne == false))
    {
      printf("first card num is: card %d\n",card1num);
      printf("seccond card num is: card %d\n",card2num);
      printf("oh no! you got it wrong!\n");
      printf("try again!\n");
      for (int i = 0; i<cards.size(); i++)
      {
        if (cards[i]->getcardstatus() == true && card1num == i || card2num == i)
        {
          cards[i]->setflipitback(true);
          printf("YOU FAILED FLIP CARD %d BACK\n", i);
        }
      }
      m_attemptOne=false;
      m_attemptTwo=false;
    }

    if ((m_attemptOne == true) && (m_attemptTwo == true) && (m_isPairedTwo == false))
    {
      printf("first card num is: card %d\n",card1num);
      printf("seccond card num is: card %d\n",card2num);
      printf("oh no! you got it wrong!\n");
      printf("try again!\n");
      for (int i = 0; i<cards.size(); i++)
      {
        if (cards[i]->getcardstatus() == true && card1num == i || card2num == i)
        {
          cards[i]->setflipitback(true);
          printf("YOU FAILED FLIP CARD %d BACK\n", i);
        }
      }
      m_attemptOne=false;
      m_attemptTwo=false;
    }

    if ((m_attemptOne == true) && (m_attemptTwo == true) && (m_isPairedThree == false))
    {
      printf("first card num is: card %d\n",card1num);
      printf("seccond card num is: card %d\n",card2num);
      printf("oh no! you got it wrong!\n");
      printf("try again!\n");
      for (int i = 0; i<cards.size(); i++)
      {
        if (cards[i]->getcardstatus() == true && card1num == i || card2num == i)
        {
          cards[i]->setflipitback(true);
          printf("YOU FAILED FLIP CARD %d BACK\n", i);
        }
      }
      m_attemptOne=false;
      m_attemptTwo=false;
    }
    
  }
  
}
  // finally update the GLWindow and re-draw
  update();
} 

