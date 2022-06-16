#ifndef CARDLIST_H
#define CARDLIST_H
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include "WindowParams.h"
#include <memory>
#include <vector>
#include <string>

class cardlist
{
	public :
		//checks for the animation and cardflip status for card x on initialization. 
		cardlist::cardlist(bool _isAnimtrue, bool _iscardflipped);
		//gets the current transformation for card x.
		ngl::Transformation getanimtransform();
		//checksface up animation for card x.
		bool getanimationstatus();
		//checksfor face down animation for card x. 
		bool getcardstatus();
		//checksface down animation for card x.
		bool getflipitback();
		//returns texture number of card x.
		int getTextureStatus();
		//set texture number.
		void setTextureStatus(int _cardTextureValue);
		//set new transformation for card x.
		void setanimtansform(ngl::Transformation _cardanimation);
		//allows function to set animation status for face up card x.
		void setanimationstatus(bool _isAnimtrue);
		//set if the card is face up or not.
		void setcardstatus(bool _iscardflipped);
		//set status of flip to face down initializer.
		void setflipitback(bool _flipitback);
		//set rotation for card transform.
		void setrotation(ngl::Vec3 _rotation);
		//set translation for card transform.
		void setposition(ngl::Vec3 _position);
		
	private :
		ngl::Transformation m_cardanimation;
		bool m_isAnimTrue = false;
		bool m_iscardflipped = false;
		bool m_flipitback = false;
		int m_cardTextureValue;

};



#endif // 
