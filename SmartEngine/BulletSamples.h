#pragma once
#ifndef BULLETSAMPLES_H
#define BULLETSAMPLES_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include "Model.h"
#include "Player.h"



btRigidBody* addBox(float width, float height, float depth, float x, float y, float z,float mass, btDiscreteDynamicsWorld* dynamicsWorld)
{
    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(x, y, z));
    btCollisionShape* box = new btBoxShape(btVector3(width/2.0,height/2.0,depth/2.0));
    btVector3 inertia(0.0, 5.0, 100.f);
    if (mass != 0.0)
        box->calculateLocalInertia(mass, inertia);
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);
    btRigidBody* body = new btRigidBody(info);
    dynamicsWorld->addRigidBody(body);
    return body;
}

void addmodel(Model* model, btDiscreteDynamicsWorld* dynamicsworld)
{
    
    for (int i = 0; i < model->meshes.size(); i++)
    {
        btAlignedObjectArray<btVector3> vertices;
        btAlignedObjectArray<unsigned int> indices;
        for (int j = 0; j < model->meshes[i].vertices.size(); j++)
        {
            vertices.push_back(btVector3(model->meshes[i].vertices[j].Position.x, model->meshes[i].vertices[j].Position.y, model->meshes[i].vertices[j].Position.z));
            
        }
        for (int j = 0; j < model->meshes[i].indices.size(); j++)
        {
            indices.push_back(model->meshes[i].indices[j]);
        }
        btTriangleIndexVertexArray* meshInterface = new btTriangleIndexVertexArray();
        btIndexedMesh part;

        part.m_vertexBase = (const unsigned char*)&(vertices[0].getX());
        part.m_vertexStride = sizeof(btScalar) * 3;
        part.m_numVertices = vertices.size();
        part.m_triangleIndexBase = (const unsigned char*)&indices[0];
        part.m_triangleIndexStride = sizeof(short) * 3;
        part.m_numTriangles = indices.size() / 3;
        part.m_indexType = PHY_SHORT;

        meshInterface->addIndexedMesh(part, PHY_SHORT);
        btTransform trans;
        bool useQuantizedAabbCompression = true;
        btBvhTriangleMeshShape* trimeshShape = new btBvhTriangleMeshShape(meshInterface, useQuantizedAabbCompression);
        trans.setOrigin(btVector3(0, 0, 0));
        btVector3 inertia(0.0, 0.0, 0.0);
        btMotionState* motion = new btDefaultMotionState(trans);
        btRigidBody::btRigidBodyConstructionInfo info(0, motion, trimeshShape, inertia);
        btRigidBody* body = new btRigidBody(info);
       // body->setFriction(btScalar(0.9)); // Трение
        dynamicsworld->addRigidBody(body);

    }
    
}

btRigidBody* addSphere(float rad, float x, float y, float z, float mass, btDiscreteDynamicsWorld* dynamicsWorld)
{
    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(x, y, z));
    btSphereShape* sphere = new btSphereShape(rad);
    btVector3 inertia(0, 0, 0);
    if (mass != 0.0)
        sphere->calculateLocalInertia(mass, inertia);
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);
    btRigidBody* body = new btRigidBody(info);
    dynamicsWorld->addRigidBody(body);
    return body;
}

