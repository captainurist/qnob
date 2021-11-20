#pragma once

#include <QtCore/QObject>

class EntityConfig;

class Entity : public QObject {
    Q_OBJECT
public:
    Entity(QObject* parent);
    virtual ~Entity();

    bool isLoaded() const {
        return m_loaded;
    }

    /**
     * \param cfg                       Entity config.
     * \throws EntityCreationException
     */
    void load(const EntityConfig& cfg);

    // TODO: does ID even belong here?

    void initializeId(const QString& id);

    const QString& id() const {
        return m_id;
    }

signals:
    void loaded();

protected:
    virtual void loadFromConfig(const EntityConfig& cfg);

private:
    bool m_loaded = false;
    QString m_id;
};
