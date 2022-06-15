#include "cardlist.h"
#include <iostream>
#include <fstream>
#include <sstream>

cardlist::cardlist(bool _isAnimtrue, bool _iscardflipped)
{
	m_isAnimTrue = _isAnimtrue;
	m_iscardflipped = _iscardflipped;
}

ngl::Transformation cardlist::getanimtransform()
{
	return m_cardanimation;
}
bool cardlist::getanimationstatus()
{
	return m_isAnimTrue;
}
bool cardlist::getcardstatus()
{
	return m_iscardflipped;
}

bool cardlist::getflipitback()
{
	return m_flipitback;
}

int cardlist::getTextureStatus()
{
	return m_cardTextureValue;
}

void cardlist::setTextureStatus(int _cardTextureValue)
{
	m_cardTextureValue = _cardTextureValue;
}

ngl::Transformation cardlist::card_ls(ngl::Vec3 _getcardPL)
{
	return m_getcardPL;
}
void cardlist::setanimtansform(ngl::Transformation _cardanimation)
{
	m_cardanimation = _cardanimation;
}
void cardlist::setanimationstatus(bool _isAnimtrue)
{
	m_isAnimTrue = _isAnimtrue;
}
void cardlist::setstartanimationstatus(bool _isStartAnimtrue)
{
	m_isStartAnimTrue = _isStartAnimtrue;
}
void cardlist::setcardstatus(bool _iscardflipped)
{
	m_iscardflipped = _iscardflipped;
}

void cardlist::setflipitback(bool _flipitback)
{
	m_flipitback = _flipitback;
}
void cardlist::setrotation(ngl::Vec3 _rotation)
{
	m_cardanimation.setRotation(_rotation);
}

void cardlist::setposition(ngl::Vec3 _position)
{
	m_cardanimation.setPosition(_position);
}

