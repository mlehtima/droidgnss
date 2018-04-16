/*
 * Copyright (C) 2018 Matti Lehtimäki.
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

#ifndef DROID_GNSS_H
#define DROID_GNSS_H

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum DroidGnssMax {
    SVS_COUNT = 64u, // 64
};

typedef int64_t DroidGnssUtcTime;

enum DroidGnssConstellationType {
    GNSS_CONSTELLATION_UNKNOWN = 0,
    GNSS_CONSTELLATION_GPS = 1,
    GNSS_CONSTELLATION_SBAS = 2,
    GNSS_CONSTELLATION_GLONASS = 3,
    GNSS_CONSTELLATION_QZSS = 4,
    GNSS_CONSTELLATION_BEIDOU = 5,
    GNSS_CONSTELLATION_GALILEO = 6,
};

enum DroidGnssLocationFlags {
    GNSS_LOCATION_HAS_LAT_LONG = 1, // 0x0001
    GNSS_LOCATION_HAS_ALTITUDE = 2, // 0x0002
    GNSS_LOCATION_HAS_SPEED = 4, // 0x0004
    GNSS_LOCATION_HAS_BEARING = 8, // 0x0008
    GNSS_LOCATION_HAS_HORIZONTAL_ACCURACY = 16, // 0x0010
    GNSS_LOCATION_HAS_VERTICAL_ACCURACY = 32, // 0x0020
    GNSS_LOCATION_HAS_SPEED_ACCURACY = 64, // 0x0040
    GNSS_LOCATION_HAS_BEARING_ACCURACY = 128, // 0x0080
};

struct DroidGnssLocation {
    uint16_t gnssLocationFlags;
    double latitudeDegrees;
    double longitudeDegrees;
    double altitudeMeters;
    float speedMetersPerSec;
    float bearingDegrees;
    float horizontalAccuracyMeters;
    float verticalAccuracyMeters;
    float speedAccuracyMetersPerSecond;
    float bearingAccuracyDegrees;
    int64_t timestamp;
};

enum DroidAGnssType {
    AGNSS_TYPE_SUPL = 1,
    AGNSS_TYPE_C2K = 2,
};

enum DroidAGnssStatusValue {
    GNSS_REQUEST_AGNSS_DATA_CONN = 1,
    GNSS_RELEASE_AGNSS_DATA_CONN = 2,
    GNSS_AGNSS_DATA_CONNECTED = 3,
    GNSS_AGNSS_DATA_CONN_DONE = 4,
    GNSS_AGNSS_DATA_CONN_FAILED = 5,
};

struct DroidAGnssStatusIpV4 {
    DroidAGnssType type;
    DroidAGnssStatusValue status;
    uint32_t ipV4Addr;
};

struct DroidAGnssStatusIpV6 {
    DroidAGnssType type;
    DroidAGnssStatusValue status;
    uint8_t ipV6Addr[16];
};

struct DroidAGnssCallback {
    void (*agnssStatusIpV4Cb)(const DroidAGnssStatusIpV4& status);
    void (*agnssStatusIpV6Cb)(const DroidAGnssStatusIpV6& status);
};

enum DroidApnIpType {
    APN_IP_INVALID = 0,
    APN_IP_IPV4 = 1,
    APN_IP_IPV6 = 2,
    APN_IP_IPV4V6 = 3,
};

struct DroidAGnss {
    void (*setCallback)(DroidAGnssCallback *callback);
    bool (*dataConnClosed)();
    bool (*dataConnFailed)();
    bool (*setServer)(DroidAGnssType type, const char *hostname, int32_t port);
    bool (*dataConnOpen)(const char *apn, DroidApnIpType apnIpType);
};

enum DroidAGnssID {
    AGNSS_ID_TYPE_IMSI = 1u, // (1 << 0L)
    AGNSS_ID_TYPE_MSISDN = 2u, // (1 << 1L)
};

struct DroidAGnssRilCallback {
    void (*requestSetIdCb)(uint32_t setIdflag);
    void (*requestRefLocCb)();
};

enum DroidAGnssRilSetIDType {
    AGNSS_SETID_TYPE_NONE = 0,
    AGNSS_SETID_TYPE_IMSI = 1,
    AGNSS_SETID_TYPE_MSISDM = 2,
};

enum DroidAGnssRilNetworkType {
    AGNSS_RIL_NETWORK_TYPE_MOBILE = 0,
    AGNSS_RIL_NETWORK_TYPE_WIFI = 1,
    AGNSS_RIL_NETWORK_TYPE_MMS = 2,
    AGNSS_RIL_NETWORK_TYPE_SUPL = 3,
    AGNSS_RIL_NETWORK_TYPE_DUN = 4,
    AGNSS_RIL_NETWORK_TYPE_HIPRI = 5,
    AGNSS_RIL_NETWORK_TYPE_WIMAX = 6,
};

enum DroidAGnssRilRefLocationType {
    AGNSS_REF_LOCATION_TYPE_GSM_CELLID = 1,
    AGNSS_REF_LOCATION_TYPE_UMTS_CELLID = 2,
    AGNSS_REF_LOCATION_TYPE_LTE_CELLID = 4,
};

struct DroidAGnssRilRefLocationCellID {
    DroidAGnssRilRefLocationType type;
    uint16_t mcc;
    uint16_t mnc;
    uint16_t lac;
    uint32_t cid;
    uint16_t tac;
    uint16_t pcid;
};

struct DroidAGnssRilRefLocation {
    DroidAGnssRilRefLocationType type;
    DroidAGnssRilRefLocationCellID cellID;
};

struct DroidAGnssRil {
    void (*setCallback)(DroidAGnssRilCallback *callback);
    void (*setRefLocation)(const DroidAGnssRilRefLocation& agnssReflocation);
    bool (*setSetId)(DroidAGnssRilSetIDType type, const char *setid);
    bool (*updateNetworkState)(bool connected, DroidAGnssRilNetworkType type, bool roaming);
    bool (*updateNetworkAvailability)(bool available, const char *apn);
};

struct DroidGnssXtraCallback {
    void (*downloadRequestCb)();
};

struct DroidGnssXtra {
    bool (*setCallback)(DroidGnssXtraCallback *callback);
    bool (*injectXtraData)(const char *xtraData);
};

enum DroidGnssNiType {
    GNSS_NI_TYPE_VOICE = 1,
    GNSS_NI_TYPE_UMTS_SUPL = 2,
    GNSS_NI_TYPE_UMTS_CTRL_PLANE = 3,
    GNSS_NI_TYPE_EMERGENCY_SUPL = 4,
};

enum DroidGnssNiNotifyFlags {
    GNSS_NI_NEED_NOTIFY = 1u, // 0x0001
    GNSS_NI_NEED_VERIFY = 2u, // 0x0002
    GNSS_NI_PRIVACY_OVERRIDE = 4u, // 0x0004
};

enum DroidGnssUserResponseType {
    GNSS_NI_RESPONSE_ACCEPT = 1,
    GNSS_NI_RESPONSE_DENY = 2,
    GNSS_NI_RESPONSE_NORESP = 3,
};

enum DroidGnssNiEncodingType {
    GNSS_NI_ENC_NONE = 0,
    GNSS_NI_ENC_SUPL_GSM_DEFAULT = 1,
    GNSS_NI_ENC_SUPL_UTF8 = 2,
    GNSS_NI_ENC_SUPL_UCS2 = 3,
    GNSS_NI_ENC_UNKNOWN = -1, // (-1)
};

struct DroidGnssNiNotification {
    int32_t notificationId;
    DroidGnssNiType niType;
    uint32_t notifyFlags;
    uint32_t timeoutSec;
    DroidGnssUserResponseType defaultResponse;
    char *requestorId;
    char *notificationMessage;
    DroidGnssNiEncodingType requestorIdEncoding;
    DroidGnssNiEncodingType notificationIdEncoding;
};

struct DroidGnssNiCallback {
    void (*niNotifyCb)(const DroidGnssNiNotification& notification);
};

struct DroidGnssNi {
    void (*setCallback)(DroidGnssNiCallback *callback);
    void (*respond)(int32_t notifId, DroidGnssUserResponseType userResponse);
};

enum DroidGnssDebugSatelliteEphemerisType {
    GNSS_DEBUG_SATELLITE_TYPE_EPHEMERIS = 0,
    GNSS_DEBUG_SATELLITE_TYPE_ALMANAC_ONLY = 1,
    GNSS_DEBUG_SATELLITE_TYPE_NOT_AVAILABLE = 2,
};

enum DroidGnssDebugSatelliteEphemerisSource {
    GNSS_DEBUG_SATELLITE_SOURCE_DEMODULATED = 0,
    GNSS_DEBUG_SATELLITE_SOURCE_SUPL_PROVIDED = 1,
    GNSS_DEBUG_SATELLITE_SOURCE_OTHER_SERVER_PROVIDED = 2,
    GNSS_DEBUG_SATELLITE_SOURCE_OTHER = 3,
};

enum DroidGnssDebugSatelliteEphemerisHealth {
    GNSS_DEBUG_SATELLITE_HEALTH_GOOD = 0,
    GNSS_DEBUG_SATELLITE_HEALTH_BAD = 1,
    GNSS_DEBUG_SATELLITE_HEALTH_UNKNOWN = 2,
};

struct DroidGnssDebugPositionDebug {
    bool valid;
    double latitudeDegrees;
    double longitudeDegrees;
    float altitudeMeters;
    float speedMetersPerSec;
    float bearingDegrees;
    double horizontalAccuracyMeters;
    double verticalAccuracyMeters;
    double speedAccuracyMetersPerSecond;
    double bearingAccuracyDegrees;
    float ageSeconds;
};

struct DroidGnssDebugTimeDebug {
    int64_t timeEstimate;
    float timeUncertaintyNs;
    float frequencyUncertaintyNsPerSec;
};

struct DroidGnssDebugSatelliteData {
    int16_t svid;
    DroidGnssConstellationType constellation;
    DroidGnssDebugSatelliteEphemerisType ephemerisType;
    DroidGnssDebugSatelliteEphemerisSource ephemerisSource;
    DroidGnssDebugSatelliteEphemerisHealth ephemerisHealth;
    float ephemerisAgeSeconds;
    bool serverPredictionIsAvailable;
    float serverPredictionAgeSeconds;
};

struct DroidGnssDebugData {
    DroidGnssDebugPositionDebug position;
    DroidGnssDebugTimeDebug time;
    DroidGnssDebugSatelliteData *satelliteDataArray;
};

struct DroidGnssDebug {
    DroidGnssDebugData (*getDebugData)();
};

enum DroidGnssCapabilities {
    GNSS_CAPABILITY_SCHEDULING = 1u, // (1 << 0)
    GNSS_CAPABILITY_MSB = 2u, // (1 << 1)
    GNSS_CAPABILITY_MSA = 4u, // (1 << 2)
    GNSS_CAPABILITY_SINGLE_SHOT = 8u, // (1 << 3)
    GNSS_CAPABILITY_ON_DEMAND_TIME = 16u, // (1 << 4)
    GNSS_CAPABILITY_GEOFENCING = 32u, // (1 << 5)
    GNSS_CAPABILITY_MEASUREMENTS = 64u, // (1 << 6)
    GNSS_CAPABILITY_NAV_MESSAGES = 128u, // (1 << 7)
};

enum DroidGnssStatusValue {
    GNSS_STATUS_NONE = 0,
    GNSS_STATUS_SESSION_BEGIN = 1,
    GNSS_STATUS_SESSION_END = 2,
    GNSS_STATUS_ENGINE_ON = 3,
    GNSS_STATUS_ENGINE_OFF = 4,
};

enum DroidGnssSvFlags {
    GNSS_SV_FLAGS_NONE = 0,
    GNSS_SV_FLAGS_HAS_EPHEMERIS_DATA = 1, // (1 << 0)
    GNSS_SV_FLAGS_HAS_ALMANAC_DATA = 2, // (1 << 1)
    GNSS_SV_FLAGS_USED_IN_FIX = 4, // (1 << 2)
    GNSS_SV_FLAGS_HAS_CARRIER_FREQUENCY = 8, // (1 << 3)
};

struct DroidGnssSvInfo {
    int16_t svid;
    DroidGnssConstellationType constellation;
    float cN0Dbhz;
    float elevationDegrees;
    float azimuthDegrees;
    float carrierFrequencyHz;
    uint8_t svFlag;
};

struct DroidGnssSvStatus {
    uint32_t numSvs;
    DroidGnssSvInfo gnssSvList[64];
};

struct DroidGnssSystemInfo {
    uint16_t yearOfHw;
};

struct DroidGnssCallback {
    void (*gnssLocationCb)(const DroidGnssLocation& location);
    void (*gnssStatusCb)(const DroidGnssStatusValue status);
    void (*gnssSvStatusCb)(const DroidGnssSvStatus& svInfo);
    void (*gnssNmeaCb)(int64_t timestamp, const char *nmea, int length);
    void (*gnssSetCapabilitesCb)(uint32_t capabilities);
    void (*gnssAcquireWakelockCb)();
    void (*gnssReleaseWakelockCb)();
    void (*gnssRequestTimeCb)();
    void (*gnssSetSystemInfoCb)(const DroidGnssSystemInfo& info);
};

enum DroidGnssPositionMode {
    GNSS_POSITION_MODE_STANDALONE = 0,
    GNSS_POSITION_MODE_MS_BASED = 1,
    GNSS_POSITION_MODE_MS_ASSISTED = 2,
};

enum DroidGnssPositionRecurrence {
    GNSS_POSITION_RECURRENCE_PERIODIC = 0u, // 0
    GNSS_POSITION_RECURRENCE_SINGLE = 1u, // 1
};

enum DroidGnssAidingData {
    GNSS_DELETE_EPHEMERIS = 1, // 0x0001
    GNSS_DELETE_ALMANAC = 2, // 0x0002
    GNSS_DELETE_POSITION = 4, // 0x0004
    GNSS_DELETE_TIME = 8, // 0x0008
    GNSS_DELETE_IONO = 16, // 0x0010
    GNSS_DELETE_UTC = 32, // 0x0020
    GNSS_DELETE_HEALTH = 64, // 0x0040
    GNSS_DELETE_SVDIR = 128, // 0x0080
    GNSS_DELETE_SVSTEER = 256, // 0x0100
    GNSS_DELETE_SADATA = 512, // 0x0200
    GNSS_DELETE_RTI = 1024, // 0x0400
    GNSS_DELETE_CELLDB_INFO = 32768, // 0x8000
    GNSS_DELETE_ALL = 65535, // 0xFFFF
};

struct DroidGnss {
    bool (*setCallback)(DroidGnssCallback *callback);
    bool (*start)();
    bool (*stop)();
    void (*cleanup)();
    bool (*injectTime)(int64_t timeMs, int64_t timeReferenceMs, int32_t uncertaintyMs);
    bool (*injectLocation)(double latitudeDegrees, double longitudeDegrees, float accuracyMeters);
    void (*deleteAidingData)(DroidGnssAidingData aidingDataFlags);
    bool (*setPositionMode)(DroidGnssPositionMode mode, DroidGnssPositionRecurrence recurrence, uint32_t minIntervalMs, uint32_t preferredAccuracyMeters, uint32_t preferredTimeMs);
    DroidAGnssRil *(*getExtensionAGnssRil)();
//    DroidGnssGeofencing *getExtensionGnssGeofencing();
    DroidAGnss *(*getExtensionAGnss)();
    DroidGnssNi *(*getExtensionGnssNi)();
//    DroidGnssMeasurement *getExtensionGnssMeasurement();
//    DroidGnssNavigationMessage *getExtensionGnssNavigationMessage();
    DroidGnssXtra *(*getExtensionXtra)();
//    DroidGnssConfiguration *getExtensionGnssConfiguration();
    DroidGnssDebug *(*getExtensionGnssDebug)();
//    DroidGnssBatching *getExtensionGnssBatching();
};

/* droidgnss.cpp */
void droid_gnss_init();
void droid_gnss_deinit();

