#ifndef _CAPTURE_H_
#define _CAPTURE_H_

#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>




// #define STREAM_WIDTH  640
// #define STREAM_HEIGHT 480

// using namespace cv;
// typedef std::chrono::high_resolution_clock Clock;

// static std::mutex mutex_;
// static std::atomic_bool isOpen;

class Capture
{
	public:

		std::atomic_bool m_IsOpen;
		// std::mutex* m_pMutex;

		std::atomic_bool ifShutdownCam;

		Capture(const int id, const int Width, const int Height ,cv::Mat &mat,std::mutex &main_pMutex,
			    cv::Ptr<cv::BackgroundSubtractor> &fgbg, const int numSubBackground,cv::Mat &fgMask);
		~Capture();

		void TrainbgSub();

		void read_cam_subBack();

		void read_cam();


	private:
		int m_id;

		int m_Width;
		int m_Height;
		
		cv::Mat &m_mat;

		std::mutex &m_pMutex;

		cv::VideoCapture *m_cap;
		
		cv::Ptr<cv::BackgroundSubtractor> &m_fgbg;
		int m_numSubBackground;
		cv::Mat &m_fgMask;

		bool m_FinishTrain;

};

#endif
