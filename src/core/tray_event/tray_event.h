#pragma once

#include <core/entity/entity.h>
#include <lib/keys/key_combination.h>

class TrayEvent : public Entity {
    Q_OBJECT
public:
    using Entity::Entity;
    virtual ~TrayEvent();

    void setEventSource(QObject* eventSource);
    QObject* eventSource() const;

    void setKey(QKeyCombination key);
    QKeyCombination key() const;

signals:
    void triggered();

protected:
    virtual void loadFromConfig(const EntityCreationContext& ctx) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QKeyCombination m_key;
    QObject* m_eventSource = nullptr;
};
