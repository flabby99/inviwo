/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2018 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <mpi/processors/opencvcudawarp.h>
#include <modules/opengl/openglutils.h>
#include <mpi/code/cudawarping.h>

namespace inviwo {

const ProcessorInfo OpencvCUDAWarp::processorInfo_{
    "org.inviwo.OpencvCUDAWarp",  // Class identifier
    "OpenCV CUDA Warp",            // Display name
    "Image Processign",            // Category
    CodeState::Experimental,       // Code state
    "CUDA, OpenCV"          // Tags
};

const ProcessorInfo OpencvCUDAWarp::getProcessorInfo() const { return processorInfo_; }

OpencvCUDAWarp::OpencvCUDAWarp()
    : Processor()
    , entryPort_("input")
    , outport_("output")
    , cameraBaseline_("cameraBaseline", "Camera Baseline", 0.5)
    , camera_("camera", "Camera") {
    
    addPort(entryPort_, "ImagePortGroup1");

    //TODO change group when have matrix
    addPort(outport_, "ImagePortGroup1");

    addProperty(cameraBaseline_);
    addProperty(camera_);

    last_image_size_ = std::vector<size_t>(2, 0);

    // Initialise cuda resources
    cv::cuda::setDevice (0);
}

void OpencvCUDAWarp::process() {
    // bind the input texture
    TextureUnit colorTexUnit;

    if (entryPort_.isReady()) {
        utilgl::bindColorTexture(entryPort_, colorTexUnit);

        size2_t input_size = entryPort_.getDimensions();
    
        //Texture2D(int arows, int acols, Format aformat, 
        //          unsigned int atexId, bool autoRelease = false);
        //Make sure that the glEnum matches the unsigned int expected
        ocvTexture_ = cv::ogl::Texture2D(
            input_size[0], input_size[1], cv::ogl::Texture2D::Format::RGBA,
            colorTexUnit.getEnum(), false
        );

        if(input_size != last_image_size_) {
            cudaTexture_ = cv::cuda::GpuMat(
                input_size[0], input_size[1], CV_32FC1
            );
        }
        ocvTexture_.copyTo(cudaTexture_);
        last_image_size_ = input_size;

        // Do the cuda warping
        // TODO call this correctly
        warp::cudaBackWarp(
            // FILL
        )
    }

    Image input_image = entryPort_.getData();
    
}
