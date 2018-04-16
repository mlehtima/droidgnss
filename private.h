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

#ifndef DROID_GNSS_PRIVATE_H
#define DROID_GNSS_PRIVATE_H

#include <droidgnss.h>
#include <android/hardware/gnss/1.0/IGnss.h>
#include "utils/Log.h"

using android::hardware::gnss::V1_0::IAGnss;
using android::hardware::gnss::V1_0::IAGnssCallback;
using android::hardware::gnss::V1_0::IAGnssCallback;
using android::hardware::gnss::V1_0::IAGnssRil;
using android::hardware::gnss::V1_0::IAGnssRilCallback;
using android::hardware::gnss::V1_0::IGnss;
using android::hardware::gnss::V1_0::IGnssBatching;
using android::hardware::gnss::V1_0::IGnssBatchingCallback;
using android::hardware::gnss::V1_0::IGnssCallback;
using android::hardware::gnss::V1_0::IGnssConfiguration;
using android::hardware::gnss::V1_0::IGnssDebug;
using android::hardware::gnss::V1_0::IGnssGeofenceCallback;
using android::hardware::gnss::V1_0::IGnssGeofencing;
using android::hardware::gnss::V1_0::IGnssMeasurement;
using android::hardware::gnss::V1_0::IGnssMeasurementCallback;
using android::hardware::gnss::V1_0::IGnssNavigationMessage;
using android::hardware::gnss::V1_0::IGnssNavigationMessageCallback;
using android::hardware::gnss::V1_0::IGnssNi;
using android::hardware::gnss::V1_0::IGnssNiCallback;
using android::hardware::gnss::V1_0::IGnssXtra;
using android::hardware::gnss::V1_0::IGnssXtraCallback;

struct GnssDeathRecipient : virtual public android::hardware::hidl_death_recipient
{
    // hidl_death_recipient interface
    virtual void serviceDied(uint64_t /*cookie*/, const android::wp<android::hidl::base::V1_0::IBase>& /*who*/) override {
      // TODO(gomo): implement a better death recovery mechanism without
      // crashing system server process as described in go//treble-gnss-death
      LOG_ALWAYS_FATAL("Abort due to IGNSS hidl service failure,"
            " restarting system server");
    }
};

android::sp<GnssDeathRecipient> gnssHalDeathRecipient = nullptr;
android::sp<IGnss> gnssHal = nullptr;
android::sp<IGnssXtra> gnssXtraIface = nullptr;
android::sp<IAGnssRil> agnssRilIface = nullptr;
android::sp<IGnssGeofencing> gnssGeofencingIface = nullptr;
android::sp<IAGnss> agnssIface = nullptr;
android::sp<IGnssBatching> gnssBatchingIface = nullptr;
android::sp<IGnssDebug> gnssDebugIface = nullptr;
android::sp<IGnssConfiguration> gnssConfigurationIface = nullptr;
android::sp<IGnssNi> gnssNiIface = nullptr;
android::sp<IGnssMeasurement> gnssMeasurementIface = nullptr;
android::sp<IGnssNavigationMessage> gnssNavigationMessageIface = nullptr;


struct GnssCallback : public IGnssCallback {
    android::hardware::Return<void> gnssLocationCb(
          const android::hardware::gnss::V1_0::GnssLocation& location) override;
    android::hardware::Return<void> gnssStatusCb(const IGnssCallback::GnssStatusValue status) override;
    android::hardware::Return<void> gnssSvStatusCb(const IGnssCallback::GnssSvStatus& svStatus) override;
    android::hardware::Return<void> gnssNmeaCb(int64_t timestamp, const android::hardware::hidl_string& nmea) override;
    android::hardware::Return<void> gnssSetCapabilitesCb(uint32_t capabilities) override;
    android::hardware::Return<void> gnssAcquireWakelockCb() override;
    android::hardware::Return<void> gnssReleaseWakelockCb() override;
    android::hardware::Return<void> gnssRequestTimeCb() override;
    android::hardware::Return<void> gnssSetSystemInfoCb(const IGnssCallback::GnssSystemInfo& info) override;

    static GnssSvInfo sGnssSvList[static_cast<uint32_t>(
            android::hardware::gnss::V1_0::GnssMax::SVS_COUNT)];
    static size_t sGnssSvListSize;

    static const char* sNmeaString;
    static size_t sNmeaStringLength;
};

IGnssCallback::GnssSvInfo GnssCallback::sGnssSvList[static_cast<uint32_t>(
        android::hardware::gnss::V1_0::GnssMax::SVS_COUNT)];
const char* GnssCallback::sNmeaString = nullptr;
size_t GnssCallback::sNmeaStringLength = 0;
size_t GnssCallback::sGnssSvListSize = 0;

android::hardware::Return<void> GnssCallback::gnssLocationCb(
        const ::android::hardware::gnss::V1_0::GnssLocation& location) {
//    ALOGW("%s\n", __func__);
    DroidGnssLocation loc;
    memcpy(&loc, &location, sizeof(DroidGnssLocation));
    droidGnssCallbacks->gnssLocationCb(loc);
    return android::hardware::Void();
}

