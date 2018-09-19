/*
	Project: TinyCog
	File: FaceKnow.cpp
	Author: Dagim Sisay <dagiopia@gmail.com>
	License: AGPL
	Date: September, 2018
*/


#ifndef _FACEKNOW_HPP_
#define _FACEKNOW_HPP_

#include <VisionCommon.hpp>

class FaceKnow
{
	public:
		enum emo_t {NEUTRAL=0, ANGRY, HAPPY, SAD, SURPRISED};
		struct face_state {
			bool smiling;
			bool talking;
			short eyes_open;
			emo_t emotion;
			bool known;
			std::string name;
		};

		FaceKnow();
		~FaceKnow();


};

#endif // _FACEKNOW_HPP_
