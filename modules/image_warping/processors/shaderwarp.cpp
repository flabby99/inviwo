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

#include <image_warping/processors/shaderwarp.h>
#include <modules/opengl/openglutils.h>

namespace inviwo {

const ProcessorInfo ShaderWarp::processorInfo_{
    "org.inviwo.ShaderWarp",  // Class identifier
    "Shader Based Backward Warp",            // Display name
    "Image Processing",            // Category
    CodeState::Experimental,       // Code state
    "Warp", "Image"          // Tags
};

const ProcessorInfo ShaderWarp::getProcessorInfo() const { return processorInfo_; }

ShaderWarp::ShaderWarp()
    : Processor()
    , entryPort_("input")
    , outport_("output")
    , cameraBaseline_("cameraBaseline", "Camera Baseline", 0.5)
    , camera_("camera", "Camera") {

    shader_.onReload([this]() { invalidate(InvalidationLevel::InvalidResources); });

    addPort(entryPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");

    addProperty(cameraBaseline_);
    addProperty(camera_);

    last_image_size_ = std::vector<size_t>(2, 0);
}

void ShaderWarp::initializeResources() {
    utilgl::addDefines(shader_, camera_);
    shader_.build();
}

void ShaderWarp::process() {
    if (entryPort_.isChanged()) {
        utilgl::bindColorTexture(entryPort_, colorTexUnit);
    
        utilgl::activateAndClearTarget(outport_);
        shader_.activate();
        
        TextureUnitContainer units;
        utilgl::bindAndSetUniforms(shader_, units, )

        
    }

    
}
