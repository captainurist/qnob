#pragma once

#include <QtGui/QIconEngine>

class FixedSizeIconEngine : public QIconEngine {
public:
    FixedSizeIconEngine(const QPixmap& pixmap) :
        m_pixmap(pixmap)
    {}

    virtual void paint(QPainter* painter, const QRect& rect, QIcon::Mode /*mode*/, QIcon::State /*state*/) override {
        painter->drawPixmap(rect.topLeft(), m_pixmap);
    }

    virtual QSize actualSize(const QSize& /*size*/, QIcon::Mode /*mode*/, QIcon::State /*state*/) override {
        return m_pixmap.size();
    }

    virtual QPixmap pixmap(const QSize& /*size*/, QIcon::Mode /*mode*/, QIcon::State /*state*/) override {
        return m_pixmap;
    }

    virtual QString key() const override {
        return lit("FixedSizeIconEngine");
    }

    virtual QIconEngine* clone() const override {
        return new FixedSizeIconEngine(m_pixmap);
    }

    virtual QList<QSize> availableSizes(QIcon::Mode /*mode*/, QIcon::State /*state*/) override {
        return { m_pixmap.size() };
    }

    virtual bool isNull() override {
        return m_pixmap.isNull();
    }

private:
    QPixmap m_pixmap;
};
