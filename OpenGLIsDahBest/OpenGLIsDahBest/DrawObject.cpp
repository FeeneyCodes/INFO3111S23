#include "globalStuff.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "cMeshObject.h"
#include "cVAOManager/cVAOManager.h"

// Returns false if it didn't find the model to draw
bool DrawObject( cMeshObject* pMeshToDraw, 
                 glm::mat4 matModel, 
                 cVAOManager* pModelManger, 
                 GLuint shaderProgram_ID )
{
                // Copy the mesh (for ease of reading)

    if ( ! pMeshToDraw->bIsVisible )
    {
        // Skip rest of rendering
        return true;
    }

    matModel = glm::mat4(1.0f);        // Identity matrix

    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                                            pMeshToDraw->position);


    glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
                                       glm::radians(pMeshToDraw->orientation.x),
                                       glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
                                       glm::radians(pMeshToDraw->orientation.y),
                                       glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
                                       glm::radians(pMeshToDraw->orientation.z),
                                       glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                                    glm::vec3(pMeshToDraw->scale, pMeshToDraw->scale, pMeshToDraw->scale));

    matModel = matTranslate * matModel; // Math order is the last thing

    matModel = matModel * matRotateX;
    matModel = matModel * matRotateY;
    matModel = matModel * matRotateZ;

    //            mModelRotationOnly = mModelRotationOnly * matRotateX;
    //            mModelRotationOnly = mModelRotationOnly * matRotateY;
    //            mModelRotationOnly = mModelRotationOnly * matRotateZ;

    matModel = matModel * matScale; // Math order is 1st thing\


    GLint matModel_UL = glGetUniformLocation(shaderProgram_ID, "matModel");
    glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(matModel));

    // Calculate inverse transpose of 4x4 matrix
    //uniform mat4x4 matModel_InvTrans;
    glm::mat4 matModl_IT = glm::inverse(glm::transpose(matModel));
    GLint matModel_InvTrans_UL = glGetUniformLocation(shaderProgram_ID, "matModel_InvTrans");
    glUniformMatrix4fv(matModel_InvTrans_UL, 1, GL_FALSE, glm::value_ptr(matModl_IT));


    //        glUseProgram(program);
    //        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
    //            mvp_location = glGetUniformLocation(shaderProgram, "MVP");
    //            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));


                // uniform vec3 colorOverrideRGB;
    GLint diffuseColor_OverrideRGB_UL = glGetUniformLocation(shaderProgram_ID, "diffuseColor_OverrideRGB");
    //            glUniform3f(colorOverrideRGB_UL, 1.0f, 0.0f, 0.0f);
    glUniform3f(diffuseColor_OverrideRGB_UL,
                pMeshToDraw->diffuseColour.r,
                pMeshToDraw->diffuseColour.g,
                pMeshToDraw->diffuseColour.b);

    GLint specularColourRGB_Power_UL = glGetUniformLocation(shaderProgram_ID, "specularColourRGB_Power");
    glUniform4f(specularColourRGB_Power_UL,
                pMeshToDraw->specularColourRGB.r,
                pMeshToDraw->specularColourRGB.g,
                pMeshToDraw->specularColourRGB.b,
                pMeshToDraw->specularPower_or_shininess);    // Snininess


    //uniform float alphaTransparency;
    GLint alphaTransparency_UL = glGetUniformLocation(shaderProgram_ID, "alphaTransparency");
    glUniform1f(alphaTransparency_UL, pMeshToDraw->alphaTransparency);

    // Is this a "debug object" (i.e. not lit)
    //uniform bool bDontLight;
    GLint bDontLight_UL = glGetUniformLocation(shaderProgram_ID, "bDontLight");
    if (pMeshToDraw->bDontLight)
    {
        glUniform1f(bDontLight_UL, (float)GL_TRUE);
    }
    else
    {
        glUniform1f(bDontLight_UL, (float)GL_FALSE);
    }



    //uniform vec3 positionOffset;
    GLint positionOffset_UL = glGetUniformLocation(shaderProgram_ID, "positionOffset");
    glUniform3f(positionOffset_UL,
                pMeshToDraw->position.x,     // 4
                pMeshToDraw->position.y,     // 0 
                pMeshToDraw->position.z);    // 0

    //uniform float scale;
    GLint scale_UL = glGetUniformLocation(shaderProgram_ID, "scale");
    glUniform1f(scale_UL,
                pMeshToDraw->scale);


    // GL_POINT, GL_LINE, and GL_FILL
    if (pMeshToDraw->isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    sModelDrawInfo modelToDraw;
    if (pModelManger->FindDrawInfoByModelName(pMeshToDraw->meshName, modelToDraw))
    {
        glBindVertexArray(modelToDraw.VAO_ID);

        glDrawElements(GL_TRIANGLES,
                       modelToDraw.numberOfIndices,
                       GL_UNSIGNED_INT,
                       (void*)0);

        // Bind it to nothing
        glBindVertexArray(0);

    }
    else
    {
        // Didn't find the object
        return false;
    }

    return true;

}// bool DrawObject( 
                 
