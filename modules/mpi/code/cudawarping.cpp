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

#include <mpi/code/cudawarping.h>
#include <math.h>

namespace warp {
    void createMapsGPU (cv::cuda::GpuMat const & x_map,
                        cv::cuda::GpuMat const & y_map,
                        cv::cuda::GpuMat & out_x_map,
                        cv::cuda::GpuMat & out_y_map,
                        cv::cuda::GpuMat const & disparity_map,
                        cv::Point2i const & ref_pos,
                        cv::Point2i const & novel_pos) {
    auto distance_x = novel_pos.x - ref_pos.x;
    auto distance_y = novel_pos.y - ref_pos.y;

    cv::cuda::multiply (disparity_map,
                        distance_x,
                        out_x_map,
                        1,
                        out_x_map.type ());

    cv::cuda::multiply (disparity_map,
                        distance_y,
                        out_y_map,
                        1,
                        out_x_map.type ());

    cv::cuda::add (x_map, out_x_map, out_x_map);
    cv::cuda::add (y_map, out_y_map, out_y_map);
    }

    // Operates in place to convert a depth map to disparity
    void convertDepthToDisparity (cv::cuda::GpuMat & depth_map,
                                  inviwo::PerspectiveCamera const & camera,
                                  float baseline) {
        float near = camera.getNearPlaneDist();
        float far = camera.getFarPlaneDist();
        float focal_length = camera.getProjectionMatrix()[0][0];
        float fov = camera.getFovy();

        // TODO can put these calcs in a more efficient manner
        cv::cuda::GpuMat identity_gpu = cv::cuda::GpuMat(
            cv::Mat::eye(depth_map.size(), depth_map.type())
        );
        cv::cuda::subtract(depth_map, 0.5, depth_map, cv::noArray(), depth_map.type());
        cv::cuda::multiply(depth_map, -2 * (far - near), depth_map, 1, depth_map.type());
        cv::cuda::multiply(depth_map, near + far, depth_map, 1, depth_map.type());
        cv::cuda::divide(
            identity_gpu, depth_map, depth_map, double(2 * near * far), 1, depth_map.type()
        );
        cv::cuda::divide(
            identity_gpu, depth_map, depth_map, baseline * focal_length, 1, depth_map.type()
        );
        
        float PI = 3.14159265
        float image_sensor_size = 2 * focal_length * tan((fov / 2) * PI / 180.0);
        cv::cuda::divide(
            image_sensor_size, depth_map, depth_map, depth_map.size().x, 1, depth_map.type()
        );
    }

    void cudaBackWarp  (cv::cuda::GpuMat & depth_map,
                        cv::cuda::GpuMat const & colour_image,
                        cv::cuda::GpuMat & output,
                        inviwo::PerspectiveCamera const & camera,
                        float baseline) {
        // TODO remove timing later on
        start = std::chrono::high_resolution_clock::now ();
        size = colour_image.size();
        type = colour_image.type();

        auto warp_map_1_gpu = cv::cuda::GpuMat (
            size, type);
        auto warp_map_2_gpu = cv::cuda::GpuMat (
            size, type);

        auto pixel_map_x_gpu = cv::cuda::GpuMat (
            size, type);
        auto pixel_map_y_gpu = cv::cuda::GpuMat (
            size, type);

        convertDepthToDisparity (
            depth_map,
            camera,
            baseline);

        for (auto i = 0; i < 64; ++i) {
            auto novel_pos = cv::Point2i {i / 8, i % 8};

            createMapsGPU (
                pixel_map_x_gpu, pixel_map_y_gpu,
                warp_map_1_gpu, warp_map_2_gpu,
                depth_map,
                ref_pos, novel_pos
            );

            cv::cuda::remap (colour_image,
                            output,
                            warp_map_1_gpu,
                            warp_map_2_gpu,
                            cv::INTER_LINEAR,
                            cv::BORDER_REPLICATE);                  
        }
        stop = std::chrono::high_resolution_clock::now ();
        elapsed =
            std::chrono::duration_cast <std::chrono::microseconds> (stop - start);
        std::cout << "Time taken: " << elapsed.count () / 1000.0 << " ms.\n";
        output_color_img_gpu.download (output_color_img);

}