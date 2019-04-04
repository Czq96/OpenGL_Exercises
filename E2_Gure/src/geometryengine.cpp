/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "geometryengine.h"

#include "math.h"

#include <QVector2D>
#include <QVector3D>

#define PI 3.14159265359

struct VertexData
{
    QVector3D position;
    QVector3D color;
};

VertexData vertices[] = {
    {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f,0.0f)},
    {QVector3D(1.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f,0.0f)},
    {QVector3D(0.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f,1.0f)},
    {QVector3D(1.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f,1.0f)},
    {QVector3D(0.0f, 0.0f, 1.0f), QVector3D(1.0f, 1.0f,0.0f)},
    {QVector3D(1.0f, 0.0f, 1.0f), QVector3D(1.0f, 1.0f,0.0f)},
    {QVector3D(0.0f, 1.0f, 1.0f), QVector3D(0.0f, 1.0f,0.0f)},
    {QVector3D(1.0f, 1.0f, 1.0f), QVector3D(0.0f, 1.0f,0.0f)},
};

const int nbrVertices = 8;

//点从0开始
GLushort indices[] = {
    0,1,2,
    1,2,3,
    4,5,6,
    5,6,7,
    0,4,5,
    0,4,6,
    2,6,0,
    2,6,3,
    3,7,6,
    3,7,1,
    1,5,7,
    1,5,0
};

const int nbrIndices = 36;

//! [0]
GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    initGeometry();
    //initCylindrGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();

}
//! [0]

void GeometryEngine::initCylindrGeometry()
{
//    int circlePointNumber = 360;
//    //使用for循环构造出一个柱子的所有点  半径为1 高为1
//    VertexData vertices2[2*circlePointNumber];
//    for(int i=0;i<circlePointNumber;i++)
//    {
//        float x=float(cos((i/(circlePointNumber/2))*PI));
//        float y=float(sin((i/(circlePointNumber/2))*PI));
//        vertices2[i] = VertexData({QVector3D(x, y, 0.0f), QVector3D(1.0f, 0.0f,0.0f)});
//        vertices2[i+circlePointNumber] = VertexData({QVector3D(x, y, 1.0f), QVector3D(0.0f, 1.0f,0.0f)});
//    }
//    int nbrVertices2 = circlePointNumber*2;

//    //使用for循环构造面 这里使用指针和动态数组会更好
//    //顶面360个点  358个底面   358*2 + 360*2 侧面  (circlePointNumber-2+circlePointNumber)*2
//    int faceNumber = (circlePointNumber-2+circlePointNumber)*2;
//    //int faceNumber = (circlePointNumber-2)*2;

//    GLushort indices2[3*faceNumber];
//    int topFaceIndex = 0;
//    int bottomFaceIndex = 3*(circlePointNumber-2);
//    int sideFaceIndex = 3*2*(circlePointNumber-2);
//    for(int i=0;i<circlePointNumber;i++)
//    {
//        //底面 0~ 358*3-1
//        indices2[topFaceIndex]=0;
//        indices2[topFaceIndex+1]=i+1;
//        indices2[topFaceIndex+2]=i+2;
//        //顶面 358*3-1 ~ 358*3-1 + 358*3 => (circlePointNumber-2)*2*3
//        indices2[bottomFaceIndex]=circlePointNumber;
//        indices2[bottomFaceIndex+1]=circlePointNumber+i+1;
//        indices2[bottomFaceIndex+2]=circlePointNumber+i+2;
//        //侧面
//        indices2[sideFaceIndex]=i;
//        indices2[sideFaceIndex+1]=i+circlePointNumber;
//        indices2[sideFaceIndex+2]=i+circlePointNumber+1>=2*circlePointNumber?circlePointNumber:i+circlePointNumber+1;
//        indices2[sideFaceIndex+3]=i;
//        indices2[sideFaceIndex+4]=i+circlePointNumber;
//        indices2[sideFaceIndex+5]=i-1<0?(circlePointNumber/2)-1:i-1;

//        topFaceIndex+=3;
//        bottomFaceIndex+=3;
//        sideFaceIndex+=6;
//    }
//    int nbrIndices2 = 3*faceNumber;

    arrayBuf.bind();
    arrayBuf.allocate(vertices2, nbrVertices2 * sizeof(VertexData));

    indexBuf.bind();
    indexBuf.allocate(indices2, nbrIndices2 * sizeof(GLushort));
//! [1]
}

void GeometryEngine::initGeometry()
{
////! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, nbrVertices * sizeof(VertexData));
    // arrayBuf.allocate(vertices2, nbrVertices2 * sizeof(VertexData)); 写两个会覆盖掉前一个


    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, nbrIndices * sizeof(GLushort));
    //indexBuf.allocate(indices2, nbrIndices2 * sizeof(GLushort));
//! [1]
}


//! [2]
//! drawCylindreGeometry
void GeometryEngine::drawGeometry(QOpenGLShaderProgram *program, QMatrix4x4 projection,QQuaternion rotation)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // x+方向是身高 高方向  y是-左 +右方向  z是+前 -后方向
    QMatrix4x4 matrixTorse;//变换的顺序从下至上
    matrixTorse.translate(0.0,0.0,-5.0);
    matrixTorse.rotate(rotation);  //鼠标事件的旋转
    matrixTorse.translate(-0.5/2,-(0.3/2.0),-0.2/2); // 这里移动的是物体坐标系
    matrixTorse.scale(0.5f,0.3f,0.2f); //改变大小 xyz三方向变动   scale 写在traslate前面有变化  关于 matrix.  的顺序

    QMatrix4x4 matrixCylin;//变换的顺序从下至上
    matrixCylin.translate(0.0,0.0,-5.0);
    matrixCylin.rotate(rotation);  //鼠标事件的旋转
    matrixCylin.translate(-0.5/2,-(0.3/2.0),-0.2/2); // 这里移动的是物体坐标系
    matrixCylin.scale(0.2f,0.2f,0.2f); //改变大小 xyz三方向变动   scale 写在traslate前面有变化  关于 matrix.  的顺序

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int colorLocation = program->attributeLocation("color");
    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    program->setUniformValue("mvp", projection * matrixTorse);
    glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);
    //GL_TRIANGLE_FAN  1,2,3,4  -->  1 2 3, 1 3 4


    initCylindrGeometry();  //生成圆柱
    glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);
    // czq  atribute GL_LINES can be changed to GL_TRIANGLES

}
