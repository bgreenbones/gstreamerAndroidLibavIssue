#include "native_binding.h"
#include <gst/gst.h>
#include <string>
#include <vector>

// TODO: include headers and define functions in a library
// #include "android_video.cpp"

#include <android/log.h>
#ifdef ANDROID
#endif

#ifdef IOS
    #include "gst_ios_init.h"
#endif

#include <pthread.h>

typedef struct _IncomingStream
{
    GstElement *m_oBin;

    GstElement *m_oSource;
    GstElement *m_oQueue;
    GstElement *m_oDepay;
    GstElement *m_oParse;
    GstElement *m_oDecode;
    GstElement *m_oConvert;
    GstElement *m_oScale;
    GstElement *m_oCaps;
    GstElement *m_oBox;

} IncomingStream;


typedef struct _RoomData
{
    GstElement *pipeline;

    GstElement *audiotestsrc;
    GstElement *audioconvert;
    GstElement *autoaudiosink;
    
    GstElement *videotestsrc;
    GstElement *videoscale;
    GstElement *videocaps;
    GstElement *videoconvert;
    GstElement *autovideosink;
    GstElement *appsink;

    IncomingStream **m_vStreams;

    GMainLoop *mainloop;

    int iWidth;
    int iHeight;
} RoomData;

RoomData *data;
IncomingStream *oStream1;
IncomingStream *oStream2;

FFI_PLUGIN_EXPORT void Init(void)
{
#ifdef IOS
    gst_ios_init();
#endif
    // init
    gst_init(NULL, NULL);
    data = (RoomData *)malloc(sizeof(RoomData));
    
}

FFI_PLUGIN_EXPORT void SetVideoDimensions(int iWidth, int iHeight) {
    data->iWidth = iWidth;
    data->iHeight = iHeight;
}

// int pthread_create(pthread_t *restrict thread,
//                           const pthread_attr_t *restrict attr,
//                           void *(*start_routine)(void *),
//                           void *restrict arg);


// pthread_t t;
void (*RenderFrameToTexture)(uint8_t*);
FFI_PLUGIN_EXPORT void SetRenderFunction(void (*RenderFunction)(uint8_t*)) {
    RenderFrameToTexture=RenderFunction;
}




static GstFlowReturn OnNewVideoBuffer(GstElement* appsink, gpointer data)
{
	RoomData* pipeData = (RoomData*)data;

	gint width, height;
	GstMapInfo map;

	// Retrieve the preroll sample asynchronously
	GstSample* sample;
    g_signal_emit_by_name (appsink, "pull-sample", &sample);

	if (sample)
	{
		GstBuffer *buffer = NULL;
		GstCaps *caps = NULL;
		GstStructure *s = NULL;

		caps = gst_sample_get_caps(sample);
		if (!caps)
		{
			g_print("could not get snapshot format\n");
			exit(-1);
		}

		s = gst_caps_get_structure(caps, 0);

		// we need to get the final caps on the buffer to get the size
		gboolean res = gst_structure_get_int(s, "width", &width);
		res |= gst_structure_get_int(s, "height", &height);
		if (!res)
		{
			g_print("could not get snapshot dimension\n");
			exit(-1);
		}

		// get the pixbuf
		buffer = gst_sample_get_buffer(sample);

		if (gst_buffer_map(buffer, &map, GST_MAP_READ))
		{
            if (RenderFrameToTexture != NULL) {
                RenderFrameToTexture(map.data); // render to texture here
            }
            
			gst_buffer_unmap(buffer, &map);
		}

		gst_sample_unref(sample);
		return GST_FLOW_OK;
	}
	else
	{
		g_print("Failed to get a sample from appsrc\n");
		return GST_FLOW_ERROR;
	}
	return GST_FLOW_OK;
}


