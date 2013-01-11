/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
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
 */

#ifndef LOCKEDDATA_H
#define LOCKEDDATA_H

#include <QtCore/QString>
#include "AbstractLockable.h"

/**
 * @file LockedData.h
 */

namespace pelican {

/**
 * @ingroup c_server
 *
 * @class LockedData
 *
 * @brief
 * A container that automatically locks and unlocks any
 * AbstractLockable object added to it.
 *
 * @details
 * On destruction the AbstractLockable read lock count will be less by one.
 */
class LockedData
{
    public:
        /// Constructs a new LockedData object.
        LockedData(const QString& name, AbstractLockable* data = 0);

        /// Destroys the LockedData object.
        ~LockedData();

        /// Copy constructor also locks the data also.
        LockedData(const LockedData& data);

        /// Returns true if the object contains valid data.
        bool isValid() const {return _data ? _data->isValid() : false;}

        /// Add a data object.
        void setData(AbstractLockable*);

        /// Return the locked object.
        AbstractLockable* object() const {return _data;}

        /// Return the name of the data.
        QString name() const {return _name;}

    private:
        AbstractLockable* _data;
        QString _name;
};

} // namespace pelican

#endif // LOCKEDDATA_H
