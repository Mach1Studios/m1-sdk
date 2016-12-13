#include "ofMain.h"

#define FRAMESIZE 1
#define STEREO_CHANNELS 2

static void interleavedAmbiXBufferSpatialRender(ofMatrix4x4 matrix, int channels, int offset, int size, float *src, float *dst) {
    int frames = (size - offset) / (channels * FRAMESIZE * sizeof(float));
    
    //    // Create the B-Format to AmbiX xform vector
    //    float b_to_ambix[16] = {1.414, 0.0, 0.0, 0.0,
    //                             0.0, 0.0, 0.0, 1.0,
    //                             0.0, 1.0, 0.0, 0.0,
    //                             0.0, 0.0, 1.0, 0.0};
    //    // invert for the Ambix to B-Format vector
    //    float ambix_to_b[16];
    
    ofMatrix4x4 imatrix;
    imatrix = matrix.getInverse();
    imatrix.rotate(0, 0, 1, 0); // rotate matrix to Ambisonic
    
    for (int frame = 0; frame < frames; frame++) {
        int inframe = frame * FRAMESIZE * channels;
        int outframe = frame * FRAMESIZE * STEREO_CHANNELS;
        
        // get the soundfield components
        // get the soundfield components
        float W, X, Y, Z;
        
        W = src[inframe + 0]; // soundfield
        Y = src[inframe + 1]; // left-right
        Z = src[inframe + 2]; // top-bottom
        X = src[inframe + 3]; // front-back
        
        // stash them in an OpenGL coordinate system vector
        ofVec4f glvector;
        ofVec4f rglvector;
        glvector[0] = Y; // left right
        glvector[1] = Z; // top bottom
        glvector[2] = X; // front back
        glvector[3] = 1;
        
        // rotate the soundfield by the inverted matrix
        rglvector = glvector * imatrix; // imatrix * glvector // ?????????????
        //	rglvector = imatrix * glvector; // imatrix * glvector // ?????????????
        
        // get the soundfield components
        float Wr = W;
        float Xr = rglvector[2]; // front back
        float Yr = rglvector[0]; // left right
        float Zr = rglvector[1]; // top bottom
        
        // decode the Soundfield to Stereo - without 90 degree phase shift component
        //float left = (0.0928 + 0.255)*Xr + (0.4699 - 0.171)*Wr + (0.3277)*Yr; //Left = (0.0928 + 0.255j)X + (0.4699 - 0.171j)W + (0.3277)Y
        //float right = (0.0928 - 0.255)*Xr + (0.4699 + 0.171)*Wr - (0.3277)*Yr; //Right= (0.0928 - 0.255j)X + (0.4699 + 0.171j)W - (0.3277)Y
        
        //float left = Wr * 0.707 + Yr * 0.5;
        //float right = Wr * 0.707 - Yr * 0.5;
        
        float left = Wr * 0.707 + Yr * 0.5 + Xr * 0.141 + Zr * 0.141;
        float right = Wr * 0.707 - Yr * 0.5 + Xr * 0.141 + Zr * 0.141;
        
        //float left = Wr * 0.707; // MOno
        //float right = Wr * 0.707;
        
        // write frame to dst
        dst[outframe] = left;
        dst[outframe + 1] = right;
    }
    
}
