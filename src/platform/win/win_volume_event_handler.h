#pragma once

#include <endpointvolume.h>

#include <QtCore/QObject>

class WinVolumeEventHandler : public QObject, public IAudioEndpointVolumeCallback {
    Q_OBJECT
public:
    WinVolumeEventHandler(const GUID& localEventGuid);

    IFACEMETHODIMP QueryInterface(REFIID iid, void** object) override;

    IFACEMETHODIMP_(ULONG) AddRef() override;

    IFACEMETHODIMP_(ULONG) Release() override;

    IFACEMETHODIMP OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA data) override;

signals:
    void notificationReceived();

private:
    ULONG m_refCount = 1;
    GUID m_localEventGuid;
};