void renderBox(bulletObject* obj, Shader * shader, Player* player)
{
    btRigidBody* sphere = obj->body;
    glColor3f(1, 0, 0);
    btVector3 extent  = ((btBoxShape*)sphere->getCollisionShape())->getHalfExtentsWithoutMargin();
    btTransform t;
    sphere->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);
    shader->Use();

    float points[]{
        -extent.x(), extent.y(), -extent.z(),
        -extent.x(), -extent.y(), -extent.z(),
        -extent.x(), extent.y(), extent.z(),
        -extent.x(), extent.y(), extent.z(),
        -extent.x(), -extent.y(), extent.z(),
        -extent.x(), -extent.y(), -extent.z(),
        
        -extent.x(), extent.y(), extent.z(),
        -extent.x(), -extent.y(), extent.z(),
        extent.x(), -extent.y(), extent.z(),
        extent.x(), -extent.y(), extent.z(),
        extent.x(), extent.y(), extent.z(),
        -extent.x(), extent.y(), extent.z(),

        extent.x(), extent.y(), extent.z(),
        extent.x(), extent.y(), -extent.z(),
        extent.x(), -extent.y(), -extent.z(),
        extent.x(), -extent.y(), -extent.z(),
        extent.x(), -extent.y(), extent.z(),
        extent.x(), extent.y(), extent.z(),

        extent.x(), extent.y(), -extent.z(),
        -extent.x(), extent.y(), -extent.z(),
        -extent.x(), -extent.y(), -extent.z(),
        -extent.x(), -extent.y(), -extent.z(),
        extent.x(), -extent.y(), -extent.z(),
        extent.x(), extent.y(), -extent.z(),

        -extent.x(), extent.y(), extent.z(),
        -extent.x(), extent.y(), -extent.z(),
        extent.x(), extent.y(), -extent.z(),
        extent.x(), extent.y(), -extent.z(),
        extent.x(), extent.y(), extent.z(),
        -extent.x(), extent.y(), extent.z(),

        -extent.x(), -extent.y(), extent.z(),
        -extent.x(), -extent.y(), -extent.z(),
        extent.x(), -extent.y(), -extent.z(),
        extent.x(), -extent.y(), -extent.z(),
        extent.x(), -extent.y(), extent.z(),
        -extent.x(), -extent.y(), extent.z(),

    };
    float colours[] = {
        1.0f, 0.0f,  0.0f,
        0.0f, 1.0f,  0.0f,
        0.0f, 0.0f,  1.0f
    };
    if (obj->hit == true)
        shader->setVec3("color", glm::vec3(0.0, 1.0, 0.0));
    else
        shader->setVec3("color", glm::vec3(0.0, 0.0, 1.0));
    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), points, GL_STATIC_DRAW);
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glm::mat4 trans = glm::make_mat4(mat);
    //trans = glm::translate(trans, glm::vec3(1.5f, -0.5f, 0.0f));
    //trans = glm::rotate(trans, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    GLuint transformLoc = glGetUniformLocation(shader->Program, "trans");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);

    glm::mat4 view = glm::mat4(1.0f);
    view = player->lookAt();
    GLuint viewLoc = glGetUniformLocation(shader->Program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
    GLuint projectionLoc = glGetUniformLocation(shader->Program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    glBindVertexArray(vao);


    glDrawArrays(GL_TRIANGLES, 0, 36);

    //glm::mat4 tr = glm::make_mat4(mat); // translation, rotation
    //glm::mat4 view = glm::mat4(1.0f);
    //view = player->lookAt();
    //glm::mat4 projection = glm::mat4(1.0f);
    //projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
    //shader.Use();
    //shader.setMat4("view", view);
    //shader.setMat4("projection", projection);
    //shader.setMat4("model", tr);
    //model->Draw(shader);
}

void renderSphere(btRigidBody* sphere, Model* model, Shader shader, Player* player)
{
    
    glColor3f(1, 0, 0);
    float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
    btTransform t;
    sphere->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);
    glm::mat4 tr = glm::make_mat4(mat); // translation, rotation
    glm::mat4 view = glm::mat4(1.0f);
    view = player->lookAt();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
    shader.Use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setMat4("model", tr);
    //model->Draw(shader);
    //glPushMatrix();
    //glMultMatrixf(mat);	// translation, rotation
    //glBegin(GL_QUADS);
    //glVertex3f(-10, 0, 10);
    //glVertex3f(-10, 0, -10);
    //glVertex3f(10, 0, -10);
    //glVertex3f(10, 0, 10);
    //glEnd();
    //glPopMatrix();
    ////gluSphere(quad, r, 20, 20);
    //glPopMatrix();
}

void renderPlane(btRigidBody* sphere)
{
    //glClear(GL_COLOR_BUFFER_BIT);
    if (sphere->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE)
        //return;
        glColor3f(1.0, 0.8, 0.8);
    btTransform t;
    sphere->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);
    glPushMatrix();
    glMultMatrixf(mat);	// translation, rotation
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.8, 0.8);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(-100.f, 0.f, -100.f);
    glVertex3f(100.f, 0.f, -100.f);
    glVertex3f(100.f, 0.f, 100.f);
    glEnd();
    glPopMatrix();
}
#endif // !BULLETSAMPLES_H
