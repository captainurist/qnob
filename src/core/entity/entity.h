#pragma once

#include <QtCore/QObject>

class EntityCreationContext;

class Entity : public QObject {
    Q_OBJECT
public:
    Entity(QObject* parent);
    virtual ~Entity();

    bool isLoaded() const {
        return m_loaded;
    }

    /**
     * \param ctx                       Entity initialization context.
     * \throws EntityCreationException
     */
    void load(const EntityCreationContext& ctx);

    // TODO: does ID even belong here?

    void initializeId(const QString& id);

    const QString& id() const {
        return m_id;
    }

signals:
    void loaded();

protected:
    virtual void loadFromConfig(const EntityCreationContext& ctx);

private:
    bool m_loaded = false;
    QString m_id;
};
