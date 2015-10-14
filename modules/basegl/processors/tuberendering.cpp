/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015 Inviwo Foundation
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

#include "tuberendering.h"
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/shader/shaderutils.h>
#include <modules/opengl/openglutils.h>
#include <modules/opengl/rendering/meshdrawergl.h>
#include <inviwo/core/rendering/meshdrawerfactory.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
ProcessorClassIdentifier(TubeRendering,  "org.inviwo.TubeRendering")
ProcessorDisplayName(TubeRendering,  "Tube Rendering")
ProcessorTags(TubeRendering, Tags::GL);
ProcessorCategory(TubeRendering, "Rendering");
ProcessorCodeState(TubeRendering, CODE_STATE_EXPERIMENTAL);

TubeRendering::TubeRendering()
    : Processor()
    , mesh_("mesh")
    , imageInport_("imageInport")
    , outport_("outport")
    , radius_("radius","Tube Radius", 0.01f , 0.0001f , 2.f , 0.0001f)
    , camera_("camera","Camera")
    , trackball_(&camera_)
    , light_("light", "Lighting", &camera_)
    , shader_("tuberendering.vert", "tuberendering.geom", "tuberendering.frag",false)
    , drawer_(nullptr)
{
    addPort(mesh_);
    addPort(imageInport_);
    addPort(outport_);

    addProperty(radius_);
    addProperty(camera_);
    addProperty(light_);

    addProperty(trackball_);

    imageInport_.setOptional(true);

    outport_.addResizeEventListener(&camera_);
    
    mesh_.onChange([this]() {drawer_ = nullptr; });

    shader_.onReload([this]() { invalidate(INVALID_OUTPUT); });
}
    
void TubeRendering::process() {
    if (!drawer_) {
        drawer_ = MeshDrawerFactory::getPtr()->create(mesh_.getData().get());
    }
    if (!drawer_) return;

    if (imageInport_.isConnected()) {
        utilgl::activateTargetAndCopySource(outport_, imageInport_ , ImageType::ColorDepth);
    }
    else {
        utilgl::activateAndClearTarget(outport_, ImageType::ColorDepth);
    }

    shader_.activate();

    utilgl::setShaderUniforms(shader_, radius_);
    utilgl::setShaderUniforms(shader_, *mesh_.getData(), "geometry");
    utilgl::setShaderUniforms(shader_, camera_, "camera");
    utilgl::setShaderUniforms(shader_, light_, "light");

  //  utilgl::PolygonModeState polygon(GL_LINE, 1, 1);
    utilgl::GlBoolState depthTest(GL_DEPTH_TEST, true);

    drawer_->draw();

    shader_.deactivate();
    utilgl::deactivateCurrentTarget();
}

void TubeRendering::initializeResources()
{
    utilgl::addShaderDefines(shader_, light_);
    shader_.rebuild();
}

} // namespace
