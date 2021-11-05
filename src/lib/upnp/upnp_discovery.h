#pragma once

#include "upnp_discovery_options.h"

class QTimer;
class UpnpDiscoverySocket;
class UpnpDiscoveryReply;

class UpnpDiscovery : public QObject {
    Q_OBJECT
public:
    UpnpDiscovery(const UpnpDiscoveryOptions& options);
    virtual ~UpnpDiscovery();

    const UpnpDiscoveryOptions& options() const;

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
    void discovered(const UpnpDiscoveryReply& reply);

private:
    void discoveryTick();
    void rediscoveryTick();
    void handleDiscovered(const UpnpDiscoveryReply& reply);

private:
    std::unique_ptr<UpnpDiscoverySocket> m_socket;
    std::unique_ptr<QTimer> m_discoveryTimer;
    std::unique_ptr<QTimer> m_rediscoveryTimer;
    int m_discoveryAttemptsLeft = 0;
};
