import 'dart:io';
import 'dart:ffi';

import 'package:flutter/material.dart';

import 'package:native_binding/native_binding.dart';
import 'package:texture_rgba_renderer/texture_rgba_renderer.dart';

class VideoTextureWidget extends StatefulWidget {
  const VideoTextureWidget(
      {super.key,
      required this.iImageWidth,
      required this.iTextureWidth,
      required this.iHeight,
      required this.iBytesPerFrame})
      : super();
  final int iImageWidth;
  final int iTextureWidth;
  final int iHeight;
  final int iBytesPerFrame;

  @override
  State<VideoTextureWidget> createState() => _VideoTextureWidgetState();
}

class _VideoTextureWidgetState extends State<VideoTextureWidget> {
  final _textureRgbaRendererPlugin = TextureRgbaRenderer();
  int textureId = -1;
  late int iHeight = widget.iHeight;
  late int iTextureWidth = widget.iTextureWidth;
  late int iImageWidth = widget.iImageWidth;
  late int iBytesPerFrame = widget.iBytesPerFrame;
  final int iStrideAlign = Platform.isMacOS ? 64 : 1;

  var key = 0;
  int texturePtr = 0;

  void RenderFunction(Pointer<Uint8> dataPtr) {
    Native.instance.onRgba(Pointer.fromAddress(texturePtr).cast<Void>(),
        dataPtr, iBytesPerFrame, iTextureWidth, iHeight, iStrideAlign);
  }

  @override
  void initState() {
    super.initState();
    _textureRgbaRendererPlugin.createTexture(key).then((textureId) {
      if (textureId != -1) {
        debugPrint("Texture register success, textureId=$textureId");
        _textureRgbaRendererPlugin.getTexturePtr(key).then((value) {
          debugPrint("texture ptr: ${value.toRadixString(16)}");
          setState(() {
            texturePtr = value;
          });
          SetRenderFunction(RenderFunction);
        });
        setState(() {
          this.textureId = textureId;
        });
      } else {
        return;
      }
    });
  }

  @override
  void dispose() {
    if (key != -1) {
      _textureRgbaRendererPlugin.closeTexture(key);
    }
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Texture(textureId: textureId);
    // return Column(
    //   crossAxisAlignment: CrossAxisAlignment.center,
    //   mainAxisAlignment: MainAxisAlignment.center,
    //   children: [
    //     Expanded(
    //       child: textureId == -1
    //           ? const Offstage()
    //           : Padding(
    //               padding: const EdgeInsets.all(8.0),
    //               child: Container(
    //                   alignment: Alignment.center,
    //                   decoration: const BoxDecoration(color: Colors.blue),
    //                   // decoration: const BoxDecoration(color: Colors.black),
    //                   width: width.toDouble(),
    //                   height: iHeight.toDouble(),
    //                   child: Texture(textureId: textureId)),
    //             ),
    //     ),
    //     Text(
    //         "texture id: $textureId, texture memory address: ${texturePtr.toRadixString(16)}"),
    //   ],
    // );
  }
}
