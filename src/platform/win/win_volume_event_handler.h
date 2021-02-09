#pragma once

#include <endpointvolume.h>
#include <mmdeviceapi.h>

#include <QtCore/QObject>

class WinVolumeEventHandler : public QObject, public IAudioEndpointVolumeCallback, public IMMNotificationClient {
    Q_OBJECT
public:
    WinVolumeEventHandler(const GUID& localEventGuid);

    /* IUnknown. */

    IFACEMETHODIMP QueryInterface(REFIID iid, void** object) override;

    IFACEMETHODIMP_(ULONG) AddRef() override;

    IFACEMETHODIMP_(ULONG) Release() override;

    /* IAudioEndpointVolumeCallback. */

    IFACEMETHODIMP OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA data) override;

    /* IMMNotificationClient. */

    IFACEMETHODIMP OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) override;

    IFACEMETHODIMP OnDeviceAdded(LPCWSTR pwstrDeviceId) override;

    IFACEMETHODIMP OnDeviceRemoved(LPCWSTR pwstrDeviceId) override;

    IFACEMETHODIMP OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId) override;

    IFACEMETHODIMP OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key) override;

signals:
    void defaultDeviceChanged();
    void volumeChangedExternally();

private:
    ULONG m_refCount = 1;
    GUID m_localEventGuid;
};
