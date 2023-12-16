#include "capture.h"


Capture::Capture(const int id, const int Width, const int Height ,cv::Mat &mat,std::mutex &main_pMutex,
				cv::Ptr<cv::BackgroundSubtractor> &fgbg, const int numSubBackground,cv::Mat &fgMask)

 				:m_id((int)id),m_Width((int)Width),m_Height((int)Height) , m_mat(mat), m_pMutex(main_pMutex),m_fgbg(fgbg),m_numSubBackground(numSubBackground),m_fgMask(fgMask)

{
	ifShutdownCam = false;
	m_cap = new cv::VideoCapture(m_id,cv::CAP_V4L2);

	if (!m_cap->isOpened())
	{
		m_IsOpen = false;
		std::cout << "Failed to open camera" << std::endl;}
	else
	{
		m_IsOpen = true;
		// m_pMutex = new std::mutex();
		std::cout << "succese to open camera" << std::endl;
		m_cap->set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));
		m_cap->set(cv::CAP_PROP_FRAME_WIDTH, m_Width);
		m_cap->set(cv::CAP_PROP_FRAME_HEIGHT, m_Height);

		m_cap->set(cv::CAP_PROP_FPS, 15);

		std::cout << "==========================================" << std::endl;

		std::cout << "m_Width：" <<  m_Width << "m_Height" << m_Height << std::endl;
		

	}
}

Capture::~Capture(){
    m_cap->release();
    // delete m_pMutex;
    std::cout << "capture is over" << std::endl;
    // cv::destroyWindow("Movie Player");
}

void Capture::read_cam()
{
	cv::Mat frame;

	int rows = m_mat.rows;
    int cols = m_mat.cols;

    int count = 0;

	while (1) {

		if(ifShutdownCam){
			std::cout << "Shut down camera" << std::endl;
			// m_cap->release();
			break;
		}

		m_cap->read(frame);

		if (!m_IsOpen)
			{
			std::cout << "Failed to open camera" << std::endl;
			break;
			}

		else{
			m_pMutex.lock();
			frame.copyTo(m_mat);
			m_pMutex.unlock();
			}

		if (count == 0){
			
			cv::Size s = m_mat.size();
			rows = s.height;
	        cols = s.width;
	        std::cout << "m_mat-height：" <<  rows << " m_mat-width" << cols << std::endl;
		}
		count += 1;
		//if (count == 1000){m_cap->release();break;}
	}
}

void Capture::read_cam_subBack()
{
	cv::Mat frame;
	int rows = m_mat.rows;
    int cols = m_mat.cols;
    cv::Mat subbackFrame;
    cv::Mat fgMaskCopy;

    std::cout << "do sub BackgroundSubtractor" << std::endl;
	while (1) {

		if(ifShutdownCam){
			std::cout << "Shut down camera" << std::endl;
			// m_cap->release();
			break;
		}

		m_cap->read(frame);

		if (!m_IsOpen)
			{
			std::cout << "Failed to open camera" << std::endl;
			break;
			}

		else{
			m_pMutex.lock();
			frame.copyTo(m_mat);
			m_pMutex.unlock();
			int sub_row = (int)m_Width/2;
			int sub_col = (int)m_Height/2;
			cv::resize(frame,subbackFrame,cv::Size(sub_row,sub_col));
			
			m_fgbg->apply(subbackFrame,fgMaskCopy);
			m_pMutex.lock();
			fgMaskCopy.copyTo(m_fgMask);
			m_pMutex.unlock();
			
			}
	}
}


void Capture::TrainbgSub()
{
	cv::Mat frame;
    int count = 0;
    cv::Mat subbackFrame;
    cv::Mat train_fgMask;
    std::cout << "Training BG Subtractor..." << std::endl;
	while (1) {
		if(ifShutdownCam){
			std::cout << "Shut down camera" << std::endl;
			break;
		}
		m_cap->read(frame);
		if (!m_IsOpen)
			{
			std::cout << "Failed to open camera" << std::endl;
			break;
			}
		else{
			// m_pMutex.lock();
			// frame.copyTo(m_mat);
			// m_pMutex.unlock();
			int sub_row = (int)m_Width/2;
			int sub_col = (int)m_Height/2;
			cv::resize(frame,subbackFrame,cv::Size(sub_row,sub_col));
			m_fgbg->apply(subbackFrame,train_fgMask);
			if (count >= m_numSubBackground){
				break;
				}

			}

		count += 1;
	}
}
