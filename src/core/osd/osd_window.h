#pragma once

#include <QtGui/QRasterWindow>

#include <util/deferred_picture.h>

class OsdWindow : public QRasterWindow {
public:
    OsdWindow(const QString& title);

    void setPicture(const DeferredPicture& picture);
    const DeferredPicture& picture() const;

protected:
    virtual void paintEvent(QPaintEvent* paintEvent) override;

private:
    DeferredPicture m_picture;
};
