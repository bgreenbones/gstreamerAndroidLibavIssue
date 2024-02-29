// ignore_for_file: non_constant_identifier_names

import 'dart:ui' as ui;

import 'package:flutter/material.dart';

class NewFrameListenable with ChangeNotifier {
  void OnNewFrame() {
    notifyListeners();
  }
}

class VideoPainter extends CustomPainter {
  VideoPainter(this.m_oImage, Listenable oRepaint) : super(repaint: oRepaint);
  ui.Image m_oImage;

  @override
  void paint(Canvas canvas, ui.Size size) {
    canvas.drawImage(m_oImage, Offset.zero, Paint());
  }

  @override
  bool shouldRepaint(covariant VideoPainter oldDelegate) {
    return m_oImage != oldDelegate.m_oImage;
  }
}
