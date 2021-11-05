#include "win_volume_event_handler.h"

WinVolumeEventHandler::WinVolumeEventHandler(const GUID& localEventGuid, QObject* parent) :
    QObject(parent),
    m_localEventGuid(localEventGuid)
{}

IFACEMETHODIMP WinVolumeEventHandler::QueryInterface(REFIID iid, void** object) {
    if (iid == __uuidof(IUnknown)) {
        *object = static_cast<IUnknown*>(static_cast<IAudioEndpointVolumeCallback*>(this));
    } else if (iid == __uuidof(IAudioEndpointVolumeCallback)) {
        *object = static_cast<IAudioEndpointVolumeCallback*>(this);
    } else if (iid == __uuidof(IMMNotificationClient)) {
        *object = static_cast<IMMNotificationClient*>(this);
    } else {
        *object = nullptr;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

IFACEMETHODIMP_(ULONG) WinVolumeEventHandler::AddRef() {
    return InterlockedIncrement(&m_refCount);
}

IFACEMETHODIMP_(ULONG) WinVolumeEventHandler::Release() {
    ULONG result = InterlockedDecrement(&m_refCount);
    if (result == 0)
        delete this;
    return result;
}

IFACEMETHODIMP WinVolumeEventHandler::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA data) {
    if (data->guidEventContext == m_localEventGuid)
        return S_OK; /* Our own event got back to us, ignore it. */

    emit volumeChangedExternally();

    return S_OK;
}

IFACEMETHODIMP WinVolumeEventHandler::OnDeviceStateChanged(LPCWSTR /*pwstrDeviceId*/, DWORD /*dwNewState*/) {
    return S_OK;
}

IFACEMETHODIMP WinVolumeEventHandler::OnDeviceAdded(LPCWSTR /*pwstrDeviceId*/) {
    return S_OK;
}

IFACEMETHODIMP WinVolumeEventHandler::OnDeviceRemoved(LPCWSTR /*pwstrDeviceId*/) {
    return S_OK;
}

IFACEMETHODIMP WinVolumeEventHandler::OnDefaultDeviceChanged(EDataFlow flow, ERole /*role*/, LPCWSTR /*pwstrDefaultDeviceId*/) {
    if (flow == eRender)
        emit defaultDeviceChanged();

    return S_OK;
}

IFACEMETHODIMP WinVolumeEventHandler::OnPropertyValueChanged(LPCWSTR /*pwstrDeviceId*/, const PROPERTYKEY /*key*/) {
    return S_OK;
}
