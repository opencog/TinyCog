/*
  File: dr-roboto.cpp
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
  Date: May, 2018
*/

#define YES_RPI 1
#define NO_RPI 0

#define _DR_ROBOTO_RPI_ NO_RPI


#include <stdio.h>
#include <vector>
#include <thread>
#include <unistd.h>

#define BUILD_Debug 1
#define BUILD_Release 0
#define DEBUG_MODE BUILD_Debug

#if DEBUG_MODE
	#define _DEBUG_
	#ifdef _DEBUG_
		#include "sense/vision/FileCapture.hpp"
	#endif
#endif


#if _DR_ROBOTO_RPI_
    #include "sense/vision/RaspiVision.hpp"
#else //_DR_ROBOTO_RPI
    #include "sense/vision/Vision.hpp"
#endif
#include "act/audio/FestivalTTS.hpp"
#include "comm/rpc/RPC_Client_Face_Emotion.hpp"

//debug consts 
#define _NEED_GUI_
#define _NEED_TIME_INFO_


// opencog headers
#define HAVE_GUILE

#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atoms/truthvalue/TruthValue.h>
#include <opencog/atoms/truthvalue/AttentionValue.h>

#include <opencog/atoms/value/FloatValue.h>
#include <opencog/atoms/value/LinkValue.h>
#include <opencog/atoms/value/StringValue.h>

#include <opencog/guile/SchemePrimitive.h>


#define SCALE 0.1


using namespace std;
using namespace opencog;


class DrRoboto
{
	private:
		AtomSpace *_as;
		bool ok;
		thread *run;
		static void sensor_loop(DrRoboto *);
		
		ImageSource *cap;

		ITColor2Gray c2g;
		ITEqualizeHist eh;
		ITDetectFace fcd;
		ITDetectSmile smd;
		FacialLandMark flm;
		ITDetectHand dh;
		FingersCount fc;
		DSaliency sal_d;
		FestivalTTS tts;
		RPC_Client_Face_Emotion ferpc;

	public:
		/* 
			XXX don't know if this is the right way to do it. 
			The AS is shared b/n this program and the guile instance once this module 
			is loaded and init_as is called from guile with the AS address as arg. 
					(init_as (cog-atomspace))   ; this should be called first
			Also the AS is just copied. 
		*/
		DrRoboto();
		~DrRoboto();
		void init_as(AtomSpace *as) { _as = as; ok = true;}
		#ifdef _DEBUG_ //XXX REMOVE WHEN DONE
			std::string _dbg_fname;
			bool _dbg_from_file;
			void cap_from_file(std::string);
		#endif
		
		//sensor functions
		void start_sense();
		void stop_sense();

		//act functions
		void say(std::string);
		//TODO add body movement functions.
};


extern "C" {
  void init_dr_roboto();
};


DrRoboto::DrRoboto() : c2g("c2g") , eh("eh") , fcd("fcd") , smd("smd") , 
                       dh("dh") , fc("fc") , sal_d(SAL_FINE_GRAINED)
{
	ok = false;
	#ifdef _DEBUG
	#endif

	#if _DR_ROBOTO_RPI_
		cap = RaspiCamCapture::init("cap", 320, 240, 20);
	#else
		cap = CamCapture::init("cap", 320, 240, 0, 20);
	#endif //_DR_ROBOTO_RPI_

} 

DrRoboto::~DrRoboto() {}


#ifdef _DEBUG_
void DrRoboto::cap_from_file(std::string file_name) 
{ 
	_dbg_from_file = true; 
	_dbg_fname = file_name; 
	cap = FileCapture::init("cap", _dbg_fname, 30); 
}
#endif 

