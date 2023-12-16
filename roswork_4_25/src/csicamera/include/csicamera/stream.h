#ifndef _STREAM_H_
#define _STREAM_H_

#include <iostream>

#include <gst/gst.h>
#include <glib.h>

#include <gst/rtsp-server/rtsp-server.h>

#include "opencv2/opencv.hpp"

#include "capture.h"


// #define STREAM_SIZE   (STREAM_WIDTH * STREAM_HEIGHT * 3)

struct stream_context {
	GstClockTime timestamp;
	GstBuffer *buffer;
	cv::Mat *mat;
	// cv::Mat *mat2;
};

struct stream_configure {
	cv::Mat *mat;
	// cv::Mat *mat2;
	// bool	dual;
};


class Stream
{
	public:
		Stream(const std::string setstrUser,const std::string setstrPasswd, const int setWidth, const int setHeight);
		void addEndpoint(std::string path, cv::Mat &mat);
//		void addDualEndpoint(std::string path, cv::Mat &mat, cv::Mat &mat2);
		void thread();
		// void SetUserPasswd(const std::string setstrUser,const std::string setstrPasswd);
		static void need_data(GstElement *appsrc, guint arg1, stream_context *c);
		// static void need_data_dual(GstElement *appsrc, guint arg1, stream_context *c);
		static void appsrc_configure(GstRTSPMediaFactory *factory, GstRTSPMedia *media, stream_configure *conf);
	private:
		std::string mystrUser;
		std::string mystrPasswd;
		const char  *user;
		const char  *passwd;

		static int STREAM_WIDTH;
		static int STREAM_HEIGHT;

		GMainLoop *loop;
		GstRTSPServer *server;
		GstRTSPMountPoints *mounts;
};

#endif
