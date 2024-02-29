// ignore_for_file: non_constant_identifier_names

import 'dart:io';
import 'dart:ui';

import 'package:flutter/material.dart';

import 'package:native_binding/native_binding.dart';
import 'package:native_binding/rendering/video_painter_widget.dart';
import 'package:native_binding/rendering/video_texture_widget.dart';

class VideoWidget extends StatefulWidget {
  const VideoWidget({super.key});

  @override
  State<VideoWidget> createState() => _VideoWidgetState();
}

class _VideoWidgetState extends State<VideoWidget> {
  late FlutterView view =
      WidgetsBinding.instance.platformDispatcher.views.first;

  // Dimensions in physical pixels (px)
  late Size size = view.physicalSize;
  late double width = size.width;
  late double height = size.height;

  // Dimensions in logical pixels (dp)
  // Size size = view.physicalSize / view.devicePixelRatio;
  // double width = size.width;
  // double height = size.height;

// With BuildContext:
// // Dimensions in logical pixels (dp)
// Size size = MediaQuery.of(context).size;
// double width = size.width;
// double height = size.height;

// // Height (without SafeArea)
// final padding = MediaQuery.of(context).viewPadding;
// double height1 = height - padding.top - padding.bottom;

// // Height (without status bar)
// double height2 = height - padding.top;

// // Height (without status and toolbar)
// double height3 = height - padding.top - kToolbarHeight;

// // To get above dimensions in physical pixels (px),
// // multiply them by `MediaQuery.of(context).devicePixelRatio`

  late int m_iHeight = size.height.toInt();
  late int m_iTextureWidth = size.width.toInt();
  // int m_iHeight = 768;
  // int m_iTextureWidth = 1377;
  var key = 0;
  final m_iStrideAlign = Platform.isMacOS ? 64 : 1;
  int get m_iImageWidth {
    var w4 = m_iTextureWidth * 4;
    var w4ps = w4 + m_iStrideAlign - 1;
    var bwns = ~(m_iStrideAlign - 1);
    var rb = w4ps & bwns;
    return (m_iTextureWidth + (rb - w4) / 4).toInt();
  }

  late int rowBytes =
      (m_iTextureWidth * 4 + m_iStrideAlign - 1) & (~(m_iStrideAlign - 1));
  late int m_iBytesPerFrame = rowBytes * m_iHeight;

  @override
  void initState() {
    super.initState();
    SetVideoDimensions(m_iImageWidth, m_iHeight);
    try {
      SetupPipeline();
    } catch (e) {
      // No specified type, handles all
      print('Something really unknown: $e');
    }
    StartPipeline();
  }

  @override
  Widget build(BuildContext context) {
    return (Platform.isMacOS || Platform.isWindows || Platform.isLinux)
        ? VideoTextureWidget(
            iImageWidth: m_iImageWidth,
            iTextureWidth: m_iTextureWidth,
            iHeight: m_iHeight,
            iBytesPerFrame: m_iBytesPerFrame)
        : VideoPainterWidget(
            m_iWidth: m_iImageWidth,
            m_iHeight: m_iHeight,
            m_iBytesPerFrame: m_iBytesPerFrame);
  }
}
