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

/*
VertexData vertices[] = {
    {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f,0.0f)},
    {QVector3D(1.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f,0.0f)},
    {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f,0.0f)},
    {QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 1.0f,0.0f)},
    {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f,1.0f)},
    {QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 0.0f,1.0f)},

};

const int nbrVertices = 6;

GLushort indices[] = {
    0,1,
    2,3,
    4,5
};

*/


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
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::initGeometry()
{
////! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, nbrVertices * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, nbrIndices * sizeof(GLushort));
//! [1]
}

//! [2]
void GeometryEngine::drawGeometry(QOpenGLShaderProgram *program, QMatrix4x4 projection,QQuaternion rotation)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();


    // Offset for position
    quintptr offset = 0;

    QMatrix4x4 matrixCentre;
    matrixCentre.translate(-0.5,-0.5,-5.5); //这个 -5.0 z 是必须的  是由于 MainWidget中的resize
    matrixCentre.rotate(rotation);  // 老师写的这个 rotation 的旋转中心是  物体自己相对坐标系中的原点

    // x+方向是身高 高方向  y是-左 +右方向  z是+前 -后方向
    QMatrix4x4 matrixTorse;//变换的顺序从下至上
    matrixTorse.translate(0.0,0.0,-5.0);
    matrixTorse.rotate(rotation);  //鼠标事件的旋转
    matrixTorse.translate(-0.5/2,-(0.3/2.0),-0.2/2); // 这里移动的是物体坐标系
    matrixTorse.scale(0.5f,0.3f,0.2f); //改变大小 xyz三方向变动   scale 写在traslate前面有变化  关于 matrix.  的顺序

    QMatrix4x4 matrixTete;
    matrixTete.translate(0.0,0.0,-5.0);
    matrixTete.rotate(rotation);
    matrixTete.translate(-0.1+0.1+0.25+0.01,-0.1,-0.1);
    matrixTete.scale(0.2f,0.2f,0.2f);

    QMatrix4x4 matrixBrasLeft;
    matrixBrasLeft.translate(0.0,0.0,-5.0);
    matrixBrasLeft.rotate(rotation);
    matrixBrasLeft.translate(0.2-0.05,-0.05-0.15-0.06,-0.05);
    matrixBrasLeft.scale(0.4f,0.1f,0.1f);
    matrixBrasLeft.rotate(15,QVector3D(0.0,0.0,1.0)); //使得手臂和身体分离的旋转

    QMatrix4x4 matrixBrasRight;
    matrixBrasRight.translate(0.0,0.0,-5.0);
    matrixBrasRight.rotate(rotation);
    matrixBrasRight.translate(0.2-0.05,-0.05+0.15+0.06,-0.05);
    matrixBrasRight.scale(0.4f,0.1f,0.1f);
    matrixBrasRight.rotate(-15,QVector3D(0.0,0.0,1.0));

    QMatrix4x4 matrixJambeRight;
    matrixJambeRight.translate(0.0,0.0,-5.0);
    matrixJambeRight.rotate(rotation);
    matrixJambeRight.translate(-0.75,-0.05-0.1,-0.05);
    matrixJambeRight.scale(0.5f,0.1f,0.1f);
    matrixJambeRight.rotate(-15,QVector3D(0.0,0.0,1.0));

    QMatrix4x4 matrixJambeLeft;
    matrixJambeLeft.translate(0.0,0.0,-5.0);
    matrixJambeLeft.rotate(rotation);
    matrixJambeLeft.translate(-0.75,-0.05+0.1,-0.05);
    matrixJambeLeft.scale(0.5f,0.1f,0.1f);
    matrixJambeLeft.rotate(15,QVector3D(0.0,0.0,1.0));

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

    /*
    Model matrix (model to world)   物体自己的坐标系
    View matrix (world to view) 视角坐标系？？
    Projection matrix (view to projection space)   投影坐标
    You would project a coordinate C onto the screen using the formula  C' = P * V * M * C

    ！对物体进行  旋转  移动之后 改变的是 物体自己的坐标系 MC
     这样物体的坐标系 就不等于世界坐标系了  每次的操作都是对物体坐标系的操作



    */
    //program->setUniformValue("mvp", projection * matrixCentre);
    // Draw cube geometry using indices from VBO 1
    //glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);
    // czq  atribute GL_LINES can be changed to GL_TRIANGLES

    program->setUniformValue("mvp", projection * matrixTorse);
    glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);
    // czq  atribute GL_LINES can be changed to GL_TRIANGLES


    program->setUniformValue("mvp", projection * matrixTete);
    glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);


    program->setUniformValue("mvp", projection * matrixBrasLeft);
    glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);

    program->setUniformValue("mvp", projection * matrixBrasRight);
    glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);


    program->setUniformValue("mvp", projection * matrixJambeRight);
    glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);

    program->setUniformValue("mvp", projection * matrixJambeLeft);
    glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);



    //glDrawElements(GL_TRIANGLES,nbrIndices, GL_UNSIGNED_SHORT, 0);
    // czq  atribute GL_LINES can be changed to GL_TRIANGLES

}
//! [2]