FFI_PLUGIN_EXPORT void SetupPipeline(void)
{
    // setup pipeline
    data->audiotestsrc = gst_element_factory_make("audiotestsrc", NULL);
    data->audioconvert = gst_element_factory_make("audioconvert", NULL);
    data->autoaudiosink = gst_element_factory_make("autoaudiosink", NULL);
    data->videotestsrc = gst_element_factory_make("videotestsrc", NULL);
    data->appsink = gst_element_factory_make("appsink", NULL);

    GError *error = NULL;

    std::string sHeight = std::to_string(data->iHeight);
    std::string sWidth = std::to_string(data->iWidth);
    std::string sHalfWidth = std::to_string(data->iWidth/2);
    std::string sHalfHeight = std::to_string(data->iHeight/2);

    bool bPortraitOrientation = data->iHeight > data->iWidth;
    std::string sCompositorLaunchString = bPortraitOrientation 
        ? "compositor name=mix sink_0::ypos=0 sink_0::width=" + sWidth + " sink_0::height=" + sHalfHeight + " sink_1::ypos=" + sHalfHeight + " sink_1::width=" + sWidth + " sink_1::height=" + sHalfHeight + " "
        : "compositor name=mix sink_0::xpos=0 sink_0::width=" + sHalfWidth + " sink_0::height=" + sHeight + " sink_1::xpos=" + sHalfWidth + " sink_1::width=" + sHalfWidth + " sink_1::height=" + sHeight + " ";
    std::string sLaunchString = // "videotestsrc pattern=18 "
        sCompositorLaunchString +
        // "rtspsrc location=rtsp://rtspstream:590a84668a36862e248a5d79814a8795@zephyr.rtsp.stream/movie latency=100 ! queue ! rtph264depay ! h264parse ! avdec_h264 "
        "! videoconvert ! videoscale ! capsfilter caps=video/x-raw,format=BGRA,width=" + sWidth + ",height=" + sHeight + ",framerate=30/1 name=finalCaps ! appsink name=appsink "
        // "rtspsrc location=rtsp://rtspstream:590a84668a36862e248a5d79814a8795@zephyr.rtsp.stream/movie latency=100 ! queue ! rtph264depay ! h264parse ! avdec_h264 "
        "videotestsrc pattern=18 "
        "! mix. "
        // "rtspsrc location=rtsp://rtspstream:77264b5b3d4be9e0617c3e6670417c2a@zephyr.rtsp.stream/pattern latency=100 ! queue ! rtph264depay ! h264parse ! avdec_h264 "
        "videotestsrc "
        "! mix. ";
    data->pipeline = gst_parse_launch(
        sLaunchString.c_str()
        , &error);

    if (error) {
        gst_printerr ("Failed to parse launch: %s\n", error->message);
        g_error_free (error);
    }

    

    GstElement *box1 = gst_bin_get_by_name(GST_BIN(data->pipeline), "box1");
    GstElement *box2 = gst_bin_get_by_name(GST_BIN(data->pipeline), "box2");
    
    data->appsink = gst_bin_get_by_name(GST_BIN(data->pipeline), "appsink");
    
    data->mainloop = g_main_loop_new(NULL, FALSE);


    g_object_set(G_OBJECT(data->appsink), "emit-signals", TRUE, NULL);
    g_signal_connect (G_OBJECT(data->appsink), "new-sample", G_CALLBACK(OnNewVideoBuffer), &data);

    if (!data->audiotestsrc || !data->audioconvert || !data->autoaudiosink || !data->pipeline)
    {
        g_printerr("Elements could not be created.\n");
        gst_object_unref(data->pipeline);
        return;
    }

    gst_bin_add_many(GST_BIN(data->pipeline), data->audiotestsrc, data->audioconvert, data->autoaudiosink, NULL);
    if (!gst_element_link_many(data->audiotestsrc, data->audioconvert, data->autoaudiosink, NULL))
    {
        g_printerr("Elements could not be created.\n");
        gst_object_unref(data->pipeline);
        return;
    }
}

FFI_PLUGIN_EXPORT void StartPipeline(void)
{
    // start pipeline
    GstStateChangeReturn ret;
    ret = gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Elements could not be created.\n");
        gst_object_unref(data->pipeline);
        return;
    }

}


FFI_PLUGIN_EXPORT void FreeResource(void)
{
    // free resources
    g_main_loop_unref(data->mainloop);
    gst_element_set_state(data->pipeline, GST_STATE_NULL);
    gst_object_unref(data->pipeline);
}


SelectOptions SelectOptionsFromVector(std::vector<std::string> vOptions) {
    SelectOptions uSelectOptions { .m_iSize=vOptions.size() };
    uSelectOptions.m_vOptions = (char**) malloc(vOptions.size() * sizeof(char*));
    for (int i = 0; i < vOptions.size(); i++) {
        uSelectOptions.m_vOptions[i] = (char*) malloc((vOptions[i].size() + 1) * sizeof(char));
        strcpy(uSelectOptions.m_vOptions[i], vOptions[i].c_str());
    }
    return uSelectOptions;
}


FFI_PLUGIN_EXPORT SelectOptions GetDeviceOptions() {
    return SelectOptionsFromVector({});
}


// A very short-lived native function.
//
// For very short-lived functions, it is fine to call them on the main isolate.
// They will block the Dart execution while running the native function, so
// only do this for native functions which are guaranteed to be short-lived.
FFI_PLUGIN_EXPORT intptr_t sum(intptr_t a, intptr_t b) { return a + b; }

// A longer-lived native function, which occupies the thread calling it.
//
// Do not call these kind of native functions in the main isolate. They will
// block Dart execution. This will cause dropped frames in Flutter applications.
// Instead, call these native functions on a separate isolate.
FFI_PLUGIN_EXPORT intptr_t sum_long_running(intptr_t a, intptr_t b)
{
    // Simulate work.
#if _WIN32
    Sleep(5000);
#else
    usleep(5000 * 1000);
#endif
    return a + b;
}
