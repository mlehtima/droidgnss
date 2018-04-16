/*
 * Copyright (C) 2009 The Android Open Source Project
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
 * Authored by: Matti Lehtimäki <matti.lehtimaki@gmail.com>
 */

#include <private.h>

extern "C" {

void DroidAGnssSetCallback(DroidAGnssCallback *callback)
{
    droidAGnssCallbacks = callback;
    if (gnssHal != nullptr) {
        android::sp<IAGnssCallback> aGnssCbIface = new AGnssCallback();
        if (agnssIface != nullptr) {
            agnssIface->setCallback(aGnssCbIface);
        } else {
            ALOGE("Unable to Initialize AGnss interface\n");
        }
    }
}

bool DroidAGnssDataConnClosed()
{
    return agnssIface->dataConnClosed();
}

bool DroidAGnssDataConnFailed()
{
    return agnssIface->dataConnFailed();
}

bool DroidAGnssSetServer(DroidAGnssType type, const char *hostname, int32_t port)
{
    return agnssIface->setServer((IAGnssCallback::AGnssType)type, hostname, port);
}

bool DroidAGnssDataConnOpen(const char *apn, DroidApnIpType apnIpType)
{
    return agnssIface->dataConnOpen(apn, (IAGnss::ApnIpType)apnIpType);
}

void DroidAGnssRilSetCallback(DroidAGnssRilCallback *callback)
{
    droidAGnssRilCallbacks = callback;
}

void DroidAGnssRilSetRefLocation(const DroidAGnssRilRefLocation& agnssReflocation)
{
    agnssRilIface->setRefLocation((const IAGnssRil::AGnssRefLocation &)agnssReflocation);
}

bool DroidAGnssRilSetSetId(DroidAGnssRilSetIDType type, const char *setid)
{
    return agnssRilIface->setSetId((IAGnssRil::SetIDType)type, setid);
}

bool DroidAGnssRilUpdateNetworkState(bool connected, DroidAGnssRilNetworkType type, bool roaming)
{
    return agnssRilIface->updateNetworkState(connected, (IAGnssRil::NetworkType)type, roaming);
}

bool DroidAGnssRilUpdateNetworkAvailability(bool available, const char *apn)
{
    return agnssRilIface->updateNetworkAvailability(available, apn);
}

bool DroidGnssXtraSetCallback(DroidGnssXtraCallback *callback)
{
    if (gnssHal != nullptr) {
        droidGnssXtraCallbacks = callback;
        android::sp<IGnssXtraCallback> gnssXtraCbIface = new GnssXtraCallback();
        if (gnssXtraIface == nullptr) {
            ALOGE("Unable to initialize GNSS Xtra interface\n");
            return false;
        } else {
            bool result = gnssXtraIface->setCallback(gnssXtraCbIface);
            if (!result) {
                gnssXtraIface = nullptr;
                ALOGE("SetCallback for Gnss Xtra Interface fails\n");
                return false;
            }
        }
        return true;
    }
    return false;
}

bool DroidGnssXtraInjectXtraData(const char *xtraData)
{
    return gnssXtraIface->injectXtraData(xtraData);
}

void DroidGnssNiSetCallback(DroidGnssNiCallback *callback)
{
    if (gnssHal != nullptr) {
        droidGnssNiCallbacks = callback;
        android::sp<IGnssNiCallback> gnssNiCbIface = new GnssNiCallback();
        if (gnssNiIface != nullptr) {
            gnssNiIface->setCallback(gnssNiCbIface);
        } else {
            ALOGE("Unable to initialize GNSS NI interface\n");
        }
    }
}

void DroidGnssNiRespond(int32_t notifId, DroidGnssUserResponseType userResponse)
{
    gnssNiIface->respond(notifId, (IGnssNiCallback::GnssUserResponseType)userResponse);
}

DroidGnssDebugData DroidGnssDebugGetDebugData()
{
    DroidGnssDebugData data;
    return data;
}

bool DroidGnssSetCallback(DroidGnssCallback *callback)
{
    droidGnssCallbacks = callback;
    ALOGE("Set GnssCallback\n");
    android::sp<IGnssCallback> gnssCbIface = new GnssCallback();
     /*
     * Fail if the main interface fails to initialize
     */
    if (gnssHal == nullptr) {
        ALOGE("Unable to Initialize GNSS HAL\n");
        return false;
    }

    bool result = gnssHal->setCallback(gnssCbIface);
    if (!result) {
        ALOGE("SetCallback for Gnss Interface fails\n");
        return false;
    }
    return true;
}

bool DroidGnssStart()
{
    return gnssHal->start();
}

bool DroidGnssStop()
{
    return gnssHal->stop();
}

void DroidGnssCleanup()
{
    gnssHal->cleanup();
}

bool DroidGnssInjectTime(int64_t timeMs, int64_t timeReferenceMs, int32_t uncertaintyMs)
{
    return gnssHal->injectTime(timeMs, timeReferenceMs, uncertaintyMs);
}

bool DroidGnssInjectLocation(double latitudeDegrees, double longitudeDegrees, float accuracyMeters)
{
    return gnssHal->injectLocation(latitudeDegrees, longitudeDegrees, accuracyMeters);
}

void DroidGnssDeleteAidingData(DroidGnssAidingData aidingDataFlags)
{
    gnssHal->deleteAidingData((IGnss::GnssAidingData)aidingDataFlags);
}

bool DroidGnssSetPositionMode(DroidGnssPositionMode mode, DroidGnssPositionRecurrence recurrence, uint32_t minIntervalMs, uint32_t preferredAccuracyMeters, uint32_t preferredTimeMs)
{
    return gnssHal->setPositionMode((IGnss::GnssPositionMode)mode, (IGnss::GnssPositionRecurrence)recurrence, minIntervalMs, preferredAccuracyMeters, preferredTimeMs);
}

DroidAGnssRil *DroidGnssGetExtensionAGnssRil()
{
    return &droidAGnssRil;
}

//DroidGnssGeofencing *DroidGnssgetExtensionGnssGeofencing()

DroidAGnss *DroidGnssGetExtensionAGnss()
{
    return &droidAGnss;
}

DroidGnssNi *DroidGnssGetExtensionGnssNi()
{
    return &droidGnssNi;
}

//DroidGnssMeasurement *DroidGnssgetExtensionGnssMeasurement()
//DroidGnssNavigationMessage *DroidGnssgetExtensionGnssNavigationMessage()

DroidGnssXtra *DroidGnssGetExtensionXtra()
{
    return &droidGnssXtra;
}

//DroidGnssConfiguration *DroidGnssgetExtensionGnssConfiguration()

DroidGnssDebug *DroidGnssGetExtensionGnssDebug()
{
    return &droidGnssDebug;
}

//DroidGnssBatching *DroidGnssgetExtensionGnssBatching()

DroidGnss *droid_gnss_get_hal()
{
    droidAGnss.setCallback = &DroidAGnssSetCallback;
    droidAGnss.dataConnClosed = &DroidAGnssDataConnClosed;
    droidAGnss.dataConnFailed = &DroidAGnssDataConnFailed;
    droidAGnss.setServer = &DroidAGnssSetServer;
    droidAGnss.dataConnOpen = &DroidAGnssDataConnOpen;

    droidAGnssRil.setCallback = &DroidAGnssRilSetCallback;
    droidAGnssRil.setRefLocation = &DroidAGnssRilSetRefLocation;
    droidAGnssRil.setSetId = &DroidAGnssRilSetSetId;
    droidAGnssRil.updateNetworkState = &DroidAGnssRilUpdateNetworkState;
    droidAGnssRil.updateNetworkAvailability = &DroidAGnssRilUpdateNetworkAvailability;

    droidGnssNi.setCallback = &DroidGnssNiSetCallback;
    droidGnssNi.respond = &DroidGnssNiRespond;

    droidGnssXtra.setCallback = &DroidGnssXtraSetCallback;
    droidGnssXtra.injectXtraData = &DroidGnssXtraInjectXtraData;

    droidGnssDebug.getDebugData = &DroidGnssDebugGetDebugData;

    droidGnss.setCallback = &DroidGnssSetCallback;
    droidGnss.start = &DroidGnssStart;
    droidGnss.stop = &DroidGnssStop;
    droidGnss.cleanup = &DroidGnssCleanup;
    droidGnss.injectTime = &DroidGnssInjectTime;
    droidGnss.injectLocation = &DroidGnssInjectLocation;
    droidGnss.deleteAidingData = &DroidGnssDeleteAidingData;
    droidGnss.setPositionMode = &DroidGnssSetPositionMode;
    droidGnss.getExtensionAGnss = &DroidGnssGetExtensionAGnss;
    droidGnss.getExtensionAGnssRil = &DroidGnssGetExtensionAGnssRil;
    droidGnss.getExtensionGnssNi = &DroidGnssGetExtensionGnssNi;
    droidGnss.getExtensionXtra = &DroidGnssGetExtensionXtra;
    droidGnss.getExtensionGnssDebug = &DroidGnssGetExtensionGnssDebug;

    gnssHal = IGnss::getService();
    if (gnssHal != nullptr) {
      gnssHalDeathRecipient = new GnssDeathRecipient();
      android::hardware::Return<bool> linked = gnssHal->linkToDeath(
          gnssHalDeathRecipient, /*cookie*/ 0);
        if (!linked.isOk()) {
            ALOGE("Transaction error in linking to GnssHAL death: %s",
                    linked.description().c_str());
        } else if (!linked) {
            ALOGW("Unable to link to GnssHal death notifications");
        } else {
            ALOGE("Link to death notification successful");
        }

        auto gnssXtra = gnssHal->getExtensionXtra();
        if (!gnssXtra.isOk()) {
            ALOGE("Unable to get a handle to Xtra");
        } else {
            gnssXtraIface = gnssXtra;
        }

        auto gnssRil = gnssHal->getExtensionAGnssRil();
        if (!gnssRil.isOk()) {
            ALOGE("Unable to get a handle to AGnssRil");
        } else {
            agnssRilIface = gnssRil;
        }

        auto gnssAgnss = gnssHal->getExtensionAGnss();
        if (!gnssAgnss.isOk()) {
            ALOGE("Unable to get a handle to AGnss");
        } else {
            agnssIface = gnssAgnss;
        }

        auto gnssNavigationMessage = gnssHal->getExtensionGnssNavigationMessage();
        if (!gnssNavigationMessage.isOk()) {
            ALOGE("Unable to get a handle to GnssNavigationMessage");
        } else {
            gnssNavigationMessageIface = gnssNavigationMessage;
        }

        auto gnssMeasurement = gnssHal->getExtensionGnssMeasurement();
        if (!gnssMeasurement.isOk()) {
            ALOGE("Unable to get a handle to GnssMeasurement");
        } else {
            gnssMeasurementIface = gnssMeasurement;
        }

        auto gnssDebug = gnssHal->getExtensionGnssDebug();
        if (!gnssDebug.isOk()) {
            ALOGE("Unable to get a handle to GnssDebug");
        } else {
            gnssDebugIface = gnssDebug;
        }

        auto gnssNi = gnssHal->getExtensionGnssNi();
        if (!gnssNi.isOk()) {
            ALOGE("Unable to get a handle to GnssNi");
        } else {
            gnssNiIface = gnssNi;
        }

        auto gnssConfiguration = gnssHal->getExtensionGnssConfiguration();
        if (!gnssConfiguration.isOk()) {
            ALOGE("Unable to get a handle to GnssConfiguration");
        } else {
            gnssConfigurationIface = gnssConfiguration;
        }

        auto gnssGeofencing = gnssHal->getExtensionGnssGeofencing();
        if (!gnssGeofencing.isOk()) {
            ALOGE("Unable to get a handle to GnssGeofencing");
        } else {
            gnssGeofencingIface = gnssGeofencing;
        }

        auto gnssBatching = gnssHal->getExtensionGnssBatching();
        if (!gnssBatching.isOk()) {
            ALOGE("Unable to get a handle to GnssBatching");
        } else {
            gnssBatchingIface = gnssBatching;
        }

        return &droidGnss;
    } else {
        ALOGE("Unable to get GNSS service\n");
        return nullptr;
    }
}

};
