/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Peter Steneteg, Timo Ropinski, Erik Sund�n
 *
 *********************************************************************************/

#ifndef IVW_VOLUME_H
#define IVW_VOLUME_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/spatialdata.h>

namespace inviwo {

class VolumeRepresentation;

class IVW_CORE_API Volume : public Data, public StructuredGridEntity<3> {
public:
    Volume(uvec3 dimensions = uvec3(128,128,128), const DataFormatBase* format = DataUINT8::get());
    Volume(const Volume&);
    Volume(VolumeRepresentation*);
    Volume& operator=(const Volume& that);
    virtual Volume* clone() const;
    virtual ~Volume();
    virtual std::string getDataInfo() const;

    uvec3 getDimension() const;
    void setDimension(const uvec3& dim);

    void setOffset(const vec3& offset);
    vec3 getOffset() const;

    mat3 getBasis() const;
    void setBasis(const mat3& basis);

    mat4 getBasisAndOffset() const;
    void setBasisAndOffset(const mat4& mat);

    mat4 getWorldTransform() const;
    void setWorldTransform(const mat4& mat);

protected:
    virtual DataRepresentation* createDefaultRepresentation();
};

} // namespace

#endif // IVW_VOLUME_H
