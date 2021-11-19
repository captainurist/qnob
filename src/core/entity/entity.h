#pragma once

#include <QtCore/QObject>

class EntityCreationContext;

class Entity : public QObject {
    Q_OBJECT
public:
    Entity(QObject* parent);
    virtual ~Entity();

    /**
     * \param ctx                       Entity initialization context.
     * \throws EntityCreationException
     */
    virtual void initialize(const EntityCreationContext& ctx);

    // TODO: does ID even belong here?

    void initializeId(const QString& id);

    const QString& id() const {
        return m_id;
    }

private:
    QString m_id;
};
