#pragma once

#if 0

#include "upnp_discovery_options.h"

class QTimer;
class UpnpDiscoveryEndpoint;
class UpnpDiscoveryMessage;

class UpnpDiscovery : public QObject {
    Q_OBJECT
public:
    UpnpDiscovery(const UpnpDiscoveryRequest& options, QObject* parent);
    virtual ~UpnpDiscovery();

    const UpnpDiscoveryRequest& options() const;

    /**
     * Starts discovery.
     */
    void start();

    /**
     * Stops discovery.
     */
    void stop();

    /**
     * \returns                         Whether this discovery object is running.
     */
    bool isRunning() const;

signals:
    void discovered(const UpnpDiscoveryMessage& reply);

private:
    void discoveryTick();
    void rediscoveryTick();
    void handleDiscovered(const UpnpDiscoveryMessage& reply);

private:
    std::unique_ptr<UpnpDiscoveryEndpoint> m_socket;
    std::unique_ptr<QTimer> m_discoveryTimer;
    std::unique_ptr<QTimer> m_rediscoveryTimer;
    int m_discoveryAttemptsLeft = 0;
};

#endif