/* private.h */
DroidGnss *droid_gnss_get_hal();

void DroidAGnssSetCallback(DroidAGnssCallback *callback);
bool DroidAGnssDataConnClosed();
bool DroidAGnssDataConnFailed();
bool DroidAGnssSetServer(DroidAGnssType type, const char *hostname, int32_t port);
bool DroidAGnssDataConnOpen(const char *apn, DroidApnIpType apnIpType);

void DroidAGnssRilSetCallback(DroidAGnssRilCallback *callback);
void DroidAGnssRilSetRefLocation(const DroidAGnssRilRefLocation& agnssReflocation);
bool DroidAGnssRilSetSetId(DroidAGnssRilSetIDType type, const char *setid);
bool DroidAGnssRilUpdateNetworkState(bool connected, DroidAGnssRilNetworkType type, bool roaming);
bool DroidAGnssRilUpdateNetworkAvailability(bool available, const char *apn);

bool DroidGnssXtraSetCallback(DroidGnssXtraCallback *callback);
bool DroidGnssXtraInjectXtraData(const char *xtraData);

void DroidGnssNiSetCallback(DroidGnssNiCallback *callback);
void DroidGnssNiRespond(int32_t notifId, DroidGnssUserResponseType userResponse);

DroidGnssDebugData DroidGnssDebugGetDebugData();