android::hardware::Return<void> GnssCallback::gnssStatusCb(const IGnssCallback::GnssStatusValue status) {
//    ALOGW("%s\n", __func__);
    DroidGnssStatusValue stat = static_cast<DroidGnssStatusValue>((int)(uint8_t)status);
    droidGnssCallbacks->gnssStatusCb(stat);
    return android::hardware::Void();
}

android::hardware::Return<void> GnssCallback::gnssSvStatusCb(const IGnssCallback::GnssSvStatus& svStatus) {
//    ALOGW("%s\n", __func__);
    DroidGnssSvStatus svStat;
    memcpy(&svStat, &svStatus, sizeof(DroidGnssSvStatus));
    droidGnssCallbacks->gnssSvStatusCb(svStat);
    return android::hardware::Void();
}

android::hardware::Return<void> GnssCallback::gnssNmeaCb(
    int64_t timestamp, const ::android::hardware::hidl_string& nmea) {
//    ALOGW("%s\n", __func__);
    droidGnssCallbacks->gnssNmeaCb(timestamp, nmea.c_str(), nmea.size());
    return android::hardware::Void();
}

android::hardware::Return<void> GnssCallback::gnssSetCapabilitesCb(uint32_t capabilities) {
//    ALOGD("%s: %du\n", __func__, capabilities);
    droidGnssCallbacks->gnssSetCapabilitesCb(capabilities);
    return android::hardware::Void();
}

android::hardware::Return<void> GnssCallback::gnssAcquireWakelockCb() {
//    acquire_wake_lock(PARTIAL_WAKE_LOCK, WAKE_LOCK_NAME);
//    ALOGW("%s\n", __func__);
    droidGnssCallbacks->gnssAcquireWakelockCb();
    return android::hardware::Void();
}

android::hardware::Return<void> GnssCallback::gnssReleaseWakelockCb() {
//    release_wake_lock(WAKE_LOCK_NAME);
//    ALOGW("%s\n", __func__);
    droidGnssCallbacks->gnssReleaseWakelockCb();
    return android::hardware::Void();
}

android::hardware::Return<void> GnssCallback::gnssRequestTimeCb() {
//    ALOGW("%s\n", __func__);
    droidGnssCallbacks->gnssRequestTimeCb();
    return android::hardware::Void();
}

android::hardware::Return<void> GnssCallback::gnssSetSystemInfoCb(const IGnssCallback::GnssSystemInfo& info) {
    ALOGW("%s: yearOfHw=%d\n", __func__, info.yearOfHw);
    DroidGnssSystemInfo sysInfo;
    memcpy(&sysInfo, &info, sizeof(DroidGnssSystemInfo));
    droidGnssCallbacks->gnssSetSystemInfoCb(sysInfo);
    return android::hardware::Void();
}

struct GnssXtraCallback : public IGnssXtraCallback {
    android::hardware::Return<void> downloadRequestCb() override;
};

/*
 * GnssXtraCallback class implements the callback methods for the IGnssXtra
 * interface.
 */
android::hardware::Return<void> GnssXtraCallback::downloadRequestCb() {
//    ALOGW("%s\n", __func__);
    droidGnssXtraCallbacks->downloadRequestCb();
    return android::hardware::Void();
}

/*
 * GnssGeofenceCallback class implements the callback methods for the
 * IGnssGeofence interface.
 */
struct GnssGeofenceCallback : public IGnssGeofenceCallback {
    // Methods from ::android::hardware::gps::V1_0::IGnssGeofenceCallback follow.
    android::hardware::Return<void> gnssGeofenceTransitionCb(
            int32_t geofenceId,
            const android::hardware::gnss::V1_0::GnssLocation& location,
            GeofenceTransition transition,
            android::hardware::gnss::V1_0::GnssUtcTime timestamp) override;
    android::hardware::Return<void> gnssGeofenceStatusCb(
            GeofenceAvailability status,
            const android::hardware::gnss::V1_0::GnssLocation& location) override;
    android::hardware::Return<void> gnssGeofenceAddCb(int32_t geofenceId,
                                   GeofenceStatus status) override;
    android::hardware::Return<void> gnssGeofenceRemoveCb(int32_t geofenceId,
                                      GeofenceStatus status) override;
    android::hardware::Return<void> gnssGeofencePauseCb(int32_t geofenceId,
                                     GeofenceStatus status) override;
    android::hardware::Return<void> gnssGeofenceResumeCb(int32_t geofenceId,
                                      GeofenceStatus status) override;
};