void DrRoboto::sensor_loop(DrRoboto *dr)
{
	cv::Mat frame, img, gr, he;
	std::vector<cv::Rect> faces, smile, hands, emo_boxes;
	facial_lms f_lms;
	cv::Point2d cent;
	std::string str;
	std::vector<std::string> emo_preds;
	size_t idx = 0, didx = 0;
	//XXX clean this up! lash new! maybe have all required nodes created from
	//   within the scheme code
	Handle h;
	Handle hl;
	Handle fs = dr->_as->add_node(CONCEPT_NODE, "face");
	Handle sm = dr->_as->add_node(PREDICATE_NODE, "smile");
	Handle lk = dr->_as->add_node(PREDICATE_NODE, "look");
	Handle em = dr->_as->add_node(PREDICATE_NODE, "emotion");
	Handle nof = dr->_as->add_node(PREDICATE_NODE, "number_of");
	Handle ey = dr->_as->add_node(CONCEPT_NODE, "eyes");
	Handle hh = dr->_as->add_node(CONCEPT_NODE, "hand");
	Handle fi = dr->_as->add_node(CONCEPT_NODE, "fingers");
	Handle pos_h = dr->_as->add_node(CONCEPT_NODE, "position");
	Handle sen_h = dr->_as->add_node(CONCEPT_NODE, "sense");
	Handle rate_h = dr->_as->add_node(CONCEPT_NODE, "rate");
	//sen_h->setValue(rate_h, createFloatValue(1000)); //u-secs to delay sensor

	ValuePtr pap;
	
	while(dr->ok) {
		frame = dr->cap->getCurrentFrame();
		
		#ifdef _DEBUG_ //XXX REMOVE WHEN DONE_
			Mat _dbg_img;
			_dbg_img = frame.clone();
		#endif

		gr = dr->c2g.Transform(frame);
		he = dr->eh.Transform(gr);

		/*
			Face Detected
			  Value
			    ConceptNode "position"
			    ConceptNode "face_x"
			    FloatValue X Y
		*/
		faces = dr->fcd.Transform(he);
		for(idx = 0; idx < faces.size(); idx++) {
			str = "face_" + std::to_string(idx+1);
			h = dr->_as->add_node(CONCEPT_NODE, str.c_str());

			pap = createFloatValue(std::vector<double>({(double) faces[idx].x + faces[idx].width/2,
			                                            (double) faces[idx].y + faces[idx].height/2}));
			h->setValue(pos_h, pap);

			img = he(faces[idx]);
			smile = dr->smd.Transform(img); //only check on the smaller face region 
			if(!smile.empty())
				pap = createFloatValue(1.0);
			else 
				pap = createFloatValue(0.0);
			h->setValue(sm, pap);
			/*
				^ this is 
				    Valuation
				      PredicateNode "smile"
				      ConceptNode "face_x"
				      FloatValue x.x
			*/

			#ifdef _DEBUG_ //XXX REMOVE WHEN DONE
				cv::rectangle(_dbg_img, faces[idx], cv::Scalar(0, 255, 0), 2, 1);
				cv::putText(_dbg_img, (smile.empty() ? "not smiling" : "smiling"), 
				            cv::Point(faces[idx].x + faces[idx].x*0.05, 
								          faces[idx].y + faces[idx].y*0.05),
								FONT_HERSHEY_COMPLEX_SMALL, 0.4, cv::Scalar(0, 0, 255),
								0.8, CV_AA);
			#endif

			// facial lms
			dr->flm.get_lm_points(frame, faces[idx], f_lms);
			/*
				TODO emo detector here... should just be a function that returns
				the detected emo along with confidence (if it's possible)

				once the emo is detected, make a link to the current face 
				with the conceptnode for the emo type and set stv based on 
				confidence (if possible) 
				
				current implementation
					 Value
				        PredicateNode "emotion"
				        ConceptNode "face_x"
				        StringValue "emotion_x"
			*/
			if(dr->ferpc.detect_emotion(img, emo_preds, emo_boxes)) {
				if(emo_preds.size() == 1) {
					pap = createStringValue(emo_preds[0]);
					h->setValue(em, pap);
					#ifdef _DEBUG_ //XXX REMOVE WHEN DONE
						cv::putText(_dbg_img, emo_preds[0], 
						            cv::Point(faces[idx].x + faces[idx].x*0.15, 
						                      faces[idx].y + faces[idx].y*0.05),
						            FONT_HERSHEY_COMPLEX_SMALL, 0.4, cv::Scalar(0, 255, 0),
						            0.8, CV_AA);
					#endif
				}
			}
			
			// TODO do eye state here
		} // end of for faces

		/*
			set number of faces
				Value
					ConceptNode "face"
					PredicateNode "number_of"
					FloatValue X

		*/
		fs->setValue(nof, createFloatValue((double)faces.size()));

		/*
			salient point
				Valuation
				    PredicateNode "look"
				    ConceptNode "eyes"
				    FloatValue X Y
		*/
		cent = dr->sal_d.sal_point(gr, gr);
		pap = createFloatValue(std::vector<double>({(double)cent.x, (double)cent.y}));
		ey->setValue(lk, pap);

		#ifdef _DEBUG_ //XXX REMOVE WHEN DONE
			cv::circle(_dbg_img, cent, 2.0, cv::Scalar(255, 0, 0), 3);
		#endif

		/*
			hand detection and finger counting
			
			** hand detection **
			Valuation
				PredicateNode "position"
				ConceptNode "hand"
				FloatValue X Y

			** finger count ** XXX not sure if this is the right way
			Valuation
			    ConceptNode "fingers"
			    ConceptNode "hand_x"
			    FloatValue X
		*/
		cv::threshold(he, he, 70, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
		hands = dr->dh.Transform(he);
		for(idx = 0; idx < hands.size(); idx++) {
			img = cv::Mat(he, cv::Rect(hands[idx].x - hands[idx].x*SCALE,
			                           hands[idx].y - hands[idx].y*SCALE,
			                           hands[idx].width + hands[idx].width*SCALE,
			                           hands[idx].height + hands[idx].height*SCALE));
			
			str = "hand_" + std::to_string(idx);
			h = dr->_as->add_node(CONCEPT_NODE, str.c_str());
			pap = createFloatValue(std::vector<double>({
			                        (double)hands[idx].x+(hands[idx].width/2), 
											(double)hands[idx].y+(hands[idx].height/2)}));
			h->setValue(pos_h, pap);
								
			pap = createFloatValue((float)dr->fc.num_fingers(img));
			h->setValue(fi, pap);

			#ifdef _DEBUG_ //XXX REMOVE WHEN DONE
				cv::rectangle(_dbg_img, hands[idx], cv::Scalar(255, 0, 0), 2, 1);
				for(didx = 0; didx < dr->fc.f_tips.size(); didx++)
					cv::circle(_dbg_img, cv::Point(dr->fc.f_tips[didx].x + hands[idx].x,
					                               dr->fc.f_tips[didx].y + hands[idx].y), 
					           2, cv::Scalar(0, 0, 255), 2);
			#endif
		}
		
		#ifdef _DEBUG_
			cv::imshow("debug", _dbg_img);
			cv::waitKey(10);
		#endif
		usleep(10000);
	} //while ok
}



void DrRoboto::start_sense()
{
	run = new thread(DrRoboto::sensor_loop, this);
}


void DrRoboto::stop_sense()
{
	ok = false;
	run->join();
	delete run; 
}


void DrRoboto::say(std::string data)
{
	tts.setSpeaker(FestivalTTS::speaker::DON);
	tts.speak(data);
}

void init_dr_roboto()
{
	DrRoboto *dr = new DrRoboto();
	define_scheme_primitive("c-init-as", &DrRoboto::init_as, dr);
	define_scheme_primitive("c-start-sensors", &DrRoboto::start_sense, dr);
	define_scheme_primitive("c-stop-sensors", &DrRoboto::stop_sense, dr);
	define_scheme_primitive("c-say", &DrRoboto::say, dr);

	#ifdef _DEBUG_
		define_scheme_primitive("c-dbg-cap-from-file", &DrRoboto::cap_from_file, dr);
	#endif
}
