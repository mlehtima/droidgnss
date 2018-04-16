/*
 * Copyright (C) 2014-2015 Jolla Ltd.
 * Copyright (C) 2018 Matti Lehtimäki
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authored by: Mohammed Hassan <mohammed.hassan@jolla.com>
 * Authored by: Matti Lehtimäki <matti.lehtimaki@gmail.com>
 */

#include "droidgnss.h"
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>

extern "C" {

DroidAGnss droidAGnss;
DroidAGnssRil droidAGnssRil;
DroidGnssNi droidGnssNi;
DroidGnssXtra droidGnssXtra;
DroidGnssDebug droidGnssDebug;
DroidGnss droidGnss;

DroidAGnssCallback *droidAGnssCallbacks;
DroidAGnssRilCallback *droidAGnssRilCallbacks;
DroidGnssNiCallback *droidGnssNiCallbacks;
DroidGnssXtraCallback *droidGnssXtraCallbacks;
DroidGnssCallback *droidGnssCallbacks;

void droid_gnss_init()
{
    android::ProcessState::self()->startThreadPool();
}

void droid_gnss_deinit()
{
    android::IPCThreadState::self()->stopProcess(false);
    android::IPCThreadState::self()->joinThreadPool();
}

};
