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
		cardlist::cardlist(bool _isAnimtrue, bool _iscardflipped);
		ngl::Transformation getanimtransform();
		bool getanimationstatus();
		bool getcardstatus();
		bool getflipitback();
		int getTextureStatus();
		void setTextureStatus(int _cardTextureValue);
		void setanimtansform(ngl::Transformation _cardanimation);
		void setanimationstatus(bool _isAnimtrue);
		void setstartanimationstatus(bool _isStartAnimtrue);
		void setcardstatus(bool _iscardflipped);
		void setflipitback(bool _flipitback);
		void setrotation(ngl::Vec3 _rotation);
		void setposition(ngl::Vec3 _position);
		ngl::Transformation card_ls(ngl::Vec3 _getcardPL);
		
	private :
		ngl::Transformation m_cardanimation;
		ngl::Transformation m_getcardPL;
		bool m_isAnimTrue = false;
		bool m_isStartAnimTrue = false;
		bool m_iscardflipped = false;
		bool m_flipitback = false;
		int m_cardTextureValue;

};



#endif // 
