#include "stream.h"


int Stream :: STREAM_WIDTH = 640;
int Stream :: STREAM_HEIGHT = 480;

Stream::Stream(const std::string setstrUser,const std::string setstrPasswd,const int setWidth, const int setHeight)
{
	gst_init(NULL, NULL); //初始化gst

	loop = g_main_loop_new (NULL, FALSE);

	/* create a server instance */
	server = gst_rtsp_server_new ();
	gst_rtsp_server_set_service(server, "6060");

	/* get the mount points for this server, every server has a default object
	 * that be used to map uri mount points to media factories */
	mounts = gst_rtsp_server_get_mount_points (server);

	STREAM_WIDTH = setWidth;
	STREAM_HEIGHT = setHeight;

	// std::cout << "streamer _____---------------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>" <<std::endl;
	// std::cout <<"STREAM_WIDTH: "<<STREAM_WIDTH<<std::endl;
	// std::cout <<"STREAM_HEIGHT: "<<STREAM_HEIGHT<<std::endl;


	mystrUser = setstrUser;
	mystrPasswd = setstrPasswd;

        user = mystrUser.data();
        passwd = mystrPasswd.data();

}


void Stream::addEndpoint(std::string path, cv::Mat &mat)
{
	stream_configure *conf;
	conf = g_new0 (stream_configure, 1);

	conf->mat = &mat;

	GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();
	gst_rtsp_media_factory_set_launch (factory,
			//"( appsrc name=mysrc ! videoconvert ! nvvidconv ! video/x-raw(memory:NVMM),format=I420 ! x264enc speed-preset=ultrafast tune=zerolatency ! rtph264pay config-interval=1 name=pay0 pt=96 )"
                       "( appsrc name=mysrc ! videoconvert ! nvvidconv ! video/x-raw(memory:NVMM),format=I420 ! omxh264enc preset-level=FastPreset ! video/x-h264, stream-format=(string)byte-stream ! rtph264pay config-interval=1 name=pay0 pt=96 )"
//"( appsrc name=mysrc ! videoconvert ! nvvidconv ! video/x-raw(memory:NVMM),format=I420 ! nvv4l2h264enc preset-level=UltraFastPreset ! video/x-h264, stream-format=(string)byte-stream ! rtph264pay config-interval=1 name=pay0 pt=96 )"

);


    g_signal_connect (factory, "media-configure", (GCallback) Stream::appsrc_configure, conf);

	gst_rtsp_mount_points_add_factory(mounts, path.c_str(), factory);

	std::cout << "[Stream] Creating new endpoint at: " << path << std::endl;

    //user - password
    gst_rtsp_media_factory_add_role (factory, user,
                                     GST_RTSP_PERM_MEDIA_FACTORY_ACCESS, G_TYPE_BOOLEAN, TRUE,
                                     GST_RTSP_PERM_MEDIA_FACTORY_CONSTRUCT, G_TYPE_BOOLEAN, TRUE, NULL);

    //user - password
}


void Stream::thread()
{
	/* don't need the ref to the mounts anymore */
	g_object_unref (mounts);

	//user - password
    GstRTSPAuth *auth = gst_rtsp_auth_new ();

    GstRTSPToken *token =
            gst_rtsp_token_new (GST_RTSP_TOKEN_MEDIA_FACTORY_ROLE, G_TYPE_STRING,
                                user, NULL);
    gchar *basic = gst_rtsp_auth_make_basic (user, passwd);
    gst_rtsp_auth_add_basic (auth, basic, token);
    g_free (basic);
    gst_rtsp_token_unref (token);

    gst_rtsp_server_set_auth (server, auth);
    g_object_unref (auth);

    //user - password

	/* attach the server to the default maincontext */
	gst_rtsp_server_attach (server, NULL);

//    if (gst_rtsp_server_attach (server, NULL) == 0)
//        goto failed;

    // g_timeout_add_seconds (2, (GSourceFunc) timeout, server);
    // g_timeout_add_seconds (10, (GSourceFunc) remove_sessions, server);


	/* start serving */
	g_main_loop_run (loop);

//    failed:
//    {
//        g_print ("failed to attach the server\n");
//        return -1;
//    }
}


void Stream::need_data(GstElement *appsrc, guint arg1, stream_context *c)
{
//    GstBuffer *buffer;
	GstMapInfo info = {0};
    guint size;
//    GstMapInfo map = {0};
	GstFlowReturn ret;
    size = STREAM_WIDTH * STREAM_HEIGHT * 3;


	gst_buffer_map(c->buffer, &info, GST_MAP_WRITE);
	memcpy(info.data, c->mat->data, size);
	gst_buffer_unmap(c->buffer, &info);

	// 30 FPS
	GST_BUFFER_PTS (c->buffer) = c->timestamp;
	GST_BUFFER_DURATION (c->buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 15);
	c->timestamp += GST_BUFFER_DURATION (c->buffer);

	g_signal_emit_by_name (appsrc, "push-buffer", c->buffer, &ret);
}


void Stream::appsrc_configure(GstRTSPMediaFactory *factory, GstRTSPMedia *media, stream_configure *conf)
{

	GstElement *element, *appsrc;
	stream_context *c;

	/* get the element used for providing the streams of the media */
	element = gst_rtsp_media_get_element (media);

	/* get our appsrc, we named it 'mysrc' with the name property */
	appsrc = gst_bin_get_by_name_recurse_up (GST_BIN (element), "mysrc");

	/* this instructs appsrc that we will be dealing with timed buffer */
	gst_util_set_object_arg (G_OBJECT (appsrc), "format", "time");


	g_object_set (G_OBJECT (appsrc), "caps",
	gst_caps_new_simple ("video/x-raw",
		"format", G_TYPE_STRING, "BGR",
		"width", G_TYPE_INT, STREAM_WIDTH,
		"height", G_TYPE_INT, STREAM_HEIGHT,
		"framerate", GST_TYPE_FRACTION, 0, 1, NULL), NULL);

	c = g_new0 (stream_context, 1);
	c->timestamp = 0;

	c->buffer = gst_buffer_new_allocate(NULL, (STREAM_WIDTH * STREAM_HEIGHT * 3), NULL);

	c->mat = conf->mat;
	// c->mat2 = conf->mat2;

	/* make sure ther data is freed when the media is gone */
	g_object_set_data_full (G_OBJECT (media), "my-extra-data", c,
			(GDestroyNotify) g_free);



	g_signal_connect (appsrc, "need-data", (GCallback) Stream::need_data, c);

	gst_object_unref (appsrc);
	gst_object_unref (element);
}
