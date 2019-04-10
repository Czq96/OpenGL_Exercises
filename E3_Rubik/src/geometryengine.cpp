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
#include <stdio.h>
#include <stdlib.h>

#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector3D color;
};

//上下底 + 侧面
VertexData vertices[] = {
    //下底
    {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f,0.0f)},
    {QVector3D(1.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f,0.0f)},
    {QVector3D(1.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f,0.0f)},
    {QVector3D(0.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f,0.0f)},
    //上底
    {QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f,1.0f)},
    {QVector3D(1.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f,1.0f)},
    {QVector3D(1.0f, 1.0f, 1.0f), QVector3D(0.0f, 1.0f,1.0f)},
    {QVector3D(0.0f, 1.0f, 1.0f), QVector3D(0.0f, 1.0f,1.0f)},
    //侧面1
    {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f,0.0f)},
    {QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f,0.0f)},
    {QVector3D(1.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f,0.0f)},
    {QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f,0.0f)},
    //侧面2
    {QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f,1.0f)},
    {QVector3D(1.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f,1.0f)},
    {QVector3D(1.0f, 1.0f, 1.0f), QVector3D(0.0f, 0.0f,1.0f)},
    {QVector3D(1.0f, 0.0f, 1.0f), QVector3D(0.0f, 0.0f,1.0f)},
    //侧面3
    {QVector3D(1.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f,1.0f)},
    {QVector3D(0.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f,1.0f)},
    {QVector3D(0.0f, 1.0f, 1.0f), QVector3D(1.0f, 0.0f,1.0f)},
    {QVector3D(1.0f, 1.0f, 1.0f), QVector3D(1.0f, 0.0f,1.0f)},
    //侧面4
    {QVector3D(0.0f, 1.0f, 0.0f), QVector3D(1.0f, 1.0f,0.0f)},
    {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f,0.0f)},
    {QVector3D(0.0f, 0.0f, 1.0f), QVector3D(1.0f, 1.0f,0.0f)},
    {QVector3D(0.0f, 1.0f, 1.0f), QVector3D(1.0f, 1.0f,0.0f)},
};

const int nbrVertices = 24;

GLushort indices[] = {
    2,1,0,
    3,2,0, // 这里不是 0， 1 ， 2 / 0 2 3 是因为用这两个的话颜色会显示在正方体内部
    4,5,6,
    4,6,7,
    8,9,10,
    8,10,11,
    12,13,14,
    12,14,15,
    16,17,18,
    16,18,19,
    20,21,22,
    20,22,23
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
void GeometryEngine::drawGeometry(QOpenGLShaderProgram *program, QMatrix4x4 VP, QQuaternion rotationAuto)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    //

    /* (xy) 00 0-1
     *      -10 -1-1
     */
    QMatrix4x4 matrixBottom00;
    matrixBottom00.translate(0,0,0);
    matrixBottom00.translate(0,0,0);

    QMatrix4x4 matrixBottom01;
    matrixBottom01.translate(0,-1.10,0);

    QMatrix4x4 matrixBottom10;
    matrixBottom10.translate(-1.10,0,0);

    QMatrix4x4 matrixBottom11;
    matrixBottom11.translate(-1.10,-1.10,0);

    QMatrix4x4 matrixTop00;
    matrixTop00.rotate(rotationAuto);
    matrixTop00.translate(0,0,1.10);

    QMatrix4x4 matrixTop01;
    matrixTop01.rotate(rotationAuto);
    matrixTop01.translate(0,-1.10,1.10);

    QMatrix4x4 matrixTop10;
    matrixTop10.rotate(rotationAuto);
    matrixTop10.translate(-1.10,0,1.10);

    QMatrix4x4 matrixTop11;
    matrixTop11.rotate(rotationAuto);
    matrixTop11.translate(-1.10,-1.10,1.10);

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

    // Draw basic cube geometry using indices from VBO 1
    //glDrawElements(GL_TRIANGLES, arrayBuf.size(), GL_UNSIGNED_SHORT, 0);

    program->setUniformValue("mvp", VP * matrixBottom00);
    glDrawElements(GL_TRIANGLES,arrayBuf.size(), GL_UNSIGNED_SHORT, 0); // 第三四个参数意义不明
    program->setUniformValue("mvp", VP * matrixBottom01);
    glDrawElements(GL_TRIANGLES,arrayBuf.size(), GL_UNSIGNED_SHORT, 0); //
    program->setUniformValue("mvp", VP * matrixBottom10);
    glDrawElements(GL_TRIANGLES,arrayBuf.size(), GL_UNSIGNED_SHORT, 0); //
    program->setUniformValue("mvp", VP * matrixBottom11);
    glDrawElements(GL_TRIANGLES,arrayBuf.size(), GL_UNSIGNED_SHORT, 0); //

    program->setUniformValue("mvp", VP * matrixTop00);
    glDrawElements(GL_TRIANGLES,arrayBuf.size(), GL_UNSIGNED_SHORT, 0); //
    program->setUniformValue("mvp", VP * matrixTop01);
    glDrawElements(GL_TRIANGLES,arrayBuf.size(), GL_UNSIGNED_SHORT, 0); //
    program->setUniformValue("mvp", VP * matrixTop10);
    glDrawElements(GL_TRIANGLES,arrayBuf.size(), GL_UNSIGNED_SHORT, 0); //
    program->setUniformValue("mvp", VP * matrixTop11);
    glDrawElements(GL_TRIANGLES,arrayBuf.size(), GL_UNSIGNED_SHORT, 0); //

}


void GeometryEngine::drawGeometry333(QOpenGLShaderProgram *program, QMatrix4x4 VP, QQuaternion rotationAuto)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

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

    //构造三重指针
    QMatrix4x4 ***matrix3;
    matrix3 = new QMatrix4x4**[3];
    for (int i = 0; i < 3; ++i)
    {
        matrix3[i] = new QMatrix4x4*[3];
        for (int j = 0; j < 3; ++j)
        {
            matrix3[i][j] = new QMatrix4x4[3];
        }
    }
    //生成所有移动矩阵
    for(int z = 0; z<3;z++)
    {
        for(int y = 0; y <3; y++)
        {
            for(int x=0; x<3; x++)
            {
                matrix3[z][y][x] = QMatrix4x4();
                //matrix3[z][y][x].translate(x-1,y-1,z-1);
                if(z==0)
                {//只动顶面
                    matrix3[z][y][x].rotate(rotationAuto);
                }
                //减0.5是为了将坐标轴中心移动到魔方体心   *1.10 是为了留出空隙
                matrix3[z][y][x].translate((x-1)*1.10-0.5,(y-1)*1.10-0.5,(z-1)*1.10-0.5);
                program->setUniformValue("mvp", VP * matrix3[z][y][x]);
                glDrawElements(GL_TRIANGLES,arrayBuf.size(), GL_UNSIGNED_SHORT, 0); //
            }
        }
    }
}
//! [2]
