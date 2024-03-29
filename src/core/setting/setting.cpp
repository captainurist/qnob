#include "setting.h"

#include "setting_backend.h"

Setting::Setting(std::unique_ptr<SettingBackend> backend, QObject* parent):
    Entity(parent),
    m_backend(std::move(backend))
{
    m_backend->setParent(this);
    connect(m_backend.get(), &SettingBackend::changedExternally, this, &Setting::updateCachedState);
    connect(m_backend.get(), &SettingBackend::changedExternally, this, &Setting::activate);
    connect(m_backend.get(), &SettingBackend::initialized, this, &Setting::updateCachedState);

    updateCachedState();
}

Setting::~Setting() {}

bool Setting::isInitialized() const {
    return m_backend->isInitialized();
}

SettingState Setting::state() const {
    return m_cachedState;
}

void Setting::setState(const SettingState& state) {
    if (!m_backend->isInitialized())
        return;

    bool hasChanges = false;

    if (m_cachedState.enabled != state.enabled) {
        m_backend->setEnabled(state.enabled);
        hasChanges = true;
    }

    if (m_cachedState.value != state.value) {
        m_backend->setValue(state.value);
        hasChanges = true;
    }

    m_cachedState = state;

    if (hasChanges)
        emit changed();
}

void Setting::activate() {
    if (!m_backend->isInitialized())
        return;

    emit activated();
}

void Setting::updateCachedState() {
    if (!m_backend->isInitialized())
        return;

    m_cachedState = { m_backend->isEnabled(), m_backend->value() };

    emit changed(); /* Comparing old & new states here is not really worth it. */
}
