#include <gst/gst.h>

/* Declaration of static plugins */
 
GST_PLUGIN_STATIC_DECLARE(coreelements);
GST_PLUGIN_STATIC_DECLARE(coretracers);
GST_PLUGIN_STATIC_DECLARE(adder);
GST_PLUGIN_STATIC_DECLARE(app);
GST_PLUGIN_STATIC_DECLARE(audioconvert);
GST_PLUGIN_STATIC_DECLARE(audiomixer);
GST_PLUGIN_STATIC_DECLARE(audiorate);
GST_PLUGIN_STATIC_DECLARE(audioresample);
GST_PLUGIN_STATIC_DECLARE(audiotestsrc);
GST_PLUGIN_STATIC_DECLARE(compositor);
GST_PLUGIN_STATIC_DECLARE(gio);
GST_PLUGIN_STATIC_DECLARE(rawparse);
GST_PLUGIN_STATIC_DECLARE(libav);
GST_PLUGIN_STATIC_DECLARE(autodetect);
GST_PLUGIN_STATIC_DECLARE(opensles);
GST_PLUGIN_STATIC_DECLARE(videobox);
GST_PLUGIN_STATIC_DECLARE(videocrop);
GST_PLUGIN_STATIC_DECLARE(autoconvert);
GST_PLUGIN_STATIC_DECLARE(rtp);
GST_PLUGIN_STATIC_DECLARE(rtpmanager);
GST_PLUGIN_STATIC_DECLARE(rtsp);

/* This is called by gst_init() */
void
gst_init_static_plugins (void)
{
  
GST_PLUGIN_STATIC_REGISTER(coreelements);
GST_PLUGIN_STATIC_REGISTER(coretracers);
GST_PLUGIN_STATIC_REGISTER(adder);
GST_PLUGIN_STATIC_REGISTER(app);
GST_PLUGIN_STATIC_REGISTER(audioconvert);
GST_PLUGIN_STATIC_REGISTER(audiomixer);
GST_PLUGIN_STATIC_REGISTER(audiorate);
GST_PLUGIN_STATIC_REGISTER(audioresample);
GST_PLUGIN_STATIC_REGISTER(audiotestsrc);
GST_PLUGIN_STATIC_REGISTER(compositor);
GST_PLUGIN_STATIC_REGISTER(gio);
GST_PLUGIN_STATIC_REGISTER(rawparse);
GST_PLUGIN_STATIC_REGISTER(libav);
GST_PLUGIN_STATIC_REGISTER(autodetect);
GST_PLUGIN_STATIC_REGISTER(opensles);
GST_PLUGIN_STATIC_REGISTER(videobox);
GST_PLUGIN_STATIC_REGISTER(videocrop);
GST_PLUGIN_STATIC_REGISTER(autoconvert);
GST_PLUGIN_STATIC_REGISTER(rtp);
GST_PLUGIN_STATIC_REGISTER(rtpmanager);
GST_PLUGIN_STATIC_REGISTER(rtsp);
}