bool DroidGnssSetCallback(DroidGnssCallback *callback);
bool DroidGnssStart();
bool DroidGnssStop();
void DroidGnssCleanup();
bool DroidGnssInjectTime(int64_t timeMs, int64_t timeReferenceMs, int32_t uncertaintyMs);
bool DroidGnssInjectLocation(double latitudeDegrees, double longitudeDegrees, float accuracyMeters);
void DroidGnssDeleteAidingData(DroidGnssAidingData aidingDataFlags);
bool DroidGnssSetPositionMode(DroidGnssPositionMode mode, DroidGnssPositionRecurrence recurrence, uint32_t minIntervalMs, uint32_t preferredAccuracyMeters, uint32_t preferredTimeMs);
DroidAGnss *DroidGnssGetExtensionAGnss();
DroidAGnssRil *DroidGnssGetExtensionAGnssRil();
DroidGnssNi *DroidGnssGetExtensionGnssNi();
DroidGnssXtra *DroidGnssGetExtensionXtra();
DroidGnssDebug *DroidGnssGetExtensionGnssDebug();

extern DroidAGnss droidAGnss;
extern DroidAGnssRil droidAGnssRil;
extern DroidGnssNi droidGnssNi;
extern DroidGnssXtra droidGnssXtra;
extern DroidGnssDebug droidGnssDebug;
extern DroidGnss droidGnss;

extern DroidAGnssCallback *droidAGnssCallbacks;
extern DroidAGnssRilCallback *droidAGnssRilCallbacks;
extern DroidGnssNiCallback *droidGnssNiCallbacks;
extern DroidGnssXtraCallback *droidGnssXtraCallbacks;
extern DroidGnssCallback *droidGnssCallbacks;

#ifdef __cplusplus
};
#endif

#endif /* DROID_SENSORS_H */
