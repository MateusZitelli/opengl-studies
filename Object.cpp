#include <iostream>
#include <fstream>
#include <Object.h>
#include <Utils.h>

using namespace std;

void Object::prepare(){
    GLvoid *colorOffset, *normalOffset;
    colorOffset = (GLvoid*)sizeof(vert[0].Position);
    normalOffset = (GLvoid*)(sizeof(vert[0].Position) + sizeof(vert[0].Color));

    ModelMatrixUniformLocation = glGetUniformLocation(ShaderId, "ModelMatrix");
    ExitOnGLError("ERROR: Could not get the shader uniform locations");

    glGenVertexArrays(1, &BufferIds[0]);
    ExitOnGLError("ERROR: Could not generate the VAO");

    glBindVertexArray(BufferIds[0]);
    ExitOnGLError("Error: Could not bind the VAO");

    glGenBuffers(2, &BufferIds[1]);
    ExitOnGLError("ERROR: Could not generate the buffer objects");

    glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nv, vert, GL_STATIC_DRAW);
    ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vert[0]), (GLvoid*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vert[0]), colorOffset);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vert[0]), normalOffset);
    ExitOnGLError("ERROR: Could not set VAO attributes");

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexT) * nf * 3, ind, GL_STATIC_DRAW);
    ExitOnGLError("ERROR: Could not bind the IBO to tha VAO");
}

void Object::draw(){
    Matrix ModelMatrix = IDENTITY_MATRIX;
    ExitOnGLError("ERROR: Could not use the shader program");

    glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.m);
    ExitOnGLError("ERROR: Could not set the shader uniforms");

    glBindVertexArray(BufferIds[0]);
    ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

    glDrawElements(GL_TRIANGLES, nf * 3, GL_UNSIGNED_BYTE, (GLvoid*)0);
    ExitOnGLError("ERROR: Could not draw the cube");

    glBindVertexArray(0);
}

void Object::remove(){
    glDeleteBuffers(2, &BufferIds[1]);
    glDeleteVertexArrays(1, &BufferIds[0]);
    ExitOnGLError("Error: Could not destroy the buffer objects");
}

void Object::print(){
    int j;
    indexT *vi;
    Vertex *v;
    vi = ind;
    for(int i = 0; i < nf; i++){
        cout << "Facet " << i << "\n";
        for(int k = 0; k < 3; k++){
            cout << "Vertex" << k << "\n";
            v = vert + *vi++;
            cout << "Position:\n";
            for(j = 0; j < 4; j++){
                cout << v->Position[j] << ", ";
            }
            cout << "\n";
            cout << "Color:\n";
            for(j = 0; j < 4; j++){
                cout << v->Color[j] << ", ";
            }
            cout << "\n";
            cout << "Normal:\n";
            for(j = 0; j < 4; j++){
                cout << v->Normal[j] << ", ";
            }
            cout << "\n\n";
        }
        cout << "\n";
    }
}

void Object::load(const char *filename, GLuint ShaderId) {
    ifstream f(filename);
    string s;
    f >> s; // Remove the OFF in the begin
    f >> nv >> nf >> ne;
    int j;
    float normalSizeSquared, normalSize;

    vert = new Vertex[nv];
    indexT *vi;
    vi = ind = new indexT[3 * nf];
    for(int i = 0; i < nv; i++){
        f >> vert[i].Position[0] >> vert[i].Position[1] >> vert[i].Position[2];
        vert[i].Position[3] = 1;
    }

    for(int i = 0; i < nf; i++){
        f >> j >> *vi++ >> *vi++ >> *vi++;
    }

    for(int i = 0; i < nv; i++){
        for(int j = 0; j < 4; j++){
            vert[i].Color[j] = 1;
            vert[i].Normal[j] = 0;
        }
    }

    vi = ind;
    // Define the vertex normals
    for(int i = 0; i < nf; i++){
        Vector ab, ac, norm; 
        Vertex *a, *b, *c;
        normalSizeSquared = 0;
        a = vert + *vi++;
        b = vert + *vi++;
        c = vert + *vi++;
    
        // Calculate the facet vectors
        for(j = 0; j < 4; j++){
            ab.values[j] = b->Position[j] - a->Position[j];
            ac.values[j] = c->Position[j] - a->Position[j];
        }

       
        // Calculate the cross product
        for(j = 0; j < 3; j++){
            int ia, ib;
            norm.values[j] = 0;
            ia = (j + 1) % 3;
            ib = (j + 2) % 3;
            norm.values[j] = ab.values[ia] * ac.values[ib] - ab.values[ib] * ac.values[ia];
            normalSizeSquared += norm.values[j] * norm.values[j];
        }

        normalSize = sqrt(normalSizeSquared);

        // Sum the normal in each vertex
        for(j = 0; j < 4; j++){
            norm.values[j] /= normalSize;
            a->Normal[j] += norm.values[j];
            b->Normal[j] += norm.values[j];
            c->Normal[j] += norm.values[j];
        }

        #ifdef VERBOSE
        cout << "Facet "<< i << "\n";
        cout << "AB:\n";
        for(j = 0; j < 4; j++){
            cout << ab.values[j] << ", ";
        }
        cout << "\n";
        cout << "AC:\n";
        for(j = 0; j < 4; j++){
            cout << ac.values[j] << ", ";
        }
        cout << "\n";
        cout << "Normals:\n";
        for(j = 0; j < 4; j++){
            cout << norm.values[j] << ", ";
        }
        cout << "\n";

        cout << "Vec A normals:\n";
        for(j = 0; j < 4; j++){
            cout << a->Normal[j] << ", ";
        }
        cout << "\n";

        cout << "Vec B normals:\n";
        for(j = 0; j < 4; j++){
            cout << b->Normal[j] << ", ";
        }
        cout << "\n";

        cout << "Vec C normals:\n";
        for(j = 0; j < 4; j++){
            cout << c->Normal[j] << ", ";
        }
        cout << "\n\n";
        #endif

    }

    // Normalize the normals
    for(int i = 0; i < nv; i++){
        Vertex *v = vert + i;
        normalSizeSquared = 0;
        for(int j = 0; j < 4; j++){
            normalSizeSquared += v->Normal[j] * v->Normal[j];
        }

        normalSize = sqrt(normalSizeSquared);

        for(int j = 0; j < 4; j++){
            v->Normal[j] /= normalSize;
        }

        #ifdef VERBOSE
        cout << "Vertex position\n";
        for(int j = 0; j < 4; j++){
            cout << v->Position[j] << ", ";
        }
        cout << "\n";
        cout << "Vertex normals\n";
        for(int j = 0; j < 4; j++){
            cout << v->Normal[j] << ", ";
        }
        cout << "\n\n";
        #endif

    }

    this->ShaderId = ShaderId;
    prepare();
}
