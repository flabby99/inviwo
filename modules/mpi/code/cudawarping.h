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

// OpenCV code
#include <opencv2/cudawarping.hpp>
#include <opencv2/opencv.hpp>
#include <inviwo/core/datastructures/camera.h>

namespace warp {
    void createMapsGPU (cv::cuda::GpuMat const & x_map,
                        cv::cuda::GpuMat const & y_map,
                        cv::cuda::GpuMat & out_x_map,
                        cv::cuda::GpuMat & out_y_map,
                        cv::cuda::GpuMat const & disparity_map,
                        cv::Point2i const & ref_pos,
                        cv::Point2i const & novel_pos);
    void cudaBackWarp  (cv::cuda::GpuMat & depth_map,
                        cv::cuda::GpuMat const & colour_image,
                        cv::cuda::GpuMat & output,
                        inviwo::PerspectiveCamera const & camera,
                        float baseline);
    void convertDepthToDisparity (cv::cuda::GpuMat & depth_map,
                                  inviwo::PerspectiveCamera const & camera,
                                  float baseline);
}