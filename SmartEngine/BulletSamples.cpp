#include "BulletSamples.h"

btRigidBody* addBox(float width, float height, float depth, float x, float y, float z, float mass, btDiscreteDynamicsWorld* dynamicsWorld)
{
    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(x, y, z));
    btCollisionShape* box = new btBoxShape(btVector3(width / 2.0, height / 2.0, depth / 2.0));
    btVector3 inertia(1.0, 1.0, 1.f);
    if (mass != 0.0)
        box->calculateLocalInertia(mass, inertia);
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);
    btRigidBody* body = new btRigidBody(info);
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

    dynamicsWorld->addRigidBody(body);
    return body;
}

unsigned int addmodel(Model* model, btDiscreteDynamicsWorld* dynamicsworld, std::vector<bulletObject*>* bodies)
{
    unsigned int pos = 0;

    for (int i = 0; i < model->meshes.size(); i++)
    {
        btAlignedObjectArray<btVector3> vertices;
        std::vector<btVector3> pos1;
        btTriangleMesh* trmesh = new btTriangleMesh();
        btScalar* points = new btScalar[model->meshes[i].vertices.size() * 3];
        btTriangleIndexVertexArray* physicMesh = new  btTriangleIndexVertexArray();
        for (int j = 0; j < model->meshes[i].vertices.size(); j++)
        {
            vertices.push_back(btVector3(model->meshes[i].vertices[j].Position.x, model->meshes[i].vertices[j].Position.y, model->meshes[i].vertices[j].Position.z));
        }
        btCollisionShape* shape = new btConvexHullShape(&(vertices[0].getX()), vertices.size());
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(btVector3(0, 0, 0));
        btVector3 inertia(0, 0, 0);
        btMotionState* motion = new btDefaultMotionState(trans);
        btRigidBody::btRigidBodyConstructionInfo info(0, motion, shape, inertia);
        btRigidBody* body = new btRigidBody(info);
        //body->setCollisionFlags(btCollisionObject:: CF_NO_CONTACT_RESPONSE);
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

        //body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
        body->setFriction(btScalar(1.0)); // Трение
        if (i == 0)
            pos = bodies->size();
        dynamicsworld->addRigidBody(body);
        bodies->push_back(new bulletObject(body, bodies->size(), 0.0, 0.0, 0.0));
    }
    return pos;
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
//
//void renderBox(bulletObject* obj, Shader * shader, Player* player)
//{
//    btRigidBody* sphere = obj->body;
//    glColor3f(1, 0, 0);
//    btVector3 extent  = ((btBoxShape*)sphere->getCollisionShape())->getHalfExtentsWithoutMargin();
//    btTransform t;
//    sphere->getMotionState()->getWorldTransform(t);
//    float mat[16];
//    t.getOpenGLMatrix(mat);
//    shader->Use();
//
//    float points[]{
//        -extent.x(), extent.y(), -extent.z(),
//        -extent.x(), -extent.y(), -extent.z(),
//        -extent.x(), extent.y(), extent.z(),
//        -extent.x(), extent.y(), extent.z(),
//        -extent.x(), -extent.y(), extent.z(),
//        -extent.x(), -extent.y(), -extent.z(),
//        
//        -extent.x(), extent.y(), extent.z(),
//        -extent.x(), -extent.y(), extent.z(),
//        extent.x(), -extent.y(), extent.z(),
//        extent.x(), -extent.y(), extent.z(),
//        extent.x(), extent.y(), extent.z(),
//        -extent.x(), extent.y(), extent.z(),
//
//        extent.x(), extent.y(), extent.z(),
//        extent.x(), extent.y(), -extent.z(),
//        extent.x(), -extent.y(), -extent.z(),
//        extent.x(), -extent.y(), -extent.z(),
//        extent.x(), -extent.y(), extent.z(),
//        extent.x(), extent.y(), extent.z(),
//
//        extent.x(), extent.y(), -extent.z(),
//        -extent.x(), extent.y(), -extent.z(),
//        -extent.x(), -extent.y(), -extent.z(),
//        -extent.x(), -extent.y(), -extent.z(),
//        extent.x(), -extent.y(), -extent.z(),
//        extent.x(), extent.y(), -extent.z(),
//
//        -extent.x(), extent.y(), extent.z(),
//        -extent.x(), extent.y(), -extent.z(),
//        extent.x(), extent.y(), -extent.z(),
//        extent.x(), extent.y(), -extent.z(),
//        extent.x(), extent.y(), extent.z(),
//        -extent.x(), extent.y(), extent.z(),
//
//        -extent.x(), -extent.y(), extent.z(),
//        -extent.x(), -extent.y(), -extent.z(),
//        extent.x(), -extent.y(), -extent.z(),
//        extent.x(), -extent.y(), -extent.z(),
//        extent.x(), -extent.y(), extent.z(),
//        -extent.x(), -extent.y(), extent.z(),
//
//    };
//    float colours[] = {
//        1.0f, 0.0f,  0.0f,
//        0.0f, 1.0f,  0.0f,
//        0.0f, 0.0f,  1.0f
//    };
//    if (obj->hit == true)
//        shader->setVec3("color", glm::vec3(0.0, 1.0, 0.0));
//    else
//        shader->setVec3("color", glm::vec3(0.0, 0.0, 1.0));
//    GLuint points_vbo = 0;
//    glGenBuffers(1, &points_vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
//    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), points, GL_STATIC_DRAW);
//    GLuint vao = 0;
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//    glEnableVertexAttribArray(0);
//
//    glm::mat4 trans = glm::make_mat4(mat);
//    //trans = glm::translate(trans, glm::vec3(1.5f, -0.5f, 0.0f));
//    //trans = glm::rotate(trans, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
//    GLuint transformLoc = glGetUniformLocation(shader->Program, "trans");
//    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);
//
//    glm::mat4 view = glm::mat4(1.0f);
//    view = player->lookAt();
//    GLuint viewLoc = glGetUniformLocation(shader->Program, "view");
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
//
//    glm::mat4 projection = glm::mat4(1.0f);
//    projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
//    GLuint projectionLoc = glGetUniformLocation(shader->Program, "projection");
//    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
//    glBindVertexArray(vao);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//}
//
//void renderSphere(btRigidBody* sphere, Model* model, Shader shader, Player* player)
//{
//    
//    glColor3f(1, 0, 0);
//    float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
//    btTransform t;
//    sphere->getMotionState()->getWorldTransform(t);
//    float mat[16];
//    t.getOpenGLMatrix(mat);
//    glm::mat4 tr = glm::make_mat4(mat); // translation, rotation
//    glm::mat4 view = glm::mat4(1.0f);
//    view = player->lookAt();
//    glm::mat4 projection = glm::mat4(1.0f);
//    projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
//    shader.Use();
//    shader.setMat4("view", view);
//    shader.setMat4("projection", projection);
//    shader.setMat4("model", tr);
//    //model->Draw(shader);
//}

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