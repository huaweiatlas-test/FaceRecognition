/**
 * ============================================================================
 *
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1 Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   2 Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   3 Neither the names of the copyright holders nor the names of the
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * ============================================================================
 */
#ifndef ATLASFACEDEMO_FACEDETECTION_H
#define ATLASFACEDEMO_FACEDETECTION_H

#include "hiaiengine/ai_model_manager.h"
#include "hiaiengine/engine.h"
#include "stream_data.h"
#include "yolo.h"
#include <utility>

#define DT_INPUT_SIZE 1
#define DT_OUTPUT_SIZE 12

class FaceDetectionEngine : public hiai::Engine {
public:
    FaceDetectionEngine()
    {}
    ~FaceDetectionEngine();

    HIAI_StatusT Init(const hiai::AIConfig& config, const std::vector<hiai::AIModelDescription>& model_desc);

    HIAI_DEFINE_PROCESS(DT_INPUT_SIZE, DT_OUTPUT_SIZE)

private:
    uint32_t kBatchSize = 1;
    uint32_t kChannel = 0;
    uint32_t kWidth = 0;
    uint32_t kHeight = 0;
    uint32_t kAlignedWidth = 0;
    uint32_t kAlignedHeight = 0;
    uint32_t kInputSize = 0;
    uint32_t maxFaceNumPerFrame = 20;
    std::shared_ptr<hiai::AIModelManager> modelManager;
    std::vector<std::shared_ptr<DeviceStreamData>> inputArgQueue;
    std::pair<std::shared_ptr<uint8_t>, uint32_t> inputDataBuffer;
    std::vector<std::shared_ptr<hiai::IAITensor>> inputTensorVec;
    std::vector<std::shared_ptr<uint8_t>> outputDataBuffer;
    std::vector<std::shared_ptr<hiai::IAITensor>> outputTensorVec;

    CYolo* mYolo = NULL;

    HIAI_StatusT ParamInit(std::map<string, string>& keyValueConfig);
    HIAI_StatusT PreAllocate(
        std::vector<hiai::TensorDimension>& inputTensorDims, std::vector<hiai::TensorDimension>& outputTensorDims);
    HIAI_StatusT DimCheck(
        std::vector<hiai::TensorDimension>& inputTensorDims, std::vector<hiai::TensorDimension>& outputTensorDims);
    HIAI_StatusT FaceSelection(std::vector<DetectInfo>& detectResult);
    HIAI_StatusT PostProcessDetectionYoloV3Tiny(void);
    HIAI_StatusT SelectiveSendData(std::shared_ptr<DeviceStreamData> deviceStreamData);
    double timeComsumed = 0;
    uint64_t runCount = 0;
};

#endif  // ATLASFACEDEMO_FACEDETECTION_H
