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

#include <QVector2D>
#include <QVector3D>

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
    //initRedCubeGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();

}
//! [0]

void GeometryEngine::initRedCubeGeometry()
{

    VertexData vertices2[] = {
        {QVector3D(0.0f, 0.0f, 2.0f), QVector3D(1.0f, 1.0f,1.0f)},
        {QVector3D(1.0f, 0.0f, 2.0f), QVector3D(1.0f, 0.0f,0.0f)},
        {QVector3D(0.0f, 1.0f, 2.0f), QVector3D(1.0f, 0.0f,0.0f)},
        {QVector3D(1.0f, 1.0f, 2.0f), QVector3D(1.0f, 1.0f,1.0f)},
        {QVector3D(0.0f, 0.0f, 3.0f), QVector3D(1.0f, 0.0f,0.0f)},
        {QVector3D(1.0f, 0.0f, 3.0f), QVector3D(1.0f, 0.0f,0.0f)},
        {QVector3D(0.0f, 1.0f, 3.0f), QVector3D(1.0f, 0.0f,0.0f)},
        {QVector3D(1.0f, 1.0f, 3.0f), QVector3D(1.0f, 1.0f,1.0f)},
    };
    const int nbrVertices2 = 8;

    GLushort indices2[] = {
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
    const int nbrIndices2 = 36;
////! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices2, nbrVertices2 * sizeof(VertexData));

    // Transfer index data to VBO 1
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
    initGeometry();
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
    // czq  atribute GL_LINES can be changed to GL_TRIANGLES

    initRedCubeGeometry();
    arrayBuf.bind();
    indexBuf.bind();

    program->setUniformValue("mvp", projection * matrixTorse);
    glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);
    // czq  atribute GL_LINES can be changed to GL_TRIANGLES
}