android::hardware::Return<void> GnssGeofenceCallback::gnssGeofenceTransitionCb(
        int32_t geofenceId,
        const android::hardware::gnss::V1_0::GnssLocation& location,
        GeofenceTransition transition,
        android::hardware::gnss::V1_0::GnssUtcTime timestamp) {
    (void)geofenceId;
    (void)location;
    (void)transition;
    (void)timestamp;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

android::hardware::Return<void> GnssGeofenceCallback::gnssGeofenceStatusCb(
        GeofenceAvailability status,
        const android::hardware::gnss::V1_0::GnssLocation& location) {
    (void)status;
    (void)location;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

android::hardware::Return<void> GnssGeofenceCallback::gnssGeofenceAddCb(int32_t geofenceId,
                                                    GeofenceStatus status) {
    (void)geofenceId;
    (void)status;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

android::hardware::Return<void> GnssGeofenceCallback::gnssGeofenceRemoveCb(int32_t geofenceId,
                                                       GeofenceStatus status) {
    (void)geofenceId;
    (void)status;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

android::hardware::Return<void> GnssGeofenceCallback::gnssGeofencePauseCb(int32_t geofenceId,
                                                      GeofenceStatus status) {
    (void)geofenceId;
    (void)status;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

android::hardware::Return<void> GnssGeofenceCallback::gnssGeofenceResumeCb(int32_t geofenceId,
                                                       GeofenceStatus status) {
    (void)geofenceId;
    (void)status;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

/*
 * GnssNavigationMessageCallback interface implements the callback methods
 * required by the IGnssNavigationMessage interface.
 */
struct GnssNavigationMessageCallback : public IGnssNavigationMessageCallback {
  /*
   * Methods from ::android::hardware::gps::V1_0::IGnssNavigationMessageCallback
   * follow.
   */
  android::hardware::Return<void> gnssNavigationMessageCb(
          const IGnssNavigationMessageCallback::GnssNavigationMessage& message) override;
};

android::hardware::Return<void> GnssNavigationMessageCallback::gnssNavigationMessageCb(
        const IGnssNavigationMessageCallback::GnssNavigationMessage& message) {

    (void)message;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

/*
 * GnssMeasurementCallback implements the callback methods required for the
 * GnssMeasurement interface.
 */
struct GnssMeasurementCallback : public IGnssMeasurementCallback {
    android::hardware::Return<void> GnssMeasurementCb(const IGnssMeasurementCallback::GnssData& data);
};


android::hardware::Return<void> GnssMeasurementCallback::GnssMeasurementCb(
        const IGnssMeasurementCallback::GnssData& data) {

    (void)data;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

struct GnssNiCallback : public IGnssNiCallback {
    android::hardware::Return<void> niNotifyCb(const IGnssNiCallback::GnssNiNotification& notification)
            override;
};

android::hardware::Return<void> GnssNiCallback::niNotifyCb(
        const IGnssNiCallback::GnssNiNotification& notification) {
    (void)notification;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

/*
 * AGnssCallback implements callback methods required by the IAGnss interface.
 */
struct AGnssCallback : public IAGnssCallback {
    // Methods from ::android::hardware::gps::V1_0::IAGnssCallback follow.
    android::hardware::Return<void> agnssStatusIpV6Cb(
      const IAGnssCallback::AGnssStatusIpV6& agps_status) override;

    android::hardware::Return<void> agnssStatusIpV4Cb(
      const IAGnssCallback::AGnssStatusIpV4& agps_status) override;
};

android::hardware::Return<void> AGnssCallback::agnssStatusIpV6Cb(
        const IAGnssCallback::AGnssStatusIpV6& agps_status) {
    (void)agps_status;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

android::hardware::Return<void> AGnssCallback::agnssStatusIpV4Cb(
        const IAGnssCallback::AGnssStatusIpV4& agps_status) {
    (void)agps_status;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

/*
 * AGnssRilCallback implements the callback methods required by the AGnssRil
 * interface.
 */
struct AGnssRilCallback : IAGnssRilCallback {
    android::hardware::Return<void> requestSetIdCb(uint32_t setIdFlag) override;
    android::hardware::Return<void> requestRefLocCb() override;
};

android::hardware::Return<void> AGnssRilCallback::requestSetIdCb(uint32_t setIdFlag) {
    (void)setIdFlag;
    ALOGW("%s\n", __func__);
    droidAGnssRilCallbacks->requestSetIdCb(setIdFlag);
    return android::hardware::Void();
}

android::hardware::Return<void> AGnssRilCallback::requestRefLocCb() {
    ALOGW("%s\n", __func__);
    droidAGnssRilCallbacks->requestRefLocCb();
    return android::hardware::Void();
}

/*
 * GnssBatchingCallback interface implements the callback methods
 * required by the IGnssBatching interface.
 */
struct GnssBatchingCallback : public IGnssBatchingCallback {
    /*
    * Methods from ::android::hardware::gps::V1_0::IGnssBatchingCallback
    * follow.
    */
    android::hardware::Return<void> gnssLocationBatchCb(
        const ::android::hardware::hidl_vec<android::hardware::gnss::V1_0::GnssLocation> & locations)
        override;
};

android::hardware::Return<void> GnssBatchingCallback::gnssLocationBatchCb(
        const ::android::hardware::hidl_vec<android::hardware::gnss::V1_0::GnssLocation> & locations) {
    (void)locations;
    ALOGW("%s\n", __func__);

    return android::hardware::Void();
}

#endif /* DROID_GNSS_PRIVATE_H */
