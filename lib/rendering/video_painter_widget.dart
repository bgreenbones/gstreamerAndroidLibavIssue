// ignore_for_file: non_constant_identifier_names

import 'dart:ffi';
import 'dart:typed_data';
import 'dart:ui' as ui;

import 'package:flutter/material.dart';

import 'package:flutter/services.dart';
import 'package:native_binding/native_binding.dart';
import 'video_painter.dart';

class VideoPainterWidget extends StatefulWidget {
  const VideoPainterWidget(
      {super.key,
      required this.m_iWidth,
      required this.m_iHeight,
      required this.m_iBytesPerFrame});
  final int m_iWidth;
  final int m_iHeight;
  final int m_iBytesPerFrame;
  @override
  State<VideoPainterWidget> createState() => _VideoWidgetState();
}

class _VideoWidgetState extends State<VideoPainterWidget> {
  late int m_iHeight = widget.m_iHeight;
  late int m_iWidth = widget.m_iWidth;
  late int m_iBytesPerFrame = widget.m_iBytesPerFrame;

  NewFrameListenable m_oFrameListenable = NewFrameListenable();
  late VideoPainter m_oVideoPainter;
  bool m_bFirstImageReceived = false;
  void onNewImage(ui.Image oNewImage) {
    if (!m_bFirstImageReceived) {
      m_oVideoPainter = VideoPainter(oNewImage, m_oFrameListenable);
      setState(() {
        m_bFirstImageReceived = true;
      });
    }
    m_oVideoPainter.m_oImage = oNewImage;
  }

  void RenderFunction(Pointer<Uint8> vFramePointer) {
    Uint8List nFrameData = Uint8List(m_iBytesPerFrame);
    for (int i = 0; i < m_iBytesPerFrame; i++) {
      nFrameData[i] = vFramePointer[i];
    }
    ui.decodeImageFromPixels(
        nFrameData, m_iWidth, m_iHeight, ui.PixelFormat.bgra8888, onNewImage);
    m_oFrameListenable.OnNewFrame();
  }

  @override
  void initState() {
    super.initState();
    SetRenderFunction(RenderFunction);
  }

  @override
  Widget build(BuildContext context) {
    return FittedBox(
        child: SizedBox(
            width: m_iWidth.toDouble(),
            height: m_iHeight.toDouble(),
            child: m_bFirstImageReceived
                ? CustomPaint(painter: m_oVideoPainter)
                : const Text("No image yet")));
  }
